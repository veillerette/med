#ifndef __HEAD_SYSTEM__
#define __HEAD_SYSTEM__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>


#define memtest(ptr) (_mem((void*)ptr, #ptr, __FILE__, __LINE__))

typedef enum{
	BLACK 	= 30,
	RED	= 31,
	GREEN	= 32,
	YELLOW	= 33,
	BLUE	= 34,
	MAGENTA	= 35,
	CYAN	= 36,
	WHITE	= 37
	} TerminalColor;

typedef unsigned int Uint;
	
extern void _mem(void *ptr, const char *str, const char *file, int line);

extern int text_color(TerminalColor color);

extern int colorprintf(TerminalColor color, const char *format, ...);



#endif
