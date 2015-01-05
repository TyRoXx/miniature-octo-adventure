#include "widget.h"


void Widget_init(Widget *w, WidgetClass const *type, Vector2i desired_size)
{
	w->type = type;
	w->desired_size = desired_size;
	w->absolute_position = Vector2i_zero;
	w->actual_size = Vector2i_zero;
}

void Widget_pack(Widget *w)
{
	w->type->pack(w);
}

void Widget_render(Widget *w, Renderer *renderer)
{
	w->type->render(w, renderer);
}

void Widget_handle_input(Widget *w, GuiInput input)
{
	w->type->handle_input(w, input);
}
