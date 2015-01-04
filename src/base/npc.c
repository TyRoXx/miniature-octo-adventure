#include "npc.h"

NPC NPC_create(Mover mover)
{
	NPC result;
	result.mover = mover;
	result.objective = Objective_Wait;
	Wait wait = { TimePoint_from_milliseconds(0) };
	result.objective_state.wait = wait;
	return result;
}

static void decide_next_objective(NPC *npc)
{
	npc->objective = Objective_MotionToPoint;
	MotionToPoint motion = { Vector2i_new(400, 300) };
	npc->objective_state.motion_to_point = motion;
}

void NPC_update(NPC *npc, struct World const *world, TimePoint now)
{
	switch (npc->objective)
	{
	case Objective_MotionToPoint:
		{
			if (npc->mover.steps_to_go <= 0)
			{
				Mover_move(&npc->mover, world, 10);
			}
			break;
		}

	case Objective_Wait:
		{
			//TODO: a proper timer
			if (TimePoint_later(npc->objective_state.wait.until, now))
			{
				return;
			}
			decide_next_objective(npc);
			break;
		}
	}
}

void NPC_free(NPC *npc)
{
	Mover_free(&npc->mover);
}
