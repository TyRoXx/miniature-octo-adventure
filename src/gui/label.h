#ifndef LABEL_H
#define LABEL_H


#include "widget.h"


typedef struct Label
{
	Widget base;
	char *text;
}
Label;

Label *Label_create(char *text, Vector2i desired_size);


#endif
