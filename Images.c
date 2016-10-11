#include "Images.h"


static int Draw_Border(SDL_Surface *surf, int size)
{
	if(NULL == surf)
		return 0;
	
	boxRGBA(surf, 0, 0, surf->w, size, 255, 0, 0, 255);
	boxRGBA(surf, surf->w, 0, surf->w - size, surf->h, 255, 0, 0, 255);
	boxRGBA(surf, surf->w, surf->h, 0, surf->h-size, 255, 0, 0, 255);
	boxRGBA(surf, 0, surf->h, size, 0, 255, 0, 0, 255);
	
	return 1;
}
SDL_Surface *Notes_SoloBasicNoire(double size, double angle)
{
	SDL_Surface *surf = SDL_CreateRGBSurface(SDL_HWSURFACE, 500, 500, 32, 0, 0, 0, 0);
	SDL_Surface *surf2 = NULL;
	memtest(surf);
	
	SDL_FillRect(surf, NULL, SDL_MapRGB(surf->format, 255, 255, 255));
	aaellipseRGBA(surf, 250, 400, 75, 50, 0, 0, 0, 255);
	aaellipseRGBA(surf, 250, 400, 74, 49, 0, 0, 0, 255);

	filledEllipseRGBA(surf, 250, 400, 75, 50, 0, 0, 0, 255);

	SDL_SetColorKey(surf, SDL_SRCCOLORKEY, SDL_MapRGB(surf->format, 255, 255, 255));

	surf2 = rotozoomSurface(surf, 30.0, 1.0, SMOOTHING_ON);
	
	boxRGBA(surf2, 465, 0, 486, 460, 0, 0, 0, 255);
	SDL_FreeSurface(surf);
	
	surf = rotozoomSurface(surf2, angle, size, SMOOTHING_ON);
	SDL_FreeSurface(surf2);
	return surf;
}

void test_note(double size, double angle, int posx, int posy, SDL_Surface *screen)
{
	SDL_Rect pos;
	SDL_Surface *surf = NULL;
	surf = Notes_SoloBasicNoire(size, angle);
	
	pos.x = posx;
	pos.y = posy;
	Draw_Border(surf, 3);
	SDL_BlitSurface(surf, NULL, screen, &pos);
	SDL_FreeSurface(surf);
}
