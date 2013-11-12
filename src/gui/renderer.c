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

Vector2i Rectangle_size(Rectangle rect)
{
	return Vector2i_new(rect.bottom_right.x - rect.top_left.x,
	                    rect.bottom_right.y - rect.top_left.y);
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
