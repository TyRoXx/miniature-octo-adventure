#ifndef LABEL_H
#define LABEL_H


#include "widget.h"
#include "renderer.h"


typedef struct Label
{
	Widget base;
	char const *text;
	TextStyle style;
}
Label;

MOA_USE_RESULT
Label Label_create(char const *text, TextStyle style, Vector2i desired_size);


#endif
