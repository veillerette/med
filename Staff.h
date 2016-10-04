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

/*
typedef struct Score Score;
struct Score
{
	Staff *lst;
	int n;
	
	char *name;
	int data;
};
*/

Staff *Staff_Alloc(const char *name);

int Staff_Init(Staff *staff, int num, Note_Duration den, int cle, char sign);

void Staff_Free(Staff **staff);

void Staff_VerifAlloc(Staff *staff);

int Staff_AddEmptyStep(Staff *staff);

int Staff_InsereEmptyStep(Staff *staff, int pos);

int Staff_DeleteStep(Staff *staff, int pos);

int Staff_Transpose(Staff *staff, char value);


#endif
