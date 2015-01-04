#include "game.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

Bool Game_init(Game *g)
{
	g->state = 0;
	g->on_enter_state.function = 0;
	g->random_file = open("/dev/urandom", O_RDONLY);
	if (g->random_file < 0)
	{
		fprintf(stderr, "Could not open random device (errno %d)\n", errno);
		return False;
	}
	g->random = make_kernel_number_generator(g->random_file);
	return True;
}

void Game_free(Game *g)
{
	if (close(g->random_file) < 0)
	{
		abort();
	}
	if (g->state)
	{
		g->state->definition->destroy(g->state);
	}
}

void Game_update(Game *g, TimeSpan delta, TimePoint now)
{
	g->state->definition->update(g->state, delta, now, g->random);
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
