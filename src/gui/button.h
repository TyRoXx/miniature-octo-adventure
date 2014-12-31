#ifndef BUTTON_H
#define BUTTON_H


#include "widget.h"


typedef struct Button
{
	Widget base;
	OwnedWidget *content;
	Color background_color;
}
Button;

Button *Button_create(OwnedWidget *content, Vector2i desired_size, Color background_color);


#endif
