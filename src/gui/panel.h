#ifndef PANEL_H
#define PANEL_H


#include "widget.h"
#include "base/ptr_vector.h"


struct Panel;

typedef void (*Layout)(struct Panel *);

typedef struct Panel
{
	Widget base;
	Layout layout;
	PtrVector children;
	Deallocator children_deallocator;
}
Panel;

MOA_USE_RESULT
Panel Panel_create(Vector2i desired_size, Layout layout, Deallocator children_deallocator);

MOA_USE_RESULT
void pack_vertically(Panel *panel);

MOA_USE_RESULT
void pack_horizontally(Panel *panel);

MOA_USE_RESULT
void pack_absolutely(Panel *panel);


#endif
