#ifndef __HEAD_STEP__
#define __HEAD_STEP__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "System.h"

typedef enum {
	NOTE_DEFAULT 		= 0,
	NOTE_SHARP		= 0x01,
	NOTE_DOUBLESHARP	= 0x02,
	NOTE_FLAT		= 0x04,
	NOTE_DOUBLEFLAT		= 0x08,
	NOTE_POINTED		= 0x10,
	NOTE_DOUBLEPOINTED	= 0x20,
	NOTE_NATURAL		= 0x40,
	NOTE_LINKED		= 0x80
	} Note_Flags;
	
typedef enum {
	STEP_DEFAULT		= 0
	/*** ***/
	} Step_Flags;

typedef enum {
	RONDE 		= 1,
	BLANCHE		= 2,
	NOIRE		= 4,
	CROCHE		= 8,
	DOUBLECROCHE	= 16,
	TRIPLECROCHE	= 32,
	QUADRUPLECROCHE	= 64
	} Note_Duration;

typedef enum {
	CLE_SOL 	= 1,
	CLE_FA 		= 2,
	CLE_UT3		= 3,
	CLE_UT4		= 4
	} Cle;

typedef struct Note Note;
struct Note
{
	char note; /* A0 = 21, B1 = 22... C8 = 108*/
	char rest; /* bool 0 = note, 1 = rest */
	
	Note_Flags flags;
	Note_Duration duration;
};

typedef struct ToNote ToNote;
struct ToNote
{
	Note *note;
	ToNote *next;
};

typedef struct Step Step;
struct Step
{
	ToNote *notes;
	
	int num;
	
	Cle cle;
	Note_Duration den;
	Step_Flags flags;
	
	signed char sign;
};

Note *Note_Alloc(char note, Note_Flags flags, Note_Duration duration, char rest);

void Note_Free(Note **note);

int Note_RealDuration(Note *note, Step *step);

char ConvertStringToID(const char *note);

Note *Note_CreateNoteFromString(const char *note, Note_Duration duration);

ToNote *ToNote_Alloc(char note, Note_Flags flags, int duration, char rest);

void ToNote_Free(ToNote **tonote);

void ToNote_FreeAll(ToNote **tonote);

Step *Step_Alloc(int num, Note_Duration den, Cle cle, char sign, Step_Flags flags);

void Step_Free(Step **step);

int ToNote_ChangeRestStatus(ToNote *notes, int id, char newStatus);

int Step_ChangeRestStatus(Step *step, int id, char newStatus);

int ToNote_DiviseRest(ToNote *tonote, int id, int newDuration, int stepNum);

int Step_DiviseRest(Step *step, int id);

int Step_Init(Step *step);

int Step_Change(Step *step, int num, Note_Duration den, Cle cle, Step_Flags flags, char sign);
						
void Note_ConsolePrintf(Note *note);

void Step_ConsoleFastPrintf(Step *step);

void ToNote_ConsolePrintf(ToNote *notes);

void Step_ConsolePrintf(Step *step);

int ToNote_Regularise(ToNote *notes);

int Step_Regularise(Step *step);

int find2min(int n);

int Step_AddNote(Step *step, int id, char note, Note_Flags flags, Note_Duration duration);

int Step_DelLocal(Step *step, int begin, int end);

int Step_Verif(Step *step);

Cle Cle_GetFromString(const char *str);

char *Cle_GetFromId(Cle cle);

int ToNote_Transpose(ToNote *tonote, char value);

int Step_Transpose(Step *step, char value);

Note_Duration Step_GetMinDuration(Step *step);

Note *Step_GetNote(Step *step, int id_note);
#endif




