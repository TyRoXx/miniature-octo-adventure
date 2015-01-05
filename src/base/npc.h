#ifndef NPC_H
#define NPC_H

#include "base/entity.h"
#include "base/number_generator.h"

typedef struct Wait
{
	TimePoint until;
}
Wait;

typedef struct MotionToPoint
{
	Vector2i destination;
}
MotionToPoint;

typedef union ObjectiveState
{
	Wait wait;
	MotionToPoint motion_to_point;
}
ObjectiveState;

typedef enum Objective
{
	Objective_Wait,
	Objective_MotionToPoint
}
Objective;

typedef struct NPC
{
	Mover mover;
	Objective objective;
	ObjectiveState objective_state;
}
NPC;

MOA_USE_RESULT
NPC NPC_create(Mover mover);
void NPC_update(NPC *npc, struct TileGrid const *world, TimeSpan delta, TimePoint now, NumberGenerator random);

#endif
