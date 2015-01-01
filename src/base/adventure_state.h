#ifndef ADVENTURE_STATE_H
#define ADVENTURE_STATE_H

#include "game_state.h"
#include "world.h"

typedef struct AdventureState
{
	GameState base;
	World world;
	Mover *avatar;
	MemoryManager memory;
#if MOA_MEMORY_DEBUGGING
	MemoryStatistics const *memory_statistics;
#endif
}
AdventureState;


extern GameStateDefinition const AdventureStateDef;


#endif
