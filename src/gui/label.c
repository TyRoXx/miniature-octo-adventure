#include "label.h"

static void Label_destroy(Widget *this_)
{
	(void)this_;
}

static void Label_pack(Widget *this_)
{
	(void)this_;
}

static void Label_render(Widget *this_, Renderer *renderer)
{
	Label * const instance = (Label *)this_;
	Rectangle dimensions;
	dimensions.top_left = instance->base.absolute_position;
	dimensions.bottom_right = dimensions.top_left;
	Vector2i_add(&dimensions.bottom_right, &instance->base.actual_size);
	Renderer_text(renderer, dimensions, Vector2i_zero, instance->text, &instance->style);
}

static void Label_handle_input(Widget *this_, GuiInput input)
{
	(void)this_;
	(void)input;
}

static WidgetClass const label_class =
{
    Label_destroy,
    Label_pack,
    Label_render,
	Label_handle_input
};

Label Label_create(char const *text, TextStyle style, Vector2i desired_size)
{
	Label instance;
	Widget_init(&instance.base, &label_class, desired_size);
	instance.text = text;
	instance.style = style;
	return instance;
}
