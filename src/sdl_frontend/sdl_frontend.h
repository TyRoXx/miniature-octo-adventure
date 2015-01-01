#ifndef SDL_FRONTEND_H
#define SDL_FRONTEND_H


#include "frontend/frontend.h"
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
	Frontend base;
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


MOA_USE_RESULT
Frontend *SDLFrontEnd_create(struct Game *game, SDLSettings settings);


#endif
