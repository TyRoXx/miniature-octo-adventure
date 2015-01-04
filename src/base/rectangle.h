#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "base/vector2i.h"

typedef struct Rectangle
{
	Vector2i top_left;
	Vector2i bottom_right;
}
Rectangle;

static inline Bool Rectangle_is_inside(Rectangle const *area, Vector2i const *point)
{
	return
		(point->x >= area->top_left.x) &&
		(point->y >= area->top_left.y) &&
		(point->x < area->bottom_right.x) &&
		(point->y < area->bottom_right.y);
}

MOA_USE_RESULT
static inline Vector2i Rectangle_size(Rectangle rect)
{
	return Vector2i_new(rect.bottom_right.x - rect.top_left.x,
	                    rect.bottom_right.y - rect.top_left.y);
}

#endif
