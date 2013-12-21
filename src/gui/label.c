#include "label.h"
#include <stdlib.h>

static void Label_destroy(Widget *this_)
{
	Label * const instance = (Label *)this_;
	free(instance->text);
	free(instance);
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

static WidgetClass const label_class =
{
    Label_destroy,
    Label_pack,
    Label_render
};

Label *Label_create(char *text, TextStyle style, Vector2i desired_size)
{
	Label * const instance = malloc(sizeof(*instance));
	if (instance)
	{
		Widget_init(&instance->base, &label_class, desired_size);
		instance->text = text;
		instance->style = style;
	}
	return instance;
}