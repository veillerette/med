#include "../include/Explorer.h"

Explorer *Explorer_Create(int x_base, int y_base, int w, int h,
			int header1, int header2, int header3, SDL_Surface *dst)
{
	char *buf = NULL;
	int i;
	Explorer *temp = (Explorer *)malloc(sizeof(Explorer));
	memtest(temp);
	
	buf = get_current_dir_name();
	
	temp->base = SDL_SetLocalRect(x_base, y_base, w, h);
	temp->dir = Directory_Create(buf);
	memtest(temp->dir);
	
	temp->dst = dst;
	temp->header1 = header1;
	temp->header2 = header2;
	temp->header3 = header3;
	
	temp->hover = 0;
	temp->select = 0;
	SDL_PollEvent(&(temp->last));
	
	temp->line = 0;
	
	free(buf);
	
	temp->pos = (SDL_Rect *)malloc(sizeof(SDL_Rect) * temp->dir->n);
	memtest(temp->pos);
	
	for(i = 0; i < temp->dir->n; i++)
		temp->pos[i] = SDL_SetLocalRect(0, 0, 0, 0);	
	
	return temp;
}

void Explorer_Free(Explorer **e)
{
	if(*e != NULL)
	{	
		if((*e)->dir != NULL)
			Directory_Free(&((*e)->dir));
		
		if((*e)->pos != NULL)
			free((*e)->pos);
		
		free(*e);
		*e = NULL;
	}
}

char *Str_Concat(int n, ...)
{
	va_list va;
	int i;
	int len = 0;
	char *res = NULL;
	
	va_start(va, n);
	for(i = 0; i < n; i++)
		len += strlen(va_arg(va, const char *));
	
	res = (char *)calloc(len + 1, sizeof(char));
	memtest(res);
	va_end(va);
	
	va_start(va, n);
	for(i = 0; i < n; i++)
	{
		strcat(res, va_arg(va, const char *));
	}
	
	va_end(va);
	return res;
}

char *Str_TopFolder(const char *path)
{
	int i;
	int n = strlen(path);
	char *res = NULL;
	
	if(n == 1 && path[0] == '/')
		return Str_Copy(path);
	
	while(path[n-1] != '/')
		n--;
	
	if(n == 1)
		n = 2;
	
	res = (char *)malloc(sizeof(char) * (n + 1));
	memtest(res);
	
	for(i = 0; i < n-1; i++)
		res[i] = path[i];
	res[i] = 0;
	return res;
}

char *Str_GoInSub(const char *path, const char *sub)
{
	char *res = NULL;
	int n = strlen(path), n2 = strlen(sub);
	
	if((NULL == path) || (NULL == sub))
		return 0;
	
	res = (char *)malloc(sizeof(char) * (n + n2 + 2));
	memtest(res);
	
	sprintf(res, "%s%s%s", path, (n==1)?"":"/", sub);
	return res;
}

int Explorer_ChangeDirectory(Explorer *e, int direct)
{
	char *new = NULL;
	int i;
	if(direct == -1) /* top folder */
	{
		new = Str_TopFolder(e->dir->name);
	}
	else /* in a directory */
	{
		new = Str_GoInSub(e->dir->name, e->dir->tab[direct]->name);
	}
	
	printf("new directory = \"%s\"\n", new);
	
	Directory_Change(&(e->dir), new);
	
	e->hover = 0;
	e->select = 0;
	e->line = 0;
	
	free(e->pos);
	e->pos = (SDL_Rect *)malloc(sizeof(SDL_Rect) * e->dir->n);
	memtest(e->pos);
	
	for(i = 0; i < e->dir->n; i++)
		e->pos[i] = SDL_SetLocalRect(0, 0, 0, 0);
		
	free(new);
	return FORCE_MAJ;
}

void Explorer_Header1(Explorer *e)
{
	boxRGBA(e->dst, e->base.x, e->base.y, 
		e->base.x + e->base.w, e->base.y + e->header1,
		220, 220, 220, 255);
	
	Moteur_WriteText(e->base.x + e->base.w / 2, e->base.y+e->header1/2,
			"Sélectionnez un fichier :", e->header1/2-4,
			EXPLORER_FONT, SDLColor_Set(40, 40, 40),
			TEXT_BLENDED, TEXT_CENTER, e->dst);
}

void Explorer_Header2(Explorer *e)
{
	Color color = SetColor(200, 200, 200);
	Color text = SetColor(40, 40, 40);
	
	boxRGBA(e->dst, e->base.x, e->base.y + e->header1, 
		e->base.x + e->base.w, e->base.y + e->header1 + e->header2,
		240, 240, 240, 255);
	
	if(e->hover == -2)
	{
		color = SetColor(0, 150, 220);
		text = SetColor(230, 230, 230);
	}
	
	e->dad = SDL_SetLocalRect(e->base.x+20, e->base.y + e->header1 + 5, 100, (e->header2-10));
	
	roundedBoxRGBA(e->dst, e->base.x+20, e->base.y + e->header1 + 5,
		e->base.x + 20 + 100, e->base.y + e->header1 + 5 + (e->header2-10),
		3, color.r, color.g, color.b, 255);
	
	Moteur_WriteText(e->dad.x + e->dad.w / 2, e->dad.y + e->dad.h/2,
		"Parent", 22, EXPLORER_FONT, SDLColor_Set(text.r, text.g, text.b), 
		TEXT_BLENDED, TEXT_CENTER, e->dst);
	
	Moteur_WriteText(e->base.x + e->base.w / 2, e->base.y+e->header1+e->header2/2,
			e->dir->name, e->header2/2-4,
			EXPLORER_FONT, SDLColor_Set(80, 80, 80),
			TEXT_BLENDED, TEXT_CENTER, e->dst);
		
	boxRGBA(e->dst, e->base.x, e->base.y + e->header1 + e->header2,
		e->base.x + e->base.w, e->base.y + e->header1 + e->header2,
		220, 220, 220, 255);
}

void Explorer_Header3(Explorer *e)
{
	Color color = SetColor(200, 200, 200);
	Color text = SetColor(40, 40, 40);
	boxRGBA(e->dst, e->base.x, e->base.y + e->base.h - e->header3, 
		e->base.x + e->base.w, e->base.y + e->base.h,
		220, 220, 220, 255);
	
	
	if(e->hover == -1)
	{
		color = SetColor(0, 150, 220);
		text = SetColor(230, 230, 230);
	}
	e->valid = SDL_SetLocalRect(e->base.x+e->base.w - 115, e->base.y+e->base.h-e->header3+8,
		100, (e->header3-16));
	
	roundedBoxRGBA(e->dst, e->base.x+e->base.w - 115, e->base.y+e->base.h-e->header3+8,
		e->base.x+e->base.w - 115 + 100, e->base.y+e->base.h-e->header3+8+(e->header3-16),
		9, color.r, color.g, color.b, 255);
		
	Moteur_WriteText(e->base.x+e->base.w - 115+50, 
		e->base.y+e->base.h-e->header3+8+(e->header3-16)/2,
		"Valider", 22, EXPLORER_FONT, SDLColor_Set(text.r, text.g, text.b), 
		TEXT_BLENDED, TEXT_CENTER, e->dst);
		
}

void Explorer_Body(Explorer *e)
{
	int size_block = 125;
	int base_y = e->base.y + e->header1 + e->header2 + 20;
	int base_x = e->base.x + 15;
	int x = base_x, y = base_y;
	int w = e->base.w - 30;
	int h = e->base.h - e->header1 - e->header2 - e->header3 - 40;
	int n_elem = w / (size_block + 30);
	int i;
	Color color;
	
	char buf[200] = "";
	int j;
	
	boxRGBA(e->dst, base_x-15, base_y-20, base_x+w+15, base_y+h+20, 255, 255, 255, 255);
	boxRGBA(e->dst, e->base.x-1, e->base.y-1, e->base.x+e->base.w+1, e->base.y+1, 0, 186, 255, 255);
	boxRGBA(e->dst, e->base.x-1, e->base.y, e->base.x+1, e->base.y+e->base.h, 0, 186, 255, 255);
	boxRGBA(e->dst, e->base.x-1, e->base.y+e->base.h, e->base.x+e->base.w+2, e->base.y+e->base.h+2, 0, 186, 255, 255);
	boxRGBA(e->dst, e->base.x+e->base.w, e->base.y, e->base.x+e->base.w+2, e->base.y+e->base.h, 0, 186, 255, 255);

	for(i = e->line * n_elem; i < e->dir->n; i++)
	{
		color = SetColor(220, 220, 220);
		if(e->dir->tab[i]->type == _DIR)
			color = SetColor(190, 190, 190);
		
		if(e->hover == i+1)
			color = SetColor(color.r, color.g, 255);
		if(e->select == i+1)
			color = SetColor(0, 160, 240);
		roundedBoxRGBA(e->dst, x, y, x+size_block, y+size_block-30, 8, color.r, color.g, color.b, 255);
		
		if(strlen(e->dir->tab[i]->name) > 28)
		{
			for(j = 0; j < 28; j++)
				buf[j] = e->dir->tab[i]->name[j];
			buf[j] = 0;
			sprintf(buf, "%s...", buf);
			Moteur_WriteParagraph(x+size_block/2+1, y+size_block-15, size_block-10,
					buf, 17, 0, 
					EXPLORER_FONT, SDLColor_Set(80, 80, 80),
					TEXT_BLENDED,TEXT_CENTER, 
					e->dst);
		}
		else
		{
			Moteur_WriteParagraph(x+size_block/2+1, y+size_block-15, size_block-10,
						e->dir->tab[i]->name, 18, 0, 
						EXPLORER_FONT, SDLColor_Set(80, 80, 80),
						TEXT_BLENDED,TEXT_CENTER, 
						e->dst);
		}
		
		e->pos[i] = SDL_SetLocalRect(x, y, size_block, size_block);
		
		x += size_block+30;
		
		if(x+size_block > base_x+w)
		{
			x = base_x;
			y += size_block+30;
			if(y+size_block+30 > base_y+h)
				break;
		}
	} 
}

int Explorer_PollEvent(Explorer *e, SDL_Event event)
{
	int n_elem = (e->base.w - 30) / (125 + 30);
	int x,y;
	int i;
	if((NULL == e))
		return 0;
	
	switch(event.type)
	{
		case SDL_MOUSEBUTTONDOWN:
			switch(event.button.button)
			{
				case SDL_BUTTON_WHEELDOWN:
					if(e->dir->n/n_elem - e->line > 4)
					{
						e->line++;
						return FORCE_MAJ;
					}
					return NONE;
		
				case SDL_BUTTON_WHEELUP:
					if(e->line > 0)
					{
						e->line--;
						return FORCE_MAJ;
					}
					return NONE;
			}
	}
	
	switch(event.type)
	{
		case SDL_QUIT:
			return QUIT;
		
		case SDL_KEYUP:
				return QUIT;
			break;
		
		case SDL_MOUSEBUTTONDOWN:
			x = event.button.x;
			y = event.button.y;
			
			if(x < e->base.x || x > e->base.x+e->base.w ||
			   y < e->base.y || y > e->base.y+e->base.h)
			{
				if(e->select != 0)
				{
					e->select = 0;
					e->last = event;
					return FORCE_MAJ;
				}
				return NONE;			
			}
			
			for(i = 0; i < e->dir->n; i++)
			{
				if(PixelInRect(x, y, e->pos[i]))
				{
					if(e->select != i+1)
					{
						e->select = i+1;
						e->last = event;
						return FORCE_MAJ;
					}
					if(e->dir->tab[i]->type == _DIR)
					{
						if(e->last.type == SDL_MOUSEBUTTONUP &&
							e->last.button.x == x && 
							e->last.button.y == y)
						{
							Explorer_ChangeDirectory(e, i);
							e->last = event;
							return FORCE_MAJ;
						}
					}
					e->last = event;
					return NONE;
				}
			}
			
			if(PixelInRect(x, y, e->dad))
			{
				Explorer_ChangeDirectory(e, -1);
				e->last = event;
				return FORCE_MAJ;
			}
			
			if(PixelInRect(x, y, e->valid))
			{
				if(e->select != -1)
				{
					if(e->select > 0)
					{
						if(e->dir->tab[e->select - 1]->type == _FILE)
							return GET_STR;
						else
							return FORCE_MAJ;
					}
					e->select = -1;
					e->last = event;
					return FORCE_MAJ;
				}
				e->last = event;
				return NONE;
			}
			
			if(e->select != 0)
			{
				e->select = 0;
				e->last = event;
				return FORCE_MAJ;
			}
			e->last = event;
			return NONE;
			break;
			
		case SDL_MOUSEMOTION:
			x = event.motion.x;
			y = event.motion.y;
			
			if(x < e->base.x || x > e->base.x+e->base.w ||
			   y < e->base.y || y > e->base.y+e->base.h)
			{
				if(e->hover != 0)
				{
					e->hover = 0;
					return FORCE_MAJ;
				}
				return NONE;			
			}
			
			for(i = 0; i < e->dir->n; i++)
			{
				if(PixelInRect(x, y, e->pos[i]))
				{
					if(e->hover != i+1)
					{
						e->hover = i+1;
						return FORCE_MAJ;
					}
					return NONE;
				}
			}
			
			if(PixelInRect(x, y, e->dad))
			{
				if(e->hover != -2)
				{
					e->hover = -2;
					return FORCE_MAJ;
				}
				return NONE;
			}
			
			if(PixelInRect(x, y, e->valid))
			{
				if(e->hover != -1)
				{
					e->hover = -1;
					return FORCE_MAJ;
				}
				return NONE;
			}
			
			if(e->hover != 0)
			{
				e->hover = 0;
				return FORCE_MAJ;
			}
			return NONE;
			break;
		
		default:
			e->last = event;
			break;
			
			
	}
	return NONE;
}

char *Explorer_FindPath(int x, int y, int w, int h, SDL_Surface *dest)
{
	int c;
	char *str = NULL;
	SDL_Event event;
	
	Explorer *e = Explorer_Create(x, y, w, h, 46, 38, 50, dest);
	
	Explorer_Header1(e);
	Explorer_Header2(e);
	Explorer_Header3(e);
	Explorer_Body(e);
	SDL_Flip(dest);
	
	c = 1;
	while(c)
	{
		SDL_WaitEvent(&event);
		switch(Explorer_PollEvent(e, event))
		{	
			case FORCE_MAJ:
				Explorer_Header1(e);
				Explorer_Header2(e);
				Explorer_Header3(e);
				Explorer_Body(e);
				SDL_Flip(dest);
				break;
			case GET_STR:
				str = Str_Concat(3, e->dir->name, "/", e->dir->tab[e->select-1]->name);
				c = 0;
				break;
			case QUIT:
				return NULL;
				
			case NONE:
				break;
		}
		
	}
	Explorer_Free(&e);
	return str;
}

