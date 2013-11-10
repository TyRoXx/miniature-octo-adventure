#ifndef BUTTON_H
#define BUTTON_H


#include "widget.h"


typedef struct Button
{
	Widget base;
	char *label;
}
Button;

Button *Button_create(char *label, Vector2i desired_size);


#endif
