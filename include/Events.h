#ifndef __HEAD_EVENTS__
#define __HEAD_EVENTS__

#include <stdio.h>
#include <stdlib.h>

#include "Step.h"
#include "Staff.h"
#include "System.h"
#include "Images.h"
#include "Window.h"

#define QUIT -42

#define FORCE_SCOREMAJ 19
#define FORCE_MAJ 17
#define SELECT 2
#define HOVER 1
#define NONE 0

typedef enum {
	OBJECT_NOTE	= 0x0001,
	OBJECT_LINE	= 0x0002,
	OBJECT_CLE	= 0x0004,
	OBJECT_SIGN	= 0x0008,
	OBJECT_STEP	= 0x0010,
	OBJECT_SHEET	= 0x0020,
	EVENT_ADDNOTE	= 0x0040,
	OBJECT_ALL	= 0xFFFF
	} Object_Type;

typedef enum {
	MODE_EDIT,
	MODE_ADD
	} Mode;

typedef struct Area Area;
struct Area
{
	SDL_Rect 	rect;
	Object_Type 	type;
	Area *		next;
	int		nbody;
	union 
	{
		struct /* OBJECT_NOTE */
		{
			Step *	step;
			int 	id_note;
		};
		
		struct /* OBJECT_STEP */
		{
			Staff *	staff;
			int 	id_step;
			int 	id_note_add; /* only when type=EVENT_ADDNOTE */
			int 	id_staff;
		};
		
		Sign **sign; /* OBJECT_SIGN */
		
		/* OBJECT_SHEET & OBJECT_LINE => Window */
	};
};

typedef struct Toolbar Toolbar;
struct Toolbar
{
	char 		sharp;
	char 		doublesharp;
	char 		flat;
	char 		doubleflat;
	int 		statusdur;
	char 		rest;	
	char		hover_button1;
	char		hover_button2;
	char		hover_button3;
	char		hover_tempo;
	char		hover_volume;
	char		natural;
	Note_Duration 	duration;
};

typedef struct Select_Node Select_Node;
struct Select_Node
{
	Area *val;
	Select_Node *next;
};

typedef struct Selection Selection;
struct Selection
{
	Select_Node *first;
	Select_Node *last;
};

typedef struct EventData EventData;
struct EventData
{
	Area *		lst; /* ordonned by surface size */
	int 		n;
	
	Area *		hover;
	SDL_Rect *	base;
	
	Score *		score;
	
	double 		r; /* zoom */
	
	Mode 		mode;
	Toolbar 	tools;
	Area *		lastArea;
	
	Selection *	tabselect;
	int 		ctrl;
};

Area *Area_Set(SDL_Rect rect, int nbody, Object_Type type, ...);

EventData *EventData_Alloc(void);

void EventData_Free(EventData **ed);

int EventData_Add(EventData *ed, Area *area);

void EventData_Console(EventData *ed);

int EventData_SetBase(EventData *ed, SDL_Rect *base);

int Events_PollMouse(SDL_Event event);

int Events_PollKeyboard(SDL_Event event);

int EventData_SetZoom(EventData *ed, double zoom);

int EventData_Flush(EventData *ed);

Area *Events_GetAreaByPixelAndType(int x, int y, Object_Type type);

int PixelInRect(int x, int y, SDL_Rect rect);

void MainEvents_AssociateScore(Score *score);

Selection *Selection_Alloc(void);

int Select_AddFirst(Area *area);

int Select_AddLast(Area *area);

void Select_Flush(void);

int Select_Remove(Area *area);

int Select_isIn(Area *area);

int Select_isEmpty(void);

Select_Node *Select_GetIterate(void);

Select_Node *Select_GetLast(void);

int Select_isOne(void);

void Select_Debug(void);

extern EventData *main_events;

#endif
