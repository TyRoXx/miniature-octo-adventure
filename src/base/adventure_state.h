#ifndef ADVENTURE_STATE_H
#define ADVENTURE_STATE_H

#include "game_state.h"
#include "world.h"
#include "gui/panel.h"
#include "gui/padding.h"
#include "gui/labeled_button.h"

typedef struct AdventureGui
{
	Widget base;
	Panel root;
	Label frame_number;
	Vector frame_number_text;
#if MOA_MEMORY_DEBUGGING
	Label active_allocations;
	Vector active_allocations_text;
	Label total_allocations;
	Vector total_allocations_text;
#endif
	Deallocator deallocator;
}
AdventureGui;

typedef struct AdventureState
{
	GameState base;
	World world;
	Mover *avatar;
	AdventureGui gui;
	MemoryManager memory;
#if MOA_MEMORY_DEBUGGING
	MemoryStatistics const *memory_statistics;
#endif
}
AdventureState;


extern GameStateDefinition const AdventureStateDef;


#endif
