#ifndef __HEAD_IMAGES__
#define __HEAD_IMAGES__

#include <stdio.h>
#include <stdlib.h>


#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_rotozoom.h>

#include "System.h"

typedef struct Graphics Graphics;
struct Graphics
{
	SDL_Surface **notes_SoloBasic;
	SDL_Surface **notes_SoloInv;
	
	SDL_Surface *line;
};

SDL_Surface *Notes_SoloBasicNoire(double size, double angle);

void test_note(double size, double angle, int posx, int posy, SDL_Surface *screen);




#endif
