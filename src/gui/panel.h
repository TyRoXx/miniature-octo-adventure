#ifndef PANEL_H
#define PANEL_H

#include "widget.h"

struct Panel;

typedef void (*Layout)(struct Panel *);

typedef struct WidgetPtrRange
{
	Widget **begin, **end;
}
WidgetPtrRange;

MOA_USE_RESULT
MOA_INLINE WidgetPtrRange WidgetPtrRange_new(Widget **begin, Widget **end)
{
	WidgetPtrRange result = {begin, end};
	return result;
}

typedef struct Panel
{
	Widget base;
	Layout layout;
	WidgetPtrRange children;
}
Panel;

MOA_USE_RESULT
Panel Panel_create(Vector2i desired_size, Layout layout);

void pack_vertically(Panel *panel);
void pack_horizontally(Panel *panel);
void pack_absolutely(Panel *panel);

#endif
