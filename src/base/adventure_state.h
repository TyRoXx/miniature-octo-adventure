#ifndef ADVENTURE_STATE_H
#define ADVENTURE_STATE_H

#include "game_state.h"
#include "world.h"
#include "base/fauna.h"
#include "base/spacial_finder.h"

typedef struct AdventureState
{
	GameState base;
	TileGrid tiles;
	Fauna fauna;
	SpacialFinder movers;
	Mover avatar;
	MemoryManager memory;
	MemoryStatistics const *memory_statistics;
}
AdventureState;


extern GameStateDefinition const AdventureStateDef;


#endif
