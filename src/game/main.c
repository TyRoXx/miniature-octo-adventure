#include "base/game.h"
#include "base/adventure_state.h"
#include "sdl_frontend/sdl_frontend.h"

static Allocation debugging_realloc(Allocation allocation, size_t size, PrivateAllocatorState state)
{
	Allocation result = realloc(allocation, size);
	if (result && !allocation)
	{
		MemoryStatistics *statistics = state;
		statistics->active_allocations += 1;
		statistics->total_allocations += 1;
	}
	return result;
}

static void debugging_free(Allocation allocation, PrivateAllocatorState state)
{
	if (allocation)
	{
		MemoryStatistics *statistics = state;
		statistics->active_allocations -= 1;
	}
	free(allocation);
}

int main(int argc, char **argv)
{
	int result = 0;

	(void)argc;
	(void)argv;

	{
		Game game;
		SDLFrontend frontend;
		SDLSettings settings;

		game.memory_statistics.active_allocations = 0;
		game.memory_statistics.total_allocations = 0;
		game.memory.allocator.realloc = debugging_realloc;
		game.memory.allocator.state = &game.memory_statistics;
		game.memory.deallocator.free = debugging_free;
		game.memory.deallocator.state = &game.memory_statistics;

		if (!Game_init(&game))
		{
			return 1;
		}

		settings.fullscreen = False;
		settings.resolution = Vector2i_new(1024, 768);

		if (!SDLFrontEnd_create(&frontend, &game, settings))
		{
			result = 1;
			goto fail_1;
		}

		if (!Game_enter_state(&game, &AdventureStateDef))
		{
			result = 1;
			goto fail_0;
		}

		if (!SDLFrontend_main_loop(&frontend))
		{
			result = 1;
		}

fail_0:
		SDLFrontend_destroy(&frontend);

fail_1:
		Game_free(&game);
	}

	return result;
}
