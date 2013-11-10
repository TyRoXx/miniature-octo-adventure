#include "button.h"
#include <stdlib.h>


static void Button_destroy(Widget *this_)
{
	Button * const button = (Button *)this_;
	free(button->label);
	free(button);
}

static void Button_pack(Widget *this_)
{
	(void)this_;
}

static void Button_render(Widget *this_, Renderer *renderer)
{
	Button * const button = (Button *)this_;
	Rectangle dimensions;
	Color const background = {127, 127, 255, 220};
	Color const text_color = {255, 255, 255, 255};
	dimensions.top_left = button->base.absolute_position;
	dimensions.bottom_right = dimensions.top_left;
	Vector2i_add(&dimensions.bottom_right, &button->base.actual_size);
	Renderer_rect_solid(renderer, dimensions, background);
	Renderer_text(renderer, dimensions, Vector2i_zero, button->label, text_color);
}

static WidgetClass const button_class =
{
	Button_destroy,
    Button_pack,
    Button_render
};

static void Button_init(Button *b, char *label, Vector2i desired_size)
{
	Widget_init(&b->base, &button_class, desired_size);
	b->label = label;
}

Button *Button_create(char *label, Vector2i desired_size)
{
	Button * const button = malloc(sizeof(*button));
	if (button)
	{
		Button_init(button, label, desired_size);
	}
	return button;
}
