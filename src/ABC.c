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
	if(note == 'z')
		return 1;
	return 0;
}

static int ABC_ParseHeader(Score *score, FILE *f)
{
	char field, text[100];
	int num = -1, den = -1, sign = -42;
	int lenght = -1;
	int c = 1;
	if((NULL == score) || (NULL == f))
		return 0;
	while(ABC_ScanHead(f, &field, text) && c)
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
			case 'V':
				fseek(f, -4, SEEK_CUR);
				c = 0;
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
	while(fgetc(f) != '\n')
		fseek(f, -2, SEEK_CUR);	
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


/*********************/

char *File_GetLine(FILE *f)
{
	char *buf = NULL;
	int i = 0, c = 5, j = 0;
	
	if((NULL == f))
		return NULL;
	
	do
	{
		c = fgetc(f);
		i++;
	}
	while(c != '\n' && c != EOF);
	
	if(i == 1 && c == EOF)
		return NULL;
	
	buf = (char *)malloc(sizeof(char) * (i + 2));
	memtest(buf);
	
	fseek(f, -i , SEEK_CUR);
	
	for(j = 0; j < i; j++)
		*(buf + j) = fgetc(f);
	
	*(buf + j) = '\0';

	return buf;
}

int ABC_IsAnHeader(const char *line)
{
	int n;
	
	if((NULL == line))
		return 0;
	
	n = strlen(line);
	
	if(n < 3)
		return 0;
	
	if(line[0] < 'A' || line[0] > 'Z')
		return 0;
	
	if(line[1] != ':')
		return 0;
	
	return 1;
}

int ABC_IsAGoodHeader(const char *line)
{
	char good_letter[8] = "XTMLQVCK";
	int i;
	
	if((NULL == line))
		return 0;
	
	for(i = 0; i < 8; i++)
		if(line[0] == good_letter[i])
			return 1;
	
	return 0;
}

int ABC_MultiSimplify(char *buf)
{
	int i;
	int n;
	
	if((NULL == buf))
		return 0;
	
	n = strlen(buf);
	
	for(i = 1; i < n; i++)
	{
		switch(buf[i])
		{
			case '^':
			case '=':
			case '_':
				buf[i] = ']';
				return 1;
			case '[':
				return 1;
			default:
				if((buf[i] >= 'a' && buf[i] <= 'g')
				   || (buf[i] >= 'A' && buf[i] <= 'G'))
				{
					buf[i] = ']';
					return 1;
				}
				break;
		}
	}
	buf[i] = ']';
	return 1;
}

char *ABC_TransformLine(const char *line, int isHeader)
{
	char bufmulti[100] = "";
	int i_multi = 0;
	char *new = NULL;
	int i,j = 0, c, k;
	int n;
	
	if((NULL == line))	
		return NULL;
	
	n = strlen(line);

	new = (char *)malloc(sizeof(char) * (n + 1));
	memtest(new);
	
	i = 0;
	while(i < n)
	{
		c = *(line + i);
		switch(c)
		{
			
			
			case '-':
			case '+':
			case '*':
				break;
			case '"':
				do
				{
					i++;
				}
				while(*(line + i) != '"');
				break;
			case '%':
				if(i != 0)
				{
					*(new + j) = '\n';
					j++;
					i = n;
				}
				break;
			case '[':
				i++;
				do
				{
					bufmulti[i_multi] = line[i];
					i_multi++;
					i++;
				}
				while(line[i] != ']');
				
				bufmulti[i_multi] = 0;
				ABC_MultiSimplify(bufmulti);
				
				k = 0;
				while(bufmulti[k] != ']')
				{
					*(new + j) = bufmulti[k];
					j++;
					k++;
				}
				i_multi = 0;
				
				break;
				
			case ':':
			case ' ':
				if(!isHeader)
					break;
			default:
				*(new + j) = *(line + i);
				j++;
				break;
		}
		i++;
	}
	if(j == 0)
	{
		free(new);
		return NULL;
	}
	*(new + j) = 0;
	return new;
}

int ABC_AppendStr(char **dest, const char *source)
{
	int n,n2;
	
	if((NULL == source))
		return 0;
	
	
	n = strlen(source)-1;

	if(NULL == *dest)
	{
		*dest = (char *)malloc(sizeof(char) * (n + 1));
		memtest(*dest);
		n2 = 0;
	}
	else
	{
		n2 = strlen(*dest);
		*dest = realloc(*dest, sizeof(char) * (n2 + n + 1));
		memtest(*dest);
	}
	
	strcpy((*dest)+n2, source);
	(*dest)[n2+n]=0;
	return 1;
}

int File_SimplifyABC(const char *destPath, const char *sourcePath)
{
	char *buf = NULL;
	FILE *dest = NULL, *source = NULL;
	char **buf_line = NULL;
	char *next = NULL;
	int i;
	
	
	if((NULL == destPath) || (NULL == sourcePath))
		return 0;

	dest = fopen(destPath, "w");
	memtest(dest);
	
	source = fopen(sourcePath, "r");
	memtest(source);
	
	buf_line = (char **)calloc(10, sizeof(char *));
	memtest(buf_line);

	do
	{		
		buf = File_GetLine(source);
	
		if(NULL == buf)
			break;
		
		if(ABC_IsAnHeader(buf))
		{
			if(ABC_IsAGoodHeader(buf))
			{
				next = File_GetLine(source);
				if(!ABC_IsAnHeader(next) && buf[0] == 'V')
				{
					char *temp = ABC_TransformLine(next, 0);
					
					if(temp != NULL)
						ABC_AppendStr(&(buf_line[(buf[2]-'0')-1]), temp);
					

				}
				else if(buf[0] != 'V')
				{
					char *temp = NULL;
					temp = ABC_TransformLine(buf, 1);
				
					if(temp != NULL)
					{
						fputs(temp, dest);	
						free(temp);
					}
					
					fseek(source, -strlen(next), SEEK_CUR);
					free(next);
				}
			}
		}
		else
		{
			char *temp = NULL;
			temp = ABC_TransformLine(buf, 0);
			
			if(temp != NULL)
			{
				fputs(temp, dest);
				free(temp);
			}
		}
		
		free(buf);
	}
	while(buf != NULL);
	
	i = 0;
	
	while(buf_line[i] != NULL)
	{
		fprintf(dest, "V:%d\n", i);
		fprintf(dest, "%s\n", buf_line[i]);
		i++;
	}
	
	
	fclose(dest);
	fclose(source);
	
	return 1;
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
	int id_score = 0;
	Note_Flags flags_next = NOTE_DEFAULT;
	int rest = 0;
	
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
		if(car == EOF)
			continue;
		if(car == 'V')
			continue;
		if(car == ':' && nextcar(f) >= '0' && nextcar(f) <= '9')
		{
			id_score = (fgetc(f) - '0');
			if(id_score == score->n && id_score != 0)
				Score_AddEmpty(score);
			fgetc(f);
			printf("change id_score to %d\n", id_score);
			step_id = 0;
			continue;
		}
		switch(car)
		{
			case '^':
				flags_next |= NOTE_SHARP;
				break;
			case '_':
				flags_next |= NOTE_FLAT;
				break;
			case '=':
				flags_next |= NOTE_NATURAL;
				flags_next &= ~NOTE_SHARP;
				flags_next &= ~NOTE_FLAT;
				break;
			default:
				break;
		}
		if(ABC_isNote(car) || car == '|' || car=='\n' || car == EOF)
		{
			if(i != 0 )
			{
				note[i] = (char)h+'0';
				if(id_score == 0 && step_id+1 == score->lst[id_score]->n)
					Score_AddEmptyStep(score);
				printf("add a note staff=%d step=%d/%d\n", id_score, step_id, score->lst[id_score]->n);
				Staff_AddNote(score->lst[id_score], step_id, note_id, 
						ConvertStringToID(note), flags, duration);
				if(note[0] == 'z')
					Staff_ChangeRestStatus(score->lst[id_score], step_id, note_id, 1);
				flags = flags_next;
				flags_next = NOTE_DEFAULT;
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
			if(nextcar(f) >= '1' && nextcar(f) <= '9')
				duration = duration * (fgetc(f) - '0');
			else
				duration = duration * 2;
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

extern Score *ABC_OpenABC(const char *path)
{
	char buf[100] = "";
	if((NULL == path))
		return NULL;
	sprintf(buf, "%s.tmp", path);
	File_SimplifyABC(buf, path);
	return ABC_ParseFile(buf);
}

/*
int main(int argc, char *argv[])
{
	File_SimplifyABC(argv[1], argv[2]);
	argc = argc;
	return 0;
}
*/
