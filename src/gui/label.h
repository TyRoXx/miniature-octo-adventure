#ifndef LABEL_H
#define LABEL_H


#include "widget.h"
#include "renderer.h"


typedef struct Label
{
	Widget base;
	char *text;
	TextStyle style;
}
Label;

Label Label_create(char *text, TextStyle style, Vector2i desired_size);


#endif
