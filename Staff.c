#include "Staff.h"

Staff *Staff_Alloc(const char *name)
{
	Staff *temp = (Staff *)malloc(sizeof(Staff));
	memtest(temp);
	
	temp->steps = (Step **)malloc(sizeof(Step *) * STAFF_BASE);
	memtest(temp->steps);
	
	temp->n = 0;
	temp->max = STAFF_BASE;
	temp->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
	memtest(temp->name);
	strcpy(temp->name, name);
	
	return temp;
}

int Staff_Init(Staff *staff, int num, Note_Duration den, int cle, char sign)
{
	int i;
	if(NULL == staff)
		return 0;
	
	if(0 == staff->n && STAFF_BASE == staff->max)
		staff->n = STAFF_BASE;
	
	for(i = 0; i < staff->n; i++)
	{
		if(*(staff->steps + i) != NULL)
			*(staff->steps + i) = Step_Alloc(num, den, cle, sign, STEP_DEFAULT);
		Step_Init(*(staff->steps + i));
	}
	return 1;
}

void Staff_Free(Staff **staff)
{
	int i;
	if(*staff != NULL)
	{
		if((*staff)->steps != NULL)
		{
			for(i = 0; i < (*staff)->n; i++)
			{
				Step_Free(&(*((*staff)->steps + i)));
			}
			free((*staff)->steps);
		}
		if((*staff)->name != NULL)
			free((*staff)->name);
		free(*staff);
		*staff = NULL;
	}
}

void Staff_VerifAlloc(Staff *staff)
{
	if(NULL == staff)
		return;
	
	if(0 == staff->n)
		return;

	if(staff->n == staff->max)
	{
		staff->steps = (Step **)realloc(staff->steps, staff->n * 2 * sizeof(Step *));
		memtest(staff->steps);
		
		staff->max *= 2;
	}
}

int Staff_AddEmptyStep(Staff *staff)
{
	Step *last = NULL;
	if(NULL == staff)
		return 0;
	
	if(0 == staff->n)
		return 0;

	Staff_VerifAlloc(staff);
	
	last = *(staff->steps + staff->n - 1);
	*(staff->steps + staff->n) = Step_Alloc(last->num, last->den, last->cle,
					last->sign, STEP_DEFAULT);
	Step_Init(*(staff->steps + staff->n));
	staff->n++;
	return 1;
}


int Staff_InsereEmptyStep(Staff *staff, int pos)
{
	int i;
	Step *last = NULL;
	if(NULL == staff)
		return 0;
	
	if(0 == staff->n || pos < 0)
		return 0;
	
	if(pos > staff->n - 1)
		return 0;
		
	Staff_VerifAlloc(staff);
	
	if(0 == pos)
		last = *staff->steps;
	else
		last = *(staff->steps - 1 + pos);
	
	if(NULL == last)
		return -1;
	
	for(i = staff->n; i >= pos + 1; i--)
		*(staff->steps + i) = *(staff->steps + i - 1);
	*(staff->steps + pos) = Step_Alloc(last->num, last->den, last->cle,
					last->sign, STEP_DEFAULT);
	Step_Init(*(staff->steps + pos));
	staff->n++;
	
	return 1;
}

int Staff_DeleteStep(Staff *staff, int pos)
{
	int i;
	if(NULL == staff)
		return 0;
	
	if(0 == staff->n || pos < 0)
		return 0;
	
	if(pos > staff->n - 1)
		return 0;
	
	Step_Free(&(*(staff->steps + pos)));
	for(i = pos; i < staff->n; i++)
		*(staff->steps + i)  = *(staff->steps + i + 1);
	staff->n--;
	return 1;
}




