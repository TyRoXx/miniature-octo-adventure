#ifndef WIDGET_H
#define WIDGET_H


#include "renderer.h"


struct WidgetClass;

typedef struct Widget
{
	struct WidgetClass const *type;
	Vector2i desired_size;
	Vector2i absolute_position;
	Vector2i actual_size;
}
Widget;

void Widget_init(Widget *w, struct WidgetClass const *type, Vector2i desired_size);
void Widget_pack(Widget *w);
void Widget_render(Widget *w, Renderer *renderer);
void Widget_destroy(Widget *w);

typedef struct WidgetClass
{
	void (*destroy)(Widget *);
	void (*pack)(Widget *);
	void (*render)(Widget *, Renderer *);
}
WidgetClass;

typedef Widget OwnedWidget;


#endif
