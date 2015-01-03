#ifndef GAME_STATE_VIEW_H
#define GAME_STATE_VIEW_H


#include "base/bool.h"
#include "base/unused.h"
#include "SDL.h"


struct GameState;
struct GameStateView;
struct SDLFrontend;


typedef struct GameStateViewType
{
	MOA_USE_RESULT struct GameStateView *(*create)(struct GameState *, struct SDLFrontend *front);
	void (*destroy)(struct GameStateView *);
	MOA_USE_RESULT Bool (*update)(struct GameStateView *);
	MOA_USE_RESULT Bool (*draw)(struct GameStateView *);
	void (*handle_event)(struct GameStateView *, SDL_Event const *);
}
GameStateViewType;


typedef struct GameStateView
{
	GameStateViewType const *type;
}
GameStateView;


#endif
