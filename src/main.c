#include "../include/Step.h"
#include "../include/Staff.h"
#include "../include/Test.h"
#include "../include/System.h"
#include "../include/Window.h"
#include "../include/Parser.h"
#include "../include/Events.h"
#include "../include/ABC.h"
#include "../include/Menu.h"

#include "../include/Audio.h"

int main(int argc, char *argv[])
{
	SDL_Event event;
	double r=2.0;
	int c=1;
	int ev;
	const SDL_VideoInfo* info = NULL;
	int clic_x = -1, clic_y = -1, mouse = 0;
	int tomaj = 0;
	Menu *menu = NULL;
	Uint32 time = 0;
	Uint32 sauv = 0;
	int x=0,y=0;
	int m = 0;
	char title[100] = "";
	Score *new_score = NULL;
	int i;
	
	sprintf(title, "%s %g", SYS_NAME, SYS_VERSION);
		
	
	Window_Init();
	menu = Menu_Create();
	new_score = Score_Alloc();
	Score_Init(new_score);
	
	if(argc==1)
	{
	
		Staff_Init(new_score->lst[0], 4, NOIRE, CLE_SOL, 0);
		Score_AddEmpty(new_score);
		for(i = 0; i < 10; i++)
			Score_AddEmptyStep(new_score);
		Staff_ChangeArmure(new_score->lst[0], 0, 0);
		Staff_ChangeArmure(new_score->lst[1], 0, 0);
		
	}
	else if(3 == argc && !strcmp(argv[1], "-abc"))
	{
		printf("begin ABC...\n");
		new_score = ABC_ParseFile(argv[2]);
		printf("mid ABC...\n");
		Score_AddEmpty(new_score);
		printf("end ABC\n");
	}

	Audio_Init(AudioConfig_Init());
	Audio_AssignateScore(new_score);
	Audio_GoToStep(0);
	
	info = SDL_GetVideoInfo();
	Window_CreateWindow(info->current_w, info->current_h, title);
	main_events = EventData_Alloc();

	
	MainEvents_AssociateScore(new_score);
	Graphics_LoadAll();
	
	EventData_SetBase(main_events, Window->pos_body);
	EventData_SetZoom(main_events, r);
	
	Score_Print(new_score, SDL_SetRect(100, 450, 0, 0));
	
	EventData_Console(main_events);
	
	Window_ApplyZoom(r);
	
	Window_DrawBody();
	
	Window_Print();
	Window_TestBox(Window->screen, Window->pos_body, r);
	Menu_Aff(menu, &x, &y);
	Menu_Console(menu->lst, 0);
	
	
	SDL_Flip(Window->screen);
	
	time = SDL_GetTicks();
	sauv = time;
	while(c)
	{
		ev = SDL_PollEvent(&event);
		sauv = SDL_GetTicks();
		Window_LittleEvent(event, &r, &c, &mouse, &clic_x, 
						&clic_y, &tomaj, &m);
		
		if(main_audio->need_refresh)
		{
			Window_DrawBody();
			Window_Print();
			Window_TestBox(Window->screen, Window->pos_body, r);
			Menu_Aff(menu, &x, &y);
			SDL_Flip(Window->screen);
			main_audio->need_refresh = 0;
		}
		if(m)
		{
			Window_ApplyZoom(r);
			Window_DrawBody();
			Window_Print();
			EventData_SetZoom(main_events, r);
			Window_TestBox(Window->screen, Window->pos_body, r);
			Menu_Aff(menu, &x, &y);
			SDL_Flip(Window->screen);
			time = SDL_GetTicks();
			m=0;
		}
		if(tomaj && !ev)
		{
			Window_DrawBody();
			Window_Print();
			Window_TestBox(Window->screen, Window->pos_body, r);
			Menu_Aff(menu, &x, &y);
			SDL_Flip(Window->screen);
			time = SDL_GetTicks();
			tomaj = 0;
		}
		if(!ev)
		{
			SDL_Delay(2);
			continue;
		}
		switch(Menu_PollMouse(menu, event))
		{
			case QUIT:
				c = 0;
				continue;
			case FORCE_SCOREMAJ:
				EventData_Flush(main_events);
				Score_Print(new_score, SDL_SetRect(100, 450, 0, 0));
				Window_ApplyZoom(r);
			case FORCE_MAJ:
				Window_DrawBody();
				Window_Print();
				Window_TestBox(Window->screen, Window->pos_body, r);
				Menu_Aff(menu, &x, &y);
				
				SDL_Flip(Window->screen);

				break;
			
			default:
				break;
		}
		
		if(menu->select != NULL)
			continue;
		
		switch(Events_PollMouse(event))
		{
			case QUIT:
				c = 0;
				continue;
			case HOVER:
			case SELECT:
				/*Window_ApplyZoom(r);*/
				Window_DrawBody();
				Window_Print();
				Window_TestBox(Window->screen, Window->pos_body, r);
				Menu_Aff(menu, &x, &y);
				SDL_Flip(Window->screen);
				break;
			case FORCE_MAJ:
				EventData_Flush(main_events);
				Score_Print(new_score, SDL_SetRect(100, 450, 0, 0));
				EventData_Console(main_events);
				Window_ApplyZoom(r);
				Window_DrawBody();
				Window_Print();
				
				main_events->hover = Events_GetAreaByPixelAndType((int)((event.button.x - main_events->base->x) * 1.0 * main_events->r), 
					 (int)((event.button.y - main_events->base->y) * 1.0 * main_events->r), EVENT_ADDNOTE);
				
				Window_TestBox(Window->screen, Window->pos_body, r);
				Menu_Aff(menu, &x, &y);
				SDL_Flip(Window->screen);
				mouse = 0;
			default:
				break;
		}
		switch(Events_PollKeyboard(event))
		{
			case QUIT:
				c = 0;
				continue;
			case FORCE_MAJ:
				EventData_Flush(main_events);
				Score_Print(new_score, SDL_SetRect(100, 450, 0, 0));
				EventData_Console(main_events);
				Window_ApplyZoom(r);
				Window_DrawBody();
				Window_Print();
				Window_TestBox(Window->screen, Window->pos_body, r);
				Menu_Aff(menu, &x, &y);
				SDL_Flip(Window->screen);
				break;
			default:
				break;
		}
		
		if(SDL_GetTicks()-sauv < 10)
		{
			SDL_Delay(1);
		}
		
	}
	if(new_score != NULL)
		Score_Free(&new_score);

	
	if(0 && 2 == argc && !strcmp(argv[1], "-parser"))
	
	{
		Window_CreateWindow(Window->max_width-200, 600, "Ma Super Fenetre\n");
		Graphics_LoadAll();
		Console_Parser();
	}

	Audio_Quit();
	EventData_Free(&main_events);
	Graphics_Quit();
	Menu_Free(&menu);
	Window_Quit();
	
	printf("end progam\n");
	exit(EXIT_SUCCESS);
}
