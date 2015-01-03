#include "padding.h"
#include "base/min_max.h"

static void Padding_destroy(Widget *this_)
{
	(void)this_;
}

static void Padding_pack(Widget *this_)
{
	Padding * const instance = (Padding *)this_;
	Vector2i const desired_content_size = instance->content->desired_size;
	instance->content->absolute_position = this_->absolute_position;
	Vector2i_add_xy(&instance->content->absolute_position, instance->amount, instance->amount);
	instance->content->actual_size.x = max_int(min_int(desired_content_size.x, instance->base.actual_size.x) - (2 * instance->amount), 0);
	instance->content->actual_size.y = max_int(min_int(desired_content_size.y, instance->base.actual_size.y) - (2 * instance->amount), 0);
	Widget_pack(instance->content);
}

static void Padding_render(Widget *this_, Renderer *renderer)
{
	Padding * const instance = (Padding *)this_;
	Widget_render(instance->content, renderer);
}

static void Padding_handle_input(Widget *this_, GuiInput input)
{
	Padding * const instance = (Padding *)this_;
	Widget_handle_input(instance->content, input);
}

static WidgetClass const padding_class =
{
	Padding_destroy,
	Padding_pack,
	Padding_render,
	Padding_handle_input
};

Padding Padding_create(Vector2i desired_size, Widget *content, int amount)
{
	Padding instance;
	Widget_init(&instance.base, &padding_class, desired_size);
	instance.content = content;
	instance.amount = amount;
	return instance;
}
