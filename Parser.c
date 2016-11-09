#include "Parser.h"

#include <readline/readline.h>
#include <readline/history.h>

char **my_split(char *base, int *argc)
{
	int n_space = 0;
	int base_n = 0;
	int i,j = 0;
	char **temp = NULL;
	char *cur1 = NULL;
	char sauv;
	char *base2 = NULL;
	
	
	
	base_n = strlen(base);
	for(i = 0; i < base_n; i++)
		if(*(base + i) == ' ')
			n_space++;
	
	base2 = (char *)malloc(sizeof(char) * (base_n + 1));
	memtest(base2);
	strcpy(base2, base);
	
	cur1 = base2;
	
	temp = (char **)malloc(sizeof(char *) * (n_space + 1));
	memtest(temp);
	*argc = (n_space + 1);
	
	for(i = 0; i <= base_n; i++)
	{
		if(((sauv = *(base2 + i)) == ' ') || (*(base2 + i) == '\0'))
		{
			
			*(base2 + i) = '\0';
			*(temp + j) = cur1;
			j++;
			cur1 = base2 + i + 1;
		}
	}
	return temp;
	
	
}
int Console_Parser(void)
{
	Staff *staff = NULL;
	SDL_Rect redim;
	SDL_Rect pos;
	int c = 1;
	double r=2.0;
	Note_Flags flags = NOTE_DEFAULT;
	
	char *temp = NULL;
	
	char **com = NULL;
	int n_com;
	
	staff = Staff_Alloc("Staff trop cool");
	Staff_Init(staff, 4, NOIRE, CLE_SOL, 0);
	
	redim.x = 0;
	redim.y = 0;
	redim.w = Window->body->w;
	redim.h = Window->body->h;
	pos.x = Window->pos_body->x;
	pos.y = Window->pos_body->y;
	pos.w = Window->pos_body->w;
	pos.h = Window->pos_body->h;
		
	Staff_Print(staff, SDL_SetRect(200, 100, 0, 0), Window->body);
	Window_DrawBodyShrink(r, redim, pos);
	SDL_Flip(Window->screen);
	
	printf("Tap : step_id note_id note_name note_duration\n");
	while(c)
	{
		flags = NOTE_DEFAULT;
		temp = readline("> ");
		if(NULL == temp)
		{
			c = 0;
			break;
		}
		if(temp[0] != '\0')
			add_history(temp);
		com = my_split(temp, &n_com);
		if(!strcmp(com[0], "exit"))
		{
			c = 0;
			break;
		}
		else if(n_com < 4 || n_com > 5)
			continue;
		if(n_com == 5 && !strcmp(com[4], "pointed"))
			flags |= NOTE_POINTED;
		Staff_AddNote(staff, atoi(com[0]), atoi(com[1]), ConvertStringToID(com[2]),
				flags, atoi(com[3]));
		Staff_Console(staff);
		
		Staff_Print(staff, SDL_SetRect(200, 100, 0, 0), Window->body);
		Window_MajBody();
		Window_DrawBodyShrink(r, redim, pos);
		SDL_Flip(Window->screen);
		
	}

	Staff_Free(&staff);
	return 1;
}
