#include "../include/Window.h"


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
	temp->ratio = 0.0;
	
	temp->menu = NULL;
	temp->pos_menu = NULL;
	temp->pal = NULL;
	temp->pos_pal = NULL;
	temp->body = NULL;
	temp->body_use = NULL;
	temp->pos_body = NULL;
	temp->nb_body = 0;
	temp->pos_link = NULL;
	
	return temp;
}

void WindowData_Free(WindowData **window)
{
	if(*window != NULL)
	{
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
		int i;
		Window->state = STATE_QUIT;
		
		if(Window->screen != NULL)
			SDL_FreeSurface(Window->screen);
		
		if(Window->menu != NULL)
			SDL_FreeSurface(Window->menu);
		
		if(Window->pal != NULL)
			SDL_FreeSurface(Window->pal);
		
		if(Window->body != NULL)
			for(i = 0; i < Window->nb_body; i++)
				SDL_FreeSurface(Window->body[i]);
				
		if(Window->body != NULL)
			free(Window->body);
		
		if(Window->body_use != NULL)
			for(i = 0; i < Window->nb_body; i++)
				SDL_FreeSurface(Window->body_use[i]);
				
		if(Window->body_use != NULL)
			free(Window->body_use);
		
		
		if(Window->pos_menu != NULL)
			SDL_FreeRect(&(Window->pos_menu));
		
		if(Window->pos_pal != NULL)
			SDL_FreeRect(&(Window->pos_pal));
			
		if(Window->pos_body != NULL)
			SDL_FreeRect(&(Window->pos_body));
		
			
		WindowData_Free(&Window);
		TTF_Quit();
		SDL_Quit();
	}
}


static int Window_InitMenu(SDL_Surface *menu)
{
	if(NULL == menu)
		return 0;
	
	SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 255, 255, 255));
	
	SDL_FillRect(menu, NULL, SDL_MapRGB(menu->format, 100, 110, 120));
	return 1;
}

static int Window_InitPal(SDL_Surface *pal)
{
	if(NULL == pal)
		return 0;
	
	SDL_FillRect(pal, NULL, SDL_MapRGB(pal->format, 255, 255, 255));
	
	SDL_FillRect(pal, NULL, SDL_MapRGB(pal->format, 65, 76, 87));
	return 1;
}

int Window_InitBody()
{
	int i;
	if(NULL == Window)
		return 0;
	for(i = 0; i < Window->nb_body; i++)
		SDL_FillRect(Window->body[i], NULL, SDL_MapRGB(Window->body[i]->format, 255, 255, 255));
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
	SDL_Surface *temp = NULL;
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
	
	{
		Window->pos_menu = SDL_SetRect(0, 0, Window->width, 80);
	
		Window->pos_pal = SDL_SetRect(0, 80, Window->width / 10, 
							Window->height * 9/10);
	
			Window->menu = SDL_CreateRGBSurface(SDL_HWSURFACE, Window->pos_menu->w, Window->pos_menu->h,
							32, 0, 0, 0, 0);
		memtest(Window->menu);
		Window_InitMenu(Window->menu);

		Window->pal = SDL_CreateRGBSurface(SDL_HWSURFACE, Window->pos_pal->w, Window->pos_pal->h,
							32, 0, 0, 0, 0);
		memtest(Window->pal);
		Window_InitPal(Window->pal);
	}
	
	{			
		Window->pos_body = SDL_SetRect((Window->width-Window->height*3/2-Window->pos_pal->w)/2+Window->pos_pal->w, Window->pos_menu->h, 
						Window->height * 3, Window->width * 2);
		printf("size body : w=%d h=%d\n", Window->height * 3, Window->width * 2);

		Window->body = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * 1);
		memtest(Window->body);
	
		Window->body_use = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * 1);
		memtest(Window->body_use);
	
		temp = SDL_CreateRGBSurface(SDL_HWSURFACE, 
				Window->pos_body->w, Window->pos_body->h,
							32, 0, 0, 0, 0);
		memtest(temp);
		Window->body[0] = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		
		Window->nb_body = 1;
		Window_InitBody();
		
		Window->body_use[0] = SDL_DisplayFormat(Window->body[0]);
		Window->ratio = 1.0;
		
		SDL_SetColorKey(Window->body[0], SDL_SRCALPHA, 
				SDL_MapRGB(Window->body[0]->format, 255, 255, 255));
					
		
	}
	
	Window->state = STATE_WINDOWED;
	
	SDL_FillRect(Window->screen, NULL, SDL_MapRGB(Window->screen->format, 255, 255, 255));
	SDL_Flip(Window->screen);
	return 1;
}

int Window_AddEmptyBody(void)
{
	SDL_Surface *temp = NULL;
	if((NULL == Window) || (NULL == Window->body))
		return 0;
	
	Window->body = (SDL_Surface **)realloc(Window->body, sizeof(SDL_Surface *) * (Window->nb_body + 1));
	memtest(Window->body);
	
	Window->body_use = (SDL_Surface **)realloc(Window->body_use, sizeof(SDL_Surface *) * (Window->nb_body + 1));
	memtest(Window->body_use);
	
	
	temp = SDL_CreateRGBSurface(SDL_HWSURFACE, 
				Window->pos_body->w, Window->pos_body->h,
							32, 0, 0, 0, 0);
	SDL_FillRect(temp, NULL, SDL_MapRGB(temp->format, 255, 255, 255));
	memtest(temp);
	Window->body[Window->nb_body] = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	
	Window->body_use[Window->nb_body] = SDL_DisplayFormat(Window->body[Window->nb_body]);
	memtest(Window->body_use[Window->nb_body]);
	
	Window->nb_body++;
	return 1;
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
	return 1;
}

int Window_ApplyZoomOnRect(SDL_Rect *rect, double zoom, double old)
{
	double fac_zoom = (1.0 / (zoom-old));
	if(NULL == rect)
		return 0;
	rect->x *= fac_zoom;
	rect->y *= fac_zoom;
	rect->w *= fac_zoom;
	rect->h *= fac_zoom;
	return 1;
}

int Window_ApplyZoom(double zoom)
{
	int i, fact;
	TestOK();
	
	for(i = 0; i < Window->nb_body; i++)
	{
		if(Window->body_use[i] != NULL)
			SDL_FreeSurface(Window->body_use[i]);
		
		if(zoom == 1)
			Window->body_use[i] = SDL_DisplayFormat(Window->body[i]);
		else 
			Window->body_use[i] = shrinkSurface(Window->body[i], (int)zoom, (int)zoom);
		memtest(Window->body_use[i]);
		
	}
	if(Window->ratio == zoom)
		fact = 1;
	else if(Window->ratio > zoom) /* + */
		fact = Window->ratio-zoom;
	else /* - */
		fact = zoom-Window->ratio;
	Window->pos_body->x = BASE_BODY_X + (Window->pos_body->x-BASE_BODY_X) * (1.0 / fact);
	Window->pos_body->y = BASE_BODY_Y + (Window->pos_body->y-BASE_BODY_Y) * (1.0 / fact);
	Window->ratio = zoom;
	return 1;
}

int Window_DrawBody()
{
	int i;
	SDL_Rect pos = {Window->pos_body->x, Window->pos_body->y, 0, 0};
	SDL_Rect sauv = {pos.x, pos.y, 0, 0};
	SDL_Rect red = {BASE_BODY_X, BASE_BODY_Y, Window->width, Window->height};
	SDL_FillRect(Window->screen, &red, SDL_MapRGB(Window->screen->format, 220, 220, 220));
	for(i = 0; i < Window->nb_body; i++)
	{
		pos.x = sauv.x;
		pos.y = sauv.y;
		SDL_BlitSurface(Window->body_use[i], NULL, Window->screen, &pos);
		sauv.x += Window->body_use[i]->w + (int)(ESP_BODY * (1.0 / Window->ratio));
		
	}
	return 1;
}

void Window_Staff(SDL_Surface *dest, int x, int y, int w)
{
	int i,j;
	for(j = 0; j < 5; j++)
	{
		for(i = 0; i < STAFF_H; i++)
		{
			hlineRGBA(dest, x, w, y+i+j*(HEAD_H), 0, 0, 0, 255);
		}
	}
}

int Window_OK(void)
{
	if(Window != NULL && STATE_WINDOWED == Window->state)
		return 1;
	printf("Error : Window may not be init with Window_Init() && Window_CreateWindow()\n");
	return 0;
}


int Window_MyEventBlit(Object_Type type, int nbody, SDL_Surface *surf, SDL_Rect *rect1, 
					SDL_Surface *dest, SDL_Rect *rect2, ...)
{
	SDL_Rect goal = {rect2->x, rect2->y, surf->w, surf->h};
	Step *step = NULL;
	int id = 0;
	va_list va;
	va_start(va, rect2);
	switch(type)
	{
		case OBJECT_NOTE:
			step = va_arg(va, Step *);
			id = va_arg(va, int);
			EventData_Add(main_events, Area_Set(goal, nbody, type, step, id));
			break;
		case OBJECT_LINE:
		case OBJECT_CLE:
		case OBJECT_STEP:
			EventData_Add(main_events, Area_Set(goal, nbody, type, va_arg(va, Staff *), va_arg(va, int)));
			break;
		case OBJECT_SIGN:
			EventData_Add(main_events, Area_Set(goal, nbody, type, va_arg(va, Sign **)));
			break;
		default:
			return 0;
	}
	
	return SDL_BlitSurface(surf, rect1, dest, rect2);
}

int Window_TestBox(SDL_Surface *dest, SDL_Rect *pos, int zoom)
{
	Area *area;
	int r=200,g=50,b=50,a=150;
	if(NULL == dest)
		return 0;

	area= main_events->lst;
	while(area != NULL)
	{
		if(((pos->x + (area->rect.x + SIZE_BODY*area->nbody)/zoom) >= main_events->base->x)  &&
			((pos->y  + area->rect.y/zoom) >= main_events->base->y))
		{
			a = 150;


			if(main_events->select == area)
			{
				r = 200;
				g = 50;
				b = 50;
			}
			else
			{
				r = 200;
				g = 200;
				b = 200;
			}
			if(area->type == EVENT_ADDNOTE)
			{
				r = 50;
				g = 50;
				b = 255;
				a = 30;
			}
			
			
			if(main_events->hover == area)
			{
				r *= 0.8;
				g *= 0.8;
				b *= 0.8;
			}
			
			if((pos->x + ((area->rect.x + SIZE_BODY*area->nbody) /zoom)) > BASE_BODY_X &&
				(pos->y  + (area->rect.y /zoom)) > BASE_BODY_Y)
			{
				if(((area->type == EVENT_ADDNOTE && main_events->mode == MODE_ADD &&
						 (area == main_events->hover || area == main_events->select) )
					||
					(area->type != EVENT_ADDNOTE && main_events->mode == MODE_EDIT)))
				{
					boxRGBA(dest, 		(pos->x + ((area->rect.x + SIZE_BODY*area->nbody) /zoom)), 
								(pos->y  + (area->rect.y /zoom)), 
								(pos->x + (area->rect.x + SIZE_BODY*area->nbody)/zoom + area->rect.w/zoom),
								(pos->y + area->rect.y/zoom + area->rect.h/zoom), 
								r, g, b, a);
				}
			}
		}
		area = area->next;
	}
	return 1;
}

int Window_GetSpaceNote(Step *step, Note *note)
{
	Note_Duration min = Step_GetMinDuration(step);
	return (log(min/note->duration + 1)*1.0/log(2) + 1) * NOTE_SPACE;
}

int Window_GetSize(Step *step)
{
	int width = 3 + NOTE_SPACE;
	ToNote *cur = NULL;
	cur = step->notes;
	while(cur != NULL)
	{
		width += Window_GetSpaceNote(step, cur->note);
		cur = cur->next;
	}
	return width;
}

int Note_Print(Staff *staff, Step *step, int id_step, int id_note, Note *note, SDL_Rect *base_pos, SDL_Surface *dest, int nbody)
{
	Note_Duration cpy = note->duration;
	int pos;
	int note_y = 0;
	int tab[] = {0, 2, 4, 5, 7, 9, 11};
	int i;
	int real_space = Window_GetSpaceNote(step, note);
	SDL_Rect adding = {base_pos->x-real_space/4, base_pos->y-HEAD_H*3, 0, HEAD_H*12};
	
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
				break;
			}
		}
		
		base_pos->y += note_y;
	}
	adding.w = real_space;
	
	if(note->flags & NOTE_SHARP)
	{
		base_pos->y -= HEAD_H - 5;
		SDL_BlitSurface(Images->Sharp, NULL, dest, base_pos);
		base_pos->y += HEAD_H - 5;
		
		base_pos->x += HEAD_W;
	}
	
	else if(note->flags & NOTE_FLAT)
	{
		base_pos->y -= 1.5*HEAD_H - 10;
		SDL_BlitSurface(Images->Flat, NULL, dest, base_pos);
		base_pos->y += 1.5*HEAD_H - 10;
		
		base_pos->x += HEAD_W*3.0/4;
	}
	else if(note->flags & NOTE_DOUBLEFLAT)
	{
		base_pos->y -= 1.5*HEAD_H - 10;
		SDL_BlitSurface(Images->DoubleFlat, NULL, dest, base_pos);
		base_pos->y += 1.5*HEAD_H - 10;
		
		base_pos->x += (HEAD_W*3.0/4)*2;
	}
	
	if(!note->rest && note_y >= 45)
	{
		for(i = 45; i <= note_y; i+= HEAD_H)
		{
			boxRGBA(dest, 
				base_pos->x - HEAD_W/2 + Images->rot_noteW + 5, 
				base_pos->y + 4 + i - note_y + Images->rot_noteH, 
				base_pos->x + HEAD_W + Images->rot_noteW + 5, 
				base_pos->y + 6 + i - note_y  + Images->rot_noteH, 
				0, 0, 0, 255);
		}
	}
	
	if(!note->rest && note_y <= -135)
	{
		for(i = -135; i >= note_y; i-= HEAD_H)
		{
			boxRGBA(dest, 
				base_pos->x - HEAD_W/2 + Images->rot_noteW + 5, 
				base_pos->y + 4 + i - note_y + Images->rot_noteH, 
				base_pos->x + HEAD_W + Images->rot_noteW + 5, 
				base_pos->y + 6 + i - note_y  + Images->rot_noteH, 
				0, 0, 0, 255);
		}
	}
	
	if(!note->rest && (note->flags & NOTE_POINTED)) /* Note pointed */
	{
		filledCircleRGBA(dest, base_pos->x + HEAD_W + 3*QUEUE_BORDER, base_pos->y + HEAD_H/5, 5, 0, 0, 0, 255);
	}
	else if(!note->rest && (note->flags & NOTE_DOUBLEPOINTED)) /* Note doublepointed */
	{
		filledCircleRGBA(dest, base_pos->x + HEAD_W + 3*QUEUE_BORDER, base_pos->y + HEAD_H/5, 5, 0, 0, 0, 255);
		filledCircleRGBA(dest, base_pos->x + HEAD_W + 7*QUEUE_BORDER, base_pos->y + HEAD_H/5, 5, 0, 0, 0, 255);
	}
	
	if(Window->_linked)
	{
		int x[] = {Window->pos_link->x+HEAD_W/2, (Window->pos_link->x+2*HEAD_W/2 + base_pos->x)/2, base_pos->x+HEAD_W/2};
		int y[] = {base_pos->y-20, base_pos->y-75,base_pos->y-20};
		int i;
		for(i = 0; i < 5; i++)
		{
			y[0] = y[2]+=i;
			PowerOfBezier(dest, x, y, 3, SetColor(0, 0, 0));
			y[0] = y[2]-=i;
		}
		Window->_linked = 0;
		free(Window->pos_link);
		Window->pos_link = NULL;
	}
	
	if(note->flags & NOTE_LINKED)
	{
		Window->_linked = 1;
		if(Window->pos_link != NULL)
		{
			free(Window->pos_link);
		}
		Window->pos_link = SDL_SetRect(base_pos->x, base_pos->y, 0, 0);
	}
	adding.w += base_pos->x - adding.x - real_space/4;

	if(note->rest)
	{
		switch(note->duration)
		{
			case RONDE:
				adding.w += real_space/2;
				break;
			case BLANCHE:
				adding.x += real_space/4;
			default:
				break;
		}
	}
	EventData_Add(main_events, Area_Set(adding, nbody, EVENT_ADDNOTE, staff, id_step, id_note));
	switch(note->duration)
	{
		case RONDE:
			if(note->rest)
			{
				base_pos->x += real_space/2;
				base_pos->y += 2*HEAD_H;
				base_pos->x-=Images->Rest_Long->w/2;
				Window_MyEventBlit(OBJECT_NOTE, nbody, Images->Rest_Long, NULL, dest, base_pos, step, id_note);
				base_pos->x+=Images->Rest_Long->w/2;
				base_pos->y -= 2*HEAD_H;
				base_pos->x += real_space/2;
				if(!note->rest || (note->rest && note->duration != RONDE && note->duration != BLANCHE))
					base_pos->y -= HEAD_H * 4;
				base_pos->y -= note_y;
				return 1;
			}
			else
			{
				Window_MyEventBlit(OBJECT_NOTE, nbody, Images->Note_headWhole, NULL, dest, base_pos, step, id_note);
			}
			
			break;
			
		case BLANCHE:
			if(note->rest)
			{
				base_pos->x += real_space/2;
				base_pos->y += 2*HEAD_H;
				base_pos->y -= Images->pos_BreveLong->y;
				base_pos->x-=Images->Rest_BreveLong->w/2;
				Window_MyEventBlit(OBJECT_NOTE, nbody, Images->Rest_BreveLong, NULL, dest, base_pos, step, id_note);
				base_pos->x+=Images->Rest_BreveLong->w/2;
				base_pos->y -= 2*HEAD_H;
				base_pos->y += Images->pos_BreveLong->y;
				base_pos->x += real_space/2;
				if(!note->rest || (note->rest && note->duration != RONDE && note->duration != BLANCHE))
					base_pos->y -= HEAD_H * 4;
				base_pos->y -= note_y;
				return 1;
			}
			else
			{
				SDL_Surface *test = NULL;
				base_pos->x -= Images->rot_noteW;
				base_pos->y -= Images->rot_noteH;
				Window_MyEventBlit(OBJECT_NOTE, nbody, Images->Note_headWhite, NULL, dest, base_pos, step, id_note);
				base_pos->x += Images->rot_noteW;
				base_pos->y += Images->rot_noteH;
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
			break;
			
		case QUADRUPLECROCHE:
		case TRIPLECROCHE:
		case DOUBLECROCHE:
		case CROCHE:
			if(note->rest)
			{
				base_pos->y -= 2*HEAD_H;
				
				switch(note->duration)
				{
					case QUADRUPLECROCHE:
						base_pos->y += 2*HEAD_H;
						base_pos->x -= 2*(HEAD_W / 4);
						Window_MyEventBlit(OBJECT_NOTE, nbody, Images->Rest_Quaver, NULL, dest, base_pos, step, id_note);
						base_pos->y -= 2*HEAD_H;
						base_pos->x += 2*(HEAD_W / 4);
					case TRIPLECROCHE:
						base_pos->y -= HEAD_H;
						base_pos->x += HEAD_W / 4;
						Window_MyEventBlit(OBJECT_NOTE, nbody, Images->Rest_Quaver, NULL, dest, base_pos, step, id_note);
						base_pos->y += HEAD_H;
						base_pos->x -= HEAD_W / 4;
					case DOUBLECROCHE:
						base_pos->y += HEAD_H;
						base_pos->x -= HEAD_W / 4;
						Window_MyEventBlit(OBJECT_NOTE, nbody, Images->Rest_Quaver, NULL, dest, base_pos, step, id_note);
						base_pos->y -= HEAD_H;
						base_pos->x += HEAD_W / 4 ;
					case CROCHE:
						Window_MyEventBlit(OBJECT_NOTE, nbody, Images->Rest_Quaver, NULL, dest, base_pos, step, id_note);
						break;
					default:
						break;
				}
				
				base_pos->y += 2*HEAD_H;
				break;
			}
			else {
				base_pos->y -= Images->note1_center->y;
				base_pos->x -= 2;
				base_pos->x+=Images->Note_Black->w;
				pos = 0;
				while(cpy > NOIRE)
				{
					if(!note->rest && note_y <= -45)
					{
						base_pos->x -= Images->Note_Black->w - QUEUE_BORDER;
						base_pos->y += QUEUE + HEAD_H;
						SDL_BlitSurface(Images->Note_CrotchetInv, NULL, dest, base_pos);
						base_pos->y -= QUEUE + HEAD_H;
						base_pos->y -= 15;
						pos -= 15;
						base_pos->x += Images->Note_Black->w - QUEUE_BORDER;
					}
					else
					{
						SDL_BlitSurface(Images->Note_Crotchet, NULL, dest, base_pos);
						base_pos->y += 20;
						pos += 20;
					}
					
					cpy /= 2;
				}
				base_pos->y -= pos;
				base_pos->x-=Images->Note_Black->w;
				base_pos->y += Images->note1_center->y;
				base_pos->x += 2;
			}
		case NOIRE:
			if(note->rest)
			{
				base_pos->y -= 2.5*HEAD_H;
				Window_MyEventBlit(OBJECT_NOTE, nbody, Images->Rest_Breve, NULL, dest, base_pos, step, id_note);
				base_pos->y += 2.5*HEAD_H;
			}
			else
			{
				SDL_Surface *test = NULL;
				
				base_pos->x -= Images->rot_noteW;
				base_pos->y -= Images->rot_noteH;
				Window_MyEventBlit(OBJECT_NOTE, nbody, Images->Note_headBlack, NULL, dest, base_pos, step, id_note);
				base_pos->x += Images->rot_noteW;
				base_pos->y += Images->rot_noteH;
				
				
				
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
			break;
		default:
			break;
	}
	
	base_pos->x += real_space;

	if(!note->rest || (note->rest && note->duration != RONDE && note->duration != BLANCHE))
		base_pos->y -= HEAD_H * 4;
	
	base_pos->y -= note_y;
	return 1;
}

int Armure_Print(Step *step, SDL_Rect *base_pos, SDL_Surface *dest)
{
	double s[] = {-1.5, 0., -2.0, -0.5, 1.0, -1.0, 0.5};
	double f[] = {0.5, -1.0, 1.0, -0.5, 1.5, 0., 2.0};
	int i;
	if((NULL == step) || (NULL == base_pos) || (base_pos->x < 0) || (base_pos->y < 0) || (NULL == dest))
		return 0;
	

	if(step->sign != 0)
		base_pos->x += NOTE_SPACE/2;
	
	
	if(step->sign < 0)
	{
		for(i = 1; i <= -step->sign; i++)
		{
			base_pos->y -= 5;
			base_pos->y += (double)f[i-1] * HEAD_H;
			SDL_BlitSurface(Images->Flat, NULL, dest, base_pos);
			base_pos->y -= (double)f[i-1] * HEAD_H;	
			base_pos->y += 5;
			base_pos->x += Images->Flat->w;
		}
	}
	else if(step->sign > 0)
	{
		for(i = 1; i <= step->sign; i++)
		{
			base_pos->y += 5;
			base_pos->y += (double)s[i-1] * HEAD_H;
			SDL_BlitSurface(Images->Sharp, NULL, dest, base_pos);	
			base_pos->y -= (double)s[i-1] * HEAD_H;	
			base_pos->y -= 5;
			base_pos->x += Images->Sharp->w;
		}
	}

	if(step->sign != 0)
	{
		base_pos->x += Images->Flat->w;
		base_pos->x += NOTE_SPACE/4;
	}
	return 1;
}

int Step_Print(Staff *staff, Step *step, int id_step, SDL_Rect *base_pos, SDL_Surface *dest, int nbody)
{
	ToNote *cur = NULL;
	SDL_Rect depass = {0, 0, 1000, 500};
	int i=0;
	if((NULL == step) || (NULL == base_pos) || (base_pos->x < 0) || (base_pos->y < 0) || (NULL == dest))
		return 0;

	depass.x = base_pos->x+3;
	depass.y = base_pos->y;
	base_pos->y -= HEAD_H ;
	base_pos->x += NOTE_SPACE/2;
	
	cur = step->notes;
	while(cur != NULL)
	{
		Note_Print(staff, step, id_step, i, cur->note, base_pos, dest, nbody);
		cur = cur->next;
		i++;
	}
	
	base_pos->x += NOTE_SPACE/2;
	
	if(base_pos->x >= dest->w)
	{
		depass.y -= HEAD_H*4;
		depass.w = base_pos->w - depass.x;
		SDL_FillRect(dest, &depass, SDL_MapRGB(dest->format, 255, 255, 255));
		base_pos->x = 100;
		base_pos->y += 380;
		base_pos->y += HEAD_H;
		return -2;
	}
	
	base_pos->y += HEAD_H;
	return 1;
}

void Window_DrawStaff(int x, int y, int x_end, SDL_Surface *dest)
{
	int i,j;
	boxRGBA(dest, x, y, x+2, y+HEAD_H*4, 0, 0, 0, 255);
	for(j = 0; j < 5; j++)
	{
		for(i = 0; i < STAFF_H; i++)
		{
			hlineRGBA(dest, x, x_end, y+i+j*(HEAD_H), 0, 0, 0, 255);
		}
	}
	boxRGBA(dest, x_end, y, x_end+2, y+HEAD_H*4, 0, 0, 0, 255);
}

int Step_PrintMesure(Step *step, SDL_Rect *base_pos, SDL_Surface *dest)
{
	SDL_Color color = {0, 0, 0, 0};
	char num[3] = "", den[3] = "";
	int x=base_pos->x, y=base_pos->y;
	if((NULL == step) || (NULL == base_pos) || (base_pos->x < 0) || (base_pos->y < 0) || (NULL == dest))
		return 0;
	y += HEAD_H+2;
	sprintf(num, "%d", step->num);
	sprintf(den, "%d", step->den);
	Moteur_WriteText(x, y, num, HEAD_H*3-18, FONT_MESURE, color, TEXT_BLENDED, TEXT_CENTER, dest);
	Moteur_WriteText(x, y+HEAD_H*2, den, HEAD_H*3-18, FONT_MESURE, color, TEXT_BLENDED, TEXT_CENTER, dest);
	base_pos->x += HEAD_W*2;
	return 1;
}

int Staff_Print(Staff *staff, SDL_Rect *base_pos)
{
	int i = 0, j = 0;
	signed char sauv = 0;
	int sauv_x, sauv_y;
	int num = -1, den = -1;
	int nbody = 0;
	int begin_y = base_pos->y;
	
	Window->_linked = 0;
	SDL_Rect goal = {0, 0, 0, 0};
	if(Window->pos_link != NULL)
		free(Window->pos_link);
	Window->pos_link = NULL;
	if((NULL == staff) || (NULL == base_pos) || (NULL == Window->body[0]))
		return 0;
	
	for(j = 0; j < Window->nb_body; j++)
		SDL_FillRect(Window->body[j], NULL, SDL_MapRGB(Window->body[j]->format, 255, 255, 255));
	
	while(i < staff->n)
	{
		if(Window_GetSize(*(staff->steps + i))+base_pos->x > Window->body[0]->w - 20)
		{
			base_pos->x = 100;
			base_pos->y += 380;
			printf("fin de ligne %d\n", base_pos->y);
			if(base_pos->y > (Window->body[0]->h - HEAD_H*6))
			{
				printf("fin de page n°%d\n", nbody+1);
				nbody++;
				printf("%d/%d\n", nbody, Window->nb_body);
				base_pos->y = begin_y;
				if(nbody == Window->nb_body)
					Window_AddEmptyBody();
			}
		}
		sauv_x = base_pos->x;
		sauv_y = base_pos->y;
		goal.x = sauv_x;
		goal.y = sauv_y;
		
		if(0 == i || (*(staff->steps + i))->sign != sauv)
		{
			Armure_Print(*(staff->steps + i), base_pos, NPAGE);
			sauv = (*(staff->steps + i))->sign;
		}
		if(0 == i || (*(staff->steps + i))->num != num || (*(staff->steps + i))->den != den)
		{
			base_pos->x += HEAD_W;
			Step_PrintMesure(*(staff->steps + i), base_pos, NPAGE);
			num = (*(staff->steps + i))->num;
			den = (*(staff->steps + i))->den;
		}
		if(Step_Print(staff, *(staff->steps + i), i, base_pos, NPAGE, nbody) == -2)
			continue;
		Window_DrawStaff(sauv_x, sauv_y, base_pos->x, NPAGE);
		goal.w = base_pos->x - goal.x;
		goal.h = HEAD_H * 4;
		EventData_Add(main_events, Area_Set(goal, nbody, OBJECT_STEP, staff, i));
		i++;
	}
	return 1;
}

int Window_LittleEvent(SDL_Event event, double *r, int *c, int *mouse,
					int *clic_x, int *clic_y, int *tomaj,
					int *m)
{
	*m = 0;
	switch(event.type)
	{
		case SDL_QUIT:
			*c = 0;
			break;
		case SDL_MOUSEBUTTONUP:
			*clic_x = event.button.x;
			*clic_y = event.button.y;
			*mouse = 0;
			break;
		case SDL_MOUSEBUTTONDOWN:
			*clic_x = event.button.x;
			*clic_y = event.button.y;
			if(*clic_x <= Window->pos_pal->w || *clic_y <= Window->pos_menu->h)
				break;
			switch(event.button.button)
			{
				case SDL_BUTTON_WHEELDOWN:
					(*r)++;
					if(*r > 7)
						*r = 7;
					else
						*m=1;
					break;
				case SDL_BUTTON_WHEELUP:
					(*r)--;
					if((*r)<1.0)
						(*r) = 1.0;
					*m=1;
					break;
			}
			*mouse = 1;
			break;
		case SDL_MOUSEMOTION:
			if(*mouse)
			{
				if(*clic_x < Window->pos_pal->w || *clic_y < Window->pos_menu->h)
					return NONE;
				if(*clic_x > event.motion.x)
				{
					Window->pos_body->x -= (*clic_x - event.motion.x);
					*tomaj = 1;
					*clic_x = event.motion.x;
				}
				else if(*clic_x < event.motion.x)
				{
					Window->pos_body->x +=(event.motion.x - *clic_x);
					*clic_x = event.motion.x;
					*tomaj = 1;
				}
				if(*clic_y > event.motion.y)
				{
					Window->pos_body->y -= (*clic_y - event.motion.y);
					*tomaj = 1;
					*clic_y = event.motion.y;
				}
				else if(*clic_y < event.motion.y)
				{
					Window->pos_body->y+=(event.motion.y - *clic_y);
					*clic_y = event.motion.y;
					*tomaj = 1;
				}
			}
			break;
	}

	
	return 0;
}



