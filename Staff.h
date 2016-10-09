#ifndef __HEAD_STAFF__
#define __HEAD_STAFF__

#include "System.h"
#include "Step.h"

#define STAFF_BASE 10

typedef struct Staff Staff;
struct Staff
{
	Step **steps; /* dynamic tab */
	int n;
	int max;
	
	char *name;
};

typedef enum {
	SIGN_TEMPO	= 0x01,
	SIGN_VELOCITY	= 0x02,
	SIGN_AUTHOR	= 0x04,
	SIGN_TITLE	= 0x08
	/* **** **/
	} Sign_Type;

/* Additionnal Signs (tempo, playing...) */
typedef struct Sign Sign;
struct Sign
{
	Sign_Type type : 16;
	Uint value : 16;
	Uint time;
	char *text;
	
	Sign *next;
};

typedef struct Score Score;
struct Score
{
	Staff *lst;
	int n;
	
	int data;
	Sign *signs;
};

Staff *Staff_Alloc(const char *name);

int Staff_Init(Staff *staff, int num, Note_Duration den, int cle, char sign);

void Staff_Free(Staff **staff);

void Staff_VerifAlloc(Staff *staff);

int Staff_AddEmptyStep(Staff *staff);

int Staff_InsereEmptyStep(Staff *staff, int pos);

int Staff_DeleteStep(Staff *staff, int pos);

int Staff_Transpose(Staff *staff, char value);

Sign *Sign_Alloc(Sign_Type type, Uint value, Uint time, char *text);

void Sign_Free(Sign **sign);

void Sign_FreeAll(Sign **sign);

Score *Score_Alloc(void);

void Score_Free(Score **score);

int Score_AddSign(Score *score, Sign_Type type, Uint value, Uint time, char *text);

int Score_ShowSignConsole(Score *score);

int Score_DeleteSign(Score *score, Sign_Type type, Uint time);

int Score_ShowSignConsole(Score *score);


#endif
