#include "sdl_frontend.h"
#include "adventure_state_view.h"
#include "base/adventure_state.h"
#include "base/game.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_main.h"
#include <assert.h>

static void SDLFrontend_destroy(Frontend *front)
{
	SDLFrontend * const sdl_front = (SDLFrontend *)front;
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

	Deallocator_free(sdl_front->game->memory.deallocator, front);
}

static Bool SDLFrontend_main_loop(Frontend *front)
{
	SDLFrontend * const sdl_front = (SDLFrontend *)front;
	SDL_Surface * const screen = sdl_front->screen;
	Game * const game = sdl_front->game;
	int is_running = 1;
	unsigned last_time = SDL_GetTicks();

	assert(sdl_front->state_view);

	while (is_running)
	{
		{
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
				{
					is_running = 0;
					break;
				}

				if (event.type == SDL_KEYUP)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						is_running = 0;
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

		{
			unsigned current_time;
			current_time = SDL_GetTicks();
			assert(current_time >= last_time);
			Game_update(game, (current_time - last_time));
			last_time = current_time;
		}

		if (!sdl_front->state_view->type->update(sdl_front->state_view))
		{
			return False;
		}

		SDL_FillRect(screen, 0, 0);

		sdl_front->state_view->type->draw(sdl_front->state_view);

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


static FrontendType const SDLFrontendType =
{
	SDLFrontend_destroy,
	SDLFrontend_main_loop
};

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

Frontend *SDLFrontEnd_create(struct Game *game, SDLSettings settings)
{
	SDLFrontend * const front = Allocator_alloc(game->memory.allocator, sizeof(*front));
	if (!front)
	{
		goto fail_0;
	}

	assert(game);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		 fprintf(stderr, "SDL error: %s\n", SDL_GetError());
		 goto fail_1;
	}

	if (TTF_Init() == -1)
	{
		fprintf(stderr, "SDL_tff error: %s\n", TTF_GetError());
		goto fail_2;
	}

	front->base.type = &SDLFrontendType;
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

	return (Frontend *)front;

fail_3:
	TTF_Quit();

fail_2:
#if SDL_MAJOR_VERSION >= 2
	SDL_FreeSurface(front->screen);
#endif
	SDL_Quit();

fail_1:
	Deallocator_free(game->memory.deallocator, front);

fail_0:
	return NULL;
}
