#include "../include/Parser.h"

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
	int c = 1;
	int i;
	double r=2.0;
	Note_Flags flags = NOTE_DEFAULT;
	Score *score = NULL;
	char *temp = NULL;
	
	char **com = NULL;
	int n_com;
	
	score = Score_Alloc();
	Score_Init(score);
	Score_AddEmpty(score);
	
	score->lst[0] = staff = Staff_Alloc("Staff trop cool");
	Staff_Init(staff, 4, NOIRE, CLE_SOL, 0);
	
	Score_Print(score, SDL_SetRect(100, 200, 0, 0));
	Window_ApplyZoom(r);
	Window_DrawBody();
	Window_Print();
	SDL_Flip(Window->screen);
	
	printf("\n");
	printf("- Delete a step : \t\tdelete step step_id\n");
	printf("- Insere an empty step : \tinsere step step_pos\n");
	printf("- Change armure : \t\tarmure step_id new_armure\n");
	printf("- New note : \t\t\tstep_id note_id note_name note_duration [pointed, sharp, flat, doubleflat, linked]\n");
	printf("- To quit : \t\t\texit\n");
	printf("- Help : \t\t\thelp\n");
	printf("\n");
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
		else if(!strcmp(com[0], "help"))
		{
			printf("\n");
			printf("- Delete a step : \t\tdelete step step_id\n");
			printf("- Insere an empty step : \tinsere step step_pos\n");
			printf("- Change armure : \t\tarmure new_armure\n");
			printf("- New note : \t\t\tstep_id note_id note_name note_duration [pointed, sharp, flat, doubleflat, linked]\n");
			printf("- To quit : \t\t\texit\n");
			printf("\n");
			continue;
		}
		if(n_com >= 4)
		{
			for(i = 4; i < n_com; i++)
			{
				if(!strcmp(com[i], "pointed"))
					flags |= NOTE_POINTED;
				else if(!strcmp(com[i], "doublepointed"))
					flags |= NOTE_DOUBLEPOINTED;
				else if(!strcmp(com[i], "sharp"))
					flags |= NOTE_SHARP;
				else if(!strcmp(com[i], "flat"))
					flags |= NOTE_FLAT;
				else if(!strcmp(com[i], "doublesharp"))
					flags |= NOTE_DOUBLESHARP;
				else if(!strcmp(com[i], "doubleflat"))
					flags |= NOTE_DOUBLEFLAT;
				else if(!strcmp(com[i], "linked"))
					flags |= NOTE_LINKED;
			}
			
			Staff_AddNote(staff, atoi(com[0]), atoi(com[1]), ConvertStringToID(com[2]),
				flags, atoi(com[3]));
		}
		else if(n_com == 3)
		{
			if(!strcmp(com[0], "armure") && (atoi(com[2]) >= -7 && atoi(com[2]) <= 7))
				Staff_ChangeArmure(staff, atoi(com[1]), atoi(com[2]));
			else if(!strcmp(com[0], "delete"))
			{
				if(!strcmp(com[1], "step"))
				{
					for(i = 2; i < n_com; i++)
					{
						if(atoi(com[i]) >= 0 && atoi(com[i]) < staff->n)
								Staff_DeleteStep(staff, atoi(com[i]));
					}
				}
			}
			else if(!strcmp(com[0], "insere"))
			{
				if(!strcmp(com[1], "step"))
				{
					if(atoi(com[2]) >= 0 && atoi(com[2]) <= staff->n)
						Staff_InsereEmptyStep(staff, atoi(com[2]));
				}
			}
		}
		Window_InitBody();
		Score_Print(score, SDL_SetRect(100, 200, 0, 0));
		Window_ApplyZoom(r);
		Window_DrawBody();
		Window_Print();
		SDL_Flip(Window->screen);
		
	}

	Staff_Free(&staff);
	return 1;
}
