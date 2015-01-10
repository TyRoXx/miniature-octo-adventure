#include "renderer.h"

void Renderer_rect_outline(Renderer *r, moa_Rectangle dimensions, Color color)
{
	r->type->rect_outline(r, dimensions, color);
}

void Renderer_rect_solid(Renderer *r, moa_Rectangle dimensions, Color color)
{
	r->type->rect_solid(r, dimensions, color);
}

void Renderer_text(Renderer *r, moa_Rectangle dimensions, Vector2i offset, char const *text, TextStyle const *style)
{
	r->type->text(r, dimensions, offset, text, style);
}

void Renderer_image(Renderer *r, ImageHandle image, Vector2i source, Vector2i destination, Vector2i size)
{
	r->type->image(r, image, source, destination, size);
}
