#include <nds.h>
#include <stdio.h>
#include <assert.h>
#include "grass_32.h"
#include "base/ptr_vector.h"
#include "base/stringize.h"

static void halt_with_message(char const *message)
{
	consoleDemoInit();
	iprintf("%s\n", message);
	for (;;);
}

#define VERIFY(x) do { if (!(x)) { halt_with_message("VERIFY(" MOA_STRINGIZE(x) ") fail at " __FILE__ ":" MOA_STRINGIZE(__LINE__)); } } while (0)

typedef struct SpritePlacement
{
	u16 *pixels;
	s16 x, y;
}
SpritePlacement;

typedef struct SpriteRenderer
{
	PtrVector visible_sprites;
}
SpriteRenderer;

static void SpriteRenderer_init(SpriteRenderer *r)
{
	PtrVector_init(&r->visible_sprites);
}

static Bool SpriteRenderer_set_visible(SpriteRenderer *r, SpritePlacement *s)
{
	return PtrVector_push_back(&r->visible_sprites, s);
}

typedef struct DrawingState
{
	OamState *oam;
	unsigned next_index;
}
DrawingState;

static void draw_sprite(void *element, void *user)
{
	DrawingState * const state = user;
	SpritePlacement const * const sprite = element;
	oamSet(state->oam, //main graphics engine context
		state->next_index++,           //oam index (0 to 127)  
		sprite->x, sprite->y,   //x and y pixle location of the sprite
		0,                    //priority, lower renders last (on top)
		0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
		SpriteSize_32x32,     
		SpriteColorFormat_256Color, 
		sprite->pixels,                  //pointer to the loaded graphics
		-1,                  //sprite rotation data  
		false,               //double the size when rotating?
		false,			//hide the sprite?
		false, false, //vflip, hflip
		false	//apply mosaic
		);
}

static void SpriteRenderer_draw(SpriteRenderer const *r, OamState *oam)
{
	DrawingState state;
	state.oam = oam;
	state.next_index = 0;
	PtrVector_for_each(&r->visible_sprites, draw_sprite, &state);
}

int main(void)
{
	size_t i;
	SpriteRenderer sprite_renderer;
	SpritePlacement sprites[48];
	SpriteRenderer_init(&sprite_renderer);

	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	oamInit(&oamMain, SpriteMapping_1D_32, false);
	oamInit(&oamSub, SpriteMapping_1D_32, false);

	u16* gfx = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	VERIFY(gfx);
	memcpy(gfx, grass_32Bitmap, grass_32BitmapLen);
	memcpy(SPRITE_PALETTE, grass_32Pal, grass_32PalLen);

	u16* gfxSub = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
	VERIFY(gfxSub);
	memcpy(gfxSub, grass_32Bitmap, grass_32BitmapLen);
	memcpy(SPRITE_PALETTE_SUB, grass_32Pal, grass_32PalLen);

	for (i = 0; i < sizeof(sprites) / sizeof(sprites[0]); ++i)
	{
		SpritePlacement * const sprite = sprites + i;
		sprite->x = 32 * (i % 8);
		sprite->y = 32 * (i / 8);
		sprite->pixels = gfx;
		VERIFY(SpriteRenderer_set_visible(&sprite_renderer, sprite));
	}

	for (;;)
	{
		SpriteRenderer_draw(&sprite_renderer, &oamMain);

		oamSet(&oamSub,
			0, 
			64, 
			32, 
			0, 
			0,
			SpriteSize_32x32, 
			SpriteColorFormat_256Color, 
			gfxSub, 
			-1, 
			false, 
			false,			
			false, false, 
			false	
			);

		swiWaitForVBlank();

		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}
}
