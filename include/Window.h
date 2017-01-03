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
#include "Events.h"
#include "Text.h"

#define ESP_BODY 100
#define BASE_BODY_X (Window->pos_pal->x + Window->pos_pal->w)
#define BASE_BODY_Y (Window->pos_menu->y + Window->pos_menu->h)
#define ABS(A) (((A)<0)?-(A):(A))
#define TestOK(); if(!Window_OK())return 0;
#define SIZE_BODY (Window->body[0]->w + ESP_BODY)
#define NPAGE (*(Window->body + nbody))

#define FONT_MESURE "media/Hack-Regular.ttf"

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
	
	SDL_Surface **body; /* The bigs surfaces (pages) */
	SDL_Surface **body_use; /* Surface for uses (pages) */
	double ratio;
	int nb_body;
	
	SDL_Rect *pos_body; /* base of first page */
	
	int _linked;
	SDL_Rect *pos_link;
	
};

WindowData *WindowData_Alloc(void);

void WindowData_Free(WindowData **window);

int Window_GetScreenSize(int *width, int *height);

int Window_Init(void);

void Window_Quit(void);

int Window_InitBody();

void SDL_FreeRect(SDL_Rect **rect);

int Window_CreateWindow(int width, int height, const char *title);

int Window_ClearWindow(Color color);

int Window_Print(void);

int Window_ApplyZoomOnRect(SDL_Rect *rect, double zoom, double old);

int Window_ApplyZoom(double zoom);

int Window_DrawBody();

void Window_Staff(SDL_Surface *dest, int x, int y, int w);

int Window_OK(void);

int Window_AddEmptyBody();

/*int Staff_Print(Staff *staff, SDL_Rect *base_pos);*/

int Score_Print(Score *score, SDL_Rect *base_pos);

void Window_DrawStaff(int x, int y, int x_end, SDL_Surface *dest);

int Window_LittleEvent(SDL_Event event, double *r, int *c, int *mouse,
					int *clic_x, int *clic_y, int *tomaj, 
					int *m);
					
int Window_TestBox(SDL_Surface *dest, SDL_Rect *pos, int zoom);

extern WindowData *Window;



#endif
