#ifndef __HEAD_EVENTS__
#define __HEAD_EVENTS__

#include <stdio.h>
#include <stdlib.h>

#include "Step.h"
#include "Staff.h"
#include "System.h"
#include "Images.h"

typedef enum {
	OBJECT_NOTE	= 0x01,
	OBJECT_LINE	= 0x02,
	OBJECT_CLE	= 0x08,
	OBJECT_SIGN	= 0x40,
	OBJECT_STEP	= 0x04,
	OBJECT_SHEET	= 0x80
	} Object_Type;

typedef struct Area Area;
struct Area
{
	SDL_Rect 	rect;
	Object_Type 	type;
	Area *		next;
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
		};
		
		Sign **sign; /* OBJECT_SIGN */
		
		/* OBJECT_SHEET & OBJECT_LINE => Window */
	};
	
	
};

typedef struct EventData EventData;
struct EventData
{
	Area *lst; /* ordonned by surface size */
	int n;
	
	Area *select;
	Area *hover;
	SDL_Rect *base;
	
};

Area *Area_Set(SDL_Rect rect, Object_Type type, ...);

EventData *EventData_Alloc(void);

void EventData_Free(EventData **ed);

int EventData_Add(EventData *ed, Area *area);

void EventData_Console(EventData *ed);

int EventData_SetBase(EventData *ed, SDL_Rect *base);


extern EventData *main_events;

#endif
