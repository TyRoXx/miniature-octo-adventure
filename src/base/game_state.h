#ifndef GAME_STATE_H
#define GAME_STATE_H


#include "base/timespan.h"
#include "base/number_generator.h"
#include <stdint.h>


struct GameState;
struct Game;


typedef struct GameStateDefinition
{
	MOA_USE_RESULT
	struct GameState *(*create)(struct Game *);
	void (*destroy)(struct GameState *);
	void (*update)(struct GameState *, TimeSpan delta, TimePoint now, NumberGenerator random);
}
GameStateDefinition;


typedef struct GameState
{
	GameStateDefinition const *definition;
}
GameState;


typedef struct MemoryStatistics
{
	uint64_t active_allocations;
	uint64_t total_allocations;
}
MemoryStatistics;


#endif
