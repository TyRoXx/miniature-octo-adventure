#include "save.h"
#include "base/serialization.h"
#ifndef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

typedef struct Mover_v1
{
	uint32_t x;
	uint32_t y;
	uint64_t appearance;
	uint8_t direction;
}
Mover_v1;

MOA_USE_RESULT
static Mover_v1 describe_mover_v1(Mover const *original)
{
	Mover_v1 description;
	assert(original->body.position.vector.x >= 0);
	assert(original->body.position.vector.y >= 0);
	description.x = (uint32_t)original->body.position.vector.x;
	description.y = (uint32_t)original->body.position.vector.y;
	description.appearance = original->body.appearance;
	description.direction = original->body.direction;
	return description;
}

static StructElement const mover_v1_definition[] =
{
	{&uint32, offsetof(Mover_v1, x)},
	{&uint32, offsetof(Mover_v1, y)},
	{&uint64, offsetof(Mover_v1, appearance)},
	{&uint8, offsetof(Mover_v1, direction)}
};

typedef struct Vector2i_v1
{
	uint32_t x;
	uint32_t y;
}
Vector2i_v1;

static StructElement const vector2i_v1_elements[] =
{
	{&uint32, offsetof(Vector2i_v1, x)},
	{&uint32, offsetof(Vector2i_v1, y)},
	{NULL, 0}
};

typedef struct NPCObjective_v1
{
	uint8_t objective;
	union
	{
		uint64_t wait_until;
		Vector2i_v1 move_destination;
	}
	state;
}
NPCObjective_v1;

typedef struct VariantElement
{
	DataType const *type;
}
VariantElement;

static VariantElement const npc_objective_v1_elements[] =
{
	{&uint64}
};

MOA_USE_RESULT
static byte_size round_bits_up_to_bytes(bit_size bits)
{
	return (bits + 7) / 8;
}

MOA_USE_RESULT
static Bool write_struct_buffered(int file, Vector *initialized_buffer, void const *instance, StructDefinition structure, Allocator allocator)
{
	bit_size const instance_bit_size = struct_size_in_bits(structure, instance);
	byte_size const instance_size = round_bits_up_to_bytes(instance_bit_size);
	if (instance_size > SIZE_MAX)
	{
		return False;
	}
	if (!Vector_resize(initialized_buffer, (size_t)instance_size, allocator))
	{
		return False;
	}
	bit_writer writer = {(byte *)Vector_data(initialized_buffer), 0};
	writer = struct_serialize(writer, instance, structure);
	ssize_t const written = write(file, Vector_data(initialized_buffer), Vector_size(initialized_buffer));
	return (written == (ssize_t)Vector_size(initialized_buffer));
}

Bool save_game_to_file(char const *file_name, Mover const *avatar, Fauna const *fauna, MemoryManager memory)
{
	int file = open(file_name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	if (file < 0)
	{
		return False;
	}

	char const * const version_tag = "MOAsav01";
	if (write(file, version_tag, strlen(version_tag)) != (ssize_t)strlen(version_tag))
	{
		close(file);
		return False;
	}

	Vector write_buffer;
	Vector_init(&write_buffer);

	Bool result = True;
	{
		Mover_v1 const avatar_description = describe_mover_v1(avatar);
		if (!write_struct_buffered(file, &write_buffer, &avatar_description, MOA_STRUCT_DEFINITION_FROM_ARRAY(mover_v1_definition), memory.allocator))
		{
			result = False;
			goto leave;
		}
	}

	uint64_t npc_count = Vector_size(&fauna->npcs) / sizeof(NPC);
	{
		byte npc_count_buffer[sizeof(npc_count)];
		bit_writer writer = {&npc_count_buffer[0], 0};
		writer = data_type_serialize(writer, &npc_count, uint64);
		ssize_t const written = write(file, npc_count_buffer, sizeof(npc_count_buffer));
		if ((written < 0) || ((size_t)written != sizeof(npc_count_buffer)))
		{
			result = False;
			goto leave;
		}
	}

	{
		for (size_t i = 0; i < npc_count; ++i)
		{
			NPC const * const npc = Vector_at(&fauna->npcs, i, sizeof(NPC));
			Mover_v1 const mover_description = describe_mover_v1(&npc->mover);
			if (!write_struct_buffered(file, &write_buffer, &mover_description, MOA_STRUCT_DEFINITION_FROM_ARRAY(mover_v1_definition), memory.allocator))
			{
				result = False;
				goto leave;
			}
		}
	}

leave:
	Vector_free(&write_buffer, memory.deallocator);
	close(file);
	return result;
}
