#include "../include/File.h"

char *File_GetExt(const char *name)
{
	int i;
	char *res = NULL;
	int n;
	
	if(NULL == name)
		return NULL;
	
	n = strlen(name);
	for(i = n - 1; *(name + i) != '.' && i > 0; i--);
	
	if(!i)
		return NULL;
	
	res = (char *)malloc(sizeof(char) * ((n - i) + 2));
	memtest(res);
	
	strcpy(res, name + i + 1);
	return res;
}

int File_WriteFirstHead(FILE *f)
{
	char temp[] = {0, 0, 'M', 'E', 'D', 0};
	if((NULL == f))
		return 0;
	return fwrite(temp, sizeof(char), 7, f); 
}

int File_ReadFirstHead(FILE *f)
{
	char temp[7] = "";
	if((NULL == f))
		return 0;
	fread(temp, sizeof(char), 7, f);
	return strcmp(temp, "\0\0MED\0");
}

int File_WriteNote(FILE *f, Note *note)
{
	return fwrite(note, sizeof(Note), 1, f);
}

int File_ReadNote(FILE *f, Note **note)
{
	return fread(*note, sizeof(Note), 1, f);
}

int File_WriteAllNotes(FILE *f, Step *step)
{
	ToNote *note = step->notes;
	int n = 0;
	
	while(note != NULL)
	{
		n++;
		note = note->next;
	}
	
	note = step->notes;
	fwrite(&n, sizeof(int), 1, f);

	while(note != NULL)
	{
		File_WriteNote(f, note->note);
		note = note->next;
	}

	
	return n;
}

int File_ReadAllNotes(FILE *f, ToNote **notes)
{
	ToNote *temp = NULL;
	int n = 0;
	int i;
	
	if((NULL == f) || (NULL == notes))
		return 0;

	fread(&n, sizeof(int), 1, f);
	
	for(i = 0; i < n; i++)
	{

		temp = (ToNote *)malloc(sizeof(ToNote));
		memtest(temp);

		temp->next = NULL;
		
		temp->note = (Note *)malloc(sizeof(Note));
		memtest(temp->note);

		File_ReadNote(f, &(temp->note));

		*notes = temp;
		notes = &((*notes)->next);

	}
	return 1;
}

int File_WriteStep(FILE *f, Step *step)
{
	if((NULL == f) || (NULL == step))
		return 0;
		
	fwrite(step, sizeof(Step), 1, f);
	File_WriteAllNotes(f, step);
	return 1;
}

int File_ReadStep(FILE *f, Step **step)
{
	if((NULL == f) || (NULL == step))
		return 0;

	*step = (Step *)malloc(sizeof(Step));
	memtest(*step);

	fread(*step, sizeof(Step), 1, f);
	
	return File_ReadAllNotes(f, &((*step)->notes));
}

int File_WriteStaff(FILE *f, Staff *staff)
{
	int i;
	if((NULL == f) || (NULL == staff))
		return 0;
		
	fwrite(staff, sizeof(Staff), 1, f);
	
	for(i = 0; i < staff->n; i++)
		File_WriteStep(f, *(staff->steps + i));
		
	return 1;
}

int File_ReadStaff(FILE *f, Staff **staff)
{
	int i;
	if((NULL == f) || (NULL == staff))
		return 0;

	*staff = (Staff *)malloc(sizeof(Staff));
	memtest(*staff);

	fread(*staff, sizeof(Staff), 1, f);
	(*staff)->name = NULL;

	(*staff)->steps = (Step **)malloc(sizeof(Step) * (*staff)->max);
	memtest((*staff)->steps);

	for(i = 0; i < (*staff)->n; i++)
		File_ReadStep(f, &((*staff)->steps[i]));
	printf("10\n");
	return 1;
}

int File_WriteScore(FILE *f, Score *score)
{
	int i;
	if((NULL == f) || (NULL == f))
		return 0;
	
	score->signs = NULL;
	
	fwrite(score, sizeof(Score), 1, f);
	
	for(i = 0; i < score->n; i++)
		File_WriteStaff(f, *(score->lst + i));
	
	return 1;
}


int File_SaveScore(const char *path, Score *score)
{
	FILE *f = NULL;
	
	if((NULL == path) || (NULL == score))
		return 0;
	
	f = fopen(path, "wb");
	if(NULL == f)
	{
		colorprintf(RED, "Impossible d'ouvrir %s\n", path);
		memtest(f);
	}
	
	File_WriteScore(f, score);
	
	fclose(f);
	return 1;
}


int File_ReadScore(FILE *f, Score **score)
{
	int i;
	if((NULL == f) || (NULL == score))
		return 0;
	
	*score = (Score *)malloc(sizeof(Score));
	memtest(*score);
	
	fread(*score, sizeof(Score), 1, f);
	
	(*score)->lst = (Staff **)malloc(sizeof(Staff *) * (*score)->n);
	memtest((*score)->lst);
	
	for(i = 0; i < (*score)->n; i++)
	{
		File_ReadStaff(f, &((*score)->lst[i]));
	}
	
	return 1;
}

int File_OpenScore(const char *path, Score **dest)
{
	FILE *f = NULL;
	
	if((NULL == path) || (NULL == dest))
		return 0;

	f = fopen(path, "rb");
	File_ReadScore(f, dest);
	
	fclose(f);
	return 1;
}



/*
int main()
{
	Step *step = NULL;
	FILE *f = NULL;
	int i;
	Score *score = NULL;
	score = Score_Alloc();
	Score_Init(score);
	Staff_Init(score->lst[0], 4, NOIRE, CLE_SOL, 0);
	Score_AddEmpty(score);
	for(i = 0; i < 10; i++)
		Score_AddEmptyStep(score);
	Staff_ChangeArmure(score->lst[0], 0, 2);
	Staff_ChangeArmure(score->lst[1], 0, 2);
	Staff_AddNote(score->lst[0], 0, 0, 120, NOTE_DEFAULT, CROCHE);
	
	Staff_AddNote(score->lst[0], 0, 1, 120, NOTE_DEFAULT, CROCHE);
	Staff_AddNote(score->lst[0], 0, 2, 120, NOTE_DEFAULT, NOIRE);
	
	printf("begin 1\n");
	f = fopen("testW.med", "wb");
	File_WriteScore(f, score);
	fclose(f);
	
	score = NULL;
	
	printf("begin 2\n");
	f = fopen("testW.med", "rb");
	File_ReadScore(f, &score);
	fclose(f);
	
	printf("begin 3\n");
	f = fopen("testO.med", "wb");
	File_WriteScore(f, score);
	fclose(f);
	return 0;
}*/






