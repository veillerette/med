#include "Test.h"

/************** NOTE *****************/

Note_Duration random_note(void)
{
	return (Note_Duration)pow(2, rand()%7);
}

int Test_NoteAlloc(void)
{
	char msg[150] = "Test_NoteAlloc > ";
	Note *note = NULL;
	int r=1;
	int v1 = rand()%100, v2 = rand()%100;
	Note_Duration n = random_note();
	
	note = Note_Alloc(v1, v2, n, 0);

	if(NULL == note)
	{
		sprintf(msg, "%s Note not allocated\n", msg);
		r &= 0;
	}
	if(note != NULL && note->note != v1)
	{
		sprintf(msg, "%s Note->note error (%d != %d)\n", msg, note->note, v1);
		r &= 0;
	}
	if(note != NULL && note->flags != v2)
	{
		sprintf(msg, "%s Note->flags error (%d != %d)\n", msg, note->flags, v2);
		r &= 0;
	}
	if(note != NULL && note->duration != n)
	{
		sprintf(msg, "%s Note->duration error (%d != %d)\n", msg, 
							note->duration, n);
		r &= 0;
	}
	if(note != NULL && note->rest != 0)
	{
		sprintf(msg, "%s Note->rest error (%d != %d)\n", msg, 
							note->rest, 0);
		r &= 0;
	}
	
	if(!r)
		colorprintf(RED, "%s", msg);
		
	Note_Free(&note);
	
	return r;
}

int Test_StepAlloc(void)
{
	char msg[150] = "Test_StepAlloc > ";
	Step *step = NULL;
	int r = 1;
	int v1 = rand()%20, v2 = rand()%20, v3 = rand()%4,
		v4 = rand()%15 - 10, v5 = rand()%100;
	
	step = Step_Alloc(v1, v2, v3, v4, v5);

	if(NULL == step)
	{
		sprintf(msg, "%s Step not allocated\n", msg);
		r &= 0;
	}
	if(step != NULL && step->num != v1)
	{
		sprintf(msg, "%s step->num error (%d != %d)\n", msg, step->num, v1);
		r &= 0;
	}
	if(step != NULL && step->den != v2)
	{
		sprintf(msg, "%s step->den error (%d != %d)\n", msg, step->den, v2);
		r &= 0;
	}
	if(step != NULL && step->cle != v3)
	{
		sprintf(msg, "%s step->cle error (%d != %d)\n", msg, 
							step->cle, v3);
		r &= 0;
	}
	if(step != NULL && step->sign != v4)
	{
		sprintf(msg, "%s step->sign error (%d != %d)\n", msg, 
							step->sign, v4);
		r &= 0;
	}
	if(step != NULL && step->flags != v5)
	{
		sprintf(msg, "%s step->flags error (%d != %d)\n", msg, 
							step->flags, v5);
		r &= 0;
	}
	
	if(!r)
		colorprintf(RED, "%s", msg);
	
	Step_Free(&step);
	
	return r;
}

int Test_NoteFree(void)
{
	char msg[150] = "Test_NoteFree > ";
	int r = 1;
	
	Note *note = Note_Alloc(13, 37, 42, 1);
	Note_Free(&note);
	
	if(note != NULL)
	{
		sprintf(msg, "%s note not freed (%p)\n", msg, 
							(void *)note);
		r &= 0;
	}
	if(!r)
		colorprintf(RED, "%s", msg);
	
	return r;
}

int Test_StepFree(void)
{
	char msg[150] = "Test_StepFree > ";
	int r = 1;
	
	Step *step = Step_Alloc(10, 11, 2, 2, 1);
	Step_Free(&step);
	
	if(step != NULL)
	{
		sprintf(msg, "%s step not freed (%p)\n", msg, 
							(void *)step);
		r &= 0;
	}
	if(step != NULL && step->notes != NULL)
	{
		sprintf(msg, "%s step->notes not freed (%p)\n", msg, 
							(void *)step->notes);
		r &= 0;
	}
	if(!r)
		colorprintf(RED, "%s", msg);
	
	return r;
}

int Test_ToNoteAlloc(void)
{
	char msg[150] = "Test_ToNoteAlloc > ";
	ToNote *tonote = NULL;
	int r = 1;
	int v1 = rand()%20, v2 = rand()%20, v3 = rand()%4,
		v4 = rand()%15;
	
	tonote = ToNote_Alloc(v1, v2, v3, v4);
	
	if(NULL == tonote)
	{
		sprintf(msg, "%s ToNote not allocated\n", msg);
		r &= 0;
	}
	if(tonote != NULL && NULL != tonote->next)
	{
		sprintf(msg, "%s Note->next error (%p != %p)\n", msg, (void *)tonote->next, NULL);
		r &= 0;
	}
	if(tonote != NULL && NULL == tonote->note)
	{
		sprintf(msg, "%s  ToNote error (%p == %p)\n", msg, (void *)tonote->note, NULL);
		r &= 0;
	}
	
	if(!r)
		colorprintf(RED, "%s", msg);
		
	ToNote_Free(&tonote);
	
	return r;
}

int Test_ToNoteFree(void)
{
	char msg[150] = "Test_ToNoteFree > ";
	int r = 1;
	
	ToNote *tonote = ToNote_Alloc(10, 11, 12, 13);
	ToNote_Free(&tonote);
	
	if(tonote != NULL)
	{
		sprintf(msg, "%s ToNote not freed (%p)\n", msg, 
							(void *)tonote);
		r &= 0;
	}
	if(!r)
		colorprintf(RED, "%s", msg);
	
	return r;
}



extern int Test_Main(void)
{
	int r = 1, l = 1;
	
	srand(time(NULL));
	
	printf("Note_Alloc() ");
	r &= (l = Test_NoteAlloc());
	if(l) colorprintf(GREEN, " Ok !\n");
	
	printf("Note_Free() ");
	r &= (l = Test_NoteFree());
	if(l) colorprintf(GREEN, " Ok !\n");
	
	printf("Step_Alloc() ");
	r &= (l = Test_StepAlloc());
	if(l) colorprintf(GREEN, " Ok !\n");
	
	printf("Step_Free() ");
	r &= (l = Test_StepFree());
	if(l) colorprintf(GREEN, " Ok !\n");
	
	printf("ToNote_Alloc() ");
	r &= (l = Test_ToNoteAlloc());
	if(l) colorprintf(GREEN, " Ok !\n");
	
	printf("ToNote_Free() ");
	r &= (l = Test_ToNoteFree());
	if(l) colorprintf(GREEN, " Ok !\n");
	
	
	if(r)
	{
		colorprintf(GREEN,"\n|--------------------|\n");
		colorprintf(GREEN,  "| Each test says OK! |\n");
		colorprintf(GREEN,  "|--------------------|\n");
	}
	else
	{
		colorprintf(RED, "\n|---------------------|\n");
		colorprintf(RED,   "| At least one not OK |\n");
		colorprintf(RED,   "|---------------------|\n");
	}
	
	return r;
}
