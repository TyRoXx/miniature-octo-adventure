#ifndef SDL_FRONTEND_H
#define SDL_FRONTEND_H


#include "data.h"


struct Game;
struct GameStateView;


typedef struct SDLSettings
{
	Vector2i resolution;
	Bool fullscreen;
}
SDLSettings;

typedef struct SDLFrontend
{
	struct Game *game;
	SDL_Surface *screen;
#if SDL_MAJOR_VERSION >= 2
	SDL_Texture *texture;
	SDL_Renderer *renderer;
	SDL_Window *window;
#endif
	Data data;
	struct GameStateView *state_view;
}
SDLFrontend;


void SDLFrontend_destroy(SDLFrontend *sdl_front);
Bool SDLFrontend_main_loop(SDLFrontend *sdl_front);

MOA_USE_RESULT
Bool SDLFrontEnd_create(SDLFrontend *front, struct Game *game, SDLSettings settings);

#endif
