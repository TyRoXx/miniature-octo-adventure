#include "sdl_frontend.h"
#include "adventure_state_view.h"
#include "base/adventure_state.h"
#include "base/game.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_main.h"
#include <assert.h>

void SDLFrontend_destroy(SDLFrontend *sdl_front)
{
	assert(sdl_front);

	if (sdl_front->state_view)
	{
		sdl_front->state_view->type->destroy(sdl_front->state_view);
	}

	Data_free(&sdl_front->data, sdl_front->game->memory.deallocator);

	TTF_Quit();
#if SDL_MAJOR_VERSION >= 2
	SDL_FreeSurface(sdl_front->screen);
	SDL_DestroyTexture(sdl_front->texture);
	SDL_DestroyRenderer(sdl_front->renderer);
	SDL_DestroyWindow(sdl_front->window);
#endif
	SDL_Quit();
}

Bool SDLFrontend_main_loop(SDLFrontend *sdl_front)
{
	SDL_Surface * const screen = sdl_front->screen;
	Game * const game = sdl_front->game;
	TimePoint last_time = TimePoint_from_milliseconds(SDL_GetTicks());

	assert(sdl_front->state_view);

	while (sdl_front->is_running)
	{
		{
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					sdl_front->is_running = 0;
					break;
				}

				if (event.type == SDL_KEYUP)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						sdl_front->is_running = 0;
						break;
					default:
						break;
					}
				}

				sdl_front->state_view->type->handle_event(
							sdl_front->state_view,
							&event);
			}
		}

		TimePoint current_time = TimePoint_from_milliseconds(SDL_GetTicks());
		Game_update(game, TimePoint_between(last_time, current_time), current_time);
		last_time = current_time;

		if (!sdl_front->state_view->type->update(sdl_front->state_view))
		{
			return False;
		}

		SDL_FillRect(screen, 0, 0);

		if (!sdl_front->state_view->type->draw(sdl_front->state_view, current_time))
		{
			return False;
		}

#if SDL_MAJOR_VERSION >= 2
		SDL_RenderClear(sdl_front->renderer);
		SDL_UpdateTexture(sdl_front->texture, NULL, sdl_front->screen->pixels, sdl_front->screen->w * 4);
		SDL_RenderCopy(sdl_front->renderer, sdl_front->texture, NULL, NULL);
		SDL_RenderPresent(sdl_front->renderer);
#else
		SDL_Flip(screen);
#endif
		SDL_Delay(16);
	}
	return True;
}

static char const * const WindowTitle = "SDL Test";

static void on_enter_game_state(void *user_data, GameState *state)
{
	SDLFrontend * const sdl_front = user_data;

	assert(sdl_front);
	assert(state);
	assert(state->definition == &AdventureStateDef);

	sdl_front->state_view = AdventureStateViewType.create(state, sdl_front);
	if (!sdl_front->state_view)
	{
		/*TODO*/
		return;
	}
	sdl_front->state_view->type = &AdventureStateViewType;
}

Bool SDLFrontEnd_create(SDLFrontend *front, struct Game *game, SDLSettings settings)
{
	assert(game);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		 fprintf(stderr, "SDL error: %s\n", SDL_GetError());
		 return False;
	}

	if (TTF_Init() == -1)
	{
		fprintf(stderr, "SDL_tff error: %s\n", TTF_GetError());
		goto fail_2;
	}

	front->game = game;
#if SDL_MAJOR_VERSION >= 2
	front->window = SDL_CreateWindow(
		WindowTitle,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		settings.resolution.x, settings.resolution.y,
		SDL_WINDOW_SHOWN
	);
	assert(front->window); /*TODO error handling*/

	front->renderer = SDL_CreateRenderer(front->window, -1, 0);
	assert(front->renderer); /*TODO error handling*/

	front->texture = SDL_CreateTexture(
		front->renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		settings.resolution.x, settings.resolution.y
	);
	assert(front->texture); /*TODO error handling*/

	front->screen = SDL_CreateRGBSurface(
		0, settings.resolution.x, settings.resolution.y, 32,
		0x00FF0000,
		0x0000FF00,
		0x000000FF,
		0xFF000000
	);
#else
	front->screen = SDL_SetVideoMode(settings.resolution.x, settings.resolution.y, 32,
	                                 SDL_HWSURFACE | SDL_DOUBLEBUF | (settings.fullscreen ? SDL_FULLSCREEN : 0));
#endif
	front->state_view = 0;
	front->is_running = True;

	if (!front->screen)
	{
		fprintf(stderr, "Cannot create screen: %s\n", SDL_GetError());
		goto fail_3;
	}

	if (!Data_init(&front->data, "data", front->screen->format, game->memory))
	{
		goto fail_3;
	}

	assert(!game->on_enter_state.function);
	game->on_enter_state.function = on_enter_game_state;
	game->on_enter_state.user_data = front;

#if SDL_MAJOR_VERSION == 1
	SDL_WM_SetCaption(WindowTitle, WindowTitle);
#endif

	return True;

fail_3:
	TTF_Quit();

fail_2:
#if SDL_MAJOR_VERSION >= 2
	SDL_FreeSurface(front->screen);
#endif
	SDL_Quit();
	return False;
}
