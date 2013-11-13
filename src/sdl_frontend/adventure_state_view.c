#include "adventure_state_view.h"
#include "camera.h"
#include "avatar_controller.h"
#include "sdl_frontend.h"
#include "gui_renderer.h"
#include "gui/panel.h"
#include "gui/button.h"
#include "gui/label.h"
#include "base/adventure_state.h"
#include "base/minmax.h"
#include <stdlib.h>
#include <assert.h>

typedef struct AdventureStateView
{
	GameStateView base;
	struct AdventureState *state;
	SDLFrontend *front;
	Camera camera;
	AvatarController avatar_controller;
}
AdventureStateView;

static void draw_animation(
	Vector2i pixel_pos,
	SDL_Surface *screen,
	Animation const *animation,
	SDL_Surface *image,
	Direction direction)
{
	AnimationSide const *side = &animation->sides[direction];
	AnimationFrame * const current_frame = side->frames + 0;
	SDL_Rect * const source = &current_frame->section;
	SDL_Rect dest;
	dest.x = (Sint16)pixel_pos.x;
	dest.y = (Sint16)pixel_pos.y;
	SDL_BlitSurface(image, source, screen, &dest);
}

static void draw_appearance(
	Vector2i pixel_pos,
	SDL_Surface *screen,
	AppearanceId appearance_id,
	AppearanceManager const *appearances,
	Direction direction)
{
	Appearance const * const appearance = AppearanceManager_get(
				appearances, appearance_id);
	Animation const *animation;

	if (!appearance)
	{
		return;
	}

	animation = &appearance->layout->animations[Anim_Idle];
	assert(animation);

	draw_animation(pixel_pos, screen, animation, appearance->image, direction);
}

static void draw_entities(
	Camera const *camera,
	SDL_Surface *screen,
	World const *world,
	AppearanceManager const *appearances,
	Vector2i resolution)
{
	Mover * begin = (Mover *)Vector_begin(&world->movers);
	Mover * const end = (Mover *)Vector_end(&world->movers);

	assert(world);
	assert(camera);
	assert(screen);
	assert(appearances);

	for (; begin != end; ++begin)
	{
		Vector2i pixel_pos;
		pixel_pos.x = begin->body.position.vector.x + (int)((- camera->position.vector.x)) + resolution.x / 2;
		pixel_pos.y = begin->body.position.vector.y + (int)((- camera->position.vector.y)) + resolution.y / 2;
		draw_appearance(
			pixel_pos,
			screen,
			begin->body.appearance,
			appearances,
			begin->body.direction);
	}
}

static void draw_layered_tile(
	Vector2i pixel_pos,
	SDL_Surface *screen,
	LayeredTile const *tile,
	AppearanceManager const *appearances,
	size_t layer_begin,
	size_t layer_end
)
{
	size_t i;
	for (i = layer_begin; i < layer_end; ++i)
	{
		TileKind const * const layer = tile->layers[i];
		if (layer)
		{
			draw_appearance(pixel_pos,
							screen,
							layer->image_id,
							appearances,
							Dir_North);
		}
	}
}

static int divide_ceil(int dividend, int divisor)
{
	assert(divisor);
	/* TODO: do this correctly for negatives */
	return (dividend + divisor - 1) / divisor;
}

static void draw_tile_layers(
	Camera const *camera,
	SDL_Surface *screen,
	TileGrid const *tiles,
	int tile_width,
	AppearanceManager const *appearances,
	size_t layer_begin,
	size_t layer_end,
	Vector2i resolution)
{
	int y;
	int visible_begin_idx = (camera->position.vector.x - resolution.x  / 2) / tile_width;
	int visible_begin_idy = (camera->position.vector.y - resolution.y / 2) / tile_width;
	int visible_end_idx   =  camera->position.vector.x + divide_ceil(resolution.x  / 2 + 1, tile_width);
	int visible_end_idy   =  camera->position.vector.y + divide_ceil(resolution.y / 2 + 1, tile_width);

	visible_begin_idx = max_int(visible_begin_idx, 0);
	visible_begin_idy = max_int(visible_begin_idy, 0);
	visible_end_idx   = min_int(visible_end_idx,   (int)tiles->width);
	visible_end_idy   = min_int(visible_end_idy,   (int)tiles->height);

	for (y = visible_begin_idy; y < visible_end_idy; ++y)
	{
		int x;
		for (x = visible_begin_idx; x < visible_end_idx; ++x)
		{
			LayeredTile const * const tile = TileGrid_get(tiles, (size_t)x, (size_t)y);
			Vector2i pixel_pos;

			assert(tile);

			pixel_pos.x = ((tile_width * x - camera->position.vector.x) + resolution.x / 2);
			pixel_pos.y = ((tile_width * y - camera->position.vector.y) + resolution.y / 2);

			draw_layered_tile(
				pixel_pos,
				screen,
				tile,
				appearances,
				layer_begin,
				layer_end
				);
		}
	}
}


static GameStateView *AdventureStateView_create(GameState *state, struct SDLFrontend *front)
{
	AdventureState * const adv_state = (AdventureState *)state;
	AdventureStateView * const adv_view = malloc(sizeof(*adv_view));

	assert(adv_state);
	assert(front);

	if (!adv_view)
	{
		goto fail_0;
	}

	if (!Camera_init(&adv_view->camera))
	{
		goto fail_1;
	}

	if (!AvatarController_init(&adv_view->avatar_controller, adv_state->avatar))
	{
		goto fail_2;
	}

	adv_view->front = front;
	adv_view->state = adv_state;
	return (GameStateView *)adv_view;

fail_2:
	Camera_free(&adv_view->camera);

fail_1:
	free(adv_view);

fail_0:
	return 0;
}

static void AdventureStateView_destroy(GameStateView *view)
{
	AdventureStateView * const adv_view = (AdventureStateView *)view;

	AvatarController_free(&adv_view->avatar_controller);
	Camera_free(&adv_view->camera);
	free(view);
}

static void AdventureStateView_update(GameStateView *view)
{
	AdventureStateView * const adv_view = (AdventureStateView *)view;

	AvatarController_update(&adv_view->avatar_controller);
}

static void draw_user_interface(
        SDL_Surface *screen,
        FontManager const *fonts)
{
	TTF_Font * const font = FontManager_find_font(fonts, 0);
	Panel * const root = Panel_create(Vector2i_new(0, 0), make_absolute_layout());
	Panel * const window = Panel_create(Vector2i_new(150, 90), make_vertical_layout());
	TextStyle const styleA = make_text_style(TextAlignment_Center, TextAlignment_Center, make_color(255, 255, 255, 255));
	TextStyle const styleB = make_text_style(TextAlignment_Left, TextAlignment_Right, make_color(255, 0, 0, 255));
	TextStyle const styleC = make_text_style(TextAlignment_Right, TextAlignment_Center, make_color(0, 255, 255, 255));
	Button * const button1 = Button_create((Widget *)Label_create(SDL_strdup("Click me 1 !!!!!!!!!!!!!!!!!!!"), styleA, Vector2i_new(100, 20)), Vector2i_new(100, 20));
	Label * const label1 = Label_create(SDL_strdup("abcdefghijklmnopqrstuvwxyz Label 1"), styleB, Vector2i_new(200, 40));
	Button * const button2 = Button_create((Widget *)Label_create(SDL_strdup("123456 Click me 654321"), styleC, Vector2i_new(80, 25)), Vector2i_new(80, 25));
	SDL_GUI_Renderer renderer;
	SDL_GUI_Renderer_init(&renderer, screen, font);
	PtrVector_push_back(&root->children, window);
	PtrVector_push_back(&window->children, button1);
	PtrVector_push_back(&window->children, label1);
	PtrVector_push_back(&window->children, button2);
	window->base.absolute_position = Vector2i_new(200, 5);
	Widget_pack(&root->base);
	Widget_render(&root->base, &renderer.base);
	Widget_destroy(&root->base);
}

static void AdventureStateView_draw(GameStateView *view)
{
	AdventureStateView * const adv_view = (AdventureStateView *)view;
	SDL_Surface * const screen = adv_view->front->screen;
	World const * const world = &adv_view->state->world;
	Vector2i screen_resolution;
	{
		SDL_Rect screen_size;
		SDL_GetClipRect(screen, &screen_size);
		screen_resolution.x = (int)screen_size.w;
		screen_resolution.y = (int)screen_size.h;
	}

	if (adv_view->state->avatar)
	{
		Vector2i const avatar_size = Vector2i_new(world->tile_width, world->tile_width);
		Camera_focus_on(&adv_view->camera, adv_view->state->avatar, avatar_size);
	}

	assert(TILE_LAYER_COUNT == 3);

	/*draw layers 0 and 1*/
	draw_tile_layers(
		&adv_view->camera,
		screen,
		&adv_view->state->world.tiles,
		world->tile_width,
		&adv_view->front->data.appearances,
		0,
		2,
	    screen_resolution
		);

	draw_entities(
		&adv_view->camera,
		screen,
		&adv_view->state->world,
		&adv_view->front->data.appearances,
	    screen_resolution
		);

	/*draw layer 2*/
	draw_tile_layers(
		&adv_view->camera,
		screen,
		&adv_view->state->world.tiles,
	    world->tile_width,
		&adv_view->front->data.appearances,
		2,
		3,
	    screen_resolution
		);

	draw_user_interface(screen, &adv_view->front->data.fonts);
}

static void AdventureStateView_handle_event(GameStateView *view, SDL_Event const *event)
{
	AdventureStateView * const adv_view = (AdventureStateView *)view;
	AvatarController * const avatar_controller = &adv_view->avatar_controller;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
		case SDLK_w: AvatarController_handle_input(avatar_controller, Dir_North, 0); break;
		case SDLK_a: AvatarController_handle_input(avatar_controller, Dir_West, 0); break;
		case SDLK_s: AvatarController_handle_input(avatar_controller, Dir_South, 0); break;
		case SDLK_d: AvatarController_handle_input(avatar_controller, Dir_East, 0); break;
		default:
			break;
		}
	}
	else if (event->type == SDL_KEYDOWN)
	{
		switch (event->key.keysym.sym)
		{
		case SDLK_w: AvatarController_handle_input(avatar_controller, Dir_North, 1); break;
		case SDLK_a: AvatarController_handle_input(avatar_controller, Dir_West, 1); break;
		case SDLK_s: AvatarController_handle_input(avatar_controller, Dir_South, 1); break;
		case SDLK_d: AvatarController_handle_input(avatar_controller, Dir_East, 1); break;
		default:
			break;
		}
	}
}

GameStateViewType const AdventureStateViewType =
{
	AdventureStateView_create,
	AdventureStateView_destroy,
	AdventureStateView_update,
	AdventureStateView_draw,
	AdventureStateView_handle_event,
};
