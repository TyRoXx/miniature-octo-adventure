#include "save.h"
#include "base/serialization.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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

typedef struct StructDefinition
{
	StructElement const *begin, *end;
}
StructDefinition;

MOA_USE_RESULT
static StructDefinition mover_v1_definition(void)
{
	static StructElement const elements[] =
	{
		{DataType_UInt32, offsetof(Mover_v1, x)},
		{DataType_UInt32, offsetof(Mover_v1, y)},
		{DataType_UInt64, offsetof(Mover_v1, appearance)},
		{DataType_UInt8, offsetof(Mover_v1, direction)}
	};
	StructDefinition result = {&elements[0], MOA_ARRAY_END(elements)};
	return result;
}

MOA_USE_RESULT
static byte_size round_bits_up_to_bytes(bit_size bits)
{
	return (bits + 7) / 8;
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

	StructDefinition const mover_definition = mover_v1_definition();
	Bool result = True;
	{
		if (!Vector_resize(&write_buffer, round_bits_up_to_bytes(struct_size_in_bits(mover_definition.begin, mover_definition.end, avatar)), memory.allocator))
		{
			result = False;
			goto leave;
		}
		bit_writer writer = {(byte *)Vector_data(&write_buffer), 0};
		Mover_v1 const avatar_description = describe_mover_v1(avatar);
		writer = struct_serialize(writer, &avatar_description, mover_definition.begin, mover_definition.end);
		ssize_t const written = write(file, Vector_data(&write_buffer), Vector_size(&write_buffer));
		if ((written < 0) || ((size_t)written != Vector_size(&write_buffer)))
		{
			result = False;
			goto leave;
		}
	}

	uint64_t npc_count = Vector_size(&fauna->npcs) / sizeof(NPC);
	{
		byte npc_count_buffer[sizeof(npc_count)];
		bit_writer writer = {&npc_count_buffer[0], 0};
		writer = data_type_serialize(writer, &npc_count, DataType_UInt64);
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
			if (!Vector_resize(&write_buffer, round_bits_up_to_bytes(struct_size_in_bits(mover_definition.begin, mover_definition.end, &npc->mover)), memory.allocator))
			{
				result = False;
				goto leave;
			}
			bit_writer writer = {(byte *)Vector_data(&write_buffer), 0};
			Mover_v1 const mover_description = describe_mover_v1(&npc->mover);
			writer = struct_serialize(writer, &mover_description, mover_definition.begin, mover_definition.end);
			ssize_t const written = write(file, Vector_data(&write_buffer), Vector_size(&write_buffer));
			if ((written < 0) || ((size_t)written != Vector_size(&write_buffer)))
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
