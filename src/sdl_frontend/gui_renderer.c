#include "gui_renderer.h"
#include "base/min_max.h"
#include "base/unreachable.h"
#include <assert.h>

static SDL_Rect to_SDL_rect(moa_Rectangle rect)
{
	SDL_Rect result;
	result.x = (Sint16)rect.top_left.x;
	result.y = (Sint16)rect.top_left.y;
	result.w = (Uint16)(rect.bottom_right.x - rect.top_left.x);
	result.h = (Uint16)(rect.bottom_right.y - rect.top_left.y);
	return result;
}

static void rect_outline(Renderer *r, moa_Rectangle dimensions, Color color)
{
	SDL_GUI_Renderer * const renderer = (SDL_GUI_Renderer *)r;
	(void)renderer;
	(void)dimensions;
	(void)color;
}

static void rect_solid(Renderer *r, moa_Rectangle dimensions, Color color)
{
	SDL_GUI_Renderer * const renderer = (SDL_GUI_Renderer *)r;
	SDL_Rect rect = to_SDL_rect(dimensions);
	SDL_FillRect(renderer->screen, &rect, SDL_MapRGBA(renderer->screen->format, color.r, color.g, color.b, color.a));
}

typedef struct TextPositioning
{
	moa_Rectangle source;
	moa_Rectangle destination;
}
TextPositioning;

static moa_Rectangle from_SDL_rect(SDL_Rect rect)
{
	moa_Rectangle result;
	Vector2i const size = Vector2i_new(rect.w, rect.h);
	result.top_left = Vector2i_new(rect.x, rect.y);
	result.bottom_right = result.top_left;
	Vector2i_add(&result.bottom_right, &size);
	return result;
}

static void center_1D(int *src_begin, int *src_end, int *dest_begin, int *dest_end)
{
	int const src = (*src_end - *src_begin);
	int const dest = (*dest_end - *dest_begin);
	int const margin = (dest - src) / 2;
	if (src < dest)
	{
		*dest_begin += margin;
		*dest_end -= margin;
	}
	else
	{
		*src_begin -= margin;
		*src_end += margin;
	}
}

static void align_left(int *src_begin, int *src_end, int *dest_begin, int *dest_end)
{
	int const src = (*src_end - *src_begin);
	int const dest = (*dest_end - *dest_begin);
	int const minimum = min_int(src, dest);
	*src_end  = (*src_begin  + min_int(*src_end, minimum));
	*dest_end = (*dest_begin + min_int(*dest_end, minimum));
}

static void align_right(int *src_begin, int *src_end, int *dest_begin, int *dest_end)
{
	int const src = (*src_end - *src_begin);
	int const dest = (*dest_end - *dest_begin);
	int const minimum = min_int(src, dest);
	*src_begin  = (*src_end  - min_int(*src_end, minimum));
	*dest_begin = (*dest_end - min_int(*dest_end, minimum));
}

static SDL_Color to_SDL_color(Color color)
{
	SDL_Color result;
	result.r = color.r;
	result.g = color.g;
	result.b = color.b;
#if SDL_MAJOR_VERSION >= 2
	result.a = color.a;
#else
	result.unused = color.a;
#endif
	return result;
}

typedef void (*alignment_function)(int *, int *, int *, int *);

static alignment_function get_alignment_function(TextAlignment alignment)
{
	switch (alignment)
	{
	case TextAlignment_Center: return center_1D;
	case TextAlignment_Left:   return align_left;
	case TextAlignment_Right:  return align_right;
	}
	MOA_UNREACHABLE();
}

static void text(Renderer *r, moa_Rectangle dimensions, Vector2i offset, char const *text, TextStyle const *style)
{
	SDL_GUI_Renderer * const renderer = (SDL_GUI_Renderer *)r;
	SDL_Surface *rendered_text;
	SDL_Color text_color = to_SDL_color(style->color);
	SDL_Rect text_src;
	TextPositioning positioning;
	SDL_Rect destination;
	TTF_Font * const font = renderer->default_font;
	(void)offset; /*TODO*/

	rendered_text = TTF_RenderUTF8_Blended(font, text, text_color);
	assert(rendered_text);

	SDL_GetClipRect(rendered_text, &text_src);

	positioning.destination = dimensions;
	positioning.source = from_SDL_rect(text_src);

	get_alignment_function(style->horizontal)(&positioning.source.top_left.x, &positioning.source.bottom_right.x, &positioning.destination.top_left.x, &positioning.destination.bottom_right.x);
	get_alignment_function(style->vertical)(&positioning.source.top_left.y, &positioning.source.bottom_right.y, &positioning.destination.top_left.y, &positioning.destination.bottom_right.y);

	text_src = to_SDL_rect(positioning.source);
	destination = to_SDL_rect(positioning.destination);

	SDL_BlitSurface(rendered_text, &text_src, renderer->screen, &destination);
	SDL_FreeSurface(rendered_text);
}

static void image(Renderer *r, ImageHandle image, Vector2i source, Vector2i destination, Vector2i size)
{
	(void)r;
	(void)image;
	(void)source;
	(void)destination;
	(void)size;
}

static RendererClass const renderer_class =
{
    rect_outline,
    rect_solid,
    text,
    image
};

void SDL_GUI_Renderer_init(SDL_GUI_Renderer *r, SDL_Surface *screen, TTF_Font *default_font)
{
	r->base.type = &renderer_class;
	r->screen = screen;
	r->default_font = default_font;
}
