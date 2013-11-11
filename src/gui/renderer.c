#include "renderer.h"


Vector2i Rectangle_size(Rectangle rect)
{
	return Vector2i_new(rect.bottom_right.x - rect.top_left.x,
	                    rect.bottom_right.y - rect.top_left.y);
}

void Renderer_rect_outline(Renderer *r, Rectangle dimensions, Color color)
{
	r->type->rect_outline(r, dimensions, color);
}

void Renderer_rect_solid(Renderer *r, Rectangle dimensions, Color color)
{
	r->type->rect_solid(r, dimensions, color);
}

void Renderer_text(Renderer *r, Rectangle dimensions, Vector2i offset, char const *text, TextAlignment alignment, Color color)
{
	r->type->text(r, dimensions, offset, text, alignment, color);
}
