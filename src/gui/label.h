#ifndef LABEL_H
#define LABEL_H


#include "widget.h"
#include "renderer.h"


typedef struct Label
{
	Widget base;
	char *text;
	TextAlignment alignment;
}
Label;

Label *Label_create(char *text, TextAlignment alignment, Vector2i desired_size);


#endif
