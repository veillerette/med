#include "Step.h"
#include "Staff.h"
#include "Test.h"
#include "System.h"
#include "Window.h"

int main(int argc, char *argv[])
{
	Step *step = NULL;
	Score *score = NULL;
	SDL_Event event;
	double r=2.0;
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
	else if(2 == argc && !strcmp(argv[1], "-sdl"))
	{	Staff *staff = NULL;
		int ctrl = 0;
		int mouse = 0;
		int clic_x, clic_y;
		int tomaj = 0;
		int ev = 0;
		SDL_Rect redim;
		SDL_Rect pos;
		
		
		Window_Init();
		Window_CreateWindow(Window->max_width-200, Window->max_height-200, "Ma Super Fenetre\n");
		
		
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
		Staff_Init(staff, 2, BLANCHE, CLE_SOL, 0);
		
		Staff_AddNote(staff, 0, 0, ConvertStringToID("do4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 0, 1, ConvertStringToID("do4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 0, 2, ConvertStringToID("do4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 0, 3, ConvertStringToID("do4"), NOTE_DEFAULT, NOIRE);
		Staff_AddNote(staff, 1, 0, ConvertStringToID("do4"), NOTE_DEFAULT, RONDE);
		Staff_AddNote(staff, 2, 0, ConvertStringToID("do4"), NOTE_DEFAULT, BLANCHE);
		Staff_AddNote(staff, 2, 1, ConvertStringToID("do4"), NOTE_DEFAULT, BLANCHE);
		Staff_DiviseRest(staff, 3, 0);
		Staff_Console(staff);
		
		Graphics_LoadAll();
		/*Window_ShowAllGraphics();*/

		redim.x = 0;
		redim.y = 0;
		redim.w = Window->body->w;
		redim.h = Window->body->h;
		pos.x = Window->pos_body->x;
		pos.y = Window->pos_body->y;
		
		Staff_Print(staff, SDL_SetRect(100, 200, 0, 0), Window->body);
		Window_Print();
		Window_DrawBodyShrink(r);
		
		
		SDL_Flip(Window->screen);
		
		while(c)
		{
			ev = SDL_PollEvent(&event);
			switch(event.type)
			{	
				case SDL_QUIT:	
					c = 0;
					break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					if((event.key.keysym.sym == SDLK_LCTRL) ||
					(event.key.keysym.sym == SDLK_RCTRL))
						ctrl = 1 - ctrl;
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
							r++;
							m=1;
							break;
						case SDL_BUTTON_WHEELUP:
							if(r <= 1)
								r = 1.0;
							else
							{
								r--;
								m=1;
							}
							break;
					}
					break;
				case SDL_MOUSEMOTION:
					if(mouse)
					{
						if(clic_x > event.motion.x)
						{
							pos.x-=BASE_MOTION;
							if(pos.x <= Window->pos_body->x - 1)
								redim.x+=BASE_MOTION;
							printf("pos_x = %d, redim.x = %d\n", pos.x, redim.x);
							clic_x = event.motion.x;
							if(pos.x < Window->pos_body->x)
								pos.x+=BASE_MOTION;
							tomaj = 1;
						}
						else if(clic_x < event.motion.x)
						{
							
							redim.x-=BASE_MOTION;
							if(redim.x<0)
								redim.x = 0;
							if(redim.x == 0)
								pos.x+=BASE_MOTION;
							
							clic_x = event.motion.x;
							tomaj = 1;
						}
					}
					break;
			}
			if(m)
			{
				
				Window_DrawBodyShrink2(r, redim, pos);
				SDL_Flip(Window->screen);
				
				m=0;
			}
			if(tomaj && !ev)
			{
				Window_DrawBodyShrink2(r, redim, pos);
				SDL_Flip(Window->screen);
				
				tomaj = 0;
			}
		}
		
		/* Window_WaitMouse(NULL, NULL);
		*/
		
		Graphics_Quit();
		Window_Quit();
	}
	
	exit(EXIT_SUCCESS);
}
