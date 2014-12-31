#include "panel.h"
#include "base/minmax.h"
#include <stdlib.h>


static void panel_child_deleter(void *child, void *user)
{
	(void)user;
	Widget_destroy(child);
}

static void Panel_destroy(Widget *this_)
{
	Panel * const panel = (Panel *)this_;
	PtrVector_for_each(&panel->children, panel_child_deleter, NULL);
	PtrVector_free(&panel->children);
	free(panel);
}

static void Panel_pack(Widget *this_)
{
	Panel * const panel = (Panel *)this_;
	panel->layout.pack_children(panel);
}

static void render_child(void *element, void *user)
{
	Widget * const child = element;
	Renderer * const renderer = user;
	Widget_render(child, renderer);
}

static void Panel_render(Widget *this_, Renderer *renderer)
{
	Panel * const panel = (Panel *)this_;
	PtrVector_for_each(&panel->children, render_child, renderer);
}

static WidgetClass const panel_class =
{
	Panel_destroy,
    Panel_pack,
	Panel_render
};

static void Panel_init(Panel *p, Vector2i desired_size, Layout layout)
{
	Widget_init(&p->base, &panel_class, desired_size);
	PtrVector_init(&p->children);
	p->layout = layout;
}

Panel *Panel_create(Vector2i desired_size, Layout layout)
{
	Panel * const panel = malloc(sizeof(*panel));
	if (panel)
	{
		Panel_init(panel, desired_size, layout);
	}
	return panel;
}

static void generic_pack(Panel *panel, Direction2 direction)
{
	Direction2 const opposite_direction = Direction2_opposite(direction);
	Vector2i const panel_size = panel->base.actual_size;
	int desired_dimension = 0;
	size_t i, c;
	Vector2i current_child_pos = panel->base.absolute_position;
	for (i = 0, c = PtrVector_size(&panel->children); i < c; ++i)
	{
		Widget const * const child = PtrVector_get(&panel->children, i);
		desired_dimension += Vector2i_get_component(&child->desired_size, direction);
	}
	for (i = 0; i < c; ++i)
	{
		Widget * const child = PtrVector_get(&panel->children, i);
		int const child_desired = Vector2i_get_component(&child->desired_size, direction);
		int const child_actual = (desired_dimension > Vector2i_get_component(&panel_size, direction)) ? (child_desired * Vector2i_get_component(&panel_size, direction) / desired_dimension) : child_desired;
		child->absolute_position = current_child_pos;
		Vector2i_add_component(&current_child_pos, direction, child_actual);
		Vector2i_set_component(&child->actual_size, direction, child_actual);
		Vector2i_set_component(
			&child->actual_size,
			opposite_direction,
			min_int(Vector2i_get_component(&panel_size, opposite_direction), Vector2i_get_component(&child->desired_size, opposite_direction))
		);
		Widget_pack(child);
	}
}

static void pack_vertically(Panel *panel)
{
	generic_pack(panel, Direction2_Down);
}

static void pack_horizontally(Panel *panel)
{
	generic_pack(panel, Direction2_Right);
}

Layout make_vertical_layout(void)
{
	Layout layout;
	layout.pack_children = pack_vertically;
	layout.user = NULL;
	return layout;
}

Layout make_horizontal_layout(void)
{
	Layout layout;
	layout.pack_children = pack_horizontally;
	layout.user = NULL;
	return layout;
}

static void pack_absolute(Panel *panel)
{
	size_t i, c;
	for (i = 0, c = PtrVector_size(&panel->children); i < c; ++i)
	{
		Widget * const child = PtrVector_get(&panel->children, i);
		child->actual_size = child->desired_size;
		Widget_pack(child);
	}
}

Layout make_absolute_layout(void)
{
	Layout layout;
	layout.pack_children = pack_absolute;
	layout.user = NULL;
	return layout;
}
