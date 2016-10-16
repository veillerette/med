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

		Step_AddNote(step, 0, 7 * 5, NOTE_DEFAULT, NOIRE);
		ToNote_ConsolePrintf(step->notes);
		

		Step_Free(&step);
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
	{
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
		
		Graphics_LoadAll();
		Window_ShowAllGraphics();
		Window_Print();
		Window_DrawBodyShrink(r);
		
		
		SDL_Flip(Window->screen);
		
		while(c)
		{
			SDL_WaitEvent(&event);
			switch(event.type)
			{	
				case SDL_QUIT:	
					c = 0;
					break;
				case SDL_MOUSEBUTTONDOWN:
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
						default:
							c = 0;
							break;
					}
				break;
			}
			if(m)
			{
				
				printf("r=%f\n", r);
				Window_DrawBodyShrink(r);
				SDL_Flip(Window->screen);
				
				m=0;
			}
		}
		
		/* Window_WaitMouse(NULL, NULL);
		*/
		
		Graphics_Quit();
		Window_Quit();
	}
	
	exit(EXIT_SUCCESS);
}
