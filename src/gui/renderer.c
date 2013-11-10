#include "renderer.h"


void Renderer_rect_outline(Renderer *r, Rectangle dimensions, Color color)
{
	r->type->rect_outline(r, dimensions, color);
}

void Renderer_rect_solid(Renderer *r, Rectangle dimensions, Color color)
{
	r->type->rect_solid(r, dimensions, color);
}

void Renderer_text(Renderer *r, Rectangle dimensions, Vector2i offset, char const *text, Color color)
{
	r->type->text(r, dimensions, offset, text, color);
}
