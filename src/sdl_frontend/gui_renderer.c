#include "gui_renderer.h"
#include <assert.h>


static SDL_Rect to_SDL_rect(Rectangle rect)
{
	SDL_Rect result;
	result.x = (Sint16)rect.top_left.x;
	result.y = (Sint16)rect.top_left.y;
	result.w = (Uint16)(rect.bottom_right.x - rect.top_left.x);
	result.h = (Uint16)(rect.bottom_right.y - rect.top_left.y);
	return result;
}

static void rect_outline(Renderer *r, Rectangle dimensions, Color color)
{
	SDL_GUI_Renderer * const renderer = (SDL_GUI_Renderer *)r;
	(void)renderer;
	(void)dimensions;
	(void)color;

}

static void rect_solid(Renderer *r, Rectangle dimensions, Color color)
{
	SDL_GUI_Renderer * const renderer = (SDL_GUI_Renderer *)r;
	SDL_Rect rect = to_SDL_rect(dimensions);
	SDL_FillRect(renderer->screen, &rect, SDL_MapRGB(renderer->screen->format, color.r, color.g, color.b));
}

static void text(Renderer *r, Rectangle dimensions, Vector2i offset, char const *text, Color color)
{
	SDL_GUI_Renderer * const renderer = (SDL_GUI_Renderer *)r;
	SDL_Surface *rendered_text;
	SDL_Color text_color;
	SDL_Rect text_size;
	SDL_Rect destination = to_SDL_rect(dimensions);

	(void)offset; /*TODO*/

	text_color.r = color.r;
	text_color.g = color.g;
	text_color.b = color.b;

	rendered_text = TTF_RenderUTF8_Blended(renderer->default_font, text, text_color);
	assert(rendered_text);
	SDL_GetClipRect(rendered_text, &text_size);
	SDL_BlitSurface(rendered_text, &text_size, renderer->screen, &destination);
	SDL_FreeSurface(rendered_text);
}

static RendererClass const renderer_class =
{
    rect_outline,
    rect_solid,
    text
};

void SDL_GUI_Renderer_init(SDL_GUI_Renderer *r, SDL_Surface *screen, TTF_Font *default_font)
{
	r->base.type = &renderer_class;
	r->screen = screen;
	r->default_font = default_font;
}
