#include "panel.h"
#include "base/min_max.h"

static void Panel_destroy(Widget *this_)
{
	Panel * const panel = (Panel *)this_;
	PtrVector_free(&panel->children, panel->children_deallocator);
}

static void Panel_pack(Widget *this_)
{
	Panel * const panel = (Panel *)this_;
	panel->layout(panel);
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

Panel Panel_create(Vector2i desired_size, Layout layout, Deallocator children_deallocator)
{
	Panel panel;
	Widget_init(&panel.base, &panel_class, desired_size);
	PtrVector_init(&panel.children);
	panel.layout = layout;
	panel.children_deallocator = children_deallocator;
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

void pack_vertically(Panel *panel)
{
	generic_pack(panel, Direction2_Down);
}

void pack_horizontally(Panel *panel)
{
	generic_pack(panel, Direction2_Right);
}

void pack_absolutely(Panel *panel)
{
	size_t i, c;
	for (i = 0, c = PtrVector_size(&panel->children); i < c; ++i)
	{
		Widget * const child = PtrVector_get(&panel->children, i);
		child->actual_size = child->desired_size;
		Widget_pack(child);
	}
}
