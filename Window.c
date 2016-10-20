#include "Window.h"

WindowData *Window = NULL;





WindowData *WindowData_Alloc(void)
{
	WindowData *temp = (WindowData *)malloc(sizeof(WindowData));
	memtest(temp);
	
	temp->screen = NULL;
	temp->width = 0;
	temp->height = 0;
	temp->max_width = 0;
	temp->max_height = 0;
	temp->state = STATE_ALLOC;
	temp->ratio = 1.0;
	
	temp->menu = NULL;
	temp->pos_menu = NULL;
	temp->pal = NULL;
	temp->pos_pal = NULL;
	temp->body = NULL;
	temp->body_use = NULL;
	temp->pos_body = NULL;
	return temp;
}

void WindowData_Free(WindowData **window)
{
	if(*window != NULL)
	{
		if((*window)->screen != NULL)
			SDL_FreeSurface((*window)->screen);
		
		free(*window);
		*window = NULL;
	}
}

int Window_GetScreenSize(int *width, int *height)
{
	const SDL_VideoInfo *infos = SDL_GetVideoInfo();
	
	if(NULL == infos)
		return 0;
	
	if(width != NULL)
		*width = infos->current_w;
	if(height != NULL)
		*height = infos->current_h;
	
	return 1;
}

int Window_Init(void)
{
	if(NULL == Window)
	{
		Window = WindowData_Alloc();
	}
	if(Window->state < STATE_SDLINIT)
	{
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			colorprintf(RED, "Error while launching SDL\n");
			exit(EXIT_FAILURE);
		}
		Window->state = STATE_SDLINIT;
	}
	if(Window->state < STATE_TTFINIT)
	{
		if(TTF_Init() != 0)
		{
			colorprintf(RED, "Error while lauching TTF\n");
			exit(EXIT_FAILURE);
		}
		Window->state = STATE_TTFINIT;
	}
	
	Window_GetScreenSize(&(Window->max_width), &(Window->max_height));
	
	return 1;
}

void Window_Quit(void)
{
	if(Window != NULL)
	{
		Window->state = STATE_QUIT;
		
		if(Window->screen != NULL)
			SDL_FreeSurface(Window->screen);
		
		if(Window->menu != NULL)
			SDL_FreeSurface(Window->menu);
		
		if(Window->pal != NULL)
			SDL_FreeSurface(Window->pal);
		
		if(Window->body != NULL)
			SDL_FreeSurface(Window->body);
		
		if(Window->body_use != NULL)
			SDL_FreeSurface(Window->body_use);
		
		if(Window->pos_menu != NULL)
			SDL_FreeRect(&(Window->pos_menu));
		
		if(Window->pos_pal != NULL)
			SDL_FreeRect(&(Window->pos_pal));
		
		if(Window->pos_body != NULL)
			SDL_FreeRect(&(Window->pos_body));
		
			
		free(Window);
		Window = NULL;
		
		TTF_Quit();
		SDL_Quit();
	}
}

static int Window_InitMenu(SDL_Surface *menu)
{
	if(NULL == menu)
		return 0;
	
	SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 255, 255, 255));
	
	SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 255, 200, 200));
	return 1;
}

static int Window_InitPal(SDL_Surface *pal)
{
	if(NULL == pal)
		return 0;
	
	SDL_FillRect(pal, NULL, SDL_MapRGB(pal->format, 255, 255, 255));
	
	SDL_FillRect(pal, NULL, SDL_MapRGB(pal->format, 200, 255, 200));
	return 1;
}

static int Window_InitBody(SDL_Surface *body)
{
	if(NULL == body)
		return 0;
	
	SDL_FillRect(body, NULL, SDL_MapRGB(body->format, 255, 255, 255));
	return 1;
}


void SDL_FreeRect(SDL_Rect **rect)
{
	if(*rect != NULL)
	{
		free(*rect);
		*rect = NULL;
	}
}

int Window_CreateWindow(int width, int height, const char *title)
{
	SDL_Surface *screen = NULL;
	if(NULL == Window)
		return 0;
	if(Window->screen != NULL)
		return -1;
	if(Window->state != STATE_TTFINIT)
		return 0;
	if(width<=0 || width>Window->max_width)
		return 0;
	if(height<=0 || height>Window->max_height)
		return 0;
	
	screen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	memtest(screen);
	
	Window->screen = screen;
	Window->width = width;
	Window->height = height;
	
	if(title != NULL)
		SDL_WM_SetCaption(title, "");
	
	Window->pos_menu = SDL_SetRect(0, 0, Window->width, Window->height / 10);
	
	Window->pos_pal = SDL_SetRect(0, Window->height / 10, Window->width / 10, 
						Window->height * 9/10);
						
	Window->pos_body = SDL_SetRect(Window->width / 10, Window->height/10, 
					Window->width * 9/10, Window->height * 9/10);
					
	Window->menu = SDL_CreateRGBSurface(SDL_HWSURFACE, Window->pos_menu->w, Window->pos_menu->h,
						32, 0, 0, 0, 0);
	memtest(Window->menu);
	Window_InitMenu(Window->menu);

	Window->pal = SDL_CreateRGBSurface(SDL_HWSURFACE, Window->pos_pal->w, Window->pos_pal->h,
						32, 0, 0, 0, 0);
	memtest(Window->pal);
	Window_InitPal(Window->pal);

	Window->body = SDL_CreateRGBSurface(SDL_HWSURFACE, Window->pos_body->w * 10, Window->pos_body->h,
						32, 0, 0, 0, 0);
	if(Window->body == NULL)
		printf("%s : ", SDL_GetError());
	memtest(Window->body);
	Window_InitBody(Window->body);
	Window->body_use = SDL_DisplayFormat(Window->body);
	SDL_SetColorKey(Window->body, SDL_SRCALPHA, SDL_MapRGB(Window->body->format, 255, 255, 255));
	
	
	Window->state = STATE_WINDOWED;
	
	SDL_FillRect(Window->screen, NULL, SDL_MapRGB(Window->screen->format, 255, 255, 255));
	SDL_Flip(Window->screen);
	return 1;
}

int Window_OK(void)
{
	if(Window != NULL && STATE_WINDOWED == Window->state)
		return 1;
	printf("Error : Window may not be init with Window_Init() && Window_CreateWindow()\n");
	return 0;
}

int Window_ClearWindow(Color color)
{
	TestOK();
	
	SDL_FillRect(Window->screen, NULL, SDL_MapRGB(Window->screen->format, 
						color.r, color.g, color.b));
	return 1;
}

int Window_Print(void)
{	
	SDL_Rect pos;
	TestOK();
	
	pos.x = Window->pos_menu->x;
	pos.y = Window->pos_menu->y;
	
	SDL_BlitSurface(Window->menu, NULL, Window->screen, &pos);
	
	pos.x = Window->pos_pal->x;
	pos.y = Window->pos_pal->y;
	
	SDL_BlitSurface(Window->pal, NULL, Window->screen, &pos);
	
	pos.x = Window->pos_body->x;
	pos.y = Window->pos_body->y;
	
	SDL_BlitSurface(Window->body_use, NULL, Window->screen, &pos);

	return 1;
}

int Window_WaitMouse(int *x, int *y)
{
	SDL_Event event;
	int c = 1;
	
	TestOK()
	
	while(c)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				c = 0;
				Window_Quit();
				exit(EXIT_SUCCESS);
				break;
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
				if(x != NULL)
					*x = event.button.x;
				if(y != NULL)
					*y = event.button.y;
				c = 0;
				break;
		}
		SDL_Delay(5);
	}
	return 1;
}

int Window_DrawBodyShrink(double ratio, SDL_Rect redim, SDL_Rect pos)
{
	if(ratio == Window->ratio)
	{
		SDL_BlitSurface(Window->body_use, &redim, Window->screen, &pos);
		return 1;
	}
	if(Window->body_use != NULL)
		SDL_FreeSurface(Window->body_use);
	Window->body_use = shrinkSurface(Window->body, (int)ratio, (int)ratio); 
	memtest(Window->body_use);

	SDL_BlitSurface(Window->body_use, &redim, Window->screen, &pos);
	Window->ratio = ratio;
	
	return 1;
}


void Window_Staff(int x, int y, int w)
{
	int i,j;
	for(j = 0; j < 5; j++)
	{
		for(i = 0; i < STAFF_H; i++)
		{
			hlineRGBA(Window->body, x, w, y+i+j*(HEAD_H), 0, 0, 0, 255);
		}
	}
}
void Window_DrawStaff(int x, int y, int x_end, SDL_Surface *dest)
{
	int i,j;
	boxRGBA(dest, x, y, x+3, y+HEAD_H*4, 0, 0, 0, 255);
	for(j = 0; j < 5; j++)
	{
		for(i = 0; i < STAFF_H; i++)
		{
			hlineRGBA(Window->body, x, x_end, y+i+j*(HEAD_H), 0, 0, 0, 255);
		}
	}
	boxRGBA(dest, x_end, y, x_end+3, y+HEAD_H*4, 0, 0, 0, 255);
}


void Window_ShowAllGraphics(void)
{
	int b,c;
	SDL_Surface *temp = NULL;
	SDL_Rect pos;
	if(NULL == Images)
		return;
	
	SDL_FillRect(Window->body, NULL, SDL_MapRGB(Window->body->format, 255, 255, 255));
	
	pos.x = b = 100;
	pos.y = c = 300;
	
	pos.y += HEAD_H * 2 + HEAD_H/2;
	
	
	if(Images->Note_headWhole != NULL)
	{
		SDL_BlitSurface(Images->Note_headWhole, NULL, Window->body, &pos);
		pos.x+= NOTE_SPACE;
	}
	if(Images->Note_Black != NULL)
	{	
		Images_DrawRotNote(Images->Note_headBlack, pos.x, pos.y, Window->body);
		pos.y -= Images->note1_center->y;
		pos.x -= 2;
		SDL_BlitSurface(Images->Note_Black, NULL, Window->body, &pos);
		pos.y += Images->note1_center->y;
		pos.x += 2;
		pos.x += NOTE_SPACE;
	}
	if(Images->Note_headWhite != NULL)
	{
		Images_DrawRotNote(Images->Note_headWhite, pos.x, pos.y, Window->body);
		pos.y -= Images->note1_center->y;
		pos.x -= 2;
		SDL_BlitSurface(Images->Note_Black, NULL, Window->body, &pos);
		pos.y += Images->note1_center->y;
		pos.x += 2;
		pos.x += NOTE_SPACE;
	}
	if(Images->Note_Black != NULL)
	{	
		Images_DrawRotNote(Images->Note_headBlack, pos.x, pos.y, Window->body);
		pos.y -= Images->note1_center->y;
		pos.x -= 2;
		SDL_BlitSurface(Images->Note_Black, NULL, Window->body, &pos);
		pos.x+=Images->Note_Black->w;
		SDL_BlitSurface(Images->Note_Crotchet, NULL, Window->body, &pos);
		pos.x-=Images->Note_Black->w;
		pos.y += Images->note1_center->y;
		pos.x += 2;
		pos.x += NOTE_SPACE;
	}
	if(Images->Note_Black != NULL)
	{	
		Images_DrawRotNote(Images->Note_headBlack, pos.x, pos.y, Window->body);
		pos.y -= Images->note1_center->y;
		pos.x -= 2;
		SDL_BlitSurface(Images->Note_Black, NULL, Window->body, &pos);
		pos.x+=Images->Note_Black->w;
		SDL_BlitSurface(Images->Note_Crotchet, NULL, Window->body, &pos);
		pos.y+=20;
		SDL_BlitSurface(Images->Note_Crotchet, NULL, Window->body, &pos);
		pos.y-=20;
		pos.x-=Images->Note_Black->w;
		pos.y += Images->note1_center->y;
		pos.x += 2;
		pos.x += NOTE_SPACE;
	}
	if(Images->Note_Black != NULL)
	{	
		Images_DrawRotNote(Images->Note_headBlack, pos.x, pos.y, Window->body);
		pos.y -= Images->note1_center->y;
		pos.x -= 2;
		SDL_BlitSurface(Images->Note_Black, NULL, Window->body, &pos);
		pos.x+=Images->Note_Black->w;
		SDL_BlitSurface(Images->Note_Crotchet, NULL, Window->body, &pos);
		pos.y+=20;
		SDL_BlitSurface(Images->Note_Crotchet, NULL, Window->body, &pos);
		pos.y+=20;
		SDL_BlitSurface(Images->Note_Crotchet, NULL, Window->body, &pos);
		pos.y-=20;
		pos.y-=20;
		pos.x-=Images->Note_Black->w;
		pos.y += Images->note1_center->y;
		pos.x += 2;
		pos.x += NOTE_SPACE;
	}
	if(Images->Note_Black != NULL)
	{	
		Images_DrawRotNote(Images->Note_headBlack, pos.x, pos.y, Window->body);
		pos.y -= Images->note1_center->y;
		pos.x -= 2;
		SDL_BlitSurface(Images->Note_Black, NULL, Window->body, &pos);
		pos.x+=Images->Note_Black->w;
		SDL_BlitSurface(Images->Note_Crotchet, NULL, Window->body, &pos);
		pos.y+=20;
		SDL_BlitSurface(Images->Note_Crotchet, NULL, Window->body, &pos);
		pos.y+=20;
		SDL_BlitSurface(Images->Note_Crotchet, NULL, Window->body, &pos);
		pos.y+=20;
		SDL_BlitSurface(Images->Note_Crotchet, NULL, Window->body, &pos);
		pos.y-=20;
		pos.y-=20;
		pos.y-=20;
		pos.x-=Images->Note_Black->w;
		pos.y += Images->note1_center->y;
		pos.x += 2;
		pos.x += NOTE_SPACE;
	}
	if(Images->Rest_Long != NULL)
	{
		pos.y -= 2*HEAD_H;
	
		pos.x -= Images->pos_Long->x;
		pos.y -= Images->pos_Long->y;
		SDL_BlitSurface(Images->Rest_Long, NULL, Window->body, &pos);
		pos.x += Images->pos_Long->x;
		pos.y += Images->pos_Long->y;
		pos.x += NOTE_SPACE;
		
		pos.y += 2*HEAD_H;
	}
	if(Images->Rest_BreveLong != NULL)
	{	
		pos.y -= 2*HEAD_H;
		
		pos.x -= Images->pos_BreveLong->x;
		pos.y -= Images->pos_BreveLong->y;
		SDL_BlitSurface(Images->Rest_BreveLong, NULL, Window->body, &pos);
		pos.x += Images->pos_BreveLong->x;
		pos.y += Images->pos_BreveLong->y;
		pos.x += NOTE_SPACE;
		
		pos.y += 2*HEAD_H;
	}
	Window_Staff(b, c, Window->width);
	
	temp = Window->body;
	Window->body = SDL_DisplayFormat(Window->body);
	SDL_FreeSurface(temp);
}

int Note_Print(Note *note, SDL_Rect *base_pos, SDL_Surface *dest)
{
	Note_Duration cpy = note->duration;
	int pos;
	int note_y = 0;
	int tab[] = {0, 2, 4, 5, 7, 9, 11};
	int i;
	int best_space = NOTE_SPACE;
	if(note->duration >= DOUBLECROCHE)
		best_space += (note->duration);
		
	if((NULL == note) || (NULL == base_pos) || (base_pos->x < 0) || (base_pos->y < 0) || (NULL == dest))
		return 0;
	if(!note->rest || (note->rest && note->duration != RONDE && note->duration != BLANCHE))
		base_pos->y += HEAD_H * 4;
	
	if(!note->rest)
	{
		for(i = 0; i < 7; i++)
		{
			if((note->note % 12) == tab[i])
			{
				note_y = -i * (HEAD_H / 2) + HEAD_H * 1.5 - (HEAD_H * 3.5) * (note->note / 12 - 5);
				printf("note %d = %d\n", note->note, note_y);
				break;
			}
		}
		base_pos->y += note_y;
	}
	
	if(!note->rest && note_y >= 45)
	{
		for(i = 45; i <= note_y; i+= HEAD_H)
		{
			boxRGBA(dest, 
				base_pos->x - HEAD_W/2 + Images->rot_noteW + best_space / 2, 
				base_pos->y + 4 + i - note_y + Images->rot_noteH, 
				base_pos->x + HEAD_W + Images->rot_noteW  + best_space / 2, 
				base_pos->y + 6 + i - note_y  + Images->rot_noteH, 
				0, 0, 0, 255);
		}
	}
	if(!note->rest && note_y <= -135)
	{
		for(i = -135; i >= note_y; i-= HEAD_H)
		{
			boxRGBA(dest, 
				base_pos->x - HEAD_W/2 + Images->rot_noteW + best_space / 2, 
				base_pos->y + 4 + i - note_y + Images->rot_noteH, 
				base_pos->x + HEAD_W + Images->rot_noteW  + best_space / 2, 
				base_pos->y + 6 + i - note_y  + Images->rot_noteH, 
				0, 0, 0, 255);
		}
	}
	switch(note->duration)
	{
		case RONDE:
			if(note->rest)
			{
				base_pos->x += (Note_RealDuration(note) * best_space ) / 2;
				base_pos->y += 2*HEAD_H;
				SDL_BlitSurface(Images->Rest_Long, NULL, dest, base_pos);
				base_pos->y -= 2*HEAD_H;
				base_pos->x += (Note_RealDuration(note) * best_space ) / 2;
			}
			else
			{
				SDL_BlitSurface(Images->Note_headWhole, NULL, dest, base_pos);
				base_pos->x += (Note_RealDuration(note) * best_space );
			}
			
			break;
			
		case BLANCHE:
			if(note->rest)
			{
				base_pos->y += 2*HEAD_H;
				base_pos->y -= Images->pos_BreveLong->y;
				base_pos->x += 2*NOTE_SPACE;
				SDL_BlitSurface(Images->Rest_BreveLong, NULL, dest, base_pos);
				base_pos->x -= 2*NOTE_SPACE;
				base_pos->y -= 2*HEAD_H;
				base_pos->y += Images->pos_BreveLong->y;
			}
			else
			{
				SDL_Surface *test = NULL;
				Images_DrawRotNote(Images->Note_headWhite, base_pos->x, base_pos->y, dest);
				base_pos->y -= Images->note1_center->y;
				base_pos->x -= 2;
				if(!note->rest && note_y <= -45)
				{
					test = rotozoomSurface(Images->Note_Black, 180.0, 1.0, 0);
					base_pos->y += QUEUE + 7;
					base_pos->x += 3;
				
					SDL_BlitSurface(test, NULL, dest, base_pos);
					base_pos->x -= 3;
					base_pos->y -= QUEUE + 7;
					SDL_FreeSurface(test);
				}
				else
					SDL_BlitSurface(Images->Note_Black, NULL, dest, base_pos);
				base_pos->y += Images->note1_center->y;
				base_pos->x += 2;
			}
			base_pos->x += (Note_RealDuration(note) * best_space );
			break;
			
		case QUADRUPLECROCHE:
		case TRIPLECROCHE:
		case DOUBLECROCHE:
		case CROCHE:
			{
				SDL_Surface *test_2 = NULL;
				
				base_pos->y -= Images->note1_center->y;
				base_pos->x -= 2;
				base_pos->x+=Images->Note_Black->w;
				pos = 0;
				if(!note->rest && note_y<= -45)
					test_2 = rotozoomSurface(Images->Note_Crotchet, 180.0, -1.0, 1);
				while(cpy > NOIRE)
				{
					if(!note->rest && note_y <= -45)
					{
						base_pos->x -= 2*Images->Note_headBlack->w - 5;
						base_pos->y += QUEUE + HEAD_H;
						SDL_BlitSurface(test_2, NULL, dest, base_pos);
						base_pos->y -= QUEUE + HEAD_H;
						base_pos->x += 2*Images->Note_headBlack->w - 5;
					}
					else
						SDL_BlitSurface(Images->Note_Crotchet, NULL, dest, base_pos);

					base_pos->y += 20;
					pos += 20;
					cpy /= 2;
				}
				base_pos->y -= pos;
				base_pos->x-=Images->Note_Black->w;
				base_pos->y += Images->note1_center->y;
				base_pos->x += 2;
			}
		case NOIRE:
			{
				SDL_Surface *test = NULL;
				Images_DrawRotNote(Images->Note_headBlack, base_pos->x, base_pos->y, dest);
				base_pos->y -= Images->note1_center->y;
				base_pos->x -= 2;
				if(!note->rest && note_y <= -45)
				{
					test = rotozoomSurface(Images->Note_Black, 180.0, 1.0, 0);
					base_pos->y += QUEUE + 7;
					base_pos->x += 3;
				
					SDL_BlitSurface(test, NULL, dest, base_pos);
					base_pos->x -= 3;
					base_pos->y -= QUEUE + 7;
					SDL_FreeSurface(test);
				}
				else
					SDL_BlitSurface(Images->Note_Black, NULL, dest, base_pos);
				base_pos->y += Images->note1_center->y;
				base_pos->x += 2;
			}
			base_pos->x += (Note_RealDuration(note) * best_space );
			break;
		default:
			break;
	}
	if(!note->rest || (note->rest && note->duration != RONDE && note->duration != BLANCHE))
		base_pos->y -= HEAD_H * 4;
	
	base_pos->y -= note_y;
	return 1;
}

int Step_Print(Step *step, SDL_Rect *base_pos, SDL_Surface *dest)
{
	ToNote *cur = NULL;
	int sauv_x, sauv_y;
	if((NULL == step) || (NULL == base_pos) || (base_pos->x < 0) || (base_pos->y < 0) || (NULL == dest))
		return 0;
	sauv_x = base_pos->x;
	sauv_y = base_pos->y;
	
	base_pos->y -= HEAD_H ;
	base_pos->x += 4 * NOTE_SPACE;
	cur = step->notes;
	while(cur != NULL)
	{
		Note_Print(cur->note, base_pos, dest);
		cur = cur->next;
	}
	
	Window_DrawStaff(sauv_x, sauv_y, base_pos->x, dest);
	
	base_pos->y += HEAD_H;
	return 1;
}

int Staff_Print(Staff *staff, SDL_Rect *base_pos, SDL_Surface *dest)
{
	int i;
	if((NULL == staff) || (NULL == base_pos) || (NULL == dest))
		return 0;
	
	SDL_FillRect(dest, NULL, SDL_MapRGB(dest->format, 255, 255, 255));
	
	for(i = 0; i < staff->n; i++)
		Step_Print(*(staff->steps + i), base_pos, dest);
	
	return 1;
}

int ClicInRect(int x, int y, SDL_Rect *rect)
{
	if(NULL == rect)
		return 0;
	if((x >= rect->x) && (x<=rect->x + rect->w) &&
		(y >= rect->y) && (y <= rect->y + rect->h))
		return 1;
	return 0;
}

/* TO DO LIST

- Polices / Images : System du moteur S en liste chainées avec avancé des plus utilisés
	=> Système Générique (A tester) OK
- Events ? Tab Génériques (A venir..)
- Commencer le module Images. OK
**/
