#include "../include/Staff.h"

Staff *Staff_Alloc(const char *name)
{
	Staff *temp = (Staff *)malloc(sizeof(Staff));
	memtest(temp);
	
	temp->steps = (Step **)calloc(STAFF_BASE, sizeof(Step *));
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
		if(NULL == *(staff->steps + i))
			*(staff->steps + i) = Step_Alloc(num, den, cle, sign, STEP_DEFAULT);
		Step_Init(*(staff->steps + i));
	}
	return 1;
}

int Staff_SetNumDen(Staff *staff, int step_id, int num, Note_Duration den)
{
	if((NULL == staff) || (step_id < 0) || (step_id > staff->n))
		return 0;
	staff->steps[step_id]->num = num;
	staff->steps[step_id]->den = den;
	return 1;
}

int Staff_AddNote(Staff *staff, int step_id, int note_id, char note, 
					Note_Flags flags, Note_Duration duration)
{
	int r;
	if(NULL == staff)
		return 0;
	if(NULL == staff->steps)
		return 0;
	if(staff->n <= step_id)
		return 0;
		
	r = Step_AddNote(*(staff->steps + step_id), note_id, note, flags, duration);
	if(r <= 0)
		return r;
	return Staff_AddNote(staff, step_id+1, 0, note, flags, r);
} 

void Staff_Console(Staff *staff)
{
	int i;
	if(NULL == staff)
		return;
	
	for(i = 0; i < staff->n; i++)
		Step_ConsoleFastPrintf(*(staff->steps + i));
	
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
	
	if(pos > staff->n)
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
	
	if(0 == staff->n || pos < 0 || 1 == staff->n)
		return 0;
	
	if(pos > staff->n - 1)
		return 0;
	
	Step_Free(&(*(staff->steps + pos)));
	for(i = pos; i < staff->n; i++)
		*(staff->steps + i)  = *(staff->steps + i + 1);
	staff->n--;
	return 1;
}

int Staff_DiviseRest(Staff *staff, int step_id, int note_id)
{
	if(NULL == staff)
		return 0;
	if((step_id < 0) || (note_id < 0))
		return 0;
	return Step_DiviseRest(*(staff->steps + step_id), note_id);
}

int Staff_ChangeRestStatus(Staff *staff, int step_id, int note_id, char newStatus)
{
	if(NULL == staff)
		return 0;
	if((step_id < 0) || (note_id < 0))
		return 0;
	return Step_ChangeRestStatus(*(staff->steps + step_id), note_id, newStatus);
}
int Staff_Transpose(Staff *staff, char value)
{
	int i;
	int r = 1;
	if(NULL == staff)
		return 0;
	
	for(i = 0; i < staff->n; i++)
		r &= Step_Transpose(*(staff->steps + i), value);
	
	return r;
}

int Staff_ChangeArmure(Staff *staff, int step_id, signed char new)
{
	int i = step_id;
	signed char sauv = (*(staff->steps + step_id))->sign;
	if(NULL == staff)
		return 0;
	if(step_id < 0)
		return 0;
	while(i < staff->n && (*(staff->steps + i))->sign == sauv)
	{
		(*(staff->steps + i))->sign = new;
		i++;
	}
	return 1;
}


int Staff_ChangeCle(Staff *staff, int step_id, Cle new)
{
	int j;
	if((NULL == staff) || (NULL == staff->steps) || (0 == staff->n) || (step_id < 0))
		return 0;
	
	for(j = step_id; j < staff->n; j++)
		staff->steps[j]->cle = new;
	
	return 1;
}

Sign *Sign_Alloc(Sign_Type type, Uint value, Uint time, char *text)
{
	Sign *temp = (Sign *)malloc(sizeof(Sign));
	memtest(temp);
	
	temp->type = type;
	temp->value = value;
	temp->time = time;
	temp->text = NULL;
	if(text != NULL)
	{
		temp->text = (char *)malloc(sizeof(char) * (strlen(text) + 1));
		memtest(temp->text);
		strcpy(temp->text, text);
	}
	temp->next = NULL;
	
	return temp;
}

void Sign_Free(Sign **sign)
{
	if(*sign != NULL)
	{
		if((*sign)->text != NULL)
			free((*sign)->text);
		free(*sign);
		*sign = NULL;
	}
} 

void Sign_FreeAll(Sign **sign)
{
	if(*sign != NULL)
	{
		Sign_FreeAll(&((*sign)->next));
		Sign_Free(sign);
	}
}

Score *Score_Alloc(void)
{
	Score *temp = (Score *)malloc(sizeof(Score));
	memtest(temp);
	
	temp->lst = (Staff **)malloc(sizeof(Staff *) * 1);
	if(NULL == temp->lst)
		exit(EXIT_FAILURE);
	temp->n = 0;
	temp->data = 0;
	temp->signs = NULL;
	temp->tempo = 120;
	
	return temp;
}

void Score_Free(Score **score)
{
	if(*score != NULL)
	{
		int i;
		for(i = 0; i < (*score)->n; i++)
			Staff_Free(&((*score)->lst[i]));
		if((*score)->lst != NULL)
			free((*score)->lst);
		if((*score)->signs != NULL)
			Sign_FreeAll(&((*score)->signs));
		free(*score);
		*score = NULL;
	}
}

int Score_AddSign(Score *score, Sign_Type type, Uint value, Uint time, char *text)
{
	Sign **cur = NULL;
	Sign *sauv = NULL;
	Sign *new = NULL;
	
	if(NULL == score)
		return 0;
	
	if(type <= 0)
		return 0;
		
	new = Sign_Alloc(type, value, time, text);
	cur = &(score->signs);
	
	while(*cur != NULL && (*cur)->time < time)
		cur = &((*cur)->next);
	
	if(NULL == *cur)
	{
		*cur = new;
		return 1;
	}
	sauv = *cur;
	*cur = new;
	new->next = sauv;
	return 1;
}

int Score_DeleteSign(Score *score, Sign_Type type, Uint time)
{
	Sign **cur = NULL;
	Sign *sauv = NULL;
	if(NULL == score)
		return 0;
		
	if(NULL == score->signs)
		return 0;
	
	cur = &(score->signs);
	
	while(*cur != NULL)
	{
		if((*cur)->time == time && (*cur)->type == type)
		{
			sauv = (*cur)->next;
			Sign_Free(cur);
			*cur = sauv;
			return 1;
		}
		cur = &((*cur)->next);
	}
	return 1;
}

int Score_ShowSignConsole(Score *score)
{
	Sign *cur = NULL;
	if(NULL == score)
		return 0;
	if(NULL == score->signs)
		return 0;
	
	cur = score->signs;
	
	while(cur != NULL)
	{
		printf("- Sign \"%d\" val = %d (time = %d)\n", 
					cur->type, cur->value, cur->time);
		cur = cur->next;
	}
	return 1;
}

int Score_Init(Score *score)
{
	if(NULL == score)
		return 0;
	if(score->n != 0)
		return 0;
	score->lst[0] = Staff_Alloc(" ");
	score->n++;
	return 1;
}

int Score_AddEmpty(Score *score)
{
	
	if(NULL == score)
		return 0;
	if(10 == score->n) /* max printable */
		return -1;
	score->lst = (Staff **)realloc(score->lst, sizeof(Staff *) * (score->n+1));
	memtest(score->lst);
	score->lst[score->n] = Staff_Alloc(" ");
	
	if(score->n > 0)
	{

		Staff_Init(score->lst[score->n], score->lst[score->n-1]->steps[0]->num, 
			score->lst[score->n-1]->steps[0]->den,  score->lst[score->n-1]->steps[0]->cle,
			score->lst[score->n-1]->steps[0]->sign);

		while(score->lst[score->n]->n < score->lst[score->n - 1]->n)
		{
			Staff_AddEmptyStep(score->lst[score->n]);
		}
	}
	score->n++;
	return 1;
}

int Score_NumberStep(Score *score)
{
	if((NULL == score) || (NULL == score->lst) || (NULL == score->lst[0]))
		return 0;
	return score->lst[0]->n;
}

int Score_SetEmptyStep(Score *score, int i)
{
	int j;
	if((NULL == score) || (NULL == score->lst) || (NULL == score->lst[0]))
		return 0;
	
	for(j = 0; j < score->n; j++)
		Staff_InsereEmptyStep(score->lst[j], i);
	return 1;
}

int Score_AddEmptyStep(Score *score)
{
	return Score_SetEmptyStep(score, Score_NumberStep(score));
}

int Score_DeleteStep(Score *score, int i)
{
	int j;
	if((NULL == score) || (NULL == score->lst) || (NULL == score->lst[0]))
		return 0;
	for(j = 0; j < score->n; j++)
		Staff_DeleteStep(score->lst[j], i);
	return 1;
}

int Score_ChangeArmure(Score *score, int i, signed char new)
{
	int j;
	if((NULL == score) || (NULL == score->lst) || (NULL == score->lst[0]))
		return 0;
	
	for(j = 0; j < score->n; j++)
		Staff_ChangeArmure(score->lst[j], i, new);
	return 1;
}

void Score_SetTempo(Score *score, int tempo)
{
	if(score != NULL)
	{
		score->tempo = tempo;
	}
}



