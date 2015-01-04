#include "npc.h"
#include "base/world.h"
#include <stdlib.h>
#include <limits.h>

NPC NPC_create(Mover mover)
{
	NPC result;
	result.mover = mover;
	result.objective = Objective_Wait;
	Wait wait = { TimePoint_from_milliseconds(0) };
	result.objective_state.wait = wait;
	return result;
}

static Direction random_direction(void)
{
	return (Direction)(rand() % 4);
}

static Vector2i find_last_reachable_point(Vector2i from, Direction direction, int distance, TileGrid const *tiles)
{
	assert(distance >= 0);
	assert(tiles);
	Vector2i last_reachable = from;
	Vector2i direction_vector = direction_to_vector(direction);
	while (is_possible_step(&last_reachable, direction, tiles) && (distance > 0))
	{
		Vector2i_add(&last_reachable, &direction_vector);
		--distance;
	}
	return last_reachable;
}

static void decide_next_objective(NPC *npc, TileGrid const *tiles)
{
	npc->objective = Objective_MotionToPoint;
	Direction favored_direction = random_direction();
	for (int i = 0; i < DIR_COUNT; ++i)
	{
		Direction tried_direction = (Direction)(((int)favored_direction + i) % DIR_COUNT);
		if (!is_possible_step(&npc->mover.body.position.vector, tried_direction, tiles))
		{
			continue;
		}
		MotionToPoint motion = { direction_to_vector(tried_direction) };
		int distance = rand() % (tile_size * 3) + tile_size / 2;
		motion.destination = find_last_reachable_point(npc->mover.body.position.vector, tried_direction, distance, tiles);
		npc->objective_state.motion_to_point = motion;
		return;
	}
	npc->objective = Objective_Wait;
	Wait wait = { TimePoint_from_milliseconds(UINT_MAX) };
	npc->objective_state.wait = wait;
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

static TimeSpan random_time_span(TimeSpan min, TimeSpan max)
{
	return TimeSpan_from_milliseconds(min.milliseconds + (unsigned)rand() % (max.milliseconds - min.milliseconds + 1));
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
				Wait wait = { TimePoint_add(now, random_time_span(TimeSpan_from_milliseconds(500), TimeSpan_from_milliseconds(2500))) };
				npc->objective = Objective_Wait;
				npc->objective_state.wait = wait;
				break;
			}

			Vector2i delta_destination = npc->objective_state.motion_to_point.destination;
			Vector2i_sub(&delta_destination, &npc->mover.body.position.vector);

			npc->mover.body.direction = vector_to_direction(delta_destination);
			assert(is_possible_step(&npc->mover.body.position.vector, npc->mover.body.direction, world));

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
			decide_next_objective(npc, world);
			break;
		}
	}

	Mover_update(&npc->mover, world, delta, now);
}

void NPC_free(NPC *npc)
{
	Mover_free(&npc->mover);
}
