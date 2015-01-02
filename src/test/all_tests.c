#include "all_tests.h"
#include "base/vector.h"
#include "base/stringize.h"
#include "gui/labeled_button.h"
#include "gui/padding.h"
#include "gui/panel.h"

static void test_vector(void);
static void test_gui_labeled_button(void);
static void test_gui_panel_vertical_layout(void);
static void test_gui_padding(void);

void moa_test_run_all(void)
{
	test_vector();
	test_gui_labeled_button();
	test_gui_panel_vertical_layout();
	test_gui_padding();
}

#define TEST(x)   do { if (!(x)) { moa_test_on_message("TEST("   MOA_STRINGIZE(x) ") fail", __FILE__, __LINE__);         } } while (0,0) /*owl silences VC++ warning C4127*/
#define ASSERT(x) do { if (!(x)) { moa_test_on_message("ASSERT(" MOA_STRINGIZE(x) ") fail", __FILE__, __LINE__); return; } } while (0,0) /*owl silences VC++ warning C4127*/

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

static void test_gui_labeled_button(void)
{
	Vector2i position = Vector2i_new(20, 30);
	Vector2i desired_size = Vector2i_new(100, 200);
	Vector2i actual_size = Vector2i_new(80, 210);
	char const * const text = "text";
	LabeledButton button = LabeledButton_create(text, make_text_style(TextAlignment_Left, TextAlignment_Right, make_color(1, 2, 3, 4)), desired_size, make_color(5, 6, 7, 8));
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
	Widget_destroy(&button.base);
}

static void test_gui_panel_vertical_layout(void)
{
	Vector2i position = Vector2i_new(20, 30);
	Vector2i desired_size = Vector2i_new(100, 200);
	Vector2i actual_size = Vector2i_new(80, 210);
	MemoryManager memory = create_standard_memory_manager();
	Panel panel = Panel_create(desired_size, make_vertical_layout(), memory.deallocator);
	Vector2i child_size = Vector2i_new(140, 200);
	Label child = Label_create("text", make_text_style(TextAlignment_Left, TextAlignment_Right, make_color(1, 2, 3, 4)), child_size);
	Vector2i child_actual_size = Vector2i_new(80, 200);

	panel.base.absolute_position = position;
	panel.base.actual_size = actual_size;
	Widget_pack(&panel.base);
	TEST(Vector2i_equal(&panel.base.absolute_position, &position));
	TEST(Vector2i_equal(&panel.base.actual_size, &actual_size));
	TEST(Vector2i_equal(&panel.base.desired_size, &desired_size));

	ASSERT(PtrVector_push_back(&panel.children, &child, memory.allocator));
	Widget_pack(&panel.base);
	TEST(Vector2i_equal(&panel.base.absolute_position, &position));
	TEST(Vector2i_equal(&panel.base.actual_size, &actual_size));
	TEST(Vector2i_equal(&panel.base.desired_size, &desired_size));
	TEST(Vector2i_equal(&child.base.absolute_position, &position));
	TEST(Vector2i_equal(&child.base.actual_size, &child_actual_size));
	TEST(Vector2i_equal(&child.base.desired_size, &child_size));

	Widget_destroy(&panel.base);
	Widget_destroy(&child.base);
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
