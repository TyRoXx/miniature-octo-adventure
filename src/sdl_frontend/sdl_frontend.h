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
	Data data;
	struct GameStateView *state_view;
}
SDLFrontend;


Frontend *SDLFrontEnd_create(struct Game *game, SDLSettings settings);


#endif
