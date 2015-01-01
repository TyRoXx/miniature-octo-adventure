#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H


#include <SDL_ttf.h>
#include "base/ptr_vector.h"


typedef size_t FontID;

typedef struct FontManager
{
	PtrVector fonts;
}
FontManager;

void FontManager_init(FontManager *m);
Bool FontManager_load(FontManager *m, MemoryManager fonts_memory, char const *font_directory);
TTF_Font *FontManager_find_font(FontManager const *m, FontID id);
void FontManager_free(FontManager *m, Deallocator fonts_deallocator);


#endif
