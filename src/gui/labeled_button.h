#ifndef LABELED_BUTTON_H
#define LABELED_BUTTON_H


#include "button.h"
#include "label.h"


typedef struct LabeledButton
{
	Widget base;
	Button button;
	Label label;
}
LabeledButton;

MOA_USE_RESULT
LabeledButton LabeledButton_create(char const *text, TextStyle style, Vector2i desired_size, Color background_color);


#endif
