#include "game.h"


Bool Game_init(Game *g)
{
	g->state = 0;
	g->on_enter_state.function = 0;
	return True;
}

void Game_free(Game *g)
{
	if (g->state)
	{
		g->state->definition->destroy(g->state);
	}
}

void Game_update(Game *g, TimeSpan delta, TimePoint now)
{
	g->state->definition->update(g->state, delta, now);
}

int Game_enter_state(Game *g, GameStateDefinition const *state)
{
	GameState * const new_state = state->create(g);
	if (!new_state)
	{
		return False;
	}
	new_state->definition = state;
	if (g->state)
	{
		g->state->definition->destroy(g->state);
	}
	g->state = new_state;
	if (g->on_enter_state.function)
	{
		g->on_enter_state.function(g->on_enter_state.user_data, new_state);
	}
	return True;
}
