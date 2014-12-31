#include "adventure_state.h"
#include "world_text_file.h"
#include "game.h"
#include "gui/button.h"
#include "gui/label.h"
#include "gui/panel.h"
#include <assert.h>
#include <stdlib.h>
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

static Bool load_world(char const *file_name, World *world)
{
	Bool result;
	FILE * const file = fopen(file_name, "r");
	if (!file)
	{
		fprintf(stderr, "Cannot open world file %s\n", file_name);
		return False;
	}

	result = load_world_from_text(world,
								  TileKinds,
								  sizeof(TileKinds) / sizeof(*TileKinds),
								  file,
								  stderr);
	fclose(file);
	return result;
}

static char *moa_strdup(char const *str)
{
	size_t const len = strlen(str);
	char *copy = malloc(len + 1);
	if (!copy)
	{
		return NULL;
	}
	memcpy(copy, str, len + 1);
	return copy;
}

static Widget *create_gui(void)
{
	/*TODO: check success */
	Panel * const root = Panel_create(Vector2i_new(0, 0), make_absolute_layout());
	Panel * const window = Panel_create(Vector2i_new(150, 400), make_vertical_layout());
	TextStyle const styleA = make_text_style(TextAlignment_Left, TextAlignment_Left, make_color(0, 255, 0, 255));
	TextStyle const styleB = make_text_style(TextAlignment_Left, TextAlignment_Left, make_color(255, 0, 0, 255));
	TextStyle const styleC = make_text_style(TextAlignment_Left, TextAlignment_Left, make_color(0, 0, 255, 255));
	Button * const button1 = Button_create((Widget *)Label_create(moa_strdup("button1"), styleA, Vector2i_new(100, 20)), Vector2i_new(100, 20), make_color(255, 255, 255, 255));
	Label * const label1 = Label_create(moa_strdup("label1"), styleB, Vector2i_new(200, 40));
	Button * const button2 = Button_create((Widget *)Label_create(moa_strdup("button2"), styleC, Vector2i_new(80, 25)), Vector2i_new(80, 25), make_color(0, 255, 255, 255));
	Button * const button3 = Button_create((Widget *)Label_create(moa_strdup("button3"), styleC, Vector2i_new(80, 25)), Vector2i_new(80, 25), make_color(255, 0, 255, 255));
	Button * const button4 = Button_create((Widget *)Label_create(moa_strdup("4"), styleC, Vector2i_new(80, 25)), Vector2i_new(80, 25), make_color(255, 255, 0, 255));
	Button * const button5 = Button_create((Widget *)Label_create(moa_strdup("5"), styleC, Vector2i_new(80, 25)), Vector2i_new(80, 25), make_color(127, 127, 127, 255));
	Panel * const panel1 = Panel_create(Vector2i_new(200, 100), make_horizontal_layout());
	PtrVector_push_back(&root->children, window);
	PtrVector_push_back(&window->children, button1);
	PtrVector_push_back(&window->children, label1);
	PtrVector_push_back(&window->children, button2);
	PtrVector_push_back(&window->children, button3);
	PtrVector_push_back(&window->children, panel1);
	PtrVector_push_back(&panel1->children, button4);
	PtrVector_push_back(&panel1->children, button5);
	window->base.absolute_position = Vector2i_new(200, 5);
	Widget_pack(&root->base);
	return (Widget *)root;
}

static GameState *AdventureState_create(Game *game)
{
	AdventureState * const adv_state = malloc(sizeof(*adv_state));
	if (!adv_state)
	{
		return NULL;
	}

	(void)game;

	{
		char const * const world_file_name = "data/world.txt";
		World * const world = &adv_state->world;

		if (load_world(world_file_name, world))
		{
			/*if there is a mover, choose the first one as the avatar*/
			if (!Vector_empty(&world->movers))
			{
				adv_state->avatar = (Mover *)Vector_begin(&world->movers);

				/*TODO: check success*/
				adv_state->gui = create_gui();

				return (GameState *)adv_state;
			}

			World_free(world);
		}

		return NULL;
	}
}

static void AdventureState_destroy(GameState *state)
{
	AdventureState * const adv_state = (AdventureState *)state;
	Widget_destroy(adv_state->gui);
	World_free(&adv_state->world);
	free(state);
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
