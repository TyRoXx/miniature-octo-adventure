#ifndef VECTOR2I_H
#define VECTOR2I_H

#include "bool.h"
#include "unused.h"
#include "unreachable.h"
#include <stddef.h>

typedef enum Direction2
{
	Direction2_Right,
	Direction2_Down
}
Direction2;

typedef struct Vector2i
{
	int x, y;
}
Vector2i;

MOA_USE_RESULT
static inline Direction2 Direction2_opposite(Direction2 direction)
{
	switch (direction)
	{
	case Direction2_Down: return Direction2_Right;
	case Direction2_Right: return Direction2_Down;
	}
	MOA_UNREACHABLE();
}

MOA_USE_RESULT
static inline Vector2i Vector2i_new(int x, int y)
{
	Vector2i result;
	result.x = x;
	result.y = y;
	return result;
}

static inline void Vector2i_add(Vector2i *dest, Vector2i const *source)
{
	dest->x += source->x;
	dest->y += source->y;
}

static inline void Vector2i_sub(Vector2i *dest, Vector2i const *source)
{
	dest->x -= source->x;
	dest->y -= source->y;
}

static inline void Vector2i_scale(Vector2i *dest, int scalar)
{
	dest->x *= scalar;
	dest->y *= scalar;
}

MOA_USE_RESULT
static inline Bool Vector2i_equal(Vector2i const *first, Vector2i const *second)
{
	return
		(first->x == second->x) &&
		(first->y == second->y);
}

MOA_USE_RESULT
static inline int Vector2i_get_component(Vector2i const *from, Direction2 component)
{
	switch (component)
	{
	case Direction2_Right: return from->x;
	case Direction2_Down: return from->y;
	}
	MOA_UNREACHABLE();
}

static inline void Vector2i_add_component(Vector2i *to, Direction2 component, int difference)
{
	switch (component)
	{
	case Direction2_Right: to->x += difference; return;
	case Direction2_Down: to->y += difference; return;
	}
	MOA_UNREACHABLE();
}

static inline void Vector2i_set_component(Vector2i *to, Direction2 component, int value)
{
	switch (component)
	{
	case Direction2_Right: to->x = value; return;
	case Direction2_Down: to->y = value; return;
	}
	MOA_UNREACHABLE();
}

static inline void Vector2i_add_xy(Vector2i *to, int x, int y)
{
	to->x += x;
	to->y += y;
}

extern Vector2i const Vector2i_zero;


#endif
