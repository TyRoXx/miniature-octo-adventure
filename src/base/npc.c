#include "npc.h"
#include <stdlib.h>

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
	MotionToPoint motion = { Vector2i_new(32, 0) };
	Vector2i_add(&motion.destination, &npc->mover.body.position.vector);
	npc->objective_state.motion_to_point = motion;
}

static Direction vector_to_direction(Vector2i vector)
{
	if (vector.x >= 0)
	{
		if (vector.y >= 0)
		{
			return (vector.x > vector.y) ? Dir_East : Dir_South;
		}
		else
		{
			return (vector.x > -vector.y) ? Dir_East : Dir_North;
		}
	}
	else
	{
		if (vector.y >= 0)
		{
			return (-vector.x > vector.y) ? Dir_West : Dir_South;
		}
		else
		{
			return (-vector.x > -vector.y) ? Dir_West : Dir_North;
		}
	}
}

void NPC_update(NPC *npc, struct TileGrid const *world, TimeSpan delta, TimePoint now)
{
	switch (npc->objective)
	{
	case Objective_MotionToPoint:
		{
			if (Vector2i_equal(&npc->objective_state.motion_to_point.destination, &npc->mover.body.position.vector))
			{
				if (npc->mover.steps_to_go)
				{
					Mover_stop(&npc->mover);
				}
				Wait wait = { TimePoint_add(now, TimeSpan_from_milliseconds(2000)) };
				npc->objective = Objective_Wait;
				npc->objective_state.wait = wait;
				break;
			}

			Vector2i delta_destination = npc->objective_state.motion_to_point.destination;
			Vector2i_sub(&delta_destination, &npc->mover.body.position.vector);

			npc->mover.body.direction = vector_to_direction(delta_destination);
			size_t steps_to_go = (size_t)labs(Vector2i_get_component(&delta_destination, Direction_to_direction2(npc->mover.body.direction)));
			if (npc->mover.steps_to_go)
			{
				assert(npc->mover.steps_to_go >= 1);
				npc->mover.steps_to_go = steps_to_go;
			}
			else
			{
				Mover_move(&npc->mover, world, steps_to_go);
			}
			break;
		}

	case Objective_Wait:
		{
			//TODO: a proper timer
			if (TimePoint_later(now, npc->objective_state.wait.until))
			{
				break;
			}
			decide_next_objective(npc);
			break;
		}
	}

	Mover_update(&npc->mover, world, delta, now);
}

void NPC_free(NPC *npc)
{
	Mover_free(&npc->mover);
}
