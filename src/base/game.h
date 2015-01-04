#ifndef GAME_H
#define GAME_H


#include "world.h"
#include "game_state.h"


typedef struct GameStateEnterCallback
{
	void (*function)(void *, GameState *);
	void *user_data;
}
GameStateEnterCallback;


typedef struct Game
{
	GameState *state;
	GameStateEnterCallback on_enter_state;
	MemoryManager memory;
#if MOA_MEMORY_DEBUGGING
	MemoryStatistics memory_statistics;
#endif
}
Game;


MOA_USE_RESULT
Bool Game_init(Game *g);
void Game_free(Game *g);

void Game_update(Game *g, TimeSpan delta, TimePoint now);

MOA_USE_RESULT
int Game_enter_state(Game *g, GameStateDefinition const *state);


#endif
