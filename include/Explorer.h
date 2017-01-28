#ifndef __HEAD_EXPLORER__
#define __HEAD_EXPLORER__

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>

#include "Window.h"
#include "File.h"

#define GET_STR	5
#define EXPLORER_FONT "media/Garamond-light.ttf"

typedef struct Explorer Explorer;
struct Explorer
{
	SDL_Rect base;
	Directory *dir;
	SDL_Rect *pos;
	
	int hover;
	int select;
	SDL_Surface *dst;
	
	SDL_Rect valid;
	SDL_Rect dad;
	
	int header1;
	int header2;
	int header3;
	
	int line;
	
	SDL_Event last;
};


/*********************************
	   DEV FUNCTIONS
**********************************/

Explorer *Explorer_Create(int x_base, int y_base, int w, int h,
			int header1, int header2, int header3, SDL_Surface *dst);


void Explorer_Free(Explorer **e);

char *Str_Copy(const char *str);

char *Str_TopFolder(const char *path);

char *Str_GoInSub(const char *path, const char *sub);

int Explorer_ChangeDirectory(Explorer *e, int direct);

void Explorer_Header1(Explorer *e);

void Explorer_Header2(Explorer *e);

void Explorer_Header3(Explorer *e);

void Explorer_Body(Explorer *e);

int Explorer_PollEvent(Explorer *e, SDL_Event event);


/*********************************
	   MAIN FUNCTIONS
**********************************/

char *Explorer_FindPath(int x, int y, int w, int h, SDL_Surface *dest);

#endif
