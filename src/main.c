#include "../include/Step.h"
#include "../include/Staff.h"
#include "../include/Test.h"
#include "../include/System.h"
#include "../include/Window.h"
#include "../include/Parser.h"
#include "../include/Events.h"

int main(int argc, char *argv[])
{
	Step *step = NULL;
	Score *score = NULL;
	SDL_Event event;
	double r=1.0;
	int c=1;
	int m=0;
	/*SDL_Surface *surf = NULL;
	SDL_Rect pos;
	int x,y;*/
	
	if(2 == argc && (!strcmp(argv[1], "-t") || !strcmp(argv[1], "-test")))
		Test_Main();
	else if(2 == argc && !strcmp(argv[1], "-step"))
	{
		step = Step_Alloc(4, NOIRE, 0, 0, STEP_DEFAULT);
		Step_Init(step);
		Step_ConsolePrintf(step);

		Step_AddNote(step, 0, ConvertStringToID("do4"), NOTE_DEFAULT, NOIRE);
		ToNote_ConsolePrintf(step->notes);
		

		Step_Free(&step);
	}
	else if(2 == argc && !strcmp(argv[1], "-staff"))
	{
		Staff *staff = NULL;
		staff = Staff_Alloc("Portée trop géniale");
		Staff_Init(staff, 4, NOIRE, CLE_SOL, 0);
		
		Staff_AddNote(staff, 5, 0, ConvertStringToID("do4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 5, 1, ConvertStringToID("sol3"), NOTE_DEFAULT, DOUBLECROCHE);
		Staff_Console(staff);
		
		Staff_Free(&staff);
	}
	else if(2 == argc && !strcmp(argv[1], "-sign"))
	{
		score = Score_Alloc();
		Score_AddSign(score, SIGN_TEMPO, 50, 0, NULL);
		Score_AddSign(score, SIGN_TEMPO, 150, 400, NULL);
		Score_AddSign(score, SIGN_TEMPO, 30, 319, NULL);
		Score_AddSign(score, SIGN_VELOCITY, 127, 0, NULL);
		Score_AddSign(score, SIGN_AUTHOR, 0, 0, "Paul Ankman");
		
		Score_ShowSignConsole(score);
		printf("\n");
		
		Score_DeleteSign(score, SIGN_VELOCITY, 0);
		Score_DeleteSign(score, SIGN_TEMPO, 0);
		Score_DeleteSign(score, SIGN_TEMPO, 400);

		Score_ShowSignConsole(score);
		Score_Free(&score);
	}
	else if((2 == argc && !strcmp(argv[1], "-sdl")) || 1 == argc)
	{	
		Staff *staff = NULL;
		int ctrl = 0;
		int mouse = 0;
		int clic_x = 0, clic_y = 0;
		int tomaj = 0;
		int ev;
		SDL_Rect redim;
		SDL_Rect pos;
		
		Window_Init();
		Window_CreateWindow(Window->max_width-200, Window->max_height-200, "Ma Super Fenetre\n");
		main_events = EventData_Alloc();
		 
		/*for(i = 0.05; i < 1.0; i+=0.05)
			test_note(i, 0, (int)(i*800+i*200), 200, Window->screen);
		*/
		/*surf = Note_CreateHead(200, 150);
		y = (int)(400 * cos(30 * M_PI/180) + 400 * sin(30 * M_PI/180));
		x = (int)(400 * sin(30 * M_PI/180) + 400 * cos(30 * M_PI/180));
		pos.x = 400;
		pos.y = 400;
		Draw_Border(surf, 1);
		SDL_BlitSurface(surf, NULL, Window->screen, &pos);
		rectangleRGBA(Window->screen, 400+surf->w/2, 400+surf->h/2, 400+surf->w/2+10, 400+surf->h/2+10, 0, 255, 0, 170);*/
		staff = Staff_Alloc("Portée trop géniale");
		Staff_Init(staff, 4, NOIRE, CLE_SOL, 0);
		Staff_ChangeArmure(staff, 0, 3);
		Staff_AddNote(staff, 0, 0, ConvertStringToID("c4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 0, 1, ConvertStringToID("g4"), NOTE_DEFAULT, NOIRE);
		
		Staff_AddNote(staff, 0, 2, ConvertStringToID("e4"), NOTE_FLAT, NOIRE);
		Staff_AddNote(staff, 0, 3, ConvertStringToID("g4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 1, 0, ConvertStringToID("f4"), NOTE_DEFAULT, BLANCHE);
		Staff_AddNote(staff, 1, 1, ConvertStringToID("b4"), NOTE_DEFAULT, BLANCHE);
		Staff_AddNote(staff, 2, 0, ConvertStringToID("d5"), NOTE_DOUBLEFLAT, NOIRE);
		Staff_AddNote(staff, 2, 1, ConvertStringToID("a5"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 2, 2, ConvertStringToID("c6"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 2, 3, ConvertStringToID("b5"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 3, 0, ConvertStringToID("g4"), NOTE_DEFAULT, CROCHE);
		Staff_AddNote(staff, 3, 1, ConvertStringToID("a4"), NOTE_DEFAULT, CROCHE);
		Staff_AddNote(staff, 3, 2, ConvertStringToID("c4"), NOTE_DEFAULT, CROCHE);
		Staff_AddNote(staff, 3, 3, ConvertStringToID("e4"), NOTE_DEFAULT, CROCHE);
		Staff_AddNote(staff, 3, 4, ConvertStringToID("g4"), NOTE_DEFAULT, CROCHE);
		Staff_AddNote(staff, 3, 5, ConvertStringToID("a4"), NOTE_DEFAULT, CROCHE);
		Staff_AddNote(staff, 3, 6, ConvertStringToID("c4"), NOTE_DEFAULT, CROCHE);
		Staff_AddNote(staff, 3, 7, ConvertStringToID("e4"), NOTE_DEFAULT, CROCHE);
		Staff_AddNote(staff, 0, 1, ConvertStringToID("g4"), NOTE_DEFAULT, DOUBLECROCHE);
		/*
		Staff_ChangeArmure(staff, 4, 2);
		Staff_AddNote(staff, 0, 0, ConvertStringToID("g4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 0, 2, ConvertStringToID("d5"), NOTE_SHARP, NOIRE);
		Staff_AddNote(staff, 0, 3, ConvertStringToID("c5"), NOTE_FLAT, NOIRE);*/
		/*Staff_AddNote(staff, 0, 1, ConvertStringToID("c4"), NOTE_DEFAULT, CROCHE);
		Staff_ChangeRestStatus(staff, 0, 1, 1);
		Staff_AddNote(staff, 0, 2, ConvertStringToID("c4"), NOTE_DEFAULT, NOIRE);
		Staff_ChangeRestStatus(staff, 0, 2, 1);
		Staff_AddNote(staff, 0, 2, ConvertStringToID("c4"), NOTE_DEFAULT, DOUBLECROCHE);
		Staff_ChangeRestStatus(staff, 0, 2, 1);
		Staff_AddNote(staff, 0, 3, ConvertStringToID("c4"), NOTE_DEFAULT, TRIPLECROCHE);
		Staff_ChangeRestStatus(staff, 0, 3, 1);
		Staff_AddNote(staff, 0, 4, ConvertStringToID("c4"), NOTE_DEFAULT, QUADRUPLECROCHE);
		Staff_ChangeRestStatus(staff, 0, 4, 1);
		Staff_AddNote(staff, 1, 0, ConvertStringToID("c4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 1, 1, ConvertStringToID("c4"), NOTE_DEFAULT, CROCHE);
		Staff_AddNote(staff, 1, 2, ConvertStringToID("c4"), NOTE_DEFAULT, NOIRE);*/
		Staff_Console(staff);
		
		Graphics_LoadAll();

		redim.x = 0;
		redim.y = 0;
		redim.w = Window->body->w;
		redim.h = Window->body->h;
		pos.x = Window->pos_body->x;
		pos.y = Window->pos_body->y;
		pos.w = Window->pos_body->w;
		pos.h = Window->pos_body->h;
		
		EventData_SetBase(main_events, Window->pos_body);
		
		Staff_Print(staff, SDL_SetRect(100, 200, 0, 0), Window->body);
		Window_Print();
		Window_DrawBodyShrink(r, redim, pos);
		
		
		EventData_Console(main_events);
		Window_TestBox(Window->screen, &pos, r);
		
		ev = SDL_Flip(Window->screen);
		
		
		printf("%d\n", ev);
		while(c)
		{
			ev = SDL_PollEvent(&event);
			switch(event.type)
			{	
				case SDL_QUIT:	
					c = 0;
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE)
					{
						c = 0;
						break;
					}
					if(((event.key.keysym.sym == SDLK_LCTRL) ||
					(event.key.keysym.sym == SDLK_RCTRL)) && !ctrl)
						ctrl = 1;
					break;
				case SDL_KEYUP:
					if(((event.key.keysym.sym == SDLK_LCTRL) ||
					(event.key.keysym.sym == SDLK_RCTRL)) && ctrl)
						ctrl = 0;
					break;
				case SDL_MOUSEBUTTONUP:
					clic_x = event.button.x;
					mouse = 0;
					break;
				case SDL_MOUSEBUTTONDOWN:
					clic_x = event.button.x;
					clic_y = event.button.y;
					if(ClicInRect(clic_x, clic_y, Window->pos_body))
						mouse = 1;
					switch(event.button.button)
					{
						case SDL_BUTTON_WHEELDOWN:
							if(ctrl)
							{
								r++;
								m=1;
							}
							else
							{
								pos.y+=BASE_WHEEL;
							
								if(pos.y >= Window->screen->h)
									pos.y = Window->screen->h;
								tomaj = 1;
							}
							break;
						case SDL_BUTTON_WHEELUP:
							if(ctrl)
							{
								if(r <= 1)
									r = 1.0;
								else
								{
									r--;
									m=1;
								}
							}
							else
							{
								pos.y-=BASE_WHEEL;
								if(pos.y <= Window->pos_body->y - 1)
									redim.y+=BASE_WHEEL;
							
								if(pos.y < Window->pos_body->y)
									pos.y+=BASE_WHEEL;
								tomaj = 1;
							}
							break;
					}
					break;
				case SDL_MOUSEMOTION:
					if(mouse)
					{
						if(clic_x > event.motion.x)
						{
							pos.x-= (clic_x - event.motion.x);
							if(pos.x <= Window->pos_body->x - 1)
								redim.x+=(clic_x - event.motion.x);
							
							if(pos.x < Window->pos_body->x)
								pos.x+=(clic_x - event.motion.x);
							tomaj = 1;
							clic_x = event.motion.x;
						}
						else if(clic_x < event.motion.x)
						{
							
							redim.x-=(event.motion.x - clic_x);
							if(redim.x<0)
								redim.x = 0;
							if(redim.x == 0)
								pos.x+=(event.motion.x - clic_x);
							
							clic_x = event.motion.x;
							tomaj = 1;
						}
						if(clic_y > event.motion.y)
						{
							pos.y-= (clic_y - event.motion.y);
							if(pos.y <= Window->pos_body->y - 1)
								redim.y+=(clic_y - event.motion.y);
							
							if(pos.y < Window->pos_body->y)
								pos.y+=(clic_y - event.motion.y);
							tomaj = 1;
							clic_y = event.motion.y;
						}
						else if(clic_y < event.motion.y)
						{
							
							redim.y-=(event.motion.y - clic_y);
							if(redim.y<0)
								redim.y = 0;
							if(redim.y == 0)
								pos.y+=(event.motion.y - clic_y);
							
							clic_y = event.motion.y;
							tomaj = 1;
						}
					}
					break;
			}
			if(m)
			{
				SDL_FillRect(Window->screen, Window->pos_body, SDL_MapRGB(Window->screen->format, 235, 235, 235));
				Window_DrawBodyShrink(r, redim, pos);
				Window_TestBox(Window->screen, SDL_SetRect(pos.x-redim.x, pos.y-redim.y, 0, 0), (int)r);
				
				SDL_Flip(Window->screen);
				
				m=0;
			}
			if(tomaj)
			{
				/*printf("x=%d, y=%d, w=%d, h=%d\n", Window->pos_body->x, Window->pos_body->y, Window->pos_body->w, Window->pos_body->h);*/
				SDL_FillRect(Window->screen, Window->pos_body, SDL_MapRGB(Window->screen->format, 235, 235, 235));
				Window_DrawBodyShrink(r, redim, pos);
				
				Window_TestBox(Window->screen, SDL_SetRect(pos.x-redim.x, pos.y-redim.y, 0, 0), (int)r);
				
				SDL_Flip(Window->screen);
				
				tomaj = 0;
			}
			if(!ev)
				SDL_Delay((int)(1000/60.0));
		}
		
		/* Window_WaitMouse(NULL, NULL);
		*/
		
		Graphics_Quit();
		Window_Quit();
	}
	else if(2 == argc && !strcmp(argv[1], "-img"))
	{
		Window_Init();
		Window_CreateWindow(Window->max_width-200, Window->max_height-200, "Ma Super Fenetre\n");
		
		Graphics_LoadAll();
		Graphics_Quit();
		Window_Quit();
	}
	else if(2 == argc && !strcmp(argv[1], "-sdlmem"))
	{
		SDL_Surface *window = NULL;
		SDL_Init(SDL_INIT_VIDEO);
		TTF_Init();
		
		window = SDL_SetVideoMode(1720, 900, 32, SDL_DOUBLEBUF);
		
		SDL_FreeSurface(window);
		
		printf("end\n");
		
		TTF_Quit();
		SDL_Quit();
	}
	else if(2 == argc && !strcmp(argv[1], "-parser"))
	{
		Window_Init();
		Window_CreateWindow(Window->max_width-200, Window->max_height-200, "Ma Super Fenetre\n");
		Graphics_LoadAll();
		Console_Parser();
		Window_Quit();
	}
	
	printf("end of main\n");
	exit(EXIT_SUCCESS);
}
