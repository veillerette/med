#include "../include/Menu.h"

Node_Array *NodeArray_Alloc(int max)
{
	Node_Array *temp = (Node_Array *)malloc(sizeof(Node_Array));
	memtest(temp);
	
	temp-> n = 0;
	temp->next = (Menu_Node **)calloc(max, sizeof(Menu_Node *));
	memtest(temp->next);
	
	temp->max = max;
	
	return temp;
}

int NodeArray_AddElem(Node_Array *na, Menu_Node *elem)
{
	if((na == NULL) || (elem == NULL))
		return 0;
	if(na->n == na->max)
	{
		na->next = (Menu_Node **)realloc(na->next, sizeof(Menu_Node *) * (na->max * 2));
		memtest(na->next);
		na->max *= 2;
	}
	
	na->next[na->n] = elem;
	na->n++;
	return 1;
}

int NodeArray_Add(Node_Array *na, const char *name, int need_select, Node_Type type, int (*f)(void))
{
	Menu_Node *elem = NULL;
	if((NULL == name))
		return 0;
	elem = MenuNode_Alloc(name, need_select, type, f);
	return NodeArray_AddElem(na, elem);
}

Menu_Node *MenuNode_Alloc(const char *name, int need_select, Node_Type type, int (*f)(void))
{
	Menu_Node *temp = (Menu_Node *)malloc(sizeof(Menu_Node));
	memtest(temp);
	
	temp->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
	memtest(temp->name);
	strcpy(temp->name, name);
	
	temp->type = type;
	temp->pos.x = -1;
	temp->pos.y = -1;
	temp->pos.h = 0;
	temp->pos.w = 0;
	switch(type)
	{
		case NODE:
			temp->next = NodeArray_Alloc(20);
			break;
		case LEAF:
			temp->need_select = need_select;
			temp->f = f;
			break;
		default:
			break;
	}
	return temp;
}

Menu *Menu_Alloc(void)
{
	Menu *temp = (Menu *)malloc(sizeof(Menu));
	memtest(temp);
	
	temp->lst = NodeArray_Alloc(10);
	temp->font = TTF_OpenFont(MENU_FONT, MENU_SIZETEXT);
	memtest(temp->font);
	
	temp->select = NULL;
	temp->hover = NULL;
	
	return temp;
}

void NodeArray_Free(Node_Array **lst)
{
	if(*lst != NULL)
	{
		int i;
		for(i = 0; i < (*lst)->n; i++)
			MenuNode_Free(&((*lst)->next[i]));
		free(*lst);
		*lst = NULL;
	}	
}

void MenuNode_Free(Menu_Node **node)
{
	if(*node != NULL)
	{
		if((*node)->name != NULL)
			free((*node)->name);
		if((*node)->type == NODE)
			NodeArray_Free(&((*node)->next));
		free(*node);
		*node = NULL;
	}
}

void Menu_Free(Menu **menu)
{
	if(*menu != NULL)
	{
		NodeArray_Free(&((*menu)->lst));
		TTF_CloseFont((*menu)->font);
		free(*menu);
		*menu = NULL;
	}
}


/************* FONCTIONS DU MENU *******************/


int menu_no_action(void)
{
	printf("menu_no_action()\n");
	return FORCE_MAJ;
}

int _Fichier_Quit(void)
{
	return QUIT;
}

int _Aide_APropos(void)
{
	SDL_Event event;
	int c;
	SDL_Color color = {200, 200, 200};
	char *text = (char *)malloc(sizeof(char) * 1000);
	boxRGBA(Window->screen, Window->width/2-500, Window->height/2-250, Window->width/2+500, Window->height/2+250,
										45, 56, 67, 255);
	sprintf(text, "%s", "A Propos");
	Moteur_WriteParagraph(Window->width/2, Window->height/2-200, 400, text, 37, 20, "media/Garamond-light.ttf", color,
				TEXT_BLENDED, TEXT_CENTER, Window->screen);
	
	sprintf(text, "%s", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Etiam lobortis justo et odio faucibus, quis consectetur nisi tincidunt. Nullam feugiat nulla ac semper fringilla. Aliquam accumsan tortor tristique posuere ultricies. Suspendisse tellus nibh, tincidunt in sollicitudin eu, fringilla ac turpis. Aliquam nunc velit, suscipit non viverra mattis, gravida in sapien. Maecenas dapibus libero leo, quis laoreet nisi condimentum et. Pellentesque odio erat, porta eu tincidunt et, faucibus ac risus. Mauris euismod sollicitudin magna non volutpat. Curabitur elementum consequat turpis quis tristique. Vestibulum blandit ligula ligula, venenatis varius lectus luctus ac. Maecenas bibendum ultrices laoreet. Praesent tristique eros lorem, vel mattis leo scelerisque id. Nunc vel finibus dui, id pharetra justo. ");								
	Moteur_WriteParagraph(Window->width/2, Window->height/2-135, 800, text, 19, 10, "media/Garamond-light.ttf", color,
				TEXT_BLENDED, TEXT_CENTER, Window->screen);
	sprintf(text, "%s", "Cliquez pour continuer...");		
	Moteur_WriteParagraph(Window->width/2, Window->height/2+120, 400, text, 19, 20, "media/Garamond.ttf", color,
				TEXT_BLENDED, TEXT_CENTER, Window->screen);
	SDL_Flip(Window->screen);
	c = 1;
	while(c)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				return QUIT;
				c = 0;
			case SDL_MOUSEBUTTONDOWN:
				c = 0;
				while(SDL_PollEvent(&event)); /* flush event queue */
				break;
			default:
				break;
		}
	}
	
	return FORCE_MAJ;
}

int _Selection_Delete(void)
{
	if(NULL == main_events->select)
		return FORCE_MAJ;
	switch(main_events->select->type)
	{
		case OBJECT_NOTE:
			Step_ChangeRestStatus(main_events->select->step, main_events->select->id_note, 1);	
			return FORCE_SCOREMAJ;
		case OBJECT_STEP:
			Staff_DeleteStep(main_events->select->staff, main_events->select->id_step);
			return FORCE_SCOREMAJ;
		default:
			break;
	}
	return FORCE_MAJ;
}

int _Selection_Deselec(void)
{
	if(main_events->select != NULL)
	{
		main_events->select = NULL;
		return FORCE_SCOREMAJ;
	}
	return FORCE_MAJ;
}

int _Selection_Divise(void)
{
	if(NULL == main_events->select)
		return FORCE_MAJ;
	Step_DiviseRest(main_events->select->step, main_events->select->id_note);
	return FORCE_SCOREMAJ;
}

int _Selection_Regularise(void)
{
	if(NULL == main_events->select || main_events->select->type != OBJECT_STEP)
		return FORCE_MAJ;
	Step_Regularise(*(main_events->select->staff->steps + main_events->select->id_step));
	return FORCE_SCOREMAJ;
}

int _Ajouter_Mesure_Before(void)
{
	if(NULL == main_events->select)
		return FORCE_MAJ;
	if(main_events->select->type != OBJECT_STEP)
		return FORCE_MAJ;
	Staff_InsereEmptyStep(main_events->select->staff, main_events->select->id_step);
	return FORCE_SCOREMAJ;
}

int _Ajouter_Mesure_After(void)
{
	if(NULL == main_events->select)
		return FORCE_MAJ;
	if(main_events->select->type != OBJECT_STEP)
		return FORCE_MAJ;
	Staff_InsereEmptyStep(main_events->select->staff, main_events->select->id_step+1);
	return FORCE_SCOREMAJ;
}

Menu *Menu_Create(void)
{
	Menu *menu = NULL;
	menu = Menu_Alloc();
	NodeArray_Add(menu->lst, "Fichier", 0, NODE, NULL);
	NodeArray_Add(menu->lst->next[0]->next, "Nouveau", 0, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[0]->next, "Ouvrir", 0, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[0]->next, "Quitter", 0, LEAF, _Fichier_Quit);
	NodeArray_Add(menu->lst, "Sélection", 0, NODE, NULL);
	NodeArray_Add(menu->lst->next[1]->next, "Supprimer", 1, LEAF, _Selection_Delete);
	NodeArray_Add(menu->lst->next[1]->next, "Désélectionner", 1, LEAF, _Selection_Deselec);
	NodeArray_Add(menu->lst->next[1]->next, "Diviser", 1, LEAF, _Selection_Divise);
	NodeArray_Add(menu->lst->next[1]->next, "Régulariser", 1, LEAF, _Selection_Regularise);
	NodeArray_Add(menu->lst, "Ajouter", 0, NODE, NULL);
	NodeArray_Add(menu->lst->next[2]->next, "Mesure", 0, NODE, menu_no_action);
	NodeArray_Add(menu->lst->next[2]->next->next[0]->next, "Avant la sélection", 1, LEAF, _Ajouter_Mesure_Before);
	NodeArray_Add(menu->lst->next[2]->next->next[0]->next, "Après la sélection", 1, LEAF, _Ajouter_Mesure_After);
	NodeArray_Add(menu->lst->next[2]->next, "Note", 0, NODE, menu_no_action);
	NodeArray_Add(menu->lst->next[2]->next->next[1]->next, "Ronde", 1, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[2]->next->next[1]->next, "Blanche", 1, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[2]->next->next[1]->next, "Noire", 1, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[2]->next->next[1]->next, "Croche", 1, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[2]->next->next[1]->next, "Double-Croche", 1, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[2]->next->next[1]->next, "Triple-Croche", 1, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[2]->next->next[1]->next, "Quadruple-Croche", 1, LEAF, menu_no_action);
	NodeArray_Add(menu->lst, "Aide", 0, NODE, NULL);
	NodeArray_Add(menu->lst->next[3]->next, "Manuel", 0, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[3]->next, "Aide en ligne", 0, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[3]->next, "A propos", 0, LEAF, _Aide_APropos);
	NodeArray_Add(menu->lst->next[3]->next, "Autres", 0, NODE, menu_no_action);
	NodeArray_Add(menu->lst->next[3]->next->next[3]->next, "Menu avec des arbres", 1, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[3]->next->next[3]->next, "D'autres fantaisies", 1, NODE, menu_no_action);
	NodeArray_Add(menu->lst->next[3]->next->next[3]->next->next[1]->next, "Et d'autres trucs fou", 1, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[3]->next->next[3]->next->next[1]->next, "Wouaa !", 1, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[3]->next->next[3]->next->next[1]->next, "C'est vraiment trop bien", 1, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[3]->next->next[3]->next->next[1]->next, "Ne fait rien", 1, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[3]->next->next[3]->next->next[1]->next, "Voilà", 1, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[3]->next->next[3]->next, "Des choses", 1, LEAF, menu_no_action);
	NodeArray_Add(menu->lst->next[3]->next->next[3]->next, "V. Veillerette", 1, LEAF, menu_no_action);

	return menu;
}

void Menu_AffBaseOne(TTF_Font *font, Menu_Node *node, int *x, int *y, int status, int dir, int max)
{
	SDL_Surface *surf = NULL, *final = NULL;
	SDL_Color color = {200, 200, 200};
	SDL_Color background = {34, 45, 56};
	SDL_Rect pos;
	int width;
	if(NULL == node)
		return;
	if(status == 1)
	{
		background.r = 56;
		background.g = 67;
		background.b = 78;
	}
	if(status == 2)
	{
		color = background;
		background.r = 0;
		background.g = 186;
		background.b = 255;
	}
	
	 
	surf = TTF_RenderUTF8_Blended(font, node->name, color);
	if(max == 0)
		width = surf->w + 14;
	else
		width = max + 25;
	final = SDL_CreateRGBSurface(SDL_HWSURFACE, width, surf->h + 2, 32, 0, 0, 0, 0);
	SDL_FillRect(final, NULL, SDL_MapRGB(final->format, background.r, background.g, background.b));
	pos.x = 7;
	pos.y = 1;
	SDL_BlitSurface(surf, NULL, final, &pos);
	
	if(max != 0)
	{
		if(node->type == NODE)
			filledTrigonRGBA(final, final->w-10, final->h/2-2, final->w-7,
					final->h/2, final->w-10, final->h/2+2,
					color.r, color.g, color.b, 255);
	}
	
	pos.x = *x;
	pos.y = *y;
	SDL_BlitSurface(final, NULL, Window->screen, &pos);
	node->pos.x = *x;
	node->pos.y = *y;
	node->pos.h = final->h;
	node->pos.w = final->w;
	if(dir == 0)
		*x += final->w;
	else
		*y += final->h;
	SDL_FreeSurface(surf);
	SDL_FreeSurface(final);
	return;
}

int MyChildrenComp(Menu_Node *goal, Menu_Node *base)
{
	int i,r = 0;
	if(base == goal)
		return 1;
	if(base->type == LEAF)
		return base == goal;
	for(i = 0; i < base->next->n; i++)
		r |= MyChildrenComp(goal, base->next->next[i]);
	return r;
}

void Menu_AffExtendOne(TTF_Font *font, Menu *menu, Menu_Node *node, int *x, int *y)
{
	int max_width = 0;
	int i;
	int temp;
	SDL_Rect pos;
	int status = 0;
	int sauv;
	if((NULL == font) || (NULL == node) || (NULL == x) || (NULL == y))
		return;
	
	for(i = 0; i < node->next->n; i++)
	{
		temp = strlen(node->next->next[i]->name);
		TTF_SizeUTF8(font, node->next->next[i]->name, &temp, NULL);
		if(temp > max_width)
			max_width = temp;
	}
	pos.x = *x;
	pos.y = *y;
	for(i = 0; i < node->next->n; i++)
	{
		status = 0;
		if(MyChildrenComp(menu->select,node->next->next[i]))
			status = 2;
		else if(menu->hover == node->next->next[i])
			status = 1;
		sauv = pos.y;
		Menu_AffBaseOne(font, node->next->next[i], (int *)&pos.x, (int *)&pos.y, status, 1, max_width);
		if(status == 2 && node->next->next[i]->type != LEAF)
		{
			*x += max_width + 25;
			*y = sauv;
			Menu_AffExtendOne(font, menu, node->next->next[i], x, y);
		}
	}
	return;
}

void Menu_Aff(Menu *menu, int *x, int *y)
{
	int i;
	int tmp_x, tmp_y;
	int status = 0;
	int height;
	int sauv_x, sauv_y;
	if((NULL == menu) || (NULL == x) || (NULL == y))
		return;
	sauv_x = *x;
	sauv_y = *y;
	TTF_SizeUTF8(menu->font, "Bonjour", NULL, &height);
	boxRGBA(Window->screen, 0, 0, Window->width, height+2, 34, 45, 56, 255);
	boxRGBA(Window->screen, 0, height+2, Window->width, height+2, 120, 130, 140, 255);
						
	for(i = 0; i < menu->lst->n; i++)
	{
		status = 0;
		tmp_x = *x;
		tmp_y = *y;
		if(MyChildrenComp(menu->select, menu->lst->next[i]))
			status = 2;
		else if(menu->hover == menu->lst->next[i])
			status = 1;
		/*printf("base(%s), status=%d  %d\n", menu->lst->next[i]->name, status, MyChildrenComp(menu->select,menu->lst->next[i]));*/
		Menu_AffBaseOne(menu->font, menu->lst->next[i], x, y, status, status==2, 0);
		if(status == 2)
			Menu_AffExtendOne(menu->font, menu, menu->lst->next[i], x, y);
		*y = tmp_y;
		*x = tmp_x += menu->lst->next[i]->pos.w;
	}
	*x = sauv_x;
	*y = sauv_y;
	printf("- - - - - - - - - - - -\n");
}

void Menu_Console(Node_Array *lst, int tab)
{
	if(lst != NULL)
	{
		int i,j;
		for(i = 0; i < lst->n; i++)
		{
			for(j = 0; j < tab; j++)
				putchar('\t');
			printf("%s\n", lst->next[i]->name);
			if(lst->next[i]->type == NODE)
				Menu_Console(lst->next[i]->next, tab+1);
		}
	}
}

Menu_Node *FindExpandZone(Menu *menu, Menu_Node *mn, int clic_x, int clic_y)
{
	int i;
	Menu_Node *res = NULL;
	SDL_Rect pos;
	if((NULL == menu) || (NULL == mn))
		return NULL;
	for(i = 0; i < mn->next->n; i++)
	{
		pos = mn->next->next[i]->pos;
		if(pos.x <= clic_x && clic_x <= pos.x+pos.w &&
			pos.y <= clic_y && clic_y <= pos.y+pos.h)
			return mn->next->next[i];
		printf("test %s==%s (%d)\n", mn->next->next[i]->name, menu->select->name, MyChildrenComp(menu->select,mn->next->next[i]));
		if(MyChildrenComp(menu->select,mn->next->next[i]) && mn->next->next[i]->type == NODE)
		{
			printf("recherche profond %s\n", mn->next->next[i]->name);
			res = FindExpandZone(menu, mn->next->next[i], clic_x, clic_y);
			if(res != NULL)
				return res;
		}
	}
	return NULL;
}

Menu_Node *FindNodeByZone(Menu *menu, int clic_x, int clic_y)
{
	int i;
	Menu_Node *res = NULL;
	SDL_Rect pos;
	if(NULL == menu)
		return NULL;
	for(i = 0; i < menu->lst->n; i++)
	{
		pos = menu->lst->next[i]->pos;
		if(pos.x <= clic_x && clic_x <= pos.x+pos.w &&
			pos.y <= clic_y && clic_y <= pos.y+pos.h)
			return menu->lst->next[i];
		if(MyChildrenComp(menu->select,menu->lst->next[i]))
		{
			res = FindExpandZone(menu, menu->lst->next[i], clic_x, clic_y);
			if(res != NULL)
				return res;
		}
	}
	return NULL;
}

int Menu_PollMouse(Menu *menu, SDL_Event event)
{
	Menu_Node *mn = NULL;
	
	switch(event.type)
	{
		case SDL_QUIT:
			return QUIT;
		case SDL_MOUSEMOTION:
			mn = FindNodeByZone(menu, event.motion.x, event.motion.y);
			if(menu->hover == mn)
				return NONE;
			menu->hover = mn;
			return FORCE_MAJ;
		case SDL_MOUSEBUTTONDOWN:
			mn = FindNodeByZone(menu, event.button.x, event.button.y);
			if(mn != NULL && mn->type == LEAF)
			{
				menu->select = NULL;
				return mn->f();
			}
			if(menu->select == mn)
				return NONE;
			menu->select = mn;
			if(mn != NULL)	
				printf("new select = %s\n", mn->name);
			else
				printf("new select = %p\n", (void *)mn);
			return FORCE_MAJ;
		default:
			break;
	}
	return NONE;
}
