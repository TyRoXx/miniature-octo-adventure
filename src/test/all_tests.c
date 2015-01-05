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

static void test_vector(void);
static void test_saturating_int(void);
static void test_gui_labeled_button(void);
static void test_gui_panel_vertical_layout(void);
static void test_gui_padding(void);
static void test_serialization(void);

void moa_test_run_all(void)
{
	test_vector();
	test_saturating_int();
	test_gui_labeled_button();
	test_gui_panel_vertical_layout();
	test_gui_padding();
	test_serialization();
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

typedef struct SerializationStruct1
{
	uint32_t a, b, c;
	uint16_t d;
	uint8_t e;
}
SerializationStruct1;

typedef enum DataType
{
	DataType_UInt32,
	DataType_UInt16,
	DataType_UInt8
}
DataType;

static size_t data_type_sizeof(DataType type)
{
	switch (type)
	{
	case DataType_UInt32: return 4;
	case DataType_UInt16: return 2;
	case DataType_UInt8: return 1;
	}
	MOA_UNREACHABLE();
}

static unsigned char *data_type_serialize(
	unsigned char *destination,
	void const *original,
	DataType type)
{
	switch (type)
	{
	case DataType_UInt8:
		destination[0] = ((unsigned char const *)original)[0];
		return destination + 1;

	case DataType_UInt16:
		{
			uint16_t const *value = original;
			destination[0] = (unsigned char)(*value >> 8);
			destination[1] = (unsigned char)(*value);
			return destination + 2;
		}

	case DataType_UInt32:
		{
			uint32_t const *value = original;
			destination[0] = (unsigned char)(*value >> 24);
			destination[1] = (unsigned char)(*value >> 16);
			destination[2] = (unsigned char)(*value >> 8);
			destination[3] = (unsigned char)(*value);
			return destination + 4;
		}
	}
	MOA_UNREACHABLE();
}

typedef struct StructElement
{
	DataType type;
	size_t offset;
}
StructElement;

static size_t struct_sizeof(StructElement const *begin, StructElement const *end)
{
	size_t size = 0;
	for (; begin != end; ++begin)
	{
		size += data_type_sizeof(begin->type);
	}
	return size;
}

static void struct_serialize(
	unsigned char *destination,
	void const *instance,
	StructElement const *begin,
	StructElement const *end)
{
	for (; begin != end; ++begin)
	{
		destination = data_type_serialize(destination, (char const *)instance + begin->offset, begin->type);
	}
}

static void test_serialization(void)
{
	SerializationStruct1 s;
	s.a = 0x11223344;
	s.b = 0x55667788;
	s.c = 0x99aabbcc;
	s.d = 0xddee;
	s.e = 0xff;
	StructElement const s_type[] =
	{
		{DataType_UInt32, offsetof(SerializationStruct1, a)},
		{DataType_UInt32, offsetof(SerializationStruct1, b)},
		{DataType_UInt32, offsetof(SerializationStruct1, c)},
		{DataType_UInt16, offsetof(SerializationStruct1, d)},
		{DataType_UInt8, offsetof(SerializationStruct1, e)},
	};
	size_t size = struct_sizeof(s_type, MOA_ARRAY_END(s_type));
	TEST(size == 12 + 2 + 1);
	unsigned char serialized[12 + 2 + 1] = {0};
	struct_serialize(serialized, &s, s_type, MOA_ARRAY_END(s_type));
	unsigned char const expected[12 + 2 + 1] =
	{
		0x11, 0x22, 0x33, 0x44,
		0x55, 0x66, 0x77, 0x88,
		0x99, 0xaa, 0xbb, 0xcc,
		0xdd, 0xee,
		0xff
	};
	TEST(memcmp(expected, serialized, sizeof(expected)) == 0);
}
