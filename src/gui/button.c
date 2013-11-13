#include "button.h"
#include <stdlib.h>


static void Button_destroy(Widget *this_)
{
	Button * const button = (Button *)this_;
	Widget_destroy(button->content);
	free(button);
}

static void Button_pack(Widget *this_)
{
	Button * const button = (Button *)this_;

	/* TODO: subtract a border */
	button->content->absolute_position = button->base.absolute_position;
	button->content->actual_size = button->base.actual_size;

	Widget_pack(button->content);
}

static void Button_render(Widget *this_, Renderer *renderer)
{
	Button * const button = (Button *)this_;
	Rectangle dimensions;
	Color const background = {127, 127, 255, 220};
	dimensions.top_left = button->base.absolute_position;
	dimensions.bottom_right = dimensions.top_left;
	Vector2i_add(&dimensions.bottom_right, &button->base.actual_size);
	Renderer_rect_solid(renderer, dimensions, background);
	Widget_render(button->content, renderer);
}

static WidgetClass const button_class =
{
	Button_destroy,
	Button_pack,
	Button_render
};

static void Button_init(Button *b, OwnedWidget *content, Vector2i desired_size)
{
	Widget_init(&b->base, &button_class, desired_size);
	b->content = content;
}

Button *Button_create(OwnedWidget *content, Vector2i desired_size)
{
	Button * const button = malloc(sizeof(*button));
	if (button)
	{
		Button_init(button, content, desired_size);
	}
	return button;
}
