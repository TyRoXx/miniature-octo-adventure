#ifndef RENDERER_H
#define RENDERER_H


#include "base/vector2i.h"


typedef struct Color
{
	unsigned char r, g, b, a;
}
Color;

MOA_USE_RESULT
Color make_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

typedef struct Rectangle
{
	Vector2i top_left;
	Vector2i bottom_right;
}
Rectangle;

MOA_USE_RESULT
Vector2i Rectangle_size(Rectangle rect);

typedef enum TextAlignment
{
	TextAlignment_Left,
	TextAlignment_Right,
	TextAlignment_Center
}
TextAlignment;

typedef struct TextStyle
{
	TextAlignment horizontal;
	TextAlignment vertical;
	Color color;
}
TextStyle;

MOA_USE_RESULT
TextStyle make_text_style(TextAlignment horizontal, TextAlignment vertical, Color color);

struct RendererClass;

typedef struct Renderer
{
	struct RendererClass const *type;
}
Renderer;

typedef void const *ImageHandle;

void Renderer_rect_outline(Renderer *r, Rectangle dimensions, Color color);
void Renderer_rect_solid(Renderer *r, Rectangle dimensions, Color color);
void Renderer_text(Renderer *r, Rectangle dimensions, Vector2i offset, char const *text, TextStyle const *style);
void Renderer_image(Renderer *r, ImageHandle image, Vector2i source, Vector2i destination, Vector2i size);

typedef struct RendererClass
{
	void (*rect_outline)(Renderer *, Rectangle, Color);
	void (*rect_solid)(Renderer *, Rectangle, Color);
	void (*text)(Renderer *, Rectangle, Vector2i, char const *, TextStyle const *);
	void (*image)(Renderer *, ImageHandle, Vector2i, Vector2i, Vector2i);
}
RendererClass;


#endif
