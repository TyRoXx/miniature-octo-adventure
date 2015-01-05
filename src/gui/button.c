#include "button.h"
#include <assert.h>

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

static void Button_handle_input(Widget *this_, GuiInput input)
{
	Button * const button = (Button *)this_;
	Rectangle bounds;
	bounds.top_left = button->base.absolute_position;
	bounds.bottom_right = bounds.top_left;
	Vector2i_add(&bounds.bottom_right, &button->base.actual_size);
	if (!Rectangle_is_inside(&bounds, &input.click_position))
	{
		return;
	}
	button->on_click(button->user_data);
}

static WidgetClass const button_class =
{
	Button_pack,
	Button_render,
	Button_handle_input
};

Button Button_create(Widget *content, Vector2i desired_size, Color background_color, void (*on_click)(void *), void *user_data)
{
	Button button;
	Widget_init(&button.base, &button_class, desired_size);
	button.content = content;
	button.background_color = background_color;
	button.on_click = on_click;
	button.user_data = user_data;
	return button;
}
