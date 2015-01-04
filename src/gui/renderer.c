#include "renderer.h"


Color make_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	Color result;
	result.r = r;
	result.g = g;
	result.b = b;
	result.a = a;
	return result;
}

TextStyle make_text_style(TextAlignment horizontal, TextAlignment vertical, Color color)
{
	TextStyle style;
	style.horizontal = horizontal;
	style.vertical = vertical;
	style.color = color;
	return style;
}

void Renderer_rect_outline(Renderer *r, Rectangle dimensions, Color color)
{
	r->type->rect_outline(r, dimensions, color);
}

void Renderer_rect_solid(Renderer *r, Rectangle dimensions, Color color)
{
	r->type->rect_solid(r, dimensions, color);
}

void Renderer_text(Renderer *r, Rectangle dimensions, Vector2i offset, char const *text, TextStyle const *style)
{
	r->type->text(r, dimensions, offset, text, style);
}

void Renderer_image(Renderer *r, ImageHandle image, Vector2i source, Vector2i destination, Vector2i size)
{
	r->type->image(r, image, source, destination, size);
}
