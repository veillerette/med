#include "../include/Images.h"

Graphics *Images = NULL;
Graphics *HoverImages = NULL;
Graphics *LittleImages = NULL;

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
	return SetColorA(r, g, b, 255);
}

SDL_Color SDLColor_Set(int r, int g, int b)
{
	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	return color;
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

SDL_Rect SDL_SetLocalRect(int x, int y, int w, int h)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
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


SDL_Surface *Note_CreateHead(int size_w, int size_h, Color color)
{
	SDL_Surface *surf =  SDL_CreateWhiteKeySurface(size_w+1, size_h+1);
	SDL_Surface *surf2 = NULL;

	memtest(surf);


	aaellipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2, size_h / 2, 0, 0, 0, 255);
	aaellipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2 - 1, size_h / 2, 0, 0, 0, 255);
	aaellipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2, size_h / 2 - 1, 0, 0, 0, 255);
	aaellipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2 - 1, size_h / 2 - 1, 0, 0, 0, 255);
	aaellipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2 - 2, size_h / 2 - 2, 0, 0, 0, 255);
	filledEllipseRGBA(surf, size_w / 2, size_h / 2, size_w / 2, size_h / 2, color.r, color.g, color.b, 255);

	surf2 = rotozoomSurface(surf, 30.0, 1.0, SMOOTHING_ON);



	Images->rot_noteH = (int)((surf2->h - surf->h) / 2.0);
	Images->rot_noteW = (int)((surf2->w - surf->w) / 2.0);
	SDL_FreeSurface(surf);
	return surf2;
}

SDL_Surface *Note_CreateHeadWhite(int size_w, int size_h, Color color)
{
	SDL_Surface *surf = NULL;
	SDL_Surface *surf2 = NULL;
	SDL_Surface *surf3 = NULL;
	SDL_Rect pos;

	surf = Note_CreateHead(size_w, size_h, color);
	memtest(surf);

	surf2 = SDL_CreateSurface(size_w+1, size_h+1);
	memtest(surf2);

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
SDL_Surface *Note_CreateCrotchetInv(int size_w, int size_h, int size_queue)
{
	int i;
	int sauv;
	SDL_Surface *surf = NULL;
	int max = size_queue + size_h / 3;
	int x[] = {0, (int)(size_w*1.25), (int)(size_w/1.5)};
	int y[] = {max - size_queue/10, max / 2, size_queue / 10};

	surf = SDL_CreateWhiteKeySurface((int)(size_w * 1.5), max);
	memtest(surf);
	for(i = 0; i < 20; i++)
	{
		sauv = x[2];
		y[0] -= i;
		PowerOfBezier(surf, x, y, 3, SetColor(0, 0, 0));
		x[2] -= (int)i/4;
		if(x[2] < 0)
			x[2] = 0;
		PowerOfBezier(surf, x, y, 3, SetColor(0, 0, 0));
		x[2] = sauv;
		y[0] += i;
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

SDL_Surface *Rest_CreateQuaver(int size_w, int size_h)
{
	SDL_Surface *surf = NULL;
	int i;
	int x[] = {size_w, size_w*3/4, size_w/2};
	int y[] = {size_h /4, size_h, size_h /2};


	surf = SDL_CreateWhiteKeySurface(size_w + 1, size_h * 2);
	memtest(surf);

	for(i = 0; i < 5; i++)
	{
		y[0]+=i;
		y[2]+=i;
		PowerOfBezier(surf, x, y, 3, SetColor(0, 0, 0));
		y[2]-=i;
		y[0]-=i;
	}
	for(i = 1; i < 8; i++)
	{
		y[2]-=i;
		PowerOfBezier(surf, x, y, 3, SetColor(0, 0, 0));
		y[2]+=i;
	}
	for(i = 0; i < QUEUE_BORDER; i++)
	{
		lineRGBA(surf, size_w / 2 + 2*QUEUE_BORDER - i, size_h * 2, size_w - i - 1 , size_h / 3 + i , 0, 0, 0, 255);
		lineRGBA(surf, size_w / 2 + 2*QUEUE_BORDER - i, size_h * 2, size_w - i, size_h / 3 + i, 0, 0, 0, 255);
	}

	filledCircleRGBA(surf, size_w/2 + 2, size_h /2, 8, 0, 0, 0, 255);

	return surf;
}

SDL_Surface *Rest_CreateBreave(int size_w, int size_h)
{
	SDL_Surface *surf = NULL;
	int i;
	int m = size_h*2.75;
	int x[] = {size_w/2, size_w/2-size_w/4, size_w/2+size_w/4};
	int y[] = {m, m*7/10, m*8.5/10};
	int x2[] = {size_w/2+size_w/4, size_w/2-size_w/8, size_w/2+size_w/4};
	int y2[] = {m*8.5/10-5, m/1.75, m/3};
	int x3[] = {size_w/2-size_w/4, size_w/2+size_w/8, size_w/2-size_w/6-5};
	int y3[] = {m/1.75-3, m/3+5, m/13+1};


	surf = SDL_CreateWhiteKeySurface(size_w + 1, size_h*3);
	memtest(surf);

	for(i = 0; i < 15; i++)
	{
		y[1]-=i;
		x[1]-=i;
		y[2]-=i/5;
		y[0]+=i/3;
		PowerOfBezier(surf, x, y, 3, SetColor(0, 0, 0));
		y[1]+=i;
		x[1]+=i;
		y[0]-=i/3;
		y[2]+=i/5;
	}

	for(i = 0; i < 5; i++)
	{
		lineRGBA(surf, size_w/2+size_w/4, m*8.5/10-i, size_w/2-size_w/4, m/1.75-i, 0, 0, 0, 255);
	}

	for(i = 0; i < 22; i++)
	{
		x2[0]--;
		y2[0]--;
		PowerOfBezier(surf, x2, y2, 3, SetColor(0, 0, 0));
	}
	for(i = 0; i < 5; i++)
	{
		lineRGBA(surf, x2[2]-i+1, y2[2]+i-1, x3[2], y3[2], 0, 0, 0, 255);
		lineRGBA(surf, x2[2]-i-1+1, y2[2]+i-1, x3[2], y3[2], 0, 0, 0, 255);
	}

	for(i = 0; i < 40; i++)
	{
		x3[1]+=i%2;
		PowerOfBezier(surf, x3, y3, 3, SetColor(0, 0, 0));
	}


	return surf;
}

SDL_Surface *Rest_CreateBreveLong(int size_w, int size_h)
{
	SDL_Surface *surf = Rest_CreateLong(size_w, size_h);

	Images->pos_BreveLong = SDL_SetRect(Images->pos_Long->x, -size_h / 2,
					Images->pos_Long->w, Images->pos_Long->h);

	return surf;
}

SDL_Surface *CreateNatural(int size_w, int size_h)
{
	SDL_Surface *surf = NULL;
	int m = size_h * 3;
	int w = size_w * 3.0 / 4;
	int i;
	int x[4] = {0, 5, w-17, w-12};
	
	for(i = 0; i < 4; i++)
		x[i] += 5;
	
	surf = SDL_CreateWhiteKeySurface(w+1, m);
	
	boxRGBA(surf, x[0], 0, x[1], size_h*2+size_h/4, 0, 0, 0, 255);
	boxRGBA(surf, x[2], size_h-size_h/5, x[3], m, 0, 0, 0, 255);
	
	for(i = 0; i < size_h/5+3; i++)
	{
		aalineRGBA(surf, x[0], size_h+6-i, x[3], size_h-i, 0, 0, 0, 255);
		aalineRGBA(surf, x[0], size_h+6-i+1, x[3], size_h-i, 0, 0, 0, 255);
		aalineRGBA(surf, x[0], size_h+6-i, x[3], size_h-i+1, 0, 0, 0, 255);
	}
	
	for(i = 0; i < size_h/4+3; i++)
	{
		aalineRGBA(surf, x[0], size_h*2+size_h/4-i, x[3], size_h*2+size_h/4-6-i, 0, 0, 0, 255);
		aalineRGBA(surf, x[0], size_h*2+size_h/4-i+1, x[3], size_h*2+size_h/4-6-i, 0, 0, 0, 255);
		aalineRGBA(surf, x[0], size_h*2+size_h/4-i, x[3], size_h*2+size_h/4-6-i+1, 0, 0, 0, 255);
	}
	return surf;
}

SDL_Surface *CreateDoubleSharp(int size_w, int size_h)
{
	SDL_Surface *surf = NULL;
	int m = size_h * 3 - 5;
	int w = size_w * 3.0 / 4;
	
	surf = SDL_CreateWhiteKeySurface(w+1, m);
		
	Draw_Border(surf, 2);
	return surf;
}

SDL_Surface *CreateSharp(int size_w, int size_h)
{
	SDL_Surface *surf = NULL;
	int m = size_h * 3 - 5;
	int w = size_w*3.0/4;
	int e = 13;
	Sint16 x[] = {0, w, w, 0};
	Sint16 y[] = {size_h-e/3, size_h-e, y[1]+e, y[0]+e};
	int i;

	surf = SDL_CreateWhiteKeySurface(w+1, m);
	memtest(surf);

	boxRGBA(surf, size_w*1.5/8-1, 5, size_w/4-1, m, 0, 0, 0, 255);
	boxRGBA(surf, w-size_w/4+1, 0, w-size_w*1.5/8+2, m-5, 0, 0, 0, 255);

	filledPolygonRGBA(surf, x, y, 4, 0, 0, 0, 255);

	for(i = 0; i < 4; i++)
		y[i] += HEAD_H - 5;

	filledPolygonRGBA(surf, x, y, 4, 0, 0, 0, 255);

	return surf;
}

SDL_Surface *CreateFlat(int size_w, int size_h)
{
	SDL_Surface *surf = NULL;
	int h = (size_h-1) * 2.5;
	int w = size_w * 3.0/4;
	int x[] = {size_w/10, w*1.1, size_w/10};
	int y[] = {h/2+5, h/2.5, h};
	int i;

	surf = SDL_CreateWhiteKeySurface(w, h);
	memtest(surf);

	boxRGBA(surf, 0, 0, size_w/10, h, 0, 0, 0, 255);

	for(i = 0; i < 11; i++)
	{
		x[1]+=i;
		y[1]-=i;
		PowerOfBezier(surf, x, y, 3, SetColor(0, 0, 0));
		x[1]-=i;
		y[1]+=i;
	}

	for(i = 0; i < 6; i++)
	{
		y[2]--;
		y[0]++;
		PowerOfBezier(surf, x, y, 3, SetColor(0, 0, 0));
	}

	return surf;
}


SDL_Surface *CreateCleSol(int size_w, int size_h)
{
	int i,j;
	SDL_Surface *surf = NULL;

	int x[] = {size_w*1.25-20, (size_w*2+size_w*1.25-40)/2, size_w*2-20};
	int y[] = {size_h*5.25, size_h*4, size_h*5};

	int x2[] = {size_w*2-20, (size_w*2.25-20)/2, size_w*0.25};
	int y2[] = {size_h*5, size_h*6.5, size_h*5};

	int x3[] = {size_w*0.25+2, 0, x[1]+5, x[1]+10};
	int y3[] = {size_h * 5 - 5, size_h*4, size_h*3, size_h*1.5};

	int x4[] = {x3[3]-6, x3[3]-17, x3[3]-45, x3[3]-30};
	int y4[] = {size_h*1.5, size_h/2, size_h*1.5, size_h*2};

	surf = SDL_CreateWhiteKeySurface(size_w * 2, size_h * 8);
	memtest(surf);

	for(i = 0; i < 20; i++)
	{
		y[1] -= i;
		x[2] += (i % 3 == 0) ? 1 : 0;
		x[0] -= (i % 10 == 0) ? 1 : 0;
		PowerOfBezier(surf, x, y, 3, SetColor(0, 0, 0));
		y[1]+=i;
	}

	for(i = 0; i < 8; i++)
	{
		arcRGBA(surf, (x2[2]+x2[0])/2+6-5, y2[0]+i%5-2-5, x2[1]-x2[2]-3+i/2-1+5, 3, 180, 0, 0, 0, 255);
		arcRGBA(surf, (x2[2]+x2[0])/2+6-5-1, y2[0]+i%5-2-5, x2[1]-x2[2]-3+i/2-1+5, 3, 180, 0, 0, 0, 255);
		arcRGBA(surf, (x2[2]+x2[0])/2+6-5, y2[0]+i%5-2-5-1, x2[1]-x2[2]-3+i/2-1+5, 3, 180, 0, 0, 0, 255);
	}

	for(i = 0; i < 8; i++)
	{
		x3[0]-=(i%2==0)?1:0;
		x3[2]--;
		x3[3]-=(i%2==0)?1:0;
		x3[1]--;
		PowerOfBezier(surf, x3, y3, 4, SetColor(0, 0, 0));
	}

	for(i = 0; i < 8; i++)
	{
		y4[1]--;
		x4[3]-=(i%2==0)?1:0;;
		x4[0]+=(i%2==0)?1:0;
		PowerOfBezier(surf, x4, y4, 4, SetColor(0, 0, 0));
	}

	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 2; j++)
			aalineRGBA(surf, x4[3]+i, y4[3], size_w*1.4+j, size_h*7, 0, 0, 0, 255);
	}

	for(i = 0; i < 4; i++)
		arcRGBA(surf, size_w*1.4-16, size_h*7-i-1, 15, 0,180, 0, 0, 0, 255);

	filledCircleRGBA(surf, size_w*1.4-22, size_h*7, 9, 0, 0, 0, 255);

	return surf;
}

SDL_Surface *CreateCleFa(int size_w, int size_h)
{
	SDL_Surface *surf = NULL;
	SDL_Color color = {50, 0, 50, 0};
	surf = SDL_CreateWhiteKeySurface(size_w * 2, size_h * 3.5);
	memtest(surf);

	Moteur_WriteText(size_w, size_h * 1.75, "FA", 70,
				"media/Garamond.ttf", color,
				TEXT_BLENDED, TEXT_CENTER,
				surf);

	Draw_Border(surf, 2);
	return surf;
}

SDL_Surface *CreateDoubleFlat(int size_w, int size_h)
{
	SDL_Surface *surf = NULL;
	int h = (size_h-1) * 2.5;
	int w = (size_w * 3.0/4) * 2;
	SDL_Rect pos = {0, 0, 0, 0};

	if(NULL == Images->Flat)
		return NULL;

	surf = SDL_CreateWhiteKeySurface(w, h);
	memtest(surf);

	SDL_BlitSurface(Images->Flat, NULL, surf, &pos);
	pos.x += w/2.0;

	SDL_BlitSurface(Images->Flat, NULL, surf, &pos);

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
	temp->Note_CrotchetInv = NULL;
	temp->Rest_Long = NULL;
	temp->pos_Long = NULL;
	temp->Rest_BreveLong = NULL;
	temp->pos_BreveLong = NULL;

	temp->Rest_Crotchet = NULL;
	temp->pos_restCrotchet = NULL;

	temp->Rest_Quaver = NULL;
	temp->pos_Quaver = NULL;

	temp->Rest_Breve = NULL;

	temp->Sharp = NULL;
	temp->Flat = NULL;
	temp->DoubleFlat = NULL;
	temp->DoubleSharp = NULL;
	temp->Natural = NULL;

	temp->rot_noteW = 0;
	temp->rot_noteH = 0;
	temp->note1_center = NULL;

	temp->Cle_Sol = NULL;
	temp->Cle_Fa = NULL;

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
		if((*graphics)->Note_White != NULL)
			SDL_FreeSurface((*graphics)->Note_White);
		if((*graphics)->note1_center != NULL)
			free((*graphics)->note1_center);

		if((*graphics)->Note_Crotchet != NULL)
			SDL_FreeSurface((*graphics)->Note_Crotchet);
		if((*graphics)->Note_CrotchetInv != NULL)
			SDL_FreeSurface((*graphics)->Note_CrotchetInv);

		if((*graphics)->Rest_Long != NULL)
			SDL_FreeSurface((*graphics)->Rest_Long);
		if((*graphics)->pos_Long != NULL)
			free((*graphics)->pos_Long);

		if((*graphics)->Rest_BreveLong != NULL)
			SDL_FreeSurface((*graphics)->Rest_BreveLong);
		if((*graphics)->pos_BreveLong != NULL)
			free((*graphics)->pos_BreveLong);

		if((*graphics)->Rest_Quaver != NULL)
			SDL_FreeSurface((*graphics)->Rest_Quaver);
		if((*graphics)->pos_Quaver != NULL)
			free((*graphics)->pos_Quaver);

		if((*graphics)->Rest_Breve != NULL)
			SDL_FreeSurface((*graphics)->Rest_Breve);

		if((*graphics)->Sharp != NULL)
			SDL_FreeSurface((*graphics)->Sharp);

		if((*graphics)->Flat != NULL)
			SDL_FreeSurface((*graphics)->Flat);
			
		if((*graphics)->DoubleFlat != NULL)
			SDL_FreeSurface((*graphics)->DoubleFlat);
		
		if((*graphics)->DoubleSharp != NULL)
			SDL_FreeSurface((*graphics)->DoubleSharp);
		
		if((*graphics)->Natural != NULL)
			SDL_FreeSurface((*graphics)->Natural);

		if((*graphics)->Cle_Sol != NULL)
			SDL_FreeSurface((*graphics)->Cle_Sol);

		if((*graphics)->Cle_Fa != NULL)
			SDL_FreeSurface((*graphics)->Cle_Fa);

		free(*graphics);
		*graphics = NULL;
	}
}

SDL_Surface *ConvertDisplayFormat(SDL_Surface *src)
{
	SDL_Surface *temp = NULL;
	if(NULL == src)
		return NULL;
	temp = SDL_DisplayFormat(src);
	SDL_FreeSurface(src);

	if(NULL == temp)
	{
		return NULL;
	}

	return temp;
}

int Graphics_Load(Graphics **data)
{
	int r = 1;
	if(NULL == (*data))
		(*data) = Graphics_Alloc();
	printf("Begin creating graphics... ");

	(*data)->Note_headBlack = ConvertDisplayFormat(Note_CreateHead(HEAD_W, HEAD_H, SetColor(0, 0, 0)));
	if(NULL == (*data)->Note_headBlack)
		r = 0;

	(*data)->Note_headWhite = ConvertDisplayFormat(Note_CreateHeadWhite(HEAD_W, HEAD_H, SetColor(0, 0, 0)));
	if(NULL == (*data)->Note_headWhite)
		r = 0;

	(*data)->Note_headWhole = ConvertDisplayFormat(Note_CreateHeadWhole(HEAD_W, HEAD_H));
	if(NULL == (*data)->Note_headWhole)
		r = 0;

	(*data)->Note_Black = ConvertDisplayFormat(Note_CreateBlack(HEAD_W, HEAD_H, QUEUE));
	if(NULL == (*data)->Note_Black)
		r = 0;

	(*data)->Note_Crotchet = ConvertDisplayFormat(Note_CreateCrotchet(HEAD_W, HEAD_H, QUEUE));
	if(NULL == (*data)->Note_Crotchet)
		r = 0;

	(*data)->Note_CrotchetInv = ConvertDisplayFormat(Note_CreateCrotchetInv(HEAD_W, HEAD_H, QUEUE));
	if(NULL == (*data)->Note_CrotchetInv)
		r = 0;

	(*data)->Rest_Long = ConvertDisplayFormat(Rest_CreateLong(HEAD_W, HEAD_H));
	if(NULL == (*data)->Rest_Long)
		r = 0;

	(*data)->Rest_BreveLong = ConvertDisplayFormat(Rest_CreateBreveLong(HEAD_W, HEAD_H));
	if(NULL == (*data)->Rest_BreveLong)
		r = 0;

	(*data)->Rest_Quaver = ConvertDisplayFormat(Rest_CreateQuaver(HEAD_W, HEAD_H));
	if(NULL == (*data)->Rest_Quaver)
		r = 0;

	(*data)->Rest_Breve = ConvertDisplayFormat(Rest_CreateBreave(HEAD_W, HEAD_H));
	if(NULL == (*data)->Rest_Breve)
		r = 0;

	(*data)->Sharp = ConvertDisplayFormat(CreateSharp(HEAD_W, HEAD_H));
	if(NULL == (*data)->Sharp)
		r = 0;

	(*data)->Flat = ConvertDisplayFormat(CreateFlat(HEAD_W, HEAD_H));
	if(NULL == (*data)->Flat)
		r = 0;

	(*data)->DoubleFlat = ConvertDisplayFormat(CreateDoubleFlat(HEAD_W, HEAD_H));
	if(NULL == (*data)->DoubleFlat)
		r = 0;
	
	(*data)->DoubleSharp = ConvertDisplayFormat(CreateDoubleSharp(HEAD_W, HEAD_H));
	if(NULL == (*data)->DoubleSharp)
		r = 0;
	
	(*data)->Natural = ConvertDisplayFormat(CreateNatural(HEAD_W, HEAD_H));
	if(NULL == (*data)->Natural)
		r = 0;

	(*data)->Cle_Sol = ConvertDisplayFormat(CreateCleSol(HEAD_W, HEAD_H));
	if(NULL == (*data)->Cle_Sol)
		r = 0;

	(*data)->Cle_Fa = ConvertDisplayFormat(CreateCleFa(HEAD_W, HEAD_H));
	if(NULL == (*data)->Cle_Fa)
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

int Graphics_LoadHover(Graphics **data)
{
	int r = 1;
	if(NULL == (*data))
		(*data) = Graphics_Alloc();
	printf("Begin creating hover graphics... ");

	(*data)->Note_headBlack = Note_CreateHead(HEAD_W, HEAD_H, SetColor(0, 0, 80));
	if(NULL == (*data)->Note_headBlack)
		r = 0;
	if(r)
		colorprintf(GREEN, "Ok !\n");
	else
	{
		colorprintf(RED, "Error 1 !\n");
		exit(EXIT_FAILURE);
	}
	return 1;
}

Uint32 SDL_GetPixel(SDL_Surface *surf, int x, int y)
{
	int bpp = surf->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surf->pixels + x * bpp + y * surf->pitch;

	switch(bpp)
	{
		case 2:
			return *(Uint16 *)p;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[2] << 16 | p[1] << 8 | p[0];
		case 4:
			return *(Uint32 *)p;
		default:
			break;
	}
	return 0;
}

int SDL_SetPixel(SDL_Surface *surf, int x, int y, Uint32 pix)
{
	int bpp = surf->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surf->pixels + x * bpp + y * surf->pitch;

	switch(bpp)
	{
		case 2:
			*(Uint16 *)p = pix;
			break;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				p[0] = (pix >> 16) & 0xFF;
				p[1] = (pix >> 8) & 0xFF;
				p[2] = (pix & 0xFF);
			}
			else
			{
				p[2] = (pix >> 16) & 0xFF;
				p[1] = (pix >> 8) & 0xFF;
				p[0] = (pix & 0xFF);
			}
			break;
		case 4:
			*(Uint32 *)p = pix;
			break;
		default:
			break;
	}
	return 1;
}

int Images_InvertBlack(SDL_Surface *surf)
{
	int x,y;
	Uint32 pix;
	if(NULL == surf)
		return 0;
	for(x = 0; x < surf->w; x++)
	{
		for(y = 0; y < surf->h; y++)
		{
			pix = SDL_GetPixel(surf, x, y);
			if(pix > 0x7FFFFF)
				pix = 0x000000;
			else
				pix = 0x778899;
			SDL_SetPixel(surf, x, y, pix);
		}
	}
	return 1;
}

SDL_Surface *Create_Little(SDL_Surface *src, int ratioX, int ratioY)
{
	SDL_Surface *dest = NULL;
	dest = shrinkSurface(src, ratioX, ratioY);
	Images_InvertBlack(dest);
	SDL_SetColorKey(dest, SDL_SRCCOLORKEY, SDL_MapRGB(dest->format, 0, 0, 0));

	return dest;
}

int Graphics_LoadLittle(Graphics **data)
{
	Graphics *cur;
	if(NULL == (*data))
		(*data) = Graphics_Alloc();
	printf("Begin creating little graphics... ");
	if(Images == NULL)
	{
		colorprintf(RED, "Error 2 !\n");
		exit(EXIT_FAILURE);
	}

	cur = *data;


	cur->Note_headBlack = Create_Little(Images->Note_headBlack, RATIO_LITTLE+1, RATIO_LITTLE+1);
	cur->Note_headWhite = Create_Little(Images->Note_headWhite, RATIO_LITTLE+1, RATIO_LITTLE+1);
	cur->Note_headWhole = Create_Little(Images->Note_headWhole, RATIO_LITTLE, RATIO_LITTLE);
	cur->Note_Black = Create_Little(Images->Note_Black, RATIO_LITTLE+1, RATIO_LITTLE+2);
	cur->Note_Crotchet = Create_Little(Images->Note_Crotchet, RATIO_LITTLE+1, RATIO_LITTLE+2);
	cur->Note_CrotchetInv = Create_Little(Images->Note_CrotchetInv, RATIO_LITTLE, RATIO_LITTLE);
	cur->Rest_Long = Create_Little(Images->Rest_Long, RATIO_LITTLE, RATIO_LITTLE);
	cur->Rest_BreveLong = Create_Little(Images->Rest_BreveLong, RATIO_LITTLE, RATIO_LITTLE);
	cur->Rest_Quaver = Create_Little(Images->Rest_Quaver, RATIO_LITTLE, RATIO_LITTLE);
	cur->Rest_Breve = Create_Little(Images->Rest_Breve, RATIO_LITTLE, RATIO_LITTLE);
	cur->Sharp = Create_Little(Images->Sharp, RATIO_LITTLE+1, RATIO_LITTLE+1);
	cur->Flat = Create_Little(Images->Flat, RATIO_LITTLE+1, RATIO_LITTLE+1);
	cur->DoubleFlat = Create_Little(Images->DoubleFlat, RATIO_LITTLE+1, RATIO_LITTLE+1);
	cur->DoubleSharp = Create_Little(Images->DoubleSharp, RATIO_LITTLE+1, RATIO_LITTLE+1);
	cur->Natural = Create_Little(Images->Natural, RATIO_LITTLE+1, RATIO_LITTLE+1);

	colorprintf(GREEN, "Ok !");
	printf("\n");
	return 1;
}

int Graphics_LoadAll(void)
{
	Graphics_Load(&Images);
	Graphics_LoadHover(&HoverImages);
	Graphics_LoadLittle(&LittleImages);
	return 1;
}

void Graphics_Quit(void)
{
	Graphics_Free(&Images);
	Graphics_Free(&HoverImages);
	Graphics_Free(&LittleImages);
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

	bezierRGBA(dest, (const Sint16*)new_x, (const Sint16*)new_y, n, 5*n,
					color.r, color.g, color.b, color.a);
	free(new_x);
	free(new_y);
	return 1;
}

int Image_DrawRectangleBorder(SDL_Surface *dst, int x, int y, int x2, int y2, int borderSize, int r, int g, int b, int a)
{
	int temp;
	SDL_Rect pos;
	if((NULL == dst) || (x < 0) || (y < 0) || (x2 < 0) || (y2 < 0) || (borderSize < 1))
		return 0;

	if(x2 < x)
	{
		temp = x;
		x = x2;
		x2 = temp;
	}
	if(y2 < y)
	{
		temp = y;
		y = y2;
		y2 = temp;
	}

	pos.w = x2 - x;
	pos.h = borderSize;
	pos.x = x;
	pos.y = y;
	SDL_FillRect(dst, &pos, SDL_MapRGBA(dst->format, r, g, b, a));

	pos.y = y2 - borderSize;
	SDL_FillRect(dst, &pos, SDL_MapRGBA(dst->format, r, g, b, a));

	pos.w = borderSize;
	pos.h = y2 - y;
	pos.x = x;
	pos.y = y;
	SDL_FillRect(dst, &pos, SDL_MapRGBA(dst->format, r, g, b, a));

	pos.x = x2 - borderSize;
	SDL_FillRect(dst, &pos, SDL_MapRGBA(dst->format, r, g, b, a));

	return 1;
}

int Image_MyShrinkRaw(SDL_Surface *dst, SDL_Surface *src, int factor)
{
	register int x,y;
	
	if((NULL == dst) || (NULL == src))
		return 0;

	for(y = 0; y < src->h; y+=factor)
	{
		for(x = 0; x < src->w; x+=factor)
		{
			SDL_SetPixel(dst, x / factor, y / factor, SDL_GetPixel(src, x, y));
		}
	}
	return 1;
}
