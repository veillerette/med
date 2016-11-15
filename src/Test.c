#include "../include/Test.h"

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
		v4 = rand()%15, v5 = rand()%100;
	
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

int Test_ConvertStringToId(void)
{
	char msg[150] = "Test_ConvertStringToID > ";
	int r = 1;
	
	char v1 = ConvertStringToID("c2");
	char v2 = ConvertStringToID("C2");
	char v3 = ConvertStringToID("E4");
	char v4 = ConvertStringToID("gs7");
	char v5 = ConvertStringToID("fff6");
	
	if(v1 != 36)
	{
		sprintf(msg, "%s Error in conversion (expected %d but found %d)\n", msg, 36, v1);
		r &= 0;
	}
	if(v2 != 36)
	{
		sprintf(msg, "%s Error in conversion (expected %d but found %d)\n", msg, 36, v2);
		r &= 0;
	}
	if(v3 != 64)
	{
		sprintf(msg, "%s Error in conversion (expected %d but found %d)\n", msg, 64, v3);
		r &= 0;
	}
	if(v4 != 104)
	{
		sprintf(msg, "%s Error in conversion (expected %d but found %d)\n", msg, 104, v4);
		r &= 0;
	}
	if(v5 != 87)
	{
		sprintf(msg, "%s Error in conversion (expected %d but found %d)\n", msg, 87, v5);
		r &= 0;
	}
	if(!r)
		colorprintf(RED, "%s", msg);
	
	return r;
}

int Test_StepInit(void)
{
	char msg[150] = "Test_StepInit > ";
	int r = 1;
	Step *step = NULL;
	
	step = Step_Alloc(4, NOIRE, CLE_SOL, 0, STEP_DEFAULT);
	if(NULL == step)
		return 0;
	
	Step_Init(step);
	
	if(NULL == step->notes)
	{
		sprintf(msg, "%s Step->notes not initialised (expected != NULL but found %p)\n", msg, (void *)step->notes);
		r &= 0;
	}
	if(step->notes != NULL && step->notes->note->note != 0)
	{
		sprintf(msg, "%s Error in value of the note (expected %d but found %d)\n", msg, 0,
										step->notes->note->note);
		r &= 0;
	}
	if(step->notes != NULL && step->notes->note->duration != RONDE)
	{
		sprintf(msg, "%s Error in duration of the note (expected %d but found %d)\n", msg, RONDE,
										step->notes->note->duration);
		r &= 0;
	}
	if(step->notes != NULL && step->notes->note->flags != NOTE_DEFAULT)
	{
		sprintf(msg, "%s Error in flags of the note (expected %d but found %d)\n", msg, NOTE_DEFAULT,
										step->notes->note->flags);
		r &= 0;
	}
	if(step->notes != NULL && step->notes->note->rest != 1)
	{
		sprintf(msg, "%s Error in rest of the note (expected %d but found %d)\n", msg, 1,
										step->notes->note->rest);
		r &= 0;
	}
	if(step->notes != NULL && step->notes->next != NULL)
	{
		sprintf(msg, "%s Error : Too much notes (next expected NULL but found %p)\n", msg,
										(void*)step->notes->next);
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
	
	printf("ConvertStringToID() ");
	r &= (l = Test_ConvertStringToId());
	if(l) colorprintf(GREEN, " Ok !\n");
	
	printf("Step_Init() ");
	r &= (l= Test_StepInit());
	if(l) colorprintf(GREEN, " Ok!\n");
	
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
