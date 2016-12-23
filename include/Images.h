#ifndef __HEAD_IMAGES__
#define __HEAD_IMAGES__

#include <stdio.h>
#include <stdlib.h>


#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_rotozoom.h>

#include "System.h"

#define HEAD_W 45
#define HEAD_H 30
#define QUEUE ((int)(HEAD_H * 2.8))
#define STAFF_H 2
#define QUEUE_BORDER 5
#define NOTE_SPACE 40

#define RATIO_LITTLE 2

typedef struct Graphics Graphics;
struct Graphics
{
	/* Têtes de Notes */
	SDL_Surface *Note_headBlack; /* Noires */
	SDL_Surface *Note_headWhite; /* Blanche */
	SDL_Surface *Note_headWhole; /* Ronde */
	int rot_noteW;
	int rot_noteH;
	
	/* Blanche et Noire */
	SDL_Surface *Note_White;
	SDL_Surface *Note_Black;
	SDL_Rect *note1_center;
	
	SDL_Surface *Note_Crotchet;
	SDL_Surface *Note_CrotchetInv;
	
	/* Rest */
	SDL_Surface *Rest_Long;
	SDL_Rect *pos_Long;
	
	SDL_Surface *Rest_BreveLong;
	SDL_Rect *pos_BreveLong;
	
	SDL_Surface *Rest_Crotchet;
	SDL_Rect *pos_restCrotchet;
	
	SDL_Surface *Rest_Quaver;
	SDL_Rect *pos_Quaver;
	
	SDL_Surface *Rest_Breve;
	
	SDL_Surface *Sharp;
	SDL_Surface *Flat;
	SDL_Surface *DoubleFlat;
};


typedef struct Color Color;
struct Color
{
	Uint r : 8;
	Uint g : 8;
	Uint b : 8;
	Uint a : 8;
};



Color SetColorA(int r, int g, int b, int a);

Color SetColor(int r, int g, int b);

SDL_Surface *SDL_CreateSurface(int w, int h);

int Draw_Border(SDL_Surface *surf, int size);

void test_note(double size, double angle, int posx, int posy, SDL_Surface *screen);

int Graphics_LoadAll(void);

void Graphics_Quit(void);

SDL_Surface *SDL_CreateWhiteKeySurface(int w, int h);

int Images_DrawRotNote(SDL_Surface *surf, int x, int y, SDL_Surface *dest);

SDL_Rect *SDL_SetRect(int x, int y, int w, int h);

int PowerOfBezier(SDL_Surface *dest, const int *x, const int *y, int n, Color color);


extern Graphics *Images;
extern Graphics *HoverImages;
extern Graphics *LittleImages;

#endif
