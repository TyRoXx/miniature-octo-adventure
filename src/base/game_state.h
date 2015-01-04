#ifndef GAME_STATE_H
#define GAME_STATE_H


#include "base/timespan.h"
#include <stdint.h>


struct GameState;
struct Game;


typedef struct GameStateDefinition
{
	struct GameState *(*create)(struct Game *);
	void (*destroy)(struct GameState *);
	void (*update)(struct GameState *, TimeSpan delta);
}
GameStateDefinition;


typedef struct GameState
{
	GameStateDefinition const *definition;
}
GameState;


#ifdef NDEBUG
#	define MOA_MEMORY_DEBUGGING 0
#else
#	define MOA_MEMORY_DEBUGGING 1
#endif


#if MOA_MEMORY_DEBUGGING
typedef struct MemoryStatistics
{
	uint64_t active_allocations;
	uint64_t total_allocations;
}
MemoryStatistics;
#endif


#endif
