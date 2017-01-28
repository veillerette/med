#ifndef __HEAD_FILE__
#define __HEAD_FILE__

#include "Staff.h"
#include <dirent.h>
#include <string.h>

typedef enum
{
	_FILE,
	_DIR
} Entry_Type;

typedef struct Entry Entry;
struct Entry
{
	char *name;
	Entry_Type type;
	
	int size;
};

typedef struct Directory Directory;
struct Directory
{	
	char *name;

	Entry **tab;
	int n;
	int m;
};

#ifndef DT_REG
#define DT_REG 8
#endif

#ifndef DT_DIR
#define DT_DIR 4
#endif

extern int scandir(const char *dirp, struct dirent ***namelist,
              int (*filter)(const struct dirent *),
              int (*compar)(const struct dirent **, const struct dirent **));

extern int alphasort(const struct dirent **a, const struct dirent **b);

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
	  FILE FUNCTIONS
**********************************/
int File_OpenScore(const char *path, Score **dest);

int File_SaveScore(const char *path, Score *score);



/*********************************
	DEV EXPLORER FUNCTIONS
**********************************/

Entry *Entry_Alloc(const char *name, Entry_Type type, int size);

void Entry_Free(Entry **entry);

Directory *Directory_Alloc(const char *name);

int Directory_Add(Directory *dir, Entry *entry);

void Directory_Debug(Directory *dir);


/*********************************
	EXPLORER FUNCTIONS
**********************************/

Directory *Directory_Create(const char *path);

int Directory_Change(Directory **dir, const char *newName);

void Directory_Free(Directory **dir);


#endif
