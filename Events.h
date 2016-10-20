#ifndef __HEAD_EVENTS__
#define __HEAD_EVENTS__

#include <stdio.h>
#include <stdlib.h>

#include "System.h"

typedef enum {
	OBJECT_NOTE	= 0x01,
	OBJECT_LINE	= 0x02,
	OBJECT_CLE	= 0x08,
	OBJECT_SIGN	= 0x40,
	OBJECT_STEP	= 0x04,
	OBJECT_TITLE	= 0x10,
	OBJECT_AUTHOR	= 0x20,
	OBJECT_SHEET	= 0x80
	} Object_Type;

typedef struct Area Area;
struct Area
{
	SDL_Rect area;
	void *data;
	Object_Type type;
	
};

#endif
