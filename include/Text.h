#ifndef __HEAD_TEXT__
#define __HEAD_TEXT__

#include "Window.h"

typedef enum{
	TEXT_LEFT	= 0x01,
	TEXT_CENTER	= 0x02,
	TEXT_RIGHT	= 0x04
	} Text_Align;

typedef enum{
	TEXT_SOLID	= 0x01,
	TEXT_BLENDED	= 0x02
	} Text_Style;


char *Str_Copy(const char *str);

int Moteur_WriteText(int x, int y, const char *text, int size, 
				const char *path_font, SDL_Color color, 
				Text_Style graphic, Text_Align align, 
				SDL_Surface *dest);

int Moteur_WriteParagraph(int x, int y, int max_w, const char *text_paragraph, int size, int esp,
				const char *path_font, SDL_Color color, 
				Text_Style graphic, Text_Align align, 
				SDL_Surface *dest);
#endif
