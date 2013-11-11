#ifndef BUTTON_H
#define BUTTON_H


#include "widget.h"


typedef struct Button
{
	Widget base;
	Widget *content;
}
Button;

Button *Button_create(Widget *content, Vector2i desired_size);


#endif
