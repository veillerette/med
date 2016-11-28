#include "../include/Step.h"
#include "../include/Staff.h"
#include "../include/Test.h"
#include "../include/System.h"
#include "../include/Window2.h"
#include "../include/Parser.h"
#include "../include/Events.h"
#include "../include/ABC.h"
#include "../include/Menu.h"

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
	
	Window_Init();
	menu = Menu_Create();
	
	if((2 == argc && !strcmp(argv[1], "-sdl")) || 1 == argc)
	{	
		Staff *staff = NULL;
		
		info = SDL_GetVideoInfo();
		Window_CreateWindow(info->current_w, info->current_h, "Ma Super Fenetre\n");
		main_events = EventData_Alloc();
		 

		staff = Staff_Alloc("Portée trop géniale");
		Staff_Init(staff, 4, NOIRE, CLE_SOL, 0);
		Staff_ChangeArmure(staff, 0, 3);
		Staff_AddNote(staff, 0, 0, ConvertStringToID("c4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 0, 1, ConvertStringToID("g4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 0, 2, ConvertStringToID("g4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 0, 3, ConvertStringToID("g4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 1, 0, ConvertStringToID("c4"), NOTE_DEFAULT, CROCHE);
		Staff_AddNote(staff, 1, 2, ConvertStringToID("e5"), NOTE_DEFAULT, CROCHE);
		Staff_Console(staff);
		
		Graphics_LoadAll();
		
		EventData_SetBase(main_events, Window->pos_body);
		EventData_SetZoom(main_events, r);
		
		Staff_Print(staff, SDL_SetRect(100, 200, 0, 0), Window->body[0]);
		
		EventData_Console(main_events);
		
		Window_ApplyZoom(r);
		
		Window_DrawBody();
		
		Window_Print();
		Menu_Aff(menu, &x, &y);
		Menu_Console(menu->lst, 0);
		
		
		SDL_Flip(Window->screen);
		time = SDL_GetTicks();
		sauv = time;
		while(c)
		{
			ev = SDL_PollEvent(&event);
			sauv = SDL_GetTicks();
			time = sauv;
			Window_LittleEvent(event, &r, &c, ev, &mouse, &clic_x, 
							&clic_y, &tomaj, &time);
			
			switch(Menu_PollMouse(menu, event))
			{
				case QUIT:
					c = 0;
					continue;
				case FORCE_MAJ:
					printf("FORCE MAAAAAAAAAAAAAAAAAAAAAAAAJ\n");
					x=0;
					y=0;
					printf("force maj MENU\n");
					Window_DrawBody();
					Window_Print();
					Menu_Aff(menu, &x, &y);
					SDL_Flip(Window->screen);
					
					break;
				default:
					break;
			}
			
			switch(Events_PollMouse(event))
			{
				case QUIT:
					c = 0;
					continue;
				case HOVER:
				case SELECT:
					/*Window_ApplyZoom(r);*/
					Window_DrawBody();
					/*Window_Print();*/
					Window_TestBox(Window->screen, Window->pos_body, r);
					SDL_Flip(Window->screen);
					break;
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
					Staff_Print(staff, SDL_SetRect(100, 200, 0, 0), Window->body[0]);
					EventData_Console(main_events);
					Window_ApplyZoom(r);
					Window_DrawBody();
					Window_Print();
					SDL_Flip(Window->screen);
					break;
				default:
					break;
			}
			
			if(sauv-SDL_GetTicks() < 10)
				SDL_Delay(1);
		}
		
		
		
		Graphics_Quit();
	}
	else if(2 == argc && !strcmp(argv[1], "-parser"))
	
	{
		Window_CreateWindow(Window->max_width-200, 600, "Ma Super Fenetre\n");
		Graphics_LoadAll();
		Console_Parser();
	}
	else if(3 == argc && !strcmp(argv[1], "-abc"))
	{
		Score *score = NULL;
		
		score = ABC_ParseFile(argv[2]);
		
		info = SDL_GetVideoInfo();
		Window_CreateWindow(info->current_w, info->current_h, "Ma Super Fenetre\n");
		main_events = EventData_Alloc();
		
		Graphics_LoadAll();
		
		EventData_SetBase(main_events, Window->pos_body);
		EventData_SetZoom(main_events, r);
		EventData_Console(main_events);
		
		Staff_Print(score->lst[0], SDL_SetRect(100, 200, 0, 0), Window->body[0]);
		
		
		Window_ApplyZoom(r);
		
		Window_DrawBody();
		Window_Print();
		SDL_Flip(Window->screen);
		time = SDL_GetTicks();
		
		Staff_Console(score->lst[0]);
		while(c)
		{
			ev = SDL_PollEvent(&event);
			Window_LittleEvent(event, &r, &c, ev, &mouse, &clic_x, 
							&clic_y, &tomaj, &time);
			switch(Events_PollMouse(event))
			{
				case QUIT:
					c = 0;
					break;
				case HOVER:
				case SELECT:
					Window_ApplyZoom(r);
					Window_DrawBody();
					Window_Print();
					Window_TestBox(Window->screen, Window->pos_body, r);
					SDL_Flip(Window->screen);
					break;
				default:
					break;
			}
			switch(Events_PollKeyboard(event))
			{
				case QUIT:
					c = 0;
					break;
				case FORCE_MAJ:
					EventData_Flush(main_events);
					Staff_Print(score->lst[0], SDL_SetRect(100, 200, 0, 0), Window->body[0]);
					EventData_Console(main_events);
					Window_ApplyZoom(r);
					Window_DrawBody();
					Window_Print();
					SDL_Flip(Window->screen);
					break;
				default:
					break;
			}
		}
		Graphics_Quit();
		Score_Free(&score);
	}
	
	Menu_Free(&menu);
	Window_Quit();
	
	printf("end progam\n");
	exit(EXIT_SUCCESS);
}
