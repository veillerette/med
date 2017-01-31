#ifndef __HEAD_ABC__
#define __HEAD_ABC__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Staff.h"

/*********************************
	   DEV FUNCTIONS
**********************************/

int ABC_ScanHead(FILE *f, char *field, char *text);

void ABC_replaceSlash(char *str);

int ABC_FindKeyFromStr(char *text);

int ABC_isNote(char note);

int ABC_ParseHeader(Score *score, FILE *f);

int nextcar(FILE *f);

char *File_GetLine(FILE *f);

int ABC_IsAnHeader(const char *line);

int ABC_IsAGoodHeader(const char *line);

int ABC_MultiSimplify(char *buf);

char *ABC_TransformLine(const char *line, int isHeader);

int ABC_AppendStr(char **dest, char *source);

int File_SimplifyABC(const char *destPath, const char *sourcePath);

extern Score *ABC_ParseFile(const char *path);

int ABC_WriteStep(FILE *f, Step *step, Note_Duration base_l);

int ABC_WriteHeaderScore(FILE *f, Score *score, Note_Duration basel);

int ABC_WriteDevScore(FILE *f, Score *score);



/*********************************
	  MAIN FUNCTIONS
**********************************/

extern Score *ABC_OpenABC(const char *path);

int ABC_WriteScore(const char *path, Score *score);


extern char abc_error[50];

#endif
