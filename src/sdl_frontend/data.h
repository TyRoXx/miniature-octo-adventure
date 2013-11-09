#ifndef DATA_H
#define DATA_H


#include "image_manager.h"
#include "appearance.h"
#include "font_manager.h"


typedef struct Data
{
	ImageManager images;
	AppearanceManager appearances;
	FontManager fonts;
}
Data;


Bool Data_init(Data *d, char const *directory, SDL_PixelFormat *format);
void Data_free(Data *d);


#endif
