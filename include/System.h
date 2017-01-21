#ifndef __HEAD_SYSTEM__
#define __HEAD_SYSTEM__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#define SYS_VERSION 	0.1
#define SYS_NAME	"med"

#define log2(n) 	(log((n)) / log(2))
#define memtest(ptr) 	(_mem((void*)ptr, #ptr, __FILE__, __LINE__))

typedef unsigned int Uint;


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

typedef struct NodeList NodeList;
struct NodeList
{
	void *data;
	char *name;
	
	int famous;
	int id;
	
	void (*Node_Free)(void *);
	
	NodeList *next;
};

typedef struct GenList GenList;
struct GenList
{
	NodeList *lst;
	int n;
	int lst_famous;
};

typedef struct Point Point;
struct Point
{
	Uint x : 16;
	Uint y : 16;
};

typedef struct Dots Dots;
struct Dots
{
	Point **tab;
	int totalheight;
	int n;
};
	
extern void _mem(void *ptr, const char *str, const char *file, int line);

extern int text_color(TerminalColor color);

extern int colorprintf(TerminalColor color, const char *format, ...);

int Dots_Init(Dots **dt);

void Dots_Free(Dots **dt);

int Dots_Add(Dots *dt, Uint x, Uint y, int height);

int Dots_Length(Dots *dt);

double Dots_CalcCoef(Dots *dt);

double Dots_CalcExtremCoef(Dots *dt);

int Dots_isLinear(Dots *dt);

int Dots_EvaluateYFromX(Dots *dt, int x);
#endif
