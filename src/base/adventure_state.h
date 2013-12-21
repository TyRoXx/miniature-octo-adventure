#ifndef ADVENTURE_STATE_H
#define ADVENTURE_STATE_H


#include "game_state.h"
#include "world.h"


struct Widget;

typedef struct AdventureState
{
	GameState base;
	World world;
	Mover *avatar;
	struct Widget *gui;
}
AdventureState;


extern GameStateDefinition const AdventureStateDef;


#endif
