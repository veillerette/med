#include "../include/Step.h"

Note *Note_Alloc(char note, Note_Flags flags, Note_Duration duration, char rest)
{
	Note *temp = (Note *)malloc(sizeof(Note));
	memtest(temp);
	
	temp->note = note;
	temp->flags = flags;
	temp->duration = duration;
	temp->rest = rest;
	
	return temp;
}

void Note_Free(Note **note)
{
	if(*note != NULL)
	{
		free(*note);
		*note = NULL;
	}
}


char ConvertStringToID(const char *note)
{
	char res = 0;
	int tab[] = {9, 11, 0, 2, 4, 5, 7};
	int n = strlen(note);
	if(NULL == note)
		return 0;
	
	if((note[n - 1] - '0') < 0 || (note[n - 1] - '0') > 8)
		return 0;
	
	if(note[0] >= 'A' && note[0] <= 'G')
		res += (tab[(note[0] - 'A')]);
	else if(note[0] >= 'a' && note[0] <= 'g')
		res += (tab[(note[0] - 'a')]);
	else
		return 0;
	
	switch(n)
	{
		case 2:
			res += (note[1] - '0' + 1) * 12;
			break;
		case 3:
		case 4:
			switch(note[1])
			{
				case 'f':
					res--;
					break;
				case 's':
					res++;
					break;
			}
			if(4 == n)
			{
				switch(note[2])
				{
					case 'f':
						res--;
						break;
					case 's':
						res++;
						break;
				}
			}
			res += (note[n - 1] - '0' + 1) * 12;
			break;
		default:
			return 0;
			break;
	}
	return res;
}

Note *Note_CreateNoteFromString(const char *note, Note_Duration duration)
{
	Note *temp = NULL;
	int n = strlen(note);
	if(NULL == note)
		return NULL;
	
	temp = Note_Alloc(ConvertStringToID(note), NOTE_DEFAULT, duration, 0);
	memtest(temp);
	
	switch(n)
	{
		case 3:
			switch(note[1])
			{
				case 'f':
					temp->flags |= NOTE_FLAT;
					break;
				case 's':
					temp->flags |= NOTE_SHARP;
					break;
			}
			break;
		case 4:
			switch(note[2])
			{
				case 'f':
					temp->flags |= NOTE_DOUBLEFLAT;
					break;
				case 's':
					temp->flags |= NOTE_DOUBLESHARP;
					break;
			}
			break;
	}
	return temp;
}


ToNote *ToNote_Alloc(char note, Note_Flags flags, int duration, char rest)
{
	ToNote *temp = (ToNote *)malloc(sizeof(ToNote));
	memtest(temp);
	
	temp->note = Note_Alloc(note, flags, duration, rest);
	temp->next = NULL;
	
	return temp;
}


void ToNote_Free(ToNote **tonote)
{
	if(*tonote != NULL)
	{
		if((*tonote)->note != NULL)
			Note_Free(&(*tonote)->note);
		free(*tonote);
		*tonote = NULL;
	}
}

void ToNote_FreeAll(ToNote **tonote)
{
	if(*tonote != NULL)
	{
		ToNote_FreeAll(&(*tonote)->next);
		ToNote_Free(tonote);
	}
}

Step *Step_Alloc(int num, Note_Duration den, Cle cle, char sign, Step_Flags flags)
{
	Step *temp = (Step *)malloc(sizeof(Step));
	memtest(temp);
	
	temp->notes = NULL;
	temp->num = num;
	temp->den = den;
	temp->cle = cle;
	temp->sign = sign;
	temp->flags = flags;
	
	return temp;
}

void Step_Free(Step **step)
{
	if(*step != NULL)
	{
		if((*step)->notes != NULL)
			ToNote_FreeAll(&(*step)->notes);
		free(*step);
		*step = NULL;
	}
}

int ToNote_ChangeRestStatus(ToNote *notes, int id, char newStatus)
{
	if(NULL == notes)
		return 0;
	if(!id)
	{
		if(notes->note != NULL)
			notes->note->rest = newStatus;
		return 0;
	}
	return ToNote_ChangeRestStatus(notes->next, id - 1, newStatus);
}

int Step_ChangeRestStatus(Step *step, int id, char newStatus)
{
	if(NULL == step)
		return 0;
	return ToNote_ChangeRestStatus(step->notes, id, newStatus);
}

int ToNote_DiviseRest(ToNote *tonote, int id, int newDuration, int stepNum)
{
	if(NULL == tonote)
		return 0;
	if(!id)
	{
		ToNote *sauv_next = tonote->next;
		int nDiv = 0;
		
		if(tonote->note->duration == QUADRUPLECROCHE)
			return 0;
		if(!tonote->note->rest)
			return 0;
			
		if(tonote->note->duration == RONDE)
			nDiv = stepNum;
		else
		{
			newDuration = tonote->note->duration * 2;
			nDiv = 2;
		}
		
		tonote->note->duration = newDuration;
		nDiv--;

		while(nDiv > 0)
		{
			tonote->next = ToNote_Alloc(0, NOTE_DEFAULT, newDuration, 1);
			tonote = tonote->next;
			nDiv--;
		}
		tonote->next = sauv_next;
	
		return 1;
	}
	return ToNote_DiviseRest(tonote->next, id - 1, newDuration, stepNum);
	
}

int Step_DiviseRest(Step *step, int id)
{
	if(NULL == step)
		return 0;
	return ToNote_DiviseRest(step->notes, id, step->den, step->num);
}

int Step_Init(Step *step)
{
	if(NULL == step)
		return 0;
	
	if(step->notes != NULL)
		ToNote_FreeAll(&(step->notes));
	step->notes = ToNote_Alloc(0, NOTE_DEFAULT, RONDE, 1);
	
	return 1;
}

int Step_Change(Step *step, int num, Note_Duration den, Cle cle, 
					Step_Flags flags, char sign)
{
	if(NULL == step)
		return 0;
	
	step->num = num;
	step->den = den;
	step->cle = cle;
	step->flags = flags;
	step->sign = sign;
	
	return 1;
}

void Note_ConsolePrintf(Note *note)
{
	if(note != NULL)
	{
		printf("(%d, %d, %d, %d) -> ",
			note->note, (int)note->flags, (int)note->duration, note->rest);
	}
}
void ToNote_ConsolePrintf(ToNote *notes)
{
	if(notes != NULL)
	{
		Note_ConsolePrintf(notes->note);
		ToNote_ConsolePrintf(notes->next);	
	}
	else
		printf("(NULL)\n");
}

void Step_ConsolePrintf(Step *step)
{
	if(step != NULL)
	{
		printf("---------------- STEP ----------------\n");
		printf("Ox29 notes - NUM = %d, DEN = %d, CLE = %d, SIGN = %d\n",
						step->num, 
						(int)step->den, (int)step->cle,
						step->sign);
		printf("Special Flags = %d\n", (int)step->flags);
		printf("--------------------------------------\n");
		printf("Format notes : (Note, Flags, Duration, Rest)\n");
		if(NULL == step->notes)
			printf("- No Notes -\n");
		else
			ToNote_ConsolePrintf(step->notes);
		printf("\n");
	}
	else
		printf("Step 'NULL'\n");
}

void Step_ConsoleFastPrintf(Step *step)
{
	if(step != NULL)
	{
		if(NULL == step->notes)
			printf("- No Notes -\n");
		else 
			ToNote_ConsolePrintf(step->notes);
	}
	else
		printf("'NULL'\n");
}

int ToNote_Regularise(ToNote *notes)
{	
	ToNote *sauv = NULL;
	if(NULL == notes || NULL == notes->next)
		return 0;
	if(notes->note->duration == notes->next->note->duration
		&& notes->note->rest && notes->next->note->rest)
	{
		notes->note->duration /= 2;
		sauv = notes->next;
		notes->next = notes->next->next;
		ToNote_Free(&sauv);
		return 1;
	}
	return ToNote_Regularise(notes->next);
	
}

int Step_Regularise(Step *step)
{
	if(NULL == step)
		return 0;
	
	while(ToNote_Regularise(step->notes));
	return 1;
}

int find2min(int n)
{
	int i, f;
	for(i = 1; i < n; i++)
	{
		f = pow(2, i);
		if(n == f)
			return n;
		else if(f > n)
			return pow(2, i - 1);
	}
	return n;
}

int comp_duration(const void *p, const void *p2)
{
	return ((Note_Duration *)p2) - ((Note_Duration *)p);
}

Note_Duration *find2rest(int duration)
{
	int i = 0;
	int f;
	Note_Duration *tab = NULL;
	
	tab = (Note_Duration *)calloc(10, sizeof(Note_Duration));
	memtest(tab);
	
	while(duration > 0)
	{
		*(tab + i) = (f = find2min(duration));
		i++;
		duration -= f;
	}
	
	qsort(tab, i, sizeof(Note_Duration), comp_duration);
	return tab;
}


int Note_RealDuration(Note *note, Step *step)
{
	int dur = 64 / note->duration;
	if(note->flags & NOTE_POINTED)
		return (int)(dur + dur/2);
	else if(note->flags & NOTE_DOUBLEPOINTED)
		return (int)(dur + dur/2 + dur/4);
	if(note->rest && note->duration == RONDE)
		return step->num * (64/step->den);
	return dur;
}

int RealDuration(Note_Duration duration, Note_Flags flags)
{
	int dur = 64 / duration;
	if(flags & NOTE_POINTED)
		return (int)(dur + dur/2);
	else if(flags & NOTE_DOUBLEPOINTED)
		return (int)(dur + dur/2 + dur/4);
	return dur;
}

int Step_AddNote(Step *step, int id, char note, Note_Flags flags,
							Note_Duration duration)
{
	ToNote **cur = NULL;
	ToNote *new_note = NULL;
	int note_duration = 0;
	int tmp_duration = 0;
	ToNote *sauv = NULL;
	Note_Duration *rest_r = NULL;
	int i;
	ToNote *special = NULL;
	
	if(NULL == step)
		return -1;
	if(NULL == step->notes)
		return -1;
	cur = &(step->notes);

	while(id > 0)
	{
		if(NULL == (*cur)->next)
			return 0;
		cur = &((*cur)->next);
		id--;
	}
	new_note = ToNote_Alloc(note, flags, duration, 0);
	new_note->next = (*cur)->next;
	note_duration = Note_RealDuration(new_note->note, step);
	
	if((*cur)->note->rest)
	{
		tmp_duration = Note_RealDuration((*cur)->note, step);
		if(note_duration == tmp_duration)
		{
			ToNote_Free(cur);
			*cur = new_note;
			return 0;
		}
	}
	
	tmp_duration = Note_RealDuration((*cur)->note, step);

	ToNote_Free(cur);
	*cur = new_note;
	note_duration -= tmp_duration;
	special=new_note;
	cur = &((*cur)->next);

	while(note_duration > 0)
	{
		if(NULL == *cur)
		{
			special->note->duration = 64/(64/special->note->duration - note_duration);
			special->note->flags |= NOTE_LINKED;
			return 64/note_duration;
		}
		tmp_duration = Note_RealDuration((*cur)->note, step);
		sauv = (*cur)->next;
		note_duration -= tmp_duration;
		ToNote_Free(cur);
		*cur = sauv;
	}

	if(0 == note_duration)
		return 0;
		
	note_duration = -note_duration;
	
	rest_r = find2rest(note_duration);

	i = 0;
	while(*(rest_r + i) > 0)
	{
		sauv = *cur;
		*cur = ToNote_Alloc(0, NOTE_DEFAULT, 64 / *(rest_r + i), 1);
		note_duration -= *(rest_r + i);
		(*cur)->next = sauv;
		cur = &(*cur)->next;
		i++;
	}
	if(rest_r != NULL)
		free(rest_r);
	
	return 0;
}

int Step_DelLocal(Step *step, int begin, int end)
{
	ToNote **cur = NULL;
	ToNote *sauv = NULL;
	int duration = 0;
	Note_Duration *rest_r = NULL;
	int i;
	
	if(NULL == step)
		return 0;
	if(NULL == step->notes)
		return 0;
	
	cur = &step->notes;
	while(begin > 0)
	{
		cur = &((*cur)->next);
		begin--;
		end--;
	}
	while(end > 0)
	{
		if(*cur == NULL)
			break;
		sauv = (*cur)->next;
		duration += Note_RealDuration((*cur)->note, step);
		ToNote_Free(cur);
		*cur = sauv;
		end--;
	}
	
	rest_r = find2rest(duration);
	
	i = 0;
	while(*(rest_r + i) > 0)
	{
		sauv = *cur;
		*cur = ToNote_Alloc(0, NOTE_DEFAULT, 64 / *(rest_r + i), 1);
		duration -= *(rest_r + i);
		(*cur)->next = sauv;
		cur = &(*cur)->next;
		i++;
	}
	
	free(rest_r);
	
	Step_Regularise(step);
	return 1;
}

int Step_Verif(Step *step)
{
	int goal = 0;
	int sum = 0;
	ToNote *cur = NULL;
	
	if(NULL == step)
		return 0;
	if(NULL == step->notes)
		return 0;
	
	goal = step->num * 64 / step->den;
	cur = step->notes;
	while(cur != NULL)
	{
		sum += Note_RealDuration(cur->note, step);
		cur = cur->next;
	}
	
	return (goal == sum)?1:0;
}

Cle Cle_GetFromString(const char *str)
{
	if(NULL == str)
		return 0;
	
	if(strlen(str) == 2)
	{
		if(!strcmp(str, "fa") || !strcmp(str, "FA") || !strcmp(str, "Fa"))
			return CLE_FA;
		if(!strcmp(str, "ut") || !strcmp(str, "UT") || !strcmp(str, "Ut"))
			return CLE_UT3;
	}
	else if(strlen(str) == 3)
	{
		if(!strcmp(str, "SOL") || !strcmp(str, "sol") || !strcmp(str, "Sol"))
			return CLE_SOL;
		if(str[0] == 'U' || str[0] == 'u')
		{
			if(str[1] == 'T' || str[1] == 't')
			{
				if(str[2] == '3')
					return CLE_UT3;
				else if(str[2] == '4')
					return CLE_UT4;
			}
		}
	}
	return 0;
}

char *Cle_GetFromId(Cle cle)
{
	char *res = NULL;
	if(cle < 1 || cle > 4)
		return NULL;
	
	res = (char *)malloc(sizeof(char) * (cle==2)?3:4);
	memtest(res);
	switch(cle)
	{
		case CLE_SOL:
			strcpy(res, "SOL");
			break;
		case CLE_FA:
			strcpy(res, "FA");
			break;
		case CLE_UT3:
			strcpy(res, "UT3");
			break;
		case CLE_UT4:
			strcpy(res, "UT4");
			break;
		default:
			strcpy(res, "SOL");
			break;
	}
	return res;
}
int ToNote_Transpose(ToNote *tonote, char value)
{
	if(NULL == tonote)
		return 0;
	while(tonote != NULL)
	{
		if(tonote->note != NULL && !tonote->note->rest)
			tonote->note->note += value;
		tonote = tonote->next;
	}
	return 1;
}

int Step_Transpose(Step *step, char value)
{
	if(NULL == step)
		return 0;
	return ToNote_Transpose(step->notes, value);
}


Note_Duration Step_GetMinDuration(Step *step)
{
	Note_Duration duration = RONDE;
	ToNote *notes = NULL;
	if(NULL == step)
		return 0;
	notes = step->notes;
	while(notes != NULL)
	{
		if(notes->note != NULL && notes->note->duration > duration)
			duration = notes->note->duration;
		notes = notes->next;
	}
	
	return duration;
}

Note *Step_GetNote(Step *step, int id_note)
{
	ToNote *notes = NULL;
	if(NULL == step || id_note < 0)
		return 0;
	notes = step->notes;
	while(notes != NULL)
	{
		if(id_note == 0)
			return notes->note;
		id_note--;
		notes = notes->next;
	}
	return NULL;
}










