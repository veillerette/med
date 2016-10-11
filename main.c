#include "Step.h"
#include "Staff.h"
#include "Test.h"
#include "System.h"
#include "Window.h"

int main(int argc, char *argv[])
{
	Step *step = NULL;
	Score *score = NULL;
	double i;
	
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
		
		Window_Print();
		for(i = 0.05; i < 1.0; i+=0.05)
			test_note(i, 0, (int)(i*800+i*200), 200, Window->screen);
		SDL_Flip(Window->screen);
		
		Window_WaitMouse(NULL, NULL);
		
		Window_Quit();
	}
	
	exit(EXIT_SUCCESS);
}
