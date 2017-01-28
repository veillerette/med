#include "../include/File.h"

int File_isExt(const char *name, const char *ext)
{
	int n = strlen(name), n2 = strlen(ext);
	int i;
	for(i = n2; i > 0; i++)
		if(name[n-1-i] != ext[n2-1-i])
			return 0;
	return 1;
}

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


/*********************************
	EXPLORER FUNCTIONS
**********************************/

Entry *Entry_Alloc(const char *name, Entry_Type type, int size)
{
	Entry *temp = (Entry *)malloc(sizeof(Entry));
	memtest(temp);
	
	temp->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
	memtest(temp->name);
	strcpy(temp->name, name);
	
	temp->type = type;
	temp->size = size;
	
	return temp;
}

void Entry_Free(Entry **entry)
{
	if(*entry != NULL)
	{
		if((*entry)->name != NULL)
			free((*entry)->name);
		
		free(*entry);
		*entry = NULL;
	}
}

Directory *Directory_Alloc(const char *name)
{
	Directory *temp = (Directory *)malloc(sizeof(Directory));
	memtest(temp);
	
	temp->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
	memtest(temp->name);
	strcpy(temp->name, name);
	
	temp->tab = (Entry **)malloc(sizeof(Entry *) * 10);
	memtest(temp->tab);
	temp->n = 0;
	temp->m = 10;
	
	return temp;
}

void Directory_Free(Directory **dir)
{
	if(*dir != NULL)
	{
		int i;
		for(i = 0; i < (*dir)->n; i++)
			Entry_Free(&((*dir)->tab[i]));
		
		if((*dir)->name != NULL)
			free((*dir)->name);
		
		free(*dir);
		*dir = NULL;
	}
}

int Directory_Add(Directory *dir, Entry *entry)
{
	if((NULL == dir) || (NULL == entry))
		return 0;
	
	if(dir->n == dir->m)
	{
		dir->tab = realloc(dir->tab, sizeof(Entry *) * dir->m * 2);
		memtest(dir->tab);
		
		dir->m *= 2;
	}
	
	dir->tab[dir->n] = entry;
	dir->n++;
	
	return 1;
}

int Directory_Change(Directory **dir, const char *newName)
{
	if(dir != NULL)
	{
		Directory_Free(dir);
		*dir = Directory_Create(newName);
	}
	return 1;
}

int File_Sort(const struct dirent **a, const struct dirent **b)
{
	const struct dirent *var = *a;
	const struct dirent *var2 = *b;
	
	if(var->d_type == var2->d_type)
		return alphasort(a, b);
		
	if(var->d_type == DT_DIR)
		return -1;
		
	return 1;
}

int File_Filter(const struct dirent *dir)
{
	if((NULL == dir))
		return 0;
		
	if(dir->d_name[0] == '.')
		return 0;
		
	if(dir->d_type == DT_DIR)
		return 1;
		
	return 1;
}



Directory *Directory_Create(const char *path)
{
	Directory *dir = NULL;
	struct dirent **namelist;
	int n,i;
	Entry_Type type;
	if((NULL == path))
		return NULL;

	dir = Directory_Alloc(path);
	n = scandir(path, &namelist, File_Filter, File_Sort);
	
	for(i = 0; i < n; i++)
	{
		switch(namelist[i]->d_type)
		{
			case DT_DIR:
				type = _DIR;
				break;
			case DT_REG:
				type = _FILE;
				break;
			default:
				continue;
		}
		Directory_Add(dir, Entry_Alloc(namelist[i]->d_name, type, 0));
	}
	
	return dir;
}

void Directory_Debug(Directory *dir)
{
	int i;
	Entry *e = NULL;
	for(i = 0; i < dir->n; i++)
	{
		e = *(dir->tab + i);
		switch(e->type)
		{
			case _FILE:
				printf("File ");
				break;
			case _DIR:
				printf("Dir ");
				break;
		}
		printf("%s %d\n", e->name, e->size);
	}
}




