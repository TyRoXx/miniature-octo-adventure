#include "labeled_button.h"

static void LabeledButton_pack(Widget *this_)
{
	LabeledButton * const instance = (LabeledButton *)this_;
	instance->button.content = &instance->label.base;
	instance->button.base.absolute_position = instance->base.absolute_position;
	instance->button.base.actual_size = instance->base.actual_size;
	Widget_pack(&instance->button.base);
}

static void LabeledButton_render(Widget *this_, Renderer *renderer)
{
	LabeledButton * const instance = (LabeledButton *)this_;
	Widget_render(&instance->button.base, renderer);
}

static void LabeledButton_handle_input(Widget *this_, GuiInput input)
{
	LabeledButton * const instance = (LabeledButton *)this_;
	Widget_handle_input(&instance->button.base, input);
}

static WidgetClass const labeled_button_class =
{
	LabeledButton_pack,
	LabeledButton_render,
	LabeledButton_handle_input
};

LabeledButton LabeledButton_create(char const *text, TextStyle style, Vector2i desired_size, Color background_color, void (*on_click)(void *), void *user_data)
{
	LabeledButton button;
	Widget_init(&button.base, &labeled_button_class, desired_size);
	button.label = Label_create(text, style, desired_size);
	button.button = Button_create(NULL, desired_size, background_color, on_click, user_data);
	return button;
}
