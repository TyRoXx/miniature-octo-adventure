#ifndef VECTOR2F_H
#define VECTOR2F_H


#include "unused.h"

typedef struct Vector2f
{
	float x, y;
}
Vector2f;


void Vector2f_add(Vector2f *dest, Vector2f const *source);
void Vector2f_sub(Vector2f *dest, Vector2f const *source);
void Vector2f_scale(Vector2f *dest, float scalar);

MOA_USE_RESULT
float Vector2f_length_sq(Vector2f const *v);

#ifndef ARM9
float Vector2f_length(Vector2f const *v);
#endif

Vector2f const Vector2f_zero;


#endif
