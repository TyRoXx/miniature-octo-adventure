#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "base/unused.h"
#include "base/algorithm.h"
#include "base/unreachable.h"
#include "base/bool.h"
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

typedef struct StringRef
{
	char const *begin, *end;
}
StringRef;

MOA_USE_RESULT
static inline StringRef StringRef_from_c_str(char const *str)
{
	StringRef ref = {str, str + strlen(str)};
	return ref;
}

MOA_USE_RESULT
static inline Bool StringRef_equal_content(StringRef first, StringRef second)
{
	size_t first_length  = (size_t)(first.end - first.begin);
	size_t second_length = (size_t)(second.end - second.begin);
	if (first_length != second_length)
	{
		return False;
	}
	return memcmp(first.begin, second.begin, first_length) == 0;
}

typedef struct SerializationStruct1
{
	uint32_t a, b;
	uint64_t c;
	uint16_t d;
	uint8_t e;
	StringRef f;
	Bool g;
	uint32_t h;
}
SerializationStruct1;

typedef unsigned char byte;
typedef uint64_t bit_size;
typedef uint64_t byte_size;

typedef struct bit_writer
{
	byte *current_byte;
	bit_size used_bits_in_byte;
}
bit_writer;

typedef struct bit_reader
{
	byte const *current_byte;
	bit_size used_bits_in_byte;
}
bit_reader;

MOA_USE_RESULT
static bit_writer write_byte(bit_writer destination, byte value)
{
	if (destination.used_bits_in_byte)
	{
		*destination.current_byte |= (byte)((value >> destination.used_bits_in_byte) << destination.used_bits_in_byte);
		++destination.current_byte;
		*destination.current_byte = (byte)(value & ((1U << destination.used_bits_in_byte) - 1U));
	}
	else
	{
		*destination.current_byte = value;
		++destination.current_byte;
	}
	return destination;
}

MOA_USE_RESULT
static bit_writer write_bytes(bit_writer destination, byte const *begin, byte const *end)
{
	if (destination.used_bits_in_byte)
	{
		for (; begin != end; ++begin)
		{
			destination = write_byte(destination, *begin);
		}
	}
	else
	{
		memmove(destination.current_byte, begin, (size_t)(end - begin));
		destination.current_byte += (end - begin);
	}
	return destination;
}

MOA_USE_RESULT
static bit_reader read_byte(bit_reader source, byte *to)
{
	if (source.used_bits_in_byte)
	{
		byte mask = (byte)((1U << source.used_bits_in_byte) - 1U);
		*to = (byte)(*source.current_byte & ~mask);
		source.current_byte += 1;
		*to |= (byte)(*source.current_byte & mask);
	}
	else
	{
		*to = *source.current_byte;
		source.current_byte += 1;
	}
	return source;
}

MOA_USE_RESULT
static bit_reader read_bytes(bit_reader source, byte *begin, byte *end)
{
	if (source.used_bits_in_byte)
	{
		for (; begin != end; ++begin)
		{
			source = read_byte(source, begin);
		}
	}
	else
	{
		memmove(begin, source.current_byte, (size_t)(end - begin));
		source.current_byte += (end - begin);
	}
	return source;
}

MOA_USE_RESULT
static bit_writer write_bit(bit_writer destination, Bool value)
{
	*destination.current_byte |= (byte)(value << destination.used_bits_in_byte);
	if (destination.used_bits_in_byte == 7)
	{
		destination.current_byte += 1;
		destination.used_bits_in_byte = 0;
	}
	else
	{
		destination.used_bits_in_byte += 1;
	}
	return destination;
}

MOA_USE_RESULT
static bit_reader read_bit(bit_reader source, Bool *to)
{
	*to = (*source.current_byte & (1U << source.used_bits_in_byte)) != 0;
	if (source.used_bits_in_byte == 7)
	{
		source.current_byte += 1;
		source.used_bits_in_byte = 0;
	}
	else
	{
		source.used_bits_in_byte += 1;
	}
	return source;
}

typedef struct DataType
{
	MOA_USE_RESULT bit_size (*size_in_bits)(void const *, void const *);
	MOA_USE_RESULT bit_writer (*write)(bit_writer, void const *, void const *);
	MOA_USE_RESULT bit_reader (*read)(bit_reader, void *, void const *);
	void const *state;
}
DataType;

MOA_USE_RESULT
static bit_size data_type_size_in_bits(DataType type, void const *instance)
{
	return type.size_in_bits(instance, type.state);
}

MOA_USE_RESULT
static bit_writer data_type_serialize(
	bit_writer destination,
	void const *instance,
	DataType type)
{
	return type.write(destination, instance, type.state);
}

MOA_USE_RESULT
static bit_reader data_type_deserialize(
	bit_reader source,
	void *instance,
	DataType type)
{
	return type.read(source, instance, type.state);
}

MOA_USE_RESULT
static bit_size uint8_size_in_bits(void const *instance, void const *state)
{
	(void)state;
	(void)instance;
	return 8;
}

MOA_USE_RESULT
static bit_writer uint8_write(bit_writer destination, void const *instance, void const *state)
{
	(void)state;
	return write_byte(destination, *(byte const *)instance);
}

MOA_USE_RESULT
static bit_reader uint8_read(bit_reader source, void *instance, void const *state)
{
	(void)state;
	return read_byte(source, instance);
}

static DataType const uint8 = {uint8_size_in_bits, uint8_write, uint8_read, NULL};

MOA_USE_RESULT
static bit_size uint16_size_in_bits(void const *instance, void const *state)
{
	(void)state;
	(void)instance;
	return 16;
}

MOA_USE_RESULT
static bit_writer uint16_write(bit_writer destination, void const *instance, void const *state)
{
	(void)state;
	uint16_t const *value = instance;
	uint16_t value_big_endian = htons(*value);
	byte const *begin = (byte const *)&value_big_endian;
	return write_bytes(destination, begin, begin + sizeof(value_big_endian));
}

MOA_USE_RESULT
static bit_reader uint16_read(bit_reader source, void *instance, void const *state)
{
	(void)state;
	source = read_bytes(source, instance, (byte *)instance + sizeof(uint16_t));
	*(uint16_t *)instance = ntohs(*(uint16_t const *)instance);
	return source;
}

static DataType const uint16 = {uint16_size_in_bits, uint16_write, uint16_read, NULL};

MOA_USE_RESULT
static bit_size uint32_size_in_bits(void const *instance, void const *state)
{
	(void)state;
	(void)instance;
	return 32;
}

MOA_USE_RESULT
static bit_writer uint32_write(bit_writer destination, void const *instance, void const *state)
{
	(void)state;
	uint32_t const *value = instance;
	uint32_t value_big_endian = htonl(*value);
	byte const *begin = (byte const *)&value_big_endian;
	return write_bytes(destination, begin, begin + sizeof(value_big_endian));
}

MOA_USE_RESULT
static bit_reader uint32_read(bit_reader source, void *instance, void const *state)
{
	(void)state;
	source = read_bytes(source, instance, (byte *)instance + sizeof(uint32_t));
	*(uint32_t *)instance = ntohl(*(uint32_t const *)instance);
	return source;
}

static DataType const uint32 = {uint32_size_in_bits, uint32_write, uint32_read, NULL};

MOA_USE_RESULT
static bit_size uint64_size_in_bits(void const *instance, void const *state)
{
	(void)state;
	(void)instance;
	return 64;
}

MOA_USE_RESULT
static bit_writer uint64_write(bit_writer destination, void const *instance, void const *state)
{
	(void)state;
	uint64_t const *value = instance;
	destination = write_byte(destination, (byte)(*value >> 56ULL));
	destination = write_byte(destination, (byte)(*value >> 48ULL));
	destination = write_byte(destination, (byte)(*value >> 40ULL));
	destination = write_byte(destination, (byte)(*value >> 32ULL));
	destination = write_byte(destination, (byte)(*value >> 24ULL));
	destination = write_byte(destination, (byte)(*value >> 16ULL));
	destination = write_byte(destination, (byte)(*value >>  8ULL));
	return write_byte(destination, (byte)*value);
}

MOA_USE_RESULT
static bit_reader uint64_read(bit_reader source, void *instance, void const *state)
{
	(void)state;
	byte buffer[sizeof(uint64_t)];
	source = read_bytes(source, buffer, MOA_ARRAY_END(buffer));
	*(uint64_t *)instance =
		((uint64_t)(buffer[0]) << 56ULL) |
		((uint64_t)(buffer[1]) << 48ULL) |
		((uint64_t)(buffer[2]) << 40ULL) |
		((uint64_t)(buffer[3]) << 32ULL) |
		((uint64_t)(buffer[4]) << 24ULL) |
		((uint64_t)(buffer[5]) << 16ULL) |
		((uint64_t)(buffer[6]) <<  8ULL) |
		((uint64_t)(buffer[7]));
	return source;
}

static DataType const uint64 = {uint64_size_in_bits, uint64_write, uint64_read, NULL};

MOA_USE_RESULT
static bit_size bool_size_in_bits(void const *instance, void const *state)
{
	(void)state;
	(void)instance;
	return 1;
}

MOA_USE_RESULT
static bit_writer bool_write(bit_writer destination, void const *instance, void const *state)
{
	(void)state;
	return write_bit(destination, *(Bool const *)instance);
}

MOA_USE_RESULT
static bit_reader bool_read(bit_reader source, void *instance, void const *state)
{
	(void)state;
	return read_bit(source, instance);
}

static DataType const bool_type = {bool_size_in_bits, bool_write, bool_read, NULL};

MOA_USE_RESULT
static bit_size string_size_in_bits(void const *instance, void const *state)
{
	(void)state;
	StringRef const * const str = instance;
	return 64 + ((byte_size)(str->end - str->begin)) * 8;
}

MOA_USE_RESULT
static bit_writer string_write(bit_writer destination, void const *instance, void const *state)
{
	(void)state;
	StringRef const *value = instance;
	size_t length = (size_t)(value->end - value->begin);
	uint64_t writeable_length = length;
	destination = data_type_serialize(destination, &writeable_length, uint64);
	for (char const *i = value->begin; i != value->end; ++i)
	{
		destination = write_byte(destination, (byte)*i);
	}
	return destination;
}

MOA_USE_RESULT
static bit_reader string_read(bit_reader source, void *instance, void const *state)
{
	(void)state;
	uint64_t length;
	source = data_type_deserialize(source, &length, uint64);
	StringRef *destination = instance;
	assert(source.used_bits_in_byte == 0); //TODO: make this work somehow with bit offsets
	destination->begin = (char const *)source.current_byte;
	source.current_byte += length;
	destination->end = (char const *)source.current_byte;
	return source;
}

static DataType const string_type = {string_size_in_bits, string_write, string_read, NULL};

typedef struct StructElement
{
	DataType const *type;
	size_t offset;
}
StructElement;

typedef struct StructDefinition
{
	StructElement const *begin, *end;
}
StructDefinition;

MOA_USE_RESULT
static inline StructDefinition StructDefinition_new(StructElement const *begin, StructElement const *end)
{
	StructDefinition result = {begin, end};
	return result;
}

MOA_USE_RESULT
static inline StructDefinition StructDefinition_from_nullterminated_elements(StructElement const *elements)
{
	StructElement const *end = elements;
	while (end->type)
	{
		++end;
	}
	return StructDefinition_new(elements, end);
}

#define MOA_STRUCT_DEFINITION_FROM_ARRAY(array) (StructDefinition_new((array) + 0, MOA_ARRAY_END((array))))

MOA_USE_RESULT
static inline bit_size struct_size_in_bits(StructDefinition type, void const *instance)
{
	bit_size size = 0;
	for (; type.begin != type.end; ++type.begin)
	{
		size += data_type_size_in_bits(*type.begin->type, (char const *)instance + type.begin->offset);
	}
	return size;
}

MOA_USE_RESULT
static inline bit_writer struct_serialize(
	bit_writer destination,
	void const *instance,
	StructDefinition type)
{
	for (; type.begin != type.end; ++type.begin)
	{
		destination = data_type_serialize(destination, (char const *)instance + type.begin->offset, *type.begin->type);
	}
	return destination;
}

MOA_USE_RESULT
static inline bit_reader struct_deserialize(
	bit_reader source,
	void *instance,
	StructDefinition type)
{
	for (; type.begin != type.end; ++type.begin)
	{
		source = data_type_deserialize(source, (char *)instance + type.begin->offset, *type.begin->type);
	}
	return source;
}

static bit_size struct_size_in_bits2(void const *instance, void const *state)
{
	StructElement const *null_terminated_elements = state;
	return struct_size_in_bits(StructDefinition_from_nullterminated_elements(null_terminated_elements), instance);
}

MOA_USE_RESULT
static bit_writer struct_write(bit_writer destination, void const *instance, void const *state)
{
	StructElement const *null_terminated_elements = state;
	return struct_serialize(destination, instance, StructDefinition_from_nullterminated_elements(null_terminated_elements));
}

MOA_USE_RESULT
static bit_reader struct_read(bit_reader source, void *instance, void const *state)
{
	StructElement const *null_terminated_elements = state;
	return struct_deserialize(source, instance, StructDefinition_from_nullterminated_elements(null_terminated_elements));
}

MOA_USE_RESULT
static inline DataType make_struct_data_type(StructElement const *null_terminated_elements)
{
	DataType type = {struct_size_in_bits2, struct_write, struct_read, null_terminated_elements};
	return type;
}

#endif
