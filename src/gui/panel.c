#include "panel.h"
#include "base/min_max.h"

static void Panel_pack(Widget *this_)
{
	Panel * const panel = (Panel *)this_;
	panel->layout(panel);
}

static void Panel_render(Widget *this_, Renderer *renderer)
{
	Panel * const panel = (Panel *)this_;
	for (Widget **i = panel->children.begin; i != panel->children.end; ++i)
	{
		Widget_render(*i, renderer);
	}
}

static void Panel_handle_input(Widget *this_, GuiInput input)
{
	Panel * const panel = (Panel *)this_;
	for (Widget **i = panel->children.begin; i != panel->children.end; ++i)
	{
		Widget_handle_input(*i, input);
	}
}

static WidgetClass const panel_class =
{
    Panel_pack,
	Panel_render,
	Panel_handle_input
};

Panel Panel_create(Vector2i desired_size, Layout layout)
{
	Panel panel;
	Widget_init(&panel.base, &panel_class, desired_size);
	panel.layout = layout;
	panel.children.begin = panel.children.end = NULL;
	return panel;
}

static void generic_pack(Panel *panel, Direction2 direction)
{
	Direction2 const opposite_direction = Direction2_opposite(direction);
	Vector2i const panel_size = panel->base.actual_size;
	int desired_dimension = 0;
	Vector2i current_child_pos = panel->base.absolute_position;
	for (Widget **i = panel->children.begin; i != panel->children.end; ++i)
	{
		desired_dimension += Vector2i_get_component(&(*i)->desired_size, direction);
	}
	for (Widget **i = panel->children.begin; i != panel->children.end; ++i)
	{
		Widget * const child = *i;
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
	for (Widget **i = panel->children.begin; i != panel->children.end; ++i)
	{
		Widget * const child = *i;
		child->actual_size = child->desired_size;
		Widget_pack(child);
	}
}
