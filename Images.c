#include "Images.h"

Graphics *Images = NULL;

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

SDL_Surface *SDL_CreateSurface(int w, int h)
{
	SDL_Surface *surf = NULL, *surf2 = NULL;
	surf = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, 0, 0, 0, 0);
	memtest(surf);
	
	surf2 = SDL_DisplayFormat(surf);
	
	if(NULL == surf2)
		return surf;
	SDL_FreeSurface(surf);
	return surf2;
}

SDL_Surface *SDL_CreateWhiteKeySurface(int w, int h)
{
	SDL_Surface *surf = NULL;
	
	surf = SDL_CreateSurface(w, h);
	
	SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, 255, 255, 255));
	SDL_SetColorKey(surf, SDL_SRCCOLORKEY, SDL_MapRGB(surf->format, 255, 255, 255));
	
	return surf;
}

int Images_DrawRotNote(SDL_Surface *surf, int x, int y, SDL_Surface *dest)
{
	SDL_Rect pos;
	/*SDL_Rect modif;*/
	if(NULL == Images)
		return 0;
	pos.x = x - Images->rot_noteW;
	pos.y = y - Images->rot_noteH;
	
	/*printf("new x =%d, y=%d\n", pos.x,  pos.y);
	modif.x = 0;
	modif.y = 0;
	if(pos.x < 0)
	{
		modif.x = -pos.x;
		pos.x = 0;
	}
	if(pos.y < 0)
	{
		modif.y = -pos.y;
		pos.y = 0;
	}*/
	SDL_BlitSurface(surf, NULL, dest, &pos);
	return 1;
}

int Draw_Border(SDL_Surface *surf, int size)
{
	if(NULL == surf)
		return 0;
	
	boxRGBA(surf, 0, 0, surf->w, size - 1, 255, 0, 0, 255);
	boxRGBA(surf, surf->w, 0, surf->w - size, surf->h, 255, 0, 0, 255);
	boxRGBA(surf, surf->w, surf->h, 0, surf->h-size, 255, 0, 0, 255);
	boxRGBA(surf, 0, surf->h, size - 1, 0, 255, 0, 0, 255);
	
	return 1;
}


SDL_Surface *Note_CreateHead(int size_w, int size_h)
{
	SDL_Surface *surf =  SDL_CreateWhiteKeySurface(size_w+1, size_h+1);
	SDL_Surface *surf2 = NULL;
	
	memtest(surf);
	
	
	aaellipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2, size_h / 2, 0, 0, 0, 255);
	aaellipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2 - 1, size_h / 2, 0, 0, 0, 255);
	aaellipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2, size_h / 2 - 1, 0, 0, 0, 255);
	aaellipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2 - 1, size_h / 2 - 1, 0, 0, 0, 255);
	aaellipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2 - 2, size_h / 2 - 2, 0, 0, 0, 255);
	filledEllipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2, size_h / 2, 0, 0, 0, 255);

	surf2 = rotozoomSurface(surf, 30.0, 1.0, SMOOTHING_ON);
	
	

	Images->rot_noteH = (int)((surf2->h - surf->h) / 2.0);
	Images->rot_noteW = (int)((surf2->w - surf->w) / 2.0);
	SDL_FreeSurface(surf);
	return surf2;
}

SDL_Surface *Note_CreateHeadWhite(int size_w, int size_h)
{
	SDL_Surface *surf = NULL;
	SDL_Surface *surf2 = NULL;
	SDL_Surface *surf3 = NULL;
	SDL_Rect pos;
	
	surf = Note_CreateHead(size_w, size_h);
	memtest(surf);
	
	surf2 = SDL_CreateSurface(size_w+1, size_h+1);
	memtest(surf2);
	
	SDL_FillRect(surf2, NULL, SDL_MapRGB(surf2->format, 0, 0, 255));
	SDL_SetColorKey(surf2, SDL_SRCCOLORKEY, SDL_MapRGB(surf2->format, 0, 0, 255));
	
	filledEllipseRGBA(surf2, size_w / 2, size_h / 2, size_w / 2 - size_w/8.0, (int)(size_h / 3.75), 255, 255, 255, 255);
	aaellipseRGBA(surf2, size_w / 2, size_h / 2, size_w / 2 -size_w/8.0, (int)(size_h / 3.75) - 1, 255, 255, 255, 255);
	surf3 = rotozoomSurface(surf2, 30.0, 1.0, SMOOTHING_ON);
	memtest(surf3);
	
	SDL_FreeSurface(surf2);
	
	pos.x = 0;
	pos.y = 0;
	
	SDL_BlitSurface(surf3, NULL, surf, &pos);

	SDL_FreeSurface(surf3);
	
	return surf;
}

SDL_Surface *Note_CreateHeadWhole(int size_w, int size_h)
{
	SDL_Surface *surf = NULL;
	SDL_Surface *surf2 = NULL;
	SDL_Surface *surf3 = NULL;
	SDL_Rect pos;
	
	surf = SDL_CreateWhiteKeySurface(size_w + 1, size_h + 1);
	memtest(surf);
	
	aaellipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2, size_h / 2, 0, 0, 0, 255);
	aaellipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2 - 1, size_h / 2, 0, 0, 0, 255);
	aaellipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2, size_h / 2 - 1, 0, 0, 0, 255);
	aaellipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2 - 1, size_h / 2 - 1, 0, 0, 0, 255);
	aaellipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2 - 2, size_h / 2 - 2, 0, 0, 0, 255);
	filledEllipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2, size_h / 2, 0, 0, 0, 255);
	
	surf2 = SDL_CreateSurface(size_h / 1.25 + 1, size_w/2.5 + 1);
	printf("w=%d, h=%d\n", size_h, size_w/3);
	memtest(surf2);
	
	SDL_FillRect(surf2, NULL, SDL_MapRGB(surf2->format, 0, 0, 255));
	SDL_SetColorKey(surf2, SDL_SRCCOLORKEY, SDL_MapRGB(surf2->format, 0, 0, 255));
	
	filledEllipseRGBA(surf2, size_h / 1.25 / 2.0, (size_w/2.5)/2.0, size_h / 1.25 /2.0, (size_w/2.5)/2.0, 255, 255, 255, 255);
	
	
	surf3 = rotozoomSurface(surf2, 120.0, 1.0, SMOOTHING_ON);
	memtest(surf3);
	
	
	
	pos.x = (surf2->w - surf3->w) / 2 + size_w / 5.0;
	pos.y = (surf2->h - surf3->h) / 2 + size_h / 5.0;
	
	SDL_BlitSurface(surf3, NULL, surf, &pos);
	
	SDL_FreeSurface(surf2);
	SDL_FreeSurface(surf3);
	return surf;
}

SDL_Surface *Note_CreateBlack(int size_w, int size_h, int size_queue)
{
	SDL_Surface *surf = NULL;

	if(NULL == Images || NULL == Images->Note_headBlack)
		return NULL;
	surf = SDL_CreateWhiteKeySurface(size_w, size_h + size_queue);
	memtest(surf);
	boxRGBA(surf, size_w-QUEUE_BORDER, 0, size_w, size_h+size_queue-size_h/2, 0, 0, 0, 255);
	
	Images->note1_center = SDL_SetRect(size_w / 2, size_queue - size_h / 3 + size_h / 2,
					surf->w, surf->h);
	memtest(Images->note1_center);
	return surf;
}

SDL_Surface *Note_CreateCrotchet(int size_w, int size_h, int size_queue)
{
	int i;
	SDL_Surface *surf = NULL;
	int max = size_queue + size_h / 3;
	int x[] = {0, (int)(size_w), size_w/2};
	int y[] = {size_queue / 10, max/2, max};

	surf = SDL_CreateWhiteKeySurface((int)(size_w * 1.5), max);
	memtest(surf);
	for(i = 0; i < 20; i++)
	{
		y[0] += i;
		x[2] -= i/4;
		PowerOfBezier(surf, x, y, 3, SetColor(0, 0, 0));
		x[2] += i/4;
		y[0] -= i;	
	}

	return surf;
}

SDL_Surface *Rest_CreateLong(int size_w, int size_h)
{
	SDL_Surface *surf = NULL;
	SDL_Rect pos;
	
	surf = SDL_CreateWhiteKeySurface(size_w, size_h / 2 + 1);
	memtest(surf);
	
	pos.x = 0;
	pos.y = 0;
	pos.w = size_w;
	pos.h = size_h / 2;
	SDL_FillRect(surf, &pos, SDL_MapRGB(surf->format, 0, 0, 0));
	
	Images->pos_Long = SDL_SetRect(pos.x, pos.y, pos.w, pos.h);
	
	return surf;
}

SDL_Surface *Rest_CreateBreveLong(int size_w, int size_h)
{
	SDL_Surface *surf = Rest_CreateLong(size_w, size_h);
	
	Images->pos_BreveLong = SDL_SetRect(Images->pos_Long->x, -size_h / 2,
					Images->pos_Long->w, Images->pos_Long->h);
	
	return surf;
}

Graphics *Graphics_Alloc(void)
{
	Graphics *temp = (Graphics *)malloc(sizeof(Graphics));
	memtest(temp);
	
	temp->Note_headBlack = NULL;
	temp->Note_headWhite = NULL;
	temp->Note_headWhole = NULL;
	temp->Note_White = NULL;
	temp->Note_Black = NULL;
	temp->Note_Crotchet = NULL;
	temp->Rest_Long = NULL;
	temp->pos_Long = NULL;
	temp->Rest_BreveLong = NULL;
	temp->pos_Long = NULL;
	
	temp->rot_noteW = 0;
	temp->rot_noteH = 0;
	temp->note1_center = NULL;
	
	
	return temp;
}

void Graphics_Free(Graphics **graphics)
{
	if(*graphics != NULL)
	{
		if((*graphics)->Note_headBlack != NULL)
			SDL_FreeSurface((*graphics)->Note_headBlack);
		if((*graphics)->Note_headWhite != NULL)
			SDL_FreeSurface((*graphics)->Note_headWhite);
		if((*graphics)->Note_headWhole != NULL)
			SDL_FreeSurface((*graphics)->Note_headWhole);
		if((*graphics)->Note_Black != NULL)
			SDL_FreeSurface((*graphics)->Note_Black);
		if((*graphics)->Note_Crotchet != NULL)
			SDL_FreeSurface((*graphics)->Note_Crotchet);
		if((*graphics)->Rest_Long != NULL)
			SDL_FreeSurface((*graphics)->Rest_Long);
		if((*graphics)->pos_Long != NULL)
			free((*graphics)->pos_Long);
		if((*graphics)->Rest_BreveLong != NULL)
			SDL_FreeSurface((*graphics)->Rest_BreveLong);
		if((*graphics)->pos_BreveLong != NULL)
			free((*graphics)->pos_BreveLong);
			
		free(*graphics);
		*graphics = NULL;
	}
}

int Graphics_LoadAll(void)
{
	int r = 1;
	if(NULL == Images)
		Images = Graphics_Alloc();
	printf("Begin creating graphics... ");
	
	Images->Note_headBlack = Note_CreateHead(HEAD_W, HEAD_H);
	if(NULL == Images->Note_headBlack)
		r = 0;

	Images->Note_headWhite = Note_CreateHeadWhite(HEAD_W, HEAD_H);
	if(NULL == Images->Note_headWhite)
		r = 0;
	
	Images->Note_headWhole = Note_CreateHeadWhole(HEAD_W, HEAD_H);
	if(NULL == Images->Note_headWhole)
		r = 0;
	
	Images->Note_Black = Note_CreateBlack(HEAD_W, HEAD_H, QUEUE);
	if(NULL == Images->Note_Black)
		r = 0;
	
	Images->Note_Crotchet = Note_CreateCrotchet(HEAD_W, HEAD_H, QUEUE);
	if(NULL == Images->Note_Crotchet)
		r = 0;
	
	Images->Rest_Long = Rest_CreateLong(HEAD_W, HEAD_H);
	if(NULL == Images->Rest_Long)
		r = 0;
	
	Images->Rest_BreveLong = Rest_CreateBreveLong(HEAD_W, HEAD_H);
	if(NULL == Images->Rest_BreveLong)
		r = 0;
	
	if(r)
		colorprintf(GREEN, "Ok !\n");
	else
	{
		colorprintf(RED, "Error !\n");
		exit(EXIT_FAILURE);
	}
	
	return 1;
}

void Graphics_Quit(void)
{
	Graphics_Free(&Images);
}

int PowerOfBezier(SDL_Surface *dest, const int *x, const int *y, int n, Color color)
{
	Sint16 *new_x = NULL, *new_y = NULL;
	int i;
	if(NULL == dest)
		return 0;
	
	new_x = (Sint16 *)malloc(sizeof(Sint16) * n);
	memtest(new_x);
	
	new_y = (Sint16 *)malloc(sizeof(Sint16) * n);
	memtest(new_y);
	
	for(i = 0; i < n; i ++)
	{
		*(new_x + i) = (Sint16) *(x + i);
		*(new_y + i) = (Sint16) *(y + i);
	}
	
	bezierRGBA(dest, (const Sint16*)new_x, (const Sint16*)new_y, n, n,
					color.r, color.g, color.b, color.a);
	free(new_x);
	free(new_y);
	return 1;
}













