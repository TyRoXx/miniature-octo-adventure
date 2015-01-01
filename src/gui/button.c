#include "button.h"
#include <stdlib.h>
#include <assert.h>

static void Button_destroy(Widget *this_)
{
	(void)this_;
}

static void Button_pack(Widget *this_)
{
	Button * const button = (Button *)this_;
	assert(button->content);

	/* TODO: subtract a border */
	button->content->absolute_position = button->base.absolute_position;
	button->content->actual_size = button->base.actual_size;

	Widget_pack(button->content);
}

static void Button_render(Widget *this_, Renderer *renderer)
{
	Button * const button = (Button *)this_;
	Rectangle dimensions;
	assert(button->content);

	dimensions.top_left = button->base.absolute_position;
	dimensions.bottom_right = dimensions.top_left;
	Vector2i_add(&dimensions.bottom_right, &button->base.actual_size);
	Renderer_rect_solid(renderer, dimensions, button->background_color);
	Widget_render(button->content, renderer);
}

static WidgetClass const button_class =
{
	Button_destroy,
	Button_pack,
	Button_render
};

Button Button_create(Widget *content, Vector2i desired_size, Color background_color)
{
	Button button;
	Widget_init(&button.base, &button_class, desired_size);
	button.content = content;
	button.background_color = background_color;
	return button;
}
