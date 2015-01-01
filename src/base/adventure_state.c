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

static void destroy_widget(void *widget, void *user)
{
	(void)user;
	Widget_destroy(widget);
}

static void AdventureGui_destroy(Widget *this_)
{
	AdventureGui * const instance = (AdventureGui *)this_;
	Widget_destroy(&instance->panel1.base);
	Widget_destroy(&instance->padding1.base);
	Widget_destroy(&instance->label1.base);

	for_each(&instance->buttons, MOA_ARRAY_END(instance->buttons), sizeof(*instance->buttons), destroy_widget, NULL);

	Widget_destroy(&instance->window.base);
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
	TextStyle const styleA = make_text_style(TextAlignment_Left, TextAlignment_Left, make_color(0, 255, 0, 255));
	TextStyle const styleB = make_text_style(TextAlignment_Left, TextAlignment_Left, make_color(255, 0, 0, 255));
	TextStyle const styleC = make_text_style(TextAlignment_Left, TextAlignment_Left, make_color(0, 0, 255, 255));

	Widget_init(&gui->base, &adventure_gui_class, Vector2i_new(500, 400));

	gui->root = Panel_create(Vector2i_new(0, 0), make_absolute_layout(), memory.deallocator);
	gui->window = Panel_create(Vector2i_new(150, 400), make_vertical_layout(), memory.deallocator);
	gui->buttons[0] = LabeledButton_create("button1", styleA, Vector2i_new(100, 20), make_color(255, 255, 255, 255));
	gui->label1 = Label_create("label1", styleB, Vector2i_new(200, 40));
	gui->buttons[1] = LabeledButton_create("button2", styleC, Vector2i_new(80, 25), make_color(0, 255, 255, 255));
	gui->buttons[2] = LabeledButton_create("button3", styleC, Vector2i_new(80, 25), make_color(255, 0, 255, 255));
	gui->buttons[3] = LabeledButton_create("4", styleC, Vector2i_new(80, 25), make_color(255, 255, 0, 255));
	gui->buttons[4] = LabeledButton_create("555", styleC, Vector2i_new(80, 40), make_color(127, 127, 127, 255));
	gui->padding1 = Padding_create(Vector2i_new(80, 30), &gui->buttons[4].base, 1);
	gui->panel1 = Panel_create(Vector2i_new(200, 100), make_horizontal_layout(), memory.deallocator);
	gui->window.base.absolute_position = Vector2i_new(200, 5);

	if (PtrVector_push_back(&gui->root.children, &gui->window, memory.allocator) &&
		PtrVector_push_back(&gui->window.children, &gui->buttons[0], memory.allocator) &&
		PtrVector_push_back(&gui->window.children, &gui->label1, memory.allocator) &&
		PtrVector_push_back(&gui->window.children, &gui->buttons[1], memory.allocator) &&
		PtrVector_push_back(&gui->window.children, &gui->buttons[2], memory.allocator) &&
		PtrVector_push_back(&gui->window.children, &gui->panel1, memory.allocator) &&
		PtrVector_push_back(&gui->panel1.children, &gui->buttons[3], memory.allocator) &&
		PtrVector_push_back(&gui->panel1.children, &gui->padding1, memory.allocator))
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
