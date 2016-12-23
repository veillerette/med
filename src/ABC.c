#include "../include/ABC.h"


char *ABC_Keys[] = {
	"C#","A#m",	/* 7 */
	"F#","D#m",   	/* 6 */
	"B","G#m",   
	"E","C#m",   
	"A","F#m",   
	"D","Bm",
	"G","Em",
	"C","Am", 	/* 0 */
	"F","Dm",
	"Bb","Gm",
	"Eb","Cm",   	/* -3 */
	"Ab","Fm",  
	"Db","Bbm",
	"Gb","Ebm",
	"Cb","Abm"};   	/* -7 */
	
static int ABC_ScanHead(FILE *f, char *field, char *text)
{
	char *str = malloc(sizeof(char) * 100);
	int n,temp;
	if((NULL == f) || (NULL == field) || (NULL == text))
		return 0;
	temp = fgetc(f);
	str = fgets(str, 100, f);
	n = strlen(str);
	if((EOF == temp) || (n < 2) || (*str != ':'))
	{
		free(str);
		return 0;
	}
	*field = temp;
	str[n-1] = str[n];
	strcpy(text, str + 1);
	free(str);
	return 1;
}

static void ABC_replaceSlash(char *str)
{
	unsigned int i;
	for(i = 0; i < strlen(str); i++)
		if(*(str + i) == '/')
		{
			*(str + i) = 0;
		}
}

static int ABC_FindKeyFromStr(char *text)
{
	int i;
	if(NULL == text)
		return 0;
	for(i = -7; i <= 7; i++)
	{
		if(!strcmp(ABC_Keys[2*(i+7)], text))
			return -i;
		if(!strcmp(ABC_Keys[2*(i+7)+1], text))
			return -i;
	}
	return -40;
}

static int ABC_isNote(char note)
{
	if(note>='A' && note<='G')
		return 1;
	if(note>='a' && note <='g')
		return 1;
	return 0;
}

static int ABC_ParseHeader(Score *score, FILE *f)
{
	char field, text[100];
	int num = -1, den = -1, sign = -42;
	int lenght = -1;
	if((NULL == score) || (NULL == f))
		return 0;
	while(ABC_ScanHead(f, &field, text))
	{
		printf("%c %s \n", field, text);
		switch(field)
		{
		
			case 'T': /* Title */
				Score_AddSign(score, SIGN_TITLE, 0, 0, text);
				break;
				
			case 'C': /* Composer */
				Score_AddSign(score, SIGN_AUTHOR, 0, 0, text);
				break;
				
			case 'M': /* Mesure */
				if(strlen(text) < 3 || strlen(text) > 5)
					break;
				ABC_replaceSlash(text);
				num = atoi(text);
				text[num/10+1] = '/';
				den = atoi(text + num/10 + 2);
				if(num < 1 || num > 20 || den < 1 || den > 64)
					colorprintf(RED, "M:%s is not a good mesure\n", text);
				break;
				
			case 'K': /* Key */
				sign = ABC_FindKeyFromStr(text);
				if(-40 == sign)
					colorprintf(RED, "K:%s is not a good key\n", text);
				break;
			case 'L': /* Basic note length */
				if(strlen(text) < 3 || strlen(text) > 6)
					break;
				lenght = atoi(text+2);
				if(lenght < 1 || lenght > 64)
					colorprintf(RED, "L:%s is not a good lenght\n", text);
				break;
			default:
				break;
		}
	}
	if(sign == -40 || (lenght < -1 || lenght > 64))
		return 0;
	if((num == -1) || (den == -1) || (sign == -42))
	{
		colorprintf(RED, "Mesure times and/or key not specified\n");
		return 0;
	}
	printf("Init with %d %d %d %d\n", num, den, CLE_SOL, sign);
	Staff_Init(score->lst[0], num, den, CLE_SOL, sign);	
	return lenght;
	
}

int nextcar(FILE *f)
{
	int car = 0;
	if(NULL == f)
		return 0;
	car = fgetc(f);
	fseek(f, - 1, SEEK_CUR);
	return car;
}

extern Score *ABC_ParseFile(const char *path)
{
	Score *score = NULL;
	FILE *f = NULL;
	int base_l = NOIRE;
	int car = 0;
	char note[3] = {0, 0, 0};
	int i=0;
	int step_id = 0, note_id = 0;
	int h = 4;
	int duration = base_l;
	Note_Flags flags = NOTE_DEFAULT;
	
	if(NULL == path)
		return NULL;
	
	f = fopen(path, "r");
	if(NULL == f)
		return NULL;
	score = Score_Alloc();
	Score_Init(score);
	
	if(0 == (base_l = ABC_ParseHeader(score, f)))
	{
		colorprintf(RED, "---- Resume : Please correct errors ----\n");
		exit(EXIT_FAILURE);
	}
	
	duration = base_l;
	
	do
	{
		car = fgetc(f);
		switch(car)
		{
			case '+':
				flags |= NOTE_SHARP;
				break;
			case '-':
				flags |= NOTE_FLAT;
				break;
			default:
				break;
		}
		if(ABC_isNote(car) || car == '|' || car=='\n' || car == EOF)
		{
			if(i != 0 )
			{
				note[i] = (char)h+'0';
				
				printf("ADDING step_id=%d note_id=%d, note=%s, duration=%d flags=%d\n",
						step_id, note_id, note, duration, flags);
				if(step_id+1 == score->lst[0]->n)
					Staff_AddEmptyStep(score->lst[0]);
				Staff_AddNote(score->lst[0], step_id, note_id, 
						ConvertStringToID(note), flags, duration);
				flags = NOTE_DEFAULT;
				if(duration != base_l)
					duration = base_l;
				note_id++;
				i = 0;
			}
			if(car!='\n' && car != EOF && car != '|')
			{
				if(car >= 'A' && car <= 'G')
					h = 4;
				else
					h = 5;
				if(nextcar(f) == '\'')
				{
					fgetc(f);
					h++;
				}
				note[i] = car;
				i++;
			}
		}
		else if(car == '/')
		{
			duration = duration * (fgetc(f) - '0');
		}
		else if(car >= '0' && car <= '9')
		{
			if(car == '0')
				duration = RONDE;
			else
				duration = duration / (car - '0');
		}
		else if(car == '\'')
			h++;
		else if(car == ',')
			h--;
		switch(car)
		{
			case '|':
				step_id++;
				note_id = 0;
				break;
			case ' ':
				break;
			default:
				break;
		}
		
	}
	while(car != EOF);
	
	
	
	
	fclose(f);
	return score;
}
