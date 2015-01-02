#ifndef VECTOR2I_H
#define VECTOR2I_H

#include "bool.h"
#include "unused.h"
#include <stddef.h>

typedef enum Direction2
{
	Direction2_Right,
	Direction2_Down
}
Direction2;

MOA_USE_RESULT
Direction2 Direction2_opposite(Direction2 direction);

typedef struct Vector2i
{
	int x, y;
}
Vector2i;


MOA_USE_RESULT
Vector2i Vector2i_new(int x, int y);
void Vector2i_add(Vector2i *dest, Vector2i const *source);
void Vector2i_sub(Vector2i *dest, Vector2i const *source);
void Vector2i_scale(Vector2i *dest, int scalar);

MOA_USE_RESULT
Bool Vector2i_equal(Vector2i const *first, Vector2i const *second);

MOA_USE_RESULT
int Vector2i_get_component(Vector2i const *from, Direction2 component);

void Vector2i_add_component(Vector2i *to, Direction2 component, int difference);

void Vector2i_set_component(Vector2i *to, Direction2 component, int value);
void Vector2i_add_xy(Vector2i *to, int x, int y);

extern Vector2i const Vector2i_zero;


#endif
