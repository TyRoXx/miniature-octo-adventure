#ifndef PANEL_H
#define PANEL_H


#include "widget.h"
#include "base/ptr_vector.h"


struct Panel;

typedef struct Layout
{
	void (*pack_children)(struct Panel *);
	void *user;
}
Layout;

typedef struct Panel
{
	Widget base;
	Layout layout;
	PtrVector children;
	Deallocator children_deallocator;
}
Panel;

Panel Panel_create(Vector2i desired_size, Layout layout, Deallocator children_deallocator);


Layout make_vertical_layout(void);
Layout make_horizontal_layout(void);
Layout make_absolute_layout(void);


#endif
