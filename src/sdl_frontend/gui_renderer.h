#ifndef SDL_GUI_RENDERER_H
#define SDL_GUI_RENDERER_H


#include "gui/renderer.h"
#include <SDL.h>
#include <SDL_ttf.h>


typedef struct SDL_GUI_Renderer
{
	Renderer base;
	SDL_Surface *screen;
	TTF_Font *default_font;
}
SDL_GUI_Renderer;

void SDL_GUI_Renderer_init(SDL_GUI_Renderer *r, SDL_Surface *screen, TTF_Font *default_font);


#endif
