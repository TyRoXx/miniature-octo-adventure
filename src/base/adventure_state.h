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
	Panel window;
	LabeledButton buttons[5];
	Label label1;
	Padding padding1;
	Panel panel1;
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
