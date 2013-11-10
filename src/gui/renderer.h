#ifndef RENDERER_H
#define RENDERER_H


#include "base/vector2i.h"


typedef struct Color
{
	unsigned char r, g, b, a;
}
Color;

typedef struct Rectangle
{
	Vector2i top_left;
	Vector2i bottom_right;
}
Rectangle;

struct RendererClass;

typedef struct Renderer
{
	struct RendererClass const *type;
}
Renderer;

void Renderer_rect_outline(Renderer *r, Rectangle dimensions, Color color);
void Renderer_rect_solid(Renderer *r, Rectangle dimensions, Color color);
void Renderer_text(Renderer *r, Rectangle dimensions, Vector2i offset, char const *text, Color color);

typedef struct RendererClass
{
	void (*rect_outline)(Renderer *, Rectangle, Color);
	void (*rect_solid)(Renderer *, Rectangle, Color);
	void (*text)(Renderer *, Rectangle, Vector2i, char const *, Color);
}
RendererClass;


#endif
