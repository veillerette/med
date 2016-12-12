#include "../include/Step.h"
#include "../include/Staff.h"
#include "../include/Test.h"
#include "../include/System.h"
#include "../include/Window.h"
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
	int m = 0;
	Staff *staff = NULL;
	Score *score = NULL;
	char title[100] = "";
	int i;
	
	sprintf(title, "%s %g", SYS_NAME, SYS_VERSION);
		
	
	Window_Init();
	menu = Menu_Create();
	if(argc==1)
	{
		staff = Staff_Alloc("Portée trop géniale");
		Staff_Init(staff, 4, NOIRE, CLE_SOL, 0);
		Staff_ChangeArmure(staff, 0, 3);
		Staff_AddNote(staff, 0, 0, 92, NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 0, 1, ConvertStringToID("g4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 0, 2, ConvertStringToID("g4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 0, 3, ConvertStringToID("g4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 1, 0, ConvertStringToID("c6"), NOTE_DEFAULT, CROCHE);
		Staff_AddNote(staff, 1, 2, ConvertStringToID("e5"), NOTE_DEFAULT, CROCHE);
		Staff_AddNote(staff, 2, 0, ConvertStringToID("g4"), NOTE_DEFAULT, DOUBLECROCHE);
		Staff_AddNote(staff, 2, 1, ConvertStringToID("g4"), NOTE_DEFAULT, DOUBLECROCHE);
		Staff_AddNote(staff, 2, 2, ConvertStringToID("g4"), NOTE_DEFAULT, DOUBLECROCHE);
		Staff_AddNote(staff, 2, 3, ConvertStringToID("g4"), NOTE_DEFAULT, DOUBLECROCHE);
		Staff_AddNote(staff, 2, 4, ConvertStringToID("g4"), NOTE_DEFAULT, DOUBLECROCHE);
		Staff_AddNote(staff, 2, 5, ConvertStringToID("g4"), NOTE_DEFAULT, DOUBLECROCHE);
		Staff_AddNote(staff, 3, 0, ConvertStringToID("a4"), NOTE_DEFAULT, BLANCHE);
		for(i = 0; i < 200; i++)
			Staff_AddEmptyStep(staff);
		Staff_Console(staff);
	}
	else if(3 == argc && !strcmp(argv[1], "-abc"))
	{
		score = ABC_ParseFile(argv[2]);
		staff = score->lst[0];
	}


	info = SDL_GetVideoInfo();
	Window_CreateWindow(info->current_w, info->current_h, title);
	main_events = EventData_Alloc();

	
	Graphics_LoadAll();
	
	EventData_SetBase(main_events, Window->pos_body);
	EventData_SetZoom(main_events, r);
	
	Staff_Print(staff, SDL_SetRect(100, 200, 0, 0), Window->body[0]);
	
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
				Staff_Print(staff, SDL_SetRect(100, 200, 0, 0), Window->body[0]);
				Window_ApplyZoom(r);
			case FORCE_MAJ:
				Window_DrawBody();
				Window_Print();
				Window_TestBox(Window->screen, Window->pos_body, r);
				Menu_Aff(menu, &x, &y);
				
				SDL_Flip(Window->screen);
				continue;
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
				Staff_Print(staff, SDL_SetRect(100, 200, 0, 0), Window->body[0]);
				EventData_Console(main_events);
				Window_ApplyZoom(r);
				Window_DrawBody();
				Window_Print();
				Window_TestBox(Window->screen, Window->pos_body, r);
				Menu_Aff(menu, &x, &y);
				SDL_Flip(Window->screen);
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
				Window_TestBox(Window->screen, Window->pos_body, r);
				Menu_Aff(menu, &x, &y);
				SDL_Flip(Window->screen);
				break;
			default:
				break;
		}
		
		if(sauv-SDL_GetTicks() < 10)
			SDL_Delay(1);
		
	}
	if(score != NULL)
		Score_Free(&score);
	else
		Staff_Free(&staff);
	
	
	if(0 && 2 == argc && !strcmp(argv[1], "-parser"))
	
	{
		Window_CreateWindow(Window->max_width-200, 600, "Ma Super Fenetre\n");
		Graphics_LoadAll();
		Console_Parser();
	}

	
	EventData_Free(&main_events);
	Graphics_Quit();
	Menu_Free(&menu);
	Window_Quit();
	
	printf("end progam\n");
	exit(EXIT_SUCCESS);
}
