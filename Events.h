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

typedef struct Object Object;
struct Object
{
	SDL_Rect area;
	Object_Type type;
	union 
	{
		struct /* OBJECT_NOTE */
		{
			Step *step_note;
			int id_note;
		};
		
		Step *step; /* OBJECT_STEP & OBJECT_CLE */
		Sign **sign; /* OBJECT_SIGN */
		
		/* OBJECT_SHEET & OBJECT_LINE => Window */
	};
	
	char hover;
	char select;
};

typedef struct Event Event;
struct Event
{
	
};

#endif
