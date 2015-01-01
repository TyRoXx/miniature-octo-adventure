#ifndef PADDING_H
#define PADDING_H


#include "widget.h"
#include "renderer.h"


typedef struct Padding
{
	Widget base;
	Widget *content;
	int amount;
}
Padding;

MOA_USE_RESULT
Padding Padding_create(Vector2i desired_size, Widget *content, int amount);


#endif
