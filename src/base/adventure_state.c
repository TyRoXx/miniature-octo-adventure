#include "adventure_state.h"
#include "world_text_file.h"
#include "game.h"
#include "algorithm.h"
#include "gui/labeled_button.h"
#include "gui/panel.h"
#include "gui/padding.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>


static TileKind const TileKinds[] =
{
	{0, 1},
	{1, 1},
	{2, 1},
	{3, 1},
	{4, 0},
	{5, 1},
	{6, 1},
	{7, 1},
	{8, 0},
	{9, 0},
};

MOA_USE_RESULT
static Bool load_world(char const *file_name, World *world, MemoryManager world_memory)
{
	Bool result;
	FILE * const file = fopen(file_name, "r");
	if (!file)
	{
		fprintf(stderr, "Cannot open world file %s\n", file_name);
		return False;
	}

	result = load_world_from_text(world,
								  world_memory,
								  TileKinds,
								  sizeof(TileKinds) / sizeof(*TileKinds),
								  file,
								  stderr);
	fclose(file);
	return result;
}

static void AdventureGui_destroy(Widget *this_)
{
	AdventureGui * const instance = (AdventureGui *)this_;
#if MOA_MEMORY_DEBUGGING
	Widget_destroy(&instance->active_allocations.base);
	Vector_free(&instance->active_allocations_text, instance->deallocator);
	Widget_destroy(&instance->total_allocations.base);
	Vector_free(&instance->total_allocations_text, instance->deallocator);
#endif
	Widget_destroy(&instance->frame_number.base);
	Vector_free(&instance->frame_number_text, instance->deallocator);
	Widget_destroy(&instance->root.base);
}

static void AdventureGui_pack(Widget *this_)
{
	AdventureGui * const instance = (AdventureGui *)this_;
	Widget_pack(&instance->root.base);
}

static void AdventureGui_render(Widget *this_, Renderer *renderer)
{
	AdventureGui * const instance = (AdventureGui *)this_;
	Widget_render(&instance->root.base, renderer);
}

static WidgetClass const adventure_gui_class =
{
	AdventureGui_destroy,
	AdventureGui_pack,
	AdventureGui_render
};

MOA_USE_RESULT
static Bool create_gui(AdventureGui *gui, MemoryManager memory)
{
	Vector2i screen_resolution = {1024, 768}; /*TODO use the actual resolution*/
#if MOA_MEMORY_DEBUGGING
	TextStyle const styleA = make_text_style(TextAlignment_Left, TextAlignment_Left, make_color(0, 255, 0, 255));
	TextStyle const styleB = make_text_style(TextAlignment_Left, TextAlignment_Left, make_color(255, 0, 0, 255));
#endif
	TextStyle const styleC = make_text_style(TextAlignment_Left, TextAlignment_Left, make_color(0, 0, 255, 255));

	int root_width = 200;
	Widget_init(&gui->base, &adventure_gui_class, Vector2i_new(root_width, screen_resolution.y));

	gui->deallocator = memory.deallocator;
	gui->root = Panel_create(Vector2i_new(root_width, screen_resolution.y), make_vertical_layout(), memory.deallocator);
	gui->root.base.actual_size = gui->root.base.desired_size;
	gui->root.base.absolute_position.x = screen_resolution.x - root_width;
	gui->root.base.absolute_position.y = 0;

#if MOA_MEMORY_DEBUGGING
	gui->active_allocations = Label_create("", styleA, Vector2i_new(300, 30));
	Vector_init(&gui->active_allocations_text);

	gui->total_allocations = Label_create("", styleB, Vector2i_new(300, 30));
	Vector_init(&gui->total_allocations_text);
#endif

	gui->frame_number = Label_create("", styleC, Vector2i_new(300, 30));
	Vector_init(&gui->frame_number_text);

	if (
#if MOA_MEMORY_DEBUGGING
		PtrVector_push_back(&gui->root.children, &gui->active_allocations, memory.allocator) &&
		PtrVector_push_back(&gui->root.children, &gui->total_allocations, memory.allocator) &&
#endif
		PtrVector_push_back(&gui->root.children, &gui->frame_number, memory.allocator))
	{
		return True;
	}

	AdventureGui_destroy(&gui->base);
	return False;
}

static GameState *AdventureState_create(Game *game)
{
	AdventureState * const adv_state = Allocator_realloc(game->memory.allocator, NULL, sizeof(*adv_state));
	if (!adv_state)
	{
		return NULL;
	}

	adv_state->memory = game->memory;
#if MOA_MEMORY_DEBUGGING
	adv_state->memory_statistics = &game->memory_statistics;
#endif

	{
		char const * const world_file_name = "data/world.txt";
		World * const world = &adv_state->world;

		if (load_world(world_file_name, world, adv_state->memory))
		{
			/*if there is a mover, choose the first one as the avatar*/
			if (!Vector_empty(&world->movers))
			{
				adv_state->avatar = (Mover *)Vector_begin(&world->movers);

				if (create_gui(&adv_state->gui, adv_state->memory))
				{
					Widget_pack(&adv_state->gui.base);
					return (GameState *)adv_state;
				}
			}

			World_free(world, adv_state->memory.deallocator);
		}

		return NULL;
	}
}

static void AdventureState_destroy(GameState *state)
{
	AdventureState * const adv_state = (AdventureState *)state;
	Widget_destroy(&adv_state->gui.base);
	World_free(&adv_state->world, adv_state->memory.deallocator);
	Deallocator_free(adv_state->memory.deallocator, state);
}

static void AdventureState_update(GameState *state, unsigned delta)
{
	AdventureState * const adv_state = (AdventureState *)state;
	World_update(&adv_state->world, delta);
}

GameStateDefinition const AdventureStateDef =
{
	AdventureState_create,
	AdventureState_destroy,
	AdventureState_update
};
