#ifndef RENDERER_H
#define RENDERER_H


#include "base/rectangle.h"


typedef struct Color
{
	unsigned char r, g, b, a;
}
Color;

MOA_USE_RESULT
MOA_INLINE Color make_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	Color result;
	result.r = r;
	result.g = g;
	result.b = b;
	result.a = a;
	return result;
}

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
MOA_INLINE TextStyle make_text_style(TextAlignment horizontal, TextAlignment vertical, Color color)
{
	TextStyle style;
	style.horizontal = horizontal;
	style.vertical = vertical;
	style.color = color;
	return style;
}

struct RendererClass;

typedef struct Renderer
{
	struct RendererClass const *type;
}
Renderer;

typedef void const *ImageHandle;

void Renderer_rect_outline(Renderer *r, moa_Rectangle dimensions, Color color);
void Renderer_rect_solid(Renderer *r, moa_Rectangle dimensions, Color color);
void Renderer_text(Renderer *r, moa_Rectangle dimensions, Vector2i offset, char const *text, TextStyle const *style);
void Renderer_image(Renderer *r, ImageHandle image, Vector2i source, Vector2i destination, Vector2i size);

typedef struct RendererClass
{
	void (*rect_outline)(Renderer *, moa_Rectangle, Color);
	void (*rect_solid)(Renderer *, moa_Rectangle, Color);
	void (*text)(Renderer *, moa_Rectangle, Vector2i, char const *, TextStyle const *);
	void (*image)(Renderer *, ImageHandle, Vector2i, Vector2i, Vector2i);
}
RendererClass;


#endif
