#include <nds.h>
#include <stdio.h>
#include "grass_32.h"

int main(void)
{
	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	oamInit(&oamMain, SpriteMapping_1D_32, false);
	oamInit(&oamSub, SpriteMapping_1D_32, false);

	u16* gfx = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	memcpy(gfx, grass_32Bitmap, grass_32BitmapLen);
	memcpy(SPRITE_PALETTE, grass_32Pal, grass_32PalLen);

	u16* gfxSub = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
	memcpy(gfxSub, grass_32Bitmap, grass_32BitmapLen);
	memcpy(SPRITE_PALETTE_SUB, grass_32Pal, grass_32PalLen);

	for (;;)
	{
		oamSet(&oamMain, //main graphics engine context
			0,           //oam index (0 to 127)  
			10, 20,   //x and y pixle location of the sprite
			0,                    //priority, lower renders last (on top)
			0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
			SpriteSize_32x32,     
			SpriteColorFormat_256Color, 
			gfx,                  //pointer to the loaded graphics
			-1,                  //sprite rotation data  
			false,               //double the size when rotating?
			false,			//hide the sprite?
			false, false, //vflip, hflip
			false	//apply mosaic
			);

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
