#ifndef __HEAD_ABC__
#define __HEAD_ABC__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Staff.h"

/*********************************
	   DEV FUNCTIONS
**********************************/

static int ABC_ScanHead(FILE *f, char *field, char *text);

static void ABC_replaceSlash(char *str);

static int ABC_FindKeyFromStr(char *text);

static int ABC_isNote(char note);

static int ABC_ParseHeader(Score *score, FILE *f);

int nextcar(FILE *f);

char *File_GetLine(FILE *f);

int ABC_IsAnHeader(const char *line);

int ABC_IsAGoodHeader(const char *line);

int ABC_MultiSimplify(char *buf);

char *ABC_TransformLine(const char *line, int isHeader);

int ABC_AppendStr(char **dest, const char *source);

int File_SimplifyABC(const char *destPath, const char *sourcePath);

extern Score *ABC_ParseFile(const char *path);


/*********************************
	  MAIN FUNCTIONS
**********************************/

extern Score *ABC_OpenABC(const char *path);

#endif
