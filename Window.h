#ifndef __HEAD_WINDOW__
#define __HEAD_WINDOW__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>

#include "System.h"
#include "Step.h"
#include "Staff.h"
#include "Images.h"

#define BASE_MOTION 2;

#define TestOK(); if(!Window_OK())return 0;
typedef enum {
	STATE_ALLOC,
	STATE_SDLINIT,
	STATE_TTFINIT,
	STATE_WINDOWED,
	STATE_ERROR,
	STATE_QUIT
	} Window_State;


typedef struct WindowData WindowData;
struct WindowData
{
	SDL_Surface *screen;
	
	int width;
	int height;
	int max_width;
	int max_height;
	
	Window_State state;
	
	/* 3 main surfaces of the screen */
	SDL_Surface *menu; /* top */
	SDL_Rect *pos_menu;
	
	SDL_Surface *pal; /* left */
	SDL_Rect *pos_pal;
	
	SDL_Surface *body; /* the sheet */
	SDL_Rect *pos_body;
};


	
typedef struct Object Object;
struct Object
{
	int type;
	void *ptr;
	SDL_Rect *pos;
};


WindowData *WindowData_Alloc(void);

void WindowData_Free(WindowData **window);

int Window_Init(void);

void Window_Quit(void);

int Window_CreateWindow(int width, int height, const char *title);

int Window_ClearWindow(Color color);

int Window_WaitMouse(int *x, int *y);

int Window_Print(void);

void SDL_FreeRect(SDL_Rect **rect);

void Window_ShowAllGraphics(void);

int Window_DrawBodyShrink(double ratio);

int Window_DrawBodyShrink2(double ratio, SDL_Rect redim, SDL_Rect pos);

int Step_Print(Step *step, SDL_Rect *base_pos, SDL_Surface *dest);

int Staff_Print(Staff *staff, SDL_Rect *base_pos, SDL_Surface *dest);

int ClicInRect(int x, int y, SDL_Rect *rect);

extern WindowData *Window;

#endif
