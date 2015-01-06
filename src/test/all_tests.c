#include "all_tests.h"
#include "base/vector.h"
#include "base/stringize.h"
#include "base/algorithm.h"
#include "base/saturating_int.h"
#include "gui/labeled_button.h"
#include "gui/padding.h"
#include "gui/panel.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

static void test_vector(void);
static void test_saturating_int(void);
static void test_gui_labeled_button(void);
static void test_gui_panel_vertical_layout(void);
static void test_gui_padding(void);
static void test_serialization_1(void);
static void test_serialization_2(void);

void moa_test_run_all(void)
{
	test_vector();
	test_saturating_int();
	test_gui_labeled_button();
	test_gui_panel_vertical_layout();
	test_gui_padding();
	test_serialization_1();
	test_serialization_2();
}

#ifdef _MSC_VER
/*owl silences VC++ warning C4127*/
#	define MOA_COMPILE_TIME_FALSE (0,0)
#else
#	define MOA_COMPILE_TIME_FALSE 0
#endif

#define TEST(x)   do { if (!(x)) { moa_test_on_message("TEST("   MOA_STRINGIZE(x) ") fail", __FILE__, __LINE__);         } } while (MOA_COMPILE_TIME_FALSE)
#define ASSERT(x) do { if (!(x)) { moa_test_on_message("ASSERT(" MOA_STRINGIZE(x) ") fail", __FILE__, __LINE__); return; } } while (MOA_COMPILE_TIME_FALSE)

static Allocation wrapped_realloc(Allocation allocation, size_t size, PrivateAllocatorState state)
{
	(void)state;
	return realloc(allocation, size);
}

static void wrapped_free(Allocation allocation, PrivateAllocatorState state)
{
	(void)state;
	free(allocation);
}

static MemoryManager create_standard_memory_manager(void)
{
	MemoryManager manager = {{wrapped_realloc, NULL}, {wrapped_free, NULL}};
	return manager;
}

static void test_vector(void)
{
	MemoryManager memory = create_standard_memory_manager();

	{
		Vector v;
		Vector_init(&v);
		TEST(Vector_empty(&v));
		TEST(Vector_size(&v) == 0);
		Vector_free(&v, memory.deallocator);
	}

	{
		Vector v;
		Vector_init(&v);
		TEST(Vector_reserve(&v, 100, memory.allocator));
		TEST(Vector_reserve(&v, 200, memory.allocator));
		TEST(Vector_reserve(&v, 50, memory.allocator));
		TEST(Vector_reserve(&v, 0, memory.allocator));
		Vector_free(&v, memory.deallocator);
	}

	{
		Vector v;
		Vector_init(&v);
		TEST(Vector_resize(&v, 100, memory.allocator));
		TEST(Vector_resize(&v, 200, memory.allocator));
		TEST(Vector_resize(&v, 50, memory.allocator));
		TEST(Vector_resize(&v, 0, memory.allocator));
		Vector_free(&v, memory.deallocator);
	}

	{
		size_t i;
		size_t const count = 1000;
		Vector v;
		Vector_init(&v);
		for (i = 0; i < count; ++i)
		{
			char c = (char)('0' + (i % 10));
			ASSERT(Vector_push_back(&v, &c, 1, memory.allocator));
		}
		for (i = 0; i < count; ++i)
		{
			char c = (char)('0' + (i % 10));
			ASSERT(Vector_data(&v)[i] == c);
		}
		Vector_free(&v, memory.deallocator);
	}
}

static void test_saturating_int(void)
{
	TEST(saturating_add(0, 0) == 0);
	TEST(saturating_add(INT_MAX, INT_MAX) == INT_MAX);
	TEST(saturating_add(INT_MIN, INT_MIN) == INT_MIN);
	TEST(saturating_add(INT_MAX, INT_MIN) == (INT_MAX + INT_MIN));
	TEST(saturating_add(-1, 1) == 0);
	TEST(saturating_add(-1, 0) == -1);
	TEST(saturating_add(0, -1) == -1);
	TEST(saturating_add(-1, -1) == -2);
}

static void handle_on_click(void *user)
{
	(void)user;
}

static void test_gui_labeled_button(void)
{
	Vector2i position = Vector2i_new(20, 30);
	Vector2i desired_size = Vector2i_new(100, 200);
	Vector2i actual_size = Vector2i_new(80, 210);
	char const * const text = "text";
	LabeledButton button = LabeledButton_create(text, make_text_style(TextAlignment_Left, TextAlignment_Right, make_color(1, 2, 3, 4)), desired_size, make_color(5, 6, 7, 8), handle_on_click, NULL);
	button.base.absolute_position = position;
	button.base.actual_size = actual_size;
	Widget_pack(&button.base);
	TEST(Vector2i_equal(&button.base.absolute_position, &position));
	TEST(Vector2i_equal(&button.base.actual_size, &actual_size));
	TEST(Vector2i_equal(&button.base.desired_size, &desired_size));
	TEST(Vector2i_equal(&button.button.base.absolute_position, &position));
	TEST(Vector2i_equal(&button.button.base.actual_size, &actual_size));
	TEST(Vector2i_equal(&button.button.base.desired_size, &desired_size));
	TEST(Vector2i_equal(&button.label.base.absolute_position, &position));
	TEST(Vector2i_equal(&button.label.base.actual_size, &actual_size));
	TEST(Vector2i_equal(&button.label.base.desired_size, &desired_size));
	TEST(text == button.label.text);
}

static void test_gui_panel_vertical_layout(void)
{
	Vector2i position = Vector2i_new(20, 30);
	Vector2i desired_size = Vector2i_new(100, 200);
	Vector2i actual_size = Vector2i_new(80, 210);
	Panel panel = Panel_create(desired_size, pack_vertically);
	Vector2i child_size = Vector2i_new(140, 200);
	Label child = Label_create("text", make_text_style(TextAlignment_Left, TextAlignment_Right, make_color(1, 2, 3, 4)), child_size);
	Vector2i child_actual_size = Vector2i_new(80, 200);

	panel.base.absolute_position = position;
	panel.base.actual_size = actual_size;
	Widget_pack(&panel.base);
	TEST(Vector2i_equal(&panel.base.absolute_position, &position));
	TEST(Vector2i_equal(&panel.base.actual_size, &actual_size));
	TEST(Vector2i_equal(&panel.base.desired_size, &desired_size));

	Widget *child_ptr = &child.base;
	panel.children.begin = &child_ptr;
	panel.children.end = &child_ptr + 1;
	Widget_pack(&panel.base);
	TEST(Vector2i_equal(&panel.base.absolute_position, &position));
	TEST(Vector2i_equal(&panel.base.actual_size, &actual_size));
	TEST(Vector2i_equal(&panel.base.desired_size, &desired_size));
	TEST(Vector2i_equal(&child.base.absolute_position, &position));
	TEST(Vector2i_equal(&child.base.actual_size, &child_actual_size));
	TEST(Vector2i_equal(&child.base.desired_size, &child_size));
}

static void test_gui_padding(void)
{
	Vector2i position = Vector2i_new(20, 30);
	Vector2i desired_size = Vector2i_new(100, 200);
	Vector2i actual_size = Vector2i_new(80, 210);
	Label content = Label_create("text", make_text_style(TextAlignment_Left, TextAlignment_Right, make_color(1, 2, 3, 4)), desired_size);
	int padding_amount = 3;
	Padding padding = Padding_create(desired_size, &content.base, padding_amount);
	Vector2i content_position = Vector2i_new(position.x + padding_amount, position.y + padding_amount);
	Vector2i content_actual_size = Vector2i_new(actual_size.x - 2 * padding_amount, desired_size.y - 2 * padding_amount);

	padding.base.absolute_position = position;
	padding.base.actual_size = actual_size;

	Widget_pack(&padding.base);
	TEST(Vector2i_equal(&padding.base.absolute_position, &position));
	TEST(Vector2i_equal(&padding.base.actual_size, &actual_size));
	TEST(Vector2i_equal(&padding.base.desired_size, &desired_size));

	TEST(Vector2i_equal(&content.base.absolute_position, &content_position));
	TEST(Vector2i_equal(&content.base.actual_size, &content_actual_size));
	TEST(Vector2i_equal(&content.base.desired_size, &desired_size));
}

typedef struct StringRef
{
	char const *begin, *end;
}
StringRef;

MOA_USE_RESULT
static StringRef StringRef_from_c_str(char const *str)
{
	StringRef ref = {str, str + strlen(str)};
	return ref;
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

typedef enum DataType
{
	DataType_UInt64,
	DataType_UInt32,
	DataType_UInt16,
	DataType_UInt8,
	DataType_String,
	DataType_Bool
}
DataType;

typedef unsigned char byte;
typedef uint64_t bit_size;

MOA_USE_RESULT
static bit_size data_type_size_in_bits(DataType type, void const *data)
{
	switch (type)
	{
	case DataType_UInt64: return 64;
	case DataType_UInt32: return 32;
	case DataType_UInt16: return 16;
	case DataType_UInt8: return 8;
	case DataType_String:
		{
			StringRef const *str = data;
			return 64 + (size_t)(str->end - str->begin) * 8;
		}
	case DataType_Bool: return 1;
	}
	MOA_UNREACHABLE();
}

typedef struct bit_writer
{
	byte *current_byte;
	bit_size used_bits_in_byte;
}
bit_writer;

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

static bit_writer data_type_serialize(
	bit_writer destination,
	void const *original,
	DataType type)
{
	switch (type)
	{
	case DataType_UInt8:
		return write_byte(destination, *(byte const *)original);

	case DataType_UInt16:
		{
			uint16_t const *value = original;
			destination = write_byte(destination, (byte)(*value >> 8));
			return write_byte(destination, (byte)*value);
		}

	case DataType_UInt32:
		{
			uint32_t const *value = original;
			uint32_t value_big_endian = htonl(*value);
			byte const *begin = (byte const *)&value_big_endian;
			return write_bytes(destination, begin, begin + sizeof(value_big_endian));
		}

	case DataType_UInt64:
		{
			uint64_t const *value = original;
			destination = write_byte(destination, (byte)(*value >> 56ULL));
			destination = write_byte(destination, (byte)(*value >> 48ULL));
			destination = write_byte(destination, (byte)(*value >> 40ULL));
			destination = write_byte(destination, (byte)(*value >> 32ULL));
			destination = write_byte(destination, (byte)(*value >> 24ULL));
			destination = write_byte(destination, (byte)(*value >> 16ULL));
			destination = write_byte(destination, (byte)(*value >>  8ULL));
			return write_byte(destination, (byte)*value);
		}

	case DataType_String:
		{
			StringRef const *value = original;
			size_t length = (size_t)(value->end - value->begin);
			assert(length <= UINT64_MAX);
			uint64_t writeable_length = (uint64_t)length;
			destination = data_type_serialize(destination, &writeable_length, DataType_UInt64);
			for (char const *i = value->begin; i != value->end; ++i)
			{
				destination = write_byte(destination, (byte)*i);
			}
			return destination;
		}

	case DataType_Bool:
		return write_bit(destination, *(Bool const *)original);
	}
	MOA_UNREACHABLE();
}

typedef struct StructElement
{
	DataType type;
	size_t offset;
}
StructElement;

MOA_USE_RESULT
static bit_size struct_size_in_bits(StructElement const *begin, StructElement const *end, void const *instance)
{
	bit_size size = 0;
	for (; begin != end; ++begin)
	{
		size += data_type_size_in_bits(begin->type, (char const *)instance + begin->offset);
	}
	return size;
}

MOA_USE_RESULT
static bit_writer struct_serialize(
	bit_writer destination,
	void const *instance,
	StructElement const *begin,
	StructElement const *end)
{
	for (; begin != end; ++begin)
	{
		destination = data_type_serialize(destination, (char const *)instance + begin->offset, begin->type);
	}
	return destination;
}

static void test_serialization_1(void)
{
	SerializationStruct1 s;
	s.a = 0x11223344;
	s.b = 0x55667788;
	s.c = 0x99aabbcc12345678;
	s.d = 0xddee;
	s.e = 0xff;
	s.f = StringRef_from_c_str("abc");
	s.g = 1;
	s.h = 0x12345678;
	StructElement const s_type[] =
	{
		{DataType_UInt32, offsetof(SerializationStruct1, a)},
		{DataType_UInt32, offsetof(SerializationStruct1, b)},
		{DataType_UInt64, offsetof(SerializationStruct1, c)},
		{DataType_UInt16, offsetof(SerializationStruct1, d)},
		{DataType_UInt8, offsetof(SerializationStruct1, e)},
		{DataType_String, offsetof(SerializationStruct1, f)},
		{DataType_Bool, offsetof(SerializationStruct1, g)},
		{DataType_UInt32, offsetof(SerializationStruct1, h)}
	};
	bit_size size = struct_size_in_bits(s_type, MOA_ARRAY_END(s_type), &s);
	unsigned char const expected[] =
	{
		0x11, 0x22, 0x33, 0x44,
		0x55, 0x66, 0x77, 0x88,
		0x99, 0xaa, 0xbb, 0xcc, 0x12, 0x34, 0x56, 0x78,
		0xdd, 0xee,
		0xff,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
		'a', 'b', 'c',
		0x01 | 0x12, 0x34, 0x56, 0x78, 0x00
	};
	TEST(size == sizeof(expected) * 8 - 7);
	byte serialized[sizeof(expected)] = {0};
	bit_writer writer = {&serialized[0], 0};
	writer = struct_serialize(writer, &s, s_type, MOA_ARRAY_END(s_type));
	TEST(writer.current_byte == MOA_ARRAY_END(serialized) - 1);
	TEST(writer.used_bits_in_byte == 1);
	TEST(memcmp(expected, serialized, sizeof(expected)) == 0);
}

static void test_serialization_2(void)
{
	byte serialized[2] = {0};
	byte const expected[2] =
	{
		0xfe,
		0x05
	};
	bit_writer writer = {&serialized[0], 1};
	writer = write_byte(writer, 0xff);
	writer = write_bit(writer, 0);
	writer = write_bit(writer, 1);
	TEST(writer.current_byte == &serialized[1]);
	TEST(writer.used_bits_in_byte == 3);
	TEST(memcmp(expected, serialized, sizeof(expected)) == 0);
}
