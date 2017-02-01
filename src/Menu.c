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
	
	temp->height = 0;
	
	temp->mode.x = 0;
	temp->mode.y = 0;
	temp->mode.h = 0;
	temp->mode.w = 0;
	
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
	SDL_Color color = {200, 200, 200, 0};
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
			Score_DeleteStep(main_events->score, main_events->select->id_step);
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
	if(main_events->select->type != OBJECT_NOTE)
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
	Score_SetEmptyStep(main_events->score, main_events->select->id_step);
	return FORCE_SCOREMAJ;
}

int _Ajouter_Mesure_After(void)
{
	if(NULL == main_events->select)
		return FORCE_MAJ;
	if(main_events->select->type != OBJECT_STEP)
		return FORCE_MAJ;
	Score_SetEmptyStep(main_events->score, main_events->select->id_step+1);
	return FORCE_SCOREMAJ;
}

int _Ajouter_Mesure_End(void)
{
	if((NULL == main_events) || (NULL == main_events->score))
		return FORCE_MAJ;
	Score_AddEmptyStep(main_events->score);
	return FORCE_SCOREMAJ;
}

int _Add_New_Staff(void)
{
	if((NULL == main_events) || (NULL == main_events->score))
		return FORCE_MAJ;
	switch(Score_AddEmpty(main_events->score))
	{
		case -1:
			Window_InteractInfo("Nombre maximal de portées atteint", 255, 100, 50);
			break;
		case 1:
			Audio_AssignateScore(main_events->score);
			break;
		default:
			break;
	}
	return FORCE_SCOREMAJ;
}

int Menu_New(void)
{
	Score *new_score = NULL;
	int i;
	
	if((NULL == main_events) || (NULL == main_events->score))
		return 0;
	
	new_score = Score_Alloc();
	
	if(NULL == new_score)
		return 0;
	
	Score_Init(new_score);
	Staff_Init(new_score->lst[0], 4, NOIRE, CLE_SOL, 0);
	Staff_ChangeArmure(new_score->lst[0], 0, 0);
	
	for(i = 0; i < 10; i++)
			Score_AddEmptyStep(new_score);
	Score_AddEmpty(new_score);
	
	
	Audio_Pause();
	
	
	Score_Free(&(main_events->score));
	main_events->score = new_score;
	
	Audio_AssignateScore(main_events->score);
	Audio_GoToStep(0);
	
	return FORCE_SCOREMAJ;
}

int Menu_OpenFile(void)
{
	char *path = NULL;
	char buf[100] = "";
	Score *new_score = NULL;
	
	if((NULL == main_events) || (NULL == main_events->score))
		return FORCE_MAJ;
	
	path = Explorer_FindPath(300, 200, Window->width-600, Window->height-400, Window->screen);
	
	if(path != NULL)
	{
		if(File_isExt(path, ".abc"))
			new_score = ABC_OpenABC(path);
		else if(File_isExt(path, ".med"))
			File_OpenScore(path, &(new_score));
		if(NULL == new_score)
		{
			sprintf(buf, "%s", abc_error);
			Window_InteractInfo(buf, 255, 100, 50);
			return FORCE_MAJ;
		}
		else
		{
		
			printf("a\n");
			Score_Free(&(main_events->score));
			main_events->score = new_score;
			printf("b\n");
		
			Audio_AssignateScore(main_events->score);
			Audio_GoToStep(0);
		}
}
	return FORCE_SCOREMAJ;
}

int Menu_SauvFileMED(void)
{
	char *path = NULL;
	if((NULL == main_events) || (NULL == main_events->score))
		return FORCE_MAJ;
	
	path = Explorer_FindPath(200, 200, Window->width-400, Window->height-400, Window->screen);
	
	if(path != NULL)
	{
		switch(File_SaveScore(path, main_events->score))
		{
			case 1:
				Window_InteractInfo("Fichier MED sauvegardé avec succès", 
								50, 240, 50);
				break;
			default:
				Window_InteractInfo("Erreur lors de l'exportation", 
								255, 100, 50);
				break;
		}
	}
	return FORCE_MAJ;
}

int Menu_SauvFileABC(void)
{
	char *path = NULL;
	if((NULL == main_events) || (NULL == main_events->score))
		return FORCE_MAJ;
	
	path = Explorer_FindPath(200, 200, Window->width-400, Window->height-400, Window->screen);
	
	if(path != NULL)
	{
		switch(ABC_WriteScore(path, main_events->score))
		{
			case 1:
				Window_InteractInfo("Fichier ABC sauvegardé avec succès", 
								50, 240, 50);
				break;
			default:
				Window_InteractInfo("Erreur lors de l'exportation", 
								255, 100, 50);
				break;
		}
	}
	return FORCE_MAJ;
}

int ChangeTonality(signed char new)
{
	int i,j;
	if((NULL == main_events) || (NULL == main_events->score))
		return 0;
	if((NULL == main_events->select) || (main_events->select->type != OBJECT_STEP))
		return 0;
	for(i = 0; i < main_events->score->n; i++)
	{
		Staff_ChangeArmure(main_events->score->lst[i], main_events->select->id_step, new);
		for(j = main_events->select->id_step; j< main_events->score->lst[i]->n; j++)
			Step_VerifArmFlags(main_events->score->lst[i]->steps[j]);
	}
	return FORCE_SCOREMAJ;
}

int __0Arm(void){ return ChangeTonality(0); }
int __1Arm(void){ return ChangeTonality(1); }
int __2Arm(void){ return ChangeTonality(2); }
int __3Arm(void){ return ChangeTonality(3); }
int __4Arm(void){ return ChangeTonality(4); }
int __5Arm(void){ return ChangeTonality(5); }
int __6Arm(void){ return ChangeTonality(6); }
int __7Arm(void){ return ChangeTonality(7); }
int ___1Arm(void){ return ChangeTonality(-1); }
int ___2Arm(void){ return ChangeTonality(-2); }
int ___3Arm(void){ return ChangeTonality(-3); }
int ___4Arm(void){ return ChangeTonality(-4); }
int ___5Arm(void){ return ChangeTonality(-5); }
int ___6Arm(void){ return ChangeTonality(-6); }
int ___7Arm(void){ return ChangeTonality(-7); }

Menu *Menu_Create(void)
{
	Menu *menu = NULL;
	menu = Menu_Alloc();
	NodeArray_Add(menu->lst, "Fichier", 0, NODE, NULL);
	NodeArray_Add(menu->lst->next[0]->next, "Nouveau", 0, LEAF, Menu_New);
	NodeArray_Add(menu->lst->next[0]->next, "Ouvrir", 0, LEAF, Menu_OpenFile);
	NodeArray_Add(menu->lst->next[0]->next, "Export en MED", 0, LEAF, Menu_SauvFileMED);
	NodeArray_Add(menu->lst->next[0]->next, "Export en ABC", 0, LEAF, Menu_SauvFileABC);
	NodeArray_Add(menu->lst->next[0]->next, "Quitter", 0, LEAF, _Fichier_Quit);
	NodeArray_Add(menu->lst, "Sélection", 0, NODE, NULL);
	NodeArray_Add(menu->lst->next[1]->next, "Supprimer", 1, LEAF, _Selection_Delete);
	NodeArray_Add(menu->lst->next[1]->next, "Désélectionner", 1, LEAF, _Selection_Deselec);
	NodeArray_Add(menu->lst->next[1]->next, "Diviser", 1, LEAF, _Selection_Divise);
	NodeArray_Add(menu->lst->next[1]->next, "Régulariser", 1, LEAF, _Selection_Regularise);
	NodeArray_Add(menu->lst->next[1]->next, "Armures", 0, NODE, NULL);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next, "DoM / Lam", 1, LEAF, __0Arm);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next, "Dièses", 0, NODE, NULL);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next, "Bémols", 0, NODE, NULL);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next->next[1]->next, "Sol Majeur / Mi mineur", 1, LEAF, __1Arm);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next->next[1]->next, "Ré Majeur / Si mineur", 1, LEAF, __2Arm);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next->next[1]->next, "La Majeur / Fa# mineur", 1, LEAF, __3Arm);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next->next[1]->next, "Mi Majeur / Do# mineur", 1, LEAF, __4Arm);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next->next[1]->next, "Si Majeur / Sol# mineur", 1, LEAF, __5Arm);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next->next[1]->next, "Fa# Majeur / Ré# mineur", 1, LEAF, __6Arm);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next->next[1]->next, "Do# Majeur / La# mineur", 1, LEAF, __7Arm);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next->next[2]->next, "Fa Majeur / Ré mineur", 1, LEAF, ___1Arm);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next->next[2]->next, "Sib Majeur / Sol mineur", 1, LEAF, ___2Arm);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next->next[2]->next, "Mib Majeur / Do mineur", 1, LEAF, ___3Arm);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next->next[2]->next, "Lab Majeur / Fa majeur", 1, LEAF, ___4Arm);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next->next[2]->next, "Réb Majeur / Sib mineur", 1, LEAF, ___5Arm);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next->next[2]->next, "Solb Majeur / Mib mineur", 1, LEAF, ___6Arm);
	NodeArray_Add(menu->lst->next[1]->next->next[4]->next->next[2]->next, "Dob Majeur / Lab mineur", 1, LEAF, ___7Arm);
	NodeArray_Add(menu->lst, "Ajouter", 0, NODE, NULL);
	NodeArray_Add(menu->lst->next[2]->next, "Mesure", 0, NODE, menu_no_action);
	NodeArray_Add(menu->lst->next[2]->next->next[0]->next, "Avant la sélection", 1, LEAF, _Ajouter_Mesure_Before);
	NodeArray_Add(menu->lst->next[2]->next->next[0]->next, "Après la sélection", 1, LEAF, _Ajouter_Mesure_After);
	NodeArray_Add(menu->lst->next[2]->next->next[0]->next, "A la fin", 0, LEAF, _Ajouter_Mesure_End);
	NodeArray_Add(menu->lst->next[2]->next, "Portée", 0, LEAF, _Add_New_Staff);

	return menu;
}

void Menu_AffBaseOne(TTF_Font *font, Menu_Node *node, int *x, int *y, int status, int dir, int max)
{
	SDL_Surface *surf = NULL, *final = NULL;
	SDL_Color color = {200, 200, 200, 0};
	SDL_Color background = {34, 45, 56, 0};
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
	menu->height = height+2;
	boxRGBA(Window->screen, 0, 0, Window->width, height+2, 34, 45, 56, 255);
	boxRGBA(Window->screen, 0, height+2, Window->width, height+2, 120, 130, 140, 255);
	
	Toolbar_PrintMode(menu);
	Toolbar_PrintNote(menu);
						
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
		if(MyChildrenComp(menu->select,mn->next->next[i]) && mn->next->next[i]->type == NODE)
		{
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
int Menu_NoSelect(void)
{
	return Window_InteractInfo("Aucune sélection active", 255, 100, 50);
}
/*
int Menu_NoSelect(void)
{
	SDL_Color color = {255, 255, 255, 0};
	SDL_Event event;
	int c;
	int complete = 0;
	boxRGBA(Window->screen, Window->width/2-100, Window->height/2-50, Window->width/2+100, Window->height/2+50,
										45, 56, 67, 255);
	Moteur_WriteText(Window->width/2, Window->height/2-35, "Aucune sélection active", 22, "media/Garamond-light.ttf", color, TEXT_BLENDED, TEXT_CENTER, Window->screen);

	Moteur_WriteText(Window->width/2, Window->height/2+20, "Cliquez pour continuer ...", 19, "media/Garamond-light.ttf", color, TEXT_BLENDED, TEXT_CENTER, Window->screen);
	SDL_Flip(Window->screen);
	c = 1;
	
	while(c)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				return FORCE_SCOREMAJ;
			case SDL_MOUSEBUTTONDOWN:
				complete = 1;
				break;
			case SDL_MOUSEBUTTONUP:
				if(complete)
					return FORCE_SCOREMAJ;
				else
					break;
			default:
				break;
		}
	}
	return FORCE_SCOREMAJ;
}*/

int Menu_PollMouse(Menu *menu, SDL_Event event)
{
	Menu_Node *mn = NULL;
	int sauv;
	int x = 0, y = 0;
	
	switch(event.type)
	{
		case SDL_QUIT:
			return QUIT;
		case SDL_MOUSEMOTION:
			mn = FindNodeByZone(menu, event.motion.x, event.motion.y);
			
			if(mn == NULL && ((sauv = ToolBar_PollMouse(menu, event)) != NONE))
				return sauv;
			
			if(menu->hover == mn)
				return NONE;
			menu->hover = mn;
			return FORCE_MAJ;
		case SDL_MOUSEBUTTONDOWN:
			mn = FindNodeByZone(menu, event.button.x, event.button.y);
			
			if(NULL == mn)
			{
				if(PixelInRect(event.button.x, event.button.y, menu->mode))
				{
					main_events->mode = 1-main_events->mode;
					return FORCE_MAJ;
				}
			}	
			
			if(mn == NULL && ((sauv = ToolBar_PollMouse(menu, event)) != NONE))
				return sauv;
				
			if(mn != NULL && mn->type == LEAF)
			{
				menu->select = NULL;
				Window_DrawBody();
				Window_Print();
				Menu_Aff(menu, &x, &y);
				SDL_Flip(Window->screen);
				if(!mn->need_select || (mn->need_select && main_events->select != NULL))
					return mn->f();
				else
					return Menu_NoSelect();
			}
			if(menu->select == mn)
				return NONE;
			menu->select = mn;
			return FORCE_MAJ;
		default:
			break;
	}
	return NONE;
}

void Toolbar_PrintMode(Menu *menu)
{
	char text[2] = {0, 0};
	SDL_Color color = {200, 220, 240, 0};
	int y = (Window->pos_menu->h - menu->height)/2 + menu->height;
	if(main_events->mode == MODE_ADD)
		text[0] = 'A';
	else
		text[0] = 'E';
	Moteur_WriteText(25, y, text, 40, 
				MENU_FONT, color, 
				TEXT_BLENDED, TEXT_CENTER, 
				Window->screen);
	roundedRectangleRGBA(Window->screen, 5, y-20, 45, y+20, 5, color.r, color.g, color.b, 255);
	roundedRectangleRGBA(Window->screen, 6, y-19, 44, y+19, 5, 200, 200, 200, 255);
	roundedRectangleRGBA(Window->screen, 7, y-18, 43, y+18, 5, 200, 200, 200, 255);
	menu->mode.x = 5;
	menu->mode.y = y-20;
	menu->mode.w = 40;
	menu->mode.h = 40;
}

static void BlitCenter(SDL_Surface *src, SDL_Rect *src_rect, SDL_Surface *dest, SDL_Rect *dest_rect)
{
	dest_rect->x -= src->w/2;
	dest_rect->y -= src->h/2;
	SDL_BlitSurface(src, src_rect, dest, dest_rect);
	dest_rect->y += src->h/2;
	dest_rect->x += src->w/2;
}

int InvertBoolean(char *boolean)
{
	*boolean = 1 - *boolean;
	return *boolean;
}
int ToolBar_PollMouse(Menu *menu, SDL_Event event)
{
	int i,x,y;
	int dy = (Window->pos_menu->h - menu->height)/2 + menu->height;
	int goal_alt = 0;
	int none=0;
	int in = 0;
	int rest;
	int sauv = 0;
	int pointed = 0;
	int playingX = 750;
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			x = event.motion.x;
			y = event.motion.y;
			
			if(x >= 866 && x <= 906 && y >= 39 && y <= 69)
			{
				if(!main_events->tools.hover_tempo)
				{
					main_events->tools.hover_tempo = 1;
					return FORCE_MAJ;
				}	
			}
			else
			{
				if(main_events->tools.hover_tempo)
				{
					main_events->tools.hover_tempo = 0;
					return FORCE_MAJ;
				}
			}
			
			if(x >= playingX && x <= playingX+40 && y >= dy-20 && y <= dy+20)
			{
				if(!main_events->tools.hover_button1)
				{
					main_events->tools.hover_button1 = 1;
					return FORCE_MAJ;
				}
			}
			else if(main_events->tools.hover_button1)
			{
				main_events->tools.hover_button1 = 0;
				return FORCE_MAJ;
			}
			
			break;
		case SDL_MOUSEBUTTONDOWN:
			x = event.button.x;
			y = event.button.y;
			
			if(x >= 866 && x <= 906 && y >= 39 && y <= 69)
			{
				switch(event.button.button)
				{
					case SDL_BUTTON_WHEELDOWN:
						Audio_SetTempo(Audio_GetTempo()-1);
						return FORCE_MAJ;
					case SDL_BUTTON_WHEELUP:
						Audio_SetTempo(Audio_GetTempo()+1);
						return FORCE_MAJ;
				}
			}
			
			if(y<dy-20 || y > dy+20)
				return NONE;
			
			if(x >= playingX && x <= playingX+40 && y >= dy-20 && y <= dy+20)
			{
				if(Audio_isPlaying())
				{
					Audio_Pause();
				}
				else
				{
					Audio_Play();
				}
				return FORCE_MAJ;
			}
				
			for(i = 0; i < 7; i++)
			{
				if(x >= 60+i*45 && x <= 100+i*45 &&
					y >= dy-20 && y <= dy+20)
				{
					main_events->tools.duration = pow(2, i);
					if(main_events->mode == MODE_EDIT && main_events->select != NULL)
					{
						if(main_events->select->type == OBJECT_NOTE)
						{
							Note *temp = Step_GetNote(main_events->select->step, main_events->select->id_note);
							rest = temp->rest;
							Step_AddNote(main_events->select->step,
									main_events->select->id_note,
									temp->note,
									temp->flags,
									main_events->tools.duration);
							Step_ChangeRestStatus(main_events->select->step,
										main_events->select->id_note,
										rest);
							return FORCE_SCOREMAJ;
						}
					}
					return FORCE_MAJ;
				}
			}
			for(i = 0; i < 4; i++)
			{
				if(x >= 410+i*45 && x <= 450+i*45 && 
					y >= dy-20 && y <= dy+20)
				{
					in = 1;
					break;
				}
			}
			for(i = 0; i < 4; i++)
			{
				if(x >= 410+i*45 && x <= 450+i*45 && 
					y >= dy-20 && y <= dy+20)
				{
					goal_alt = i;
				}
				else
					goal_alt = -1;
				none = 0;
				if(goal_alt >= 0)
				{
					switch(goal_alt)
					{
						case 0:
							none = InvertBoolean(&(main_events->tools.sharp));
							break;
						case 1:
							none = InvertBoolean(&(main_events->tools.flat));
							break;
						case 2:
							none = InvertBoolean(&(main_events->tools.doublesharp));
							break;
						case 3:
							none = InvertBoolean(&(main_events->tools.doubleflat));
							break;
					}
					if(main_events->mode == MODE_EDIT && main_events->select != NULL)
					{
						if(main_events->select->type == OBJECT_NOTE)
						{
							Note *temp = Step_GetNote(main_events->select->step, main_events->select->id_note);
							rest = temp->rest;
							temp->flags = NOTE_DEFAULT;
							if(none)
							{
								switch(goal_alt)
								{
									case 0:
										temp->flags |= NOTE_SHARP;
										break;
									case 1:
										temp->flags |= NOTE_FLAT;
										break;
									case 2:
										temp->flags |= NOTE_DOUBLESHARP;
										break;
									case 3:
										temp->flags |= NOTE_DOUBLEFLAT;
										break;
								}
							}
							Step_AddNote(main_events->select->step,
									main_events->select->id_note,
									temp->note,
									temp->flags,
									main_events->tools.duration);
							Step_ChangeRestStatus(main_events->select->step,
										main_events->select->id_note,
										rest);
							return FORCE_SCOREMAJ;
						}
					}
				}
				else if(in)
				{
					switch(i)
					{
						case 0:
							main_events->tools.sharp = 0;
							break;
						case 1:
							main_events->tools.flat = 0;
							break;
						case 2:
							main_events->tools.doublesharp = 0;
							break;
						case 3:
							main_events->tools.doubleflat = 0;
							break;
					}
				}
			}
			pointed = 0;
			sauv = main_events->tools.statusdur;
			for(i = 0; i < 2; i++)
			{
				if(x >= 625+i*45 && x <= 665+i*45 && 
					y >= dy-20 && y <= dy+20)
				{
					if(main_events->tools.statusdur == 0)
						main_events->tools.statusdur = i+1;
					else
						main_events->tools.statusdur = 0;
					pointed = 1;
				}
			}
			if(!pointed)
				main_events->tools.statusdur = 0;
				
			if(sauv != main_events->tools.statusdur)
			{
				if(main_events->mode == MODE_EDIT && main_events->select != NULL)
				{
					Note *temp = Step_GetNote(main_events->select->step, main_events->select->id_note);
								rest = temp->rest;
					temp->flags = NOTE_DEFAULT;
					if(main_events->tools.sharp)
						temp->flags |= NOTE_SHARP;
					else if(main_events->tools.flat)
						temp->flags |= NOTE_FLAT;
					else if(main_events->tools.doublesharp)
						temp->flags |= NOTE_DOUBLESHARP;
					else if(main_events->tools.doubleflat)
						temp->flags |= NOTE_DOUBLEFLAT;
					switch(main_events->tools.statusdur)
					{
						case 1:
							temp->flags |= NOTE_POINTED;
							break;
						case 2:
							temp->flags |= NOTE_DOUBLEPOINTED;
							break;
					}
					Step_AddNote(main_events->select->step,
							main_events->select->id_note,
							temp->note,
							temp->flags,
							main_events->tools.duration);
					Step_ChangeRestStatus(main_events->select->step,
								main_events->select->id_note,
								rest);
					return FORCE_SCOREMAJ;
				}
				return FORCE_MAJ;
			}
			
			if(in)
				return FORCE_MAJ;
			break;
	}
	return NONE;
}

void Toolbar_PrintNote(Menu *menu)
{
	int i;
	SDL_Rect pos;
	int n, t;
	int y = (Window->pos_menu->h - menu->height)/2 + menu->height;
	char buf[5] = "";
	SDL_Color text = {70, 90, 110, 255};
	SDL_Color back = {105, 125, 145, 255};
	SDL_Color temp;
	
	pos.y = y;
	for(i = 0; i < 7; i++)
	{
		pos.x = 80+i*45;
		if(i == log(main_events->tools.duration)/log(2))
			roundedBoxRGBA(Window->screen, 60+i*45, y-20, 100+i*45, y+20, 3, 75, 85, 95, 255);
		else
			roundedBoxRGBA(Window->screen, 60+i*45, y-20, 100+i*45, y+20, 3, 90, 100, 110, 255);
		pos.y -= 2;
		switch(i)
		{
			case 0:
				pos.y += 2;
				BlitCenter(LittleImages->Note_headWhole, NULL, Window->screen, &pos);
				pos.y -= 2;
				break;
			case 1:
				pos.y += 12;
				BlitCenter(LittleImages->Note_headWhite, NULL, Window->screen, &pos);
				pos.y -= 12;
				BlitCenter(LittleImages->Note_Black, NULL, Window->screen, &pos);
				break;
			case 2:
				pos.y += 12;
				BlitCenter(LittleImages->Note_headBlack, NULL, Window->screen, &pos);
				pos.y -= 12;
				BlitCenter(LittleImages->Note_Black, NULL, Window->screen, &pos);
				break;
			case 3:
			case 4:
			case 5:
			case 6:
				n = i;
				t = 0;
				pos.x -= 5;
				pos.y += 12;
				BlitCenter(LittleImages->Note_headBlack, NULL, Window->screen, &pos);
				pos.y -= 12;
				BlitCenter(LittleImages->Note_Black, NULL, Window->screen, &pos);
				while(n >= 3)
				{
					pos.x += 17;
					pos.y -= 5;
					BlitCenter(LittleImages->Note_Crotchet, NULL, Window->screen, &pos);
					pos.x -= 17;
					pos.y += 5;
					pos.y += 5;
					t += 5;
					n--;
				}
				pos.y -= t;
				pos.x += 5;
				break;
				
		}
		pos.y += 2;
	}
	pos.x += 40;
	pos.y -= 23;
	roundedBoxRGBA(Window->screen, pos.x, pos.y, pos.x+1, pos.y+46, 7, 90, 100, 110, 255);
	pos.x += 40;
	pos.y += 23;
	
	if(main_events->tools.sharp)
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 75, 85, 95, 255);
	else
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 90, 100, 110, 255);
	BlitCenter(LittleImages->Sharp, NULL, Window->screen, &pos);
	
	pos.x += 45;
	
	if(main_events->tools.flat)
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 75, 85, 95, 255);
	else
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 90, 100, 110, 255);
	BlitCenter(LittleImages->Flat, NULL, Window->screen, &pos);
	
	pos.x += 45;
	
	if(main_events->tools.doublesharp) 
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 75, 85, 95, 255);
	else
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 90, 100, 110, 255);
	BlitCenter(LittleImages->Sharp, NULL, Window->screen, &pos); /* Must be DoubleSharp !! */
	
	pos.x += 45;
	
	if(main_events->tools.doubleflat)
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 75, 85, 95, 255);
	else
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 90, 100, 110, 255);
	BlitCenter(LittleImages->DoubleFlat, NULL, Window->screen, &pos);


	pos.x += 40;
	pos.y -= 23;
	roundedBoxRGBA(Window->screen, pos.x, pos.y, pos.x+1, pos.y+46, 7, 90, 100, 110, 255);
	pos.x += 40;
	pos.y += 23;
	
	if(main_events->tools.statusdur == 1)
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 75, 85, 95, 255);
	else
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 90, 100, 110, 255);
	filledCircleRGBA(Window->screen, pos.x, pos.y, 4, 119, 136, 153, 255);
	
	pos.x += 45;
	
	
	if(main_events->tools.statusdur == 2)
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 75, 85, 95, 255);
	else
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 90, 100, 110, 255);
	filledCircleRGBA(Window->screen, pos.x-6, pos.y, 4, 119, 136, 153, 255);
	filledCircleRGBA(Window->screen, pos.x+6, pos.y, 4, 119, 136, 153, 255);
	
	
	pos.x += 40;
	pos.y -= 23;
	roundedBoxRGBA(Window->screen, pos.x, pos.y, pos.x+1, pos.y+46, 7, 90, 100, 110, 255);
	pos.x += 20;
	pos.y += 23;

	
	if(!main_events->tools.hover_button1) 
		roundedBoxRGBA(Window->screen, pos.x, pos.y-20, pos.x+40, pos.y+20, 3, 105, 60, 50, 255);
	else
		roundedBoxRGBA(Window->screen, pos.x, pos.y-20, pos.x+40, pos.y+20, 3, 150, 60, 50, 255);
	
	pos.y = pos.y-20;
	
	if(!Audio_isPlaying())
		filledTrigonRGBA(Window->screen, pos.x+13, pos.y+7, pos.x+33, pos.y+20, pos.x+13, pos.y+33, 200, 200, 200, 255);
	else
	{
		boxRGBA(Window->screen, pos.x+10, pos.y+7, pos.x+10+4, pos.y+33, 200, 200, 200, 255);
		boxRGBA(Window->screen, pos.x+30-4, pos.y+7, pos.x+30, pos.y+33, 200, 200, 200, 255);
	}
	
	pos.x += 20;
	
	pos.y += 20;
	pos.x += 40;
	pos.y -= 23;
	roundedBoxRGBA(Window->screen, pos.x, pos.y, pos.x+1, pos.y+46, 7, 90, 100, 110, 255);
	pos.x += 20;
	pos.y += 20;
	
	pos.y += 12;
	BlitCenter(LittleImages->Note_headBlack, NULL, Window->screen, &pos);
	pos.y -= 12;
	BlitCenter(LittleImages->Note_Black, NULL, Window->screen, &pos);
	
	pos.x += 16;
	boxRGBA(Window->screen, pos.x, pos.y+3, pos.x+10, pos.y+4, 119, 135, 153, 255);
	boxRGBA(Window->screen, pos.x, pos.y+8, pos.x+10, pos.y+9, 119, 135, 153, 255);
	
	sprintf(buf, "%d", Audio_GetTempo());
	
	if(main_events->tools.hover_tempo)
	{
		temp = text;
		text = back;
		back = temp;
	}
	
	roundedBoxRGBA(Window->screen, pos.x+20, pos.y-10, pos.x+60, pos.y+20, 5, back.r, back.g, back.b, 255);
	
	Moteur_WriteText(pos.x+41, pos.y+5, buf, 28, 
				MENU_FONT, text, 
				TEXT_BLENDED, TEXT_CENTER, 
				Window->screen);
	
}




