#include "../include/Text.h"



char *Str_Copy(const char *str)
{
	char *res = NULL;
	res = (char *)malloc(sizeof(char) * (strlen(str) + 1));
	memtest(res);

	strcpy(res, str);
	return res;
}

int Moteur_WriteText(int x, int y, const char *text, int size,
				const char *path_font, SDL_Color color,
				Text_Style graphic, Text_Align align,
				SDL_Surface *dest)
{
	SDL_Surface *temp = NULL;
	SDL_Rect pos;
	TTF_Font *font = NULL;
	font = TTF_OpenFont(path_font, size);
	if(NULL == font)
		return 0;

	switch(graphic)
	{
		case TEXT_SOLID:
			temp = TTF_RenderUTF8_Solid(font, text, color);
			break;
		case TEXT_BLENDED:
			temp = TTF_RenderUTF8_Blended(font, text, color);
			break;
	}

	if(NULL == temp)
		return 0;

	switch(align)
	{
		case TEXT_LEFT:
			pos.x = x;
			pos.y = y;
			break;
		case TEXT_CENTER:
			pos.x = (int)(x - temp->w/2.0);
			pos.y = (int)(y - temp->h/2.0);
			break;
		case TEXT_RIGHT:
			pos.x = x - temp->w;
			pos.y = y - temp->h;
			break;
	}

	SDL_BlitSurface(temp, NULL, dest, &pos);
	TTF_CloseFont(font);
	SDL_FreeSurface(temp);
	return 1;
}


int Moteur_WriteParagraph(int x, int y, int max_w, const char *text_paragraph, int size, int esp,
				const char *path_font, SDL_Color color,
				Text_Style graphic, Text_Align align,
				SDL_Surface *dest)
{
	int i = 1;
	int w,h;
	char sauv = '\0';
	int count = 0;
	char *text = NULL;
	TTF_Font *font = NULL;
	char *sauv_text = NULL;
	font = TTF_OpenFont(path_font, size);
	
	if(NULL == font)
		return 0;
	
	text = Str_Copy(text_paragraph);
	sauv_text = text;
	
	while((unsigned int)i < strlen(text))
	{

		sauv = text[i];

		text[i] = '\0';

		TTF_SizeUTF8(font, text, &w, &h);
		text[i] = sauv;
		if(w >= max_w)
		{

			if(i <= 0)
				return 0;

			text[i] = '\0';

			Moteur_WriteText(x, y+count*(h - 3 + esp), text, size,
				 path_font, color,
				graphic, align,
				dest);

			count++;
			text[i] = sauv;
			text += (i + 1);
			i = 0;
		}
		i++;
	}
	Moteur_WriteText(x, y+count*(h - 3 + esp), text, size, path_font, color,
							graphic, align, dest);

	TTF_CloseFont(font);
	free(sauv_text);

	return count+1;

}

int getSizeStr(char *str)
{
	int i = 0;
	while(str++ != 0 && ++i);
}
