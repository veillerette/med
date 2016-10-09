#include "Window.h"

WindowData *Window = NULL;



Color SetColorA(int r, int g, int b, int a)
{
	Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return color;
}

Color SetColor(int r, int g, int b)
{
	return SetColorA(r, b, b, 255);
}

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
	
	SDL_FillRect(body, NULL, SDL_MapRGB(body->format, 200, 200, 255));
	return 1;
}

SDL_Rect *SDL_SetRect(int x, int y, int w, int h)
{
	SDL_Rect *temp = (SDL_Rect *)malloc(sizeof(SDL_Rect));
	memtest(temp);
	
	temp->x = x;
	temp->y = y;
	temp->w = w;
	temp->h = h;
	
	return temp;
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

	Window->body = SDL_CreateRGBSurface(SDL_HWSURFACE, Window->pos_body->w, Window->pos_body->h,
						32, 0, 0, 0, 0);
	memtest(Window->body);
	Window_InitBody(Window->body);
	
	
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
	
	SDL_BlitSurface(Window->body, NULL, Window->screen, &pos);
	
	
	pos.x = Window->pos_body->x + 200;
	pos.y = Window->pos_body->y + 200;
	SDL_BlitSurface(Notes_SoloBasicNoire(), NULL, Window->screen, &pos);
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


/* TO DO LIST

- Polices / Images : System de Survie/Moteur en liste chainées avec avancé des plus utilisés
- Events ?
- Commencer le module Images.
**/
