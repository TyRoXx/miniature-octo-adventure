#include "adventure_state_view.h"
#include "sdl_frontend.h"
#include "gui_renderer.h"
#include "gui/panel.h"
#include "gui/button.h"
#include "gui/labeled_button.h"
#include "base/adventure_state.h"
#include "base/camera.h"
#include "base/avatar_controller.h"
#include "base/min_max.h"
#include <assert.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdlib.h>

typedef struct AdventureGui
{
	Widget base;
	Panel root;
	Label frame_number;
	Vector frame_number_text;
#if MOA_MEMORY_DEBUGGING
	Label active_allocations;
	Vector active_allocations_text;
	Label total_allocations;
	Vector total_allocations_text;
#endif
	LabeledButton exit;
	Deallocator deallocator;
}
AdventureGui;

static void AdventureGui_destroy(Widget *this_)
{
	AdventureGui * const instance = (AdventureGui *)this_;
	Widget_destroy(&instance->exit.base);
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

static void AdventureGui_handle_input(Widget *this_, GuiInput input)
{
	AdventureGui * const instance = (AdventureGui *)this_;
	Widget_handle_input(&instance->root.base, input);
}

static WidgetClass const adventure_gui_class =
{
	AdventureGui_destroy,
	AdventureGui_pack,
	AdventureGui_render,
	AdventureGui_handle_input
};

static void stop_running(void *user)
{
	Bool *is_running = user;
	*is_running = False;
}

MOA_USE_RESULT
static Bool create_gui(AdventureGui *gui, MemoryManager memory, Vector2i screen_resolution, Bool *game_is_running)
{
#if MOA_MEMORY_DEBUGGING
	TextStyle const styleA = make_text_style(TextAlignment_Left, TextAlignment_Left, make_color(0, 255, 0, 255));
	TextStyle const styleB = make_text_style(TextAlignment_Left, TextAlignment_Left, make_color(255, 0, 0, 255));
#endif
	TextStyle const styleC = make_text_style(TextAlignment_Left, TextAlignment_Left, make_color(0, 0, 255, 255));

	int root_width = 120;
	Widget_init(&gui->base, &adventure_gui_class, Vector2i_new(root_width, screen_resolution.y));

	gui->deallocator = memory.deallocator;
	gui->root = Panel_create(Vector2i_new(root_width, screen_resolution.y), pack_vertically, memory.deallocator);
	gui->root.base.actual_size = gui->root.base.desired_size;
	gui->root.base.absolute_position.x = screen_resolution.x - root_width;
	gui->root.base.absolute_position.y = 0;

#if MOA_MEMORY_DEBUGGING
	gui->active_allocations = Label_create("", styleA, Vector2i_new(300, 20));
	Vector_init(&gui->active_allocations_text);

	gui->total_allocations = Label_create("", styleB, Vector2i_new(300, 20));
	Vector_init(&gui->total_allocations_text);
#endif

	gui->frame_number = Label_create("", styleC, Vector2i_new(300, 20));
	Vector_init(&gui->frame_number_text);

	gui->exit = LabeledButton_create("Exit", styleA, Vector2i_new(300, 20), make_color(255, 0, 0, 255), stop_running, game_is_running);

	if (
#if MOA_MEMORY_DEBUGGING
		PtrVector_push_back(&gui->root.children, &gui->active_allocations, memory.allocator) &&
		PtrVector_push_back(&gui->root.children, &gui->total_allocations, memory.allocator) &&
#endif
		PtrVector_push_back(&gui->root.children, &gui->frame_number, memory.allocator) &&
		PtrVector_push_back(&gui->root.children, &gui->exit, memory.allocator))
	{
		return True;
	}

	AdventureGui_destroy(&gui->base);
	return False;
}

typedef struct AdventureStateView
{
	GameStateView base;
	struct AdventureState *state;
	SDLFrontend *front;
	Camera camera;
	AvatarController avatar_controller;
	uint64_t current_frame;
	AdventureGui gui;
}
AdventureStateView;

static void draw_animation(
	Vector2i pixel_pos,
	SDL_Surface *screen,
	Animation const *animation,
	size_t frame,
	SDL_Surface *image,
	Direction direction)
{
	AnimationSide const *side = &animation->sides[direction];
	assert(frame < side->frame_count);
	AnimationFrame * const current_frame = side->frames + frame;
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
	AnimationType animation_id,
	size_t animation_frame,
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

	animation = &appearance->layout->animations[animation_id];
	assert(animation);

	Vector2i_add(&pixel_pos, &appearance->layout->offset);
	draw_animation(pixel_pos, screen, animation, animation_frame, appearance->image, direction);
}

static int compare_entity_in_front(void const *left, void const *right)
{
	Entity const * const left_entity = *(Entity **)left;
	Entity const * const right_entity = *(Entity **)right;
	int const left_y = left_entity->position.vector.y;
	int const right_y = right_entity->position.vector.y;
	if (left_y < right_y)
	{
		return -1;
	}
	if (left_y == right_y)
	{
		return 0;
	}
	return 1;
}

static void update_animation(Mover *m, TimePoint now)
{
	TimeSpan time_in_animation = TimePoint_between(m->animation_start, now);
	TimeSpan time_per_frame = TimeSpan_from_milliseconds(250);
	unsigned current_frame = time_in_animation.milliseconds / time_per_frame.milliseconds;
	unsigned frame_count = m->body.animation == Anim_Move ? 4 : 1;
	m->body.current_animation_frame = current_frame % frame_count;
}

typedef struct AddMoverArgs
{
	PtrVector *added_to;
	Bool success;
	Allocator allocator;
	TimePoint now;
}
AddMoverArgs;

static ContinueFlag add_updated_mover(void *mover, void *user)
{
	AddMoverArgs *args = user;
	update_animation(mover, args->now);
	args->success = PtrVector_push_back(args->added_to, mover, args->allocator);
	return (args->success ? Continue_Yes : Continue_No);
}

MOA_USE_RESULT
static Bool draw_entities(
	Camera const *camera,
	SDL_Surface *screen,
	SpacialFinder const *movers,
	AppearanceManager const *appearances,
	Vector2i resolution,
	TimePoint now,
	MemoryManager memory)
{
	PtrVector entities_in_z_order;
	size_t i;

	assert(camera);
	assert(screen);
	assert(appearances);

	Rectangle displayed_area;

	PtrVector_init(&entities_in_z_order);
	AddMoverArgs args = {&entities_in_z_order, True, memory.allocator, now};
	SpacialFinder_enumerate_area(movers, displayed_area, add_updated_mover, &args);
	if (!args.success)
	{
		PtrVector_free(&entities_in_z_order, memory.deallocator);
		return False;
	}

	qsort(PtrVector_begin(&entities_in_z_order), PtrVector_size(&entities_in_z_order), sizeof(Entity *), compare_entity_in_front);

	for (i = 0; i < PtrVector_size(&entities_in_z_order); ++i)
	{
		Entity const * const body = PtrVector_get(&entities_in_z_order, i);
		Vector2i pixel_pos;
		pixel_pos.x = body->position.vector.x + (int)((-camera->position.vector.x)) + resolution.x / 2;
		pixel_pos.y = body->position.vector.y + (int)((-camera->position.vector.y)) + resolution.y / 2;
		draw_appearance(
			pixel_pos,
			screen,
			body->appearance,
			body->animation,
			body->current_animation_frame,
			appearances,
			body->direction
		);
	}

	PtrVector_free(&entities_in_z_order, memory.deallocator);
	return True;
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
							Anim_Idle,
							0,
							appearances,
							Dir_North);
		}
	}
}

MOA_USE_RESULT
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
	int visible_begin_idx = (camera->position.vector.x - resolution.x / 2) / tile_width;
	int visible_begin_idy = (camera->position.vector.y - resolution.y / 2) / tile_width;
	int visible_end_idx   =  camera->position.vector.x + divide_ceil(resolution.x / 2 + 1, tile_width);
	int visible_end_idy   =  camera->position.vector.y + divide_ceil(resolution.y / 2 + 1, tile_width);

	visible_begin_idx = max_int(visible_begin_idx, 0);
	visible_begin_idy = max_int(visible_begin_idy, 0);
	visible_end_idx   = min_int(visible_end_idx,   (int)tiles->width);
	visible_end_idy   = min_int(visible_end_idy,   (int)tiles->height);

	for (int y = visible_begin_idy; y < visible_end_idy; ++y)
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
	AdventureStateView * const adv_view = Allocator_alloc(adv_state->memory.allocator, sizeof(*adv_view));

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

	if (!AvatarController_init(&adv_view->avatar_controller, &adv_state->avatar))
	{
		goto fail_2;
	}

	if (!create_gui(&adv_view->gui, adv_state->memory, Vector2i_new(front->screen->w, front->screen->h), &front->is_running))
	{
		goto fail_3;
	}
	Widget_pack(&adv_view->gui.base);

	adv_view->current_frame = 0;
	adv_view->front = front;
	adv_view->state = adv_state;
	return (GameStateView *)adv_view;

fail_3:
	AvatarController_free(&adv_view->avatar_controller);

fail_2:
	Camera_free(&adv_view->camera);

fail_1:
	Deallocator_free(adv_state->memory.deallocator, adv_view);

fail_0:
	return NULL;
}

static void AdventureStateView_destroy(GameStateView *view)
{
	AdventureStateView * const adv_view = (AdventureStateView *)view;

	AvatarController_free(&adv_view->avatar_controller);
	Camera_free(&adv_view->camera);
	Widget_destroy(&adv_view->gui.base);
	Deallocator_free(adv_view->state->memory.deallocator, view);
}

static Bool AdventureStateView_update(GameStateView *view)
{
	AdventureStateView * const adv_view = (AdventureStateView *)view;

	AvatarController_update(&adv_view->avatar_controller, &adv_view->state->world);

#if MOA_MEMORY_DEBUGGING
	{
		MemoryStatistics const *stats = adv_view->state->memory_statistics;

		if (!Vector_printf(&adv_view->gui.active_allocations_text, adv_view->state->memory.allocator, "Active: %" PRIu64, stats->active_allocations))
		{
			return False;
		}
		adv_view->gui.active_allocations.text = adv_view->gui.active_allocations_text.data;

		if (!Vector_printf(&adv_view->gui.total_allocations_text, adv_view->state->memory.allocator, "Total: %" PRIu64, stats->total_allocations))
		{
			return False;
		}
		adv_view->gui.total_allocations.text = adv_view->gui.total_allocations_text.data;
	}
#endif

	if (!Vector_printf(&adv_view->gui.frame_number_text, adv_view->state->memory.allocator, "Frame: %" PRIu64, adv_view->current_frame))
	{
		return False;
	}
	adv_view->gui.frame_number.text = adv_view->gui.frame_number_text.data;

	return True;
}

static void draw_user_interface(
	Widget *root,
    SDL_Surface *screen,
    FontManager const *fonts)
{
	TTF_Font * const font = FontManager_find_font(fonts, 0);
	SDL_GUI_Renderer renderer;
	SDL_GUI_Renderer_init(&renderer, screen, font);
	Widget_render(root, &renderer.base);
}

static Bool AdventureStateView_draw(GameStateView *view, TimePoint now)
{
	AdventureStateView * const adv_view = (AdventureStateView *)view;
	SDL_Surface * const screen = adv_view->front->screen;
	AdventureState * const adventure = adv_view->state;
	World const * const world = &adventure->world;
	Vector2i screen_resolution;
	{
		SDL_Rect screen_size;
		SDL_GetClipRect(screen, &screen_size);
		screen_resolution.x = (int)screen_size.w;
		screen_resolution.y = (int)screen_size.h;
	}

	{
		Vector2i const avatar_size = Vector2i_new(world->tile_width, world->tile_width);
		Camera_focus_on(&adv_view->camera, &adv_view->state->avatar, avatar_size);
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

	if (!draw_entities(
		&adv_view->camera,
		screen,
		&adv_view->state->movers,
		&adv_view->front->data.appearances,
	    screen_resolution,
		now,
		adv_view->state->memory
		))
	{
		return False;
	}

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

	draw_user_interface(&adv_view->gui.base, screen, &adv_view->front->data.fonts);

	adv_view->current_frame += 1;
	return True;
}

static void AdventureStateView_handle_event(GameStateView *view, SDL_Event const *event)
{
	AdventureStateView * const adv_view = (AdventureStateView *)view;
	AvatarController * const avatar_controller = &adv_view->avatar_controller;
	World const * const world = &adv_view->state->world;

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.sym)
		{
		case SDLK_w: AvatarController_handle_input(avatar_controller, world, Dir_North, False); break;
		case SDLK_a: AvatarController_handle_input(avatar_controller, world, Dir_West, False); break;
		case SDLK_s: AvatarController_handle_input(avatar_controller, world, Dir_South, False); break;
		case SDLK_d: AvatarController_handle_input(avatar_controller, world, Dir_East, False); break;
		default:
			break;
		}
	}
	else if (event->type == SDL_KEYDOWN)
	{
		switch (event->key.keysym.sym)
		{
		case SDLK_w: AvatarController_handle_input(avatar_controller, world, Dir_North, True); break;
		case SDLK_a: AvatarController_handle_input(avatar_controller, world, Dir_West, True); break;
		case SDLK_s: AvatarController_handle_input(avatar_controller, world, Dir_South, True); break;
		case SDLK_d: AvatarController_handle_input(avatar_controller, world, Dir_East, True); break;
		default:
			break;
		}
	}
	else if (event->type == SDL_MOUSEBUTTONUP)
	{
		GuiInput input;
		input.click_position = Vector2i_new(event->motion.x, event->motion.y);
		Widget_handle_input(&adv_view->gui.base, input);
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
