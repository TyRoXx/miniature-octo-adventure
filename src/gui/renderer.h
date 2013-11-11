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

Vector2i Rectangle_size(Rectangle rect);

typedef enum TextAlignment
{
	TextAlignment_Left,
	TextAlignment_Right,
	TextAlignment_Center
}
TextAlignment;

struct RendererClass;

typedef struct Renderer
{
	struct RendererClass const *type;
}
Renderer;

void Renderer_rect_outline(Renderer *r, Rectangle dimensions, Color color);
void Renderer_rect_solid(Renderer *r, Rectangle dimensions, Color color);
void Renderer_text(Renderer *r, Rectangle dimensions, Vector2i offset, char const *text, TextAlignment alignment, Color color);

typedef struct RendererClass
{
	void (*rect_outline)(Renderer *, Rectangle, Color);
	void (*rect_solid)(Renderer *, Rectangle, Color);
	void (*text)(Renderer *, Rectangle, Vector2i, char const *, TextAlignment, Color);
}
RendererClass;


#endif
