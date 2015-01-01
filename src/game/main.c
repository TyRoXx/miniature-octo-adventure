#include "base/game.h"
#include "base/adventure_state.h"
#include "sdl_frontend/sdl_frontend.h"


int main(int argc, char **argv)
{
	int result = 0;

	(void)argc;
	(void)argv;

	{
		Game game;
		Frontend *frontend;
		SDLSettings settings;

		game.memory = create_standard_memory_manager();

		if (!Game_init(&game))
		{
			return 1;
		}

		settings.fullscreen = False;
		settings.resolution = Vector2i_new(1024, 768);

		frontend = SDLFrontEnd_create(&game, settings);
		if (!frontend)
		{
			result = 1;
			goto fail_1;
		}

		if (!Game_enter_state(&game, &AdventureStateDef))
		{
			result = 1;
			goto fail_0;
		}

		frontend->type->main_loop(frontend);

fail_0:
		frontend->type->destroy(frontend);

fail_1:
		Game_free(&game);
	}

	return result;
}
