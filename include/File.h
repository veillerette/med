#ifndef __HEAD_FILE__
#define __HEAD_FILE__

#include "Staff.h"
#include <string.h>

/*********************************
	   DEV FUNCTIONS
**********************************/

/* All these functions use a binary FILE */

char *File_GetExt(const char *name);

int File_WriteFirstHead(FILE *f);

int File_ReadFirstHead(FILE *f);

int File_WriteNote(FILE *f, Note *note);

int File_ReadNote(FILE *f, Note **note);

int File_WriteAllNotes(FILE *f, Step *step);

int File_ReadAllNotes(FILE *f, ToNote **notes);

int File_WriteStep(FILE *f, Step *step);

int File_ReadStep(FILE *f, Step **step);

int File_WriteStaff(FILE *f, Staff *staff);

int File_ReadStaff(FILE *f, Staff **staff);

int File_WriteScore(FILE *f, Score *score);

int File_ReadScore(FILE *f, Score **score);

/*********************************
	  MAIN FUNCTIONS
**********************************/
int File_OpenScore(const char *path, Score **dest);

int File_SaveScore(const char *path, Score *score);


#endif
