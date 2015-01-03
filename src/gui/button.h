#ifndef BUTTON_H
#define BUTTON_H


#include "widget.h"


typedef struct Button
{
	Widget base;
	Widget *content;
	Color background_color;
	void (*on_click)(void *);
	void *user_data;
}
Button;

MOA_USE_RESULT
Button Button_create(Widget *content, Vector2i desired_size, Color background_color, void (*on_click)(void *), void *user_data);


#endif
