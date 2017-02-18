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
	int maj = 0;
	Select_Node *sn = NULL;
	if(Select_isEmpty() || main_events->mode != MODE_EDIT)
		return FORCE_MAJ;
	
	sn = Select_GetIterate();
	while(sn != NULL)
	{
		switch(sn->val->type)
		{
			case OBJECT_NOTE:
				Step_ChangeRestStatus(sn->val->step,
						sn->val->id_note, 1);
				maj = 1;
				break;
			case OBJECT_STEP:
				Score_DeleteStep(main_events->score, sn->val->id_step);
				maj = 1;
				break;
			default:
				break;
		}
		sn = sn->next;
	}
	
	if(maj)
		return FORCE_SCOREMAJ;
		
	return FORCE_MAJ;
}

int _Selection_Deselec(void)
{
	Select_Flush();
	return FORCE_MAJ;
}

int _Selection_Divise(void)
{
	int maj = 0;
	Select_Node *sn = NULL;
	
	if(Select_isEmpty())
		return FORCE_MAJ;
	
	sn = Select_GetIterate();
	
	while(sn != NULL)
	{
		switch(sn->val->type)
		{
			case OBJECT_NOTE:
				if(Step_Divise(sn->val->step,
						sn->val->id_note))
					maj = 1;
				break;
			default:
				break;
		}
		sn = sn->next;
	}
	
	if(maj)
		return FORCE_SCOREMAJ;
	
	return FORCE_MAJ;
}

int _Selection_Regularise(void)
{
	int maj = 0;
	Select_Node *sn = NULL;
	
	if(Select_isEmpty())
		return FORCE_MAJ;
		
	sn = Select_GetIterate();
	
	while(sn != NULL)
	{
		switch(sn->val->type)
		{
			case OBJECT_STEP:
				Step_Regularise(*(sn->val->staff->steps + sn->val->id_step));
				maj = 1;
				break;
			default:
				break;
		}
		sn = sn->next;
	}
	if(maj)
		return FORCE_SCOREMAJ;
	return FORCE_MAJ;
}

int _Ajouter_Mesure_Before(void)
{
	Select_Node *sn = NULL;	
	if(Select_isEmpty())
		return FORCE_MAJ;
	
	if(main_events->tabselect->last != (sn = Select_GetIterate()) 
	  || sn->val->type != OBJECT_STEP)
	{
		Window_InteractInfo("Veuillez ne sélectionner qu'une seule mesure", 255, 100, 50);
		return FORCE_MAJ;
	}

	Score_SetEmptyStep(main_events->score, sn->val->id_step);
	return FORCE_SCOREMAJ;
}

int _Ajouter_Mesure_After(void)
{
	Select_Node *sn = NULL;	
	if(Select_isEmpty())
		return FORCE_MAJ;
	
	if(main_events->tabselect->last != (sn = Select_GetIterate()) 
	  || sn->val->type != OBJECT_STEP)
	{
		Window_InteractInfo("Veuillez ne sélectionner qu'une seule mesure", 255, 100, 50);
		return FORCE_MAJ;
	}

	Score_SetEmptyStep(main_events->score, sn->val->id_step+1);
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


void ButtonRect(int x, int y, SDL_Rect rect, int *valid, int *refresh)
{
	if(PixelInRect(x, y, rect))
	{
		if(!(*valid))
		{
			*valid = 1;
			*refresh = 1;
		}
	}
	else
	{
		if(*valid)
		{
			*valid = 0;
			*refresh = 1;
		}
	}
}

int Menu_ConfigAudio(void)
{
	int header1 = 30;
	int r = 50, g = 50, b = 255;
	int h = Window->height, w = Window->width;
	SDL_Rect box = SDL_SetLocalRect(w/2-((w*0.55)/2), h/2-(h/4), w*0.55, h*0.5);
	SDL_Rect valid = SDL_SetLocalRect(box.x+box.w/2-50, box.y+box.h-50, 100, 40);
	
	int c = 1;
	int x,y;
	SDL_Color text = {90, 90, 90, 0};
	int isValid=0;
	int refresh = 1;
	SDL_Event event;
	
	while(c)
	{
		if(refresh)
		{
			
			Window_InteractBackground(box, r, g, b, header1, isValid, valid);
			
				
			Moteur_WriteText(box.x + box.w/2, box.y+header1/2, "Configuration Audio", header1-5,
					FONT_INTERFACE, text,
					TEXT_BLENDED, TEXT_CENTER,
					Window->screen);

			SDL_Flip(Window->screen);
			refresh = 0;
		}
		SDL_PollEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				c = 0;
				return QUIT;
				break;
					
			case SDL_MOUSEMOTION:
				x = event.motion.x;
				y = event.motion.y;
				
				ButtonRect(x, y, valid, &isValid, &refresh);
				
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				x = event.button.x;
				y = event.button.y;

				if(PixelInRect(x, y, valid))
				{
					return FORCE_MAJ;
				}
				break;
				
			case SDL_KEYUP:
				return 0;
		}
		SDL_Delay(2);
	}
	return 0;
}

int Menu_AddEmptyStep(void)
{
	int header1 = 30;
	int r =  30, g = 200, b = 255;
	int h = Window->height, w = Window->width;
	SDL_Rect box = SDL_SetLocalRect(w/2-200, h/2-100, 400, 200);
	SDL_Rect valid = SDL_SetLocalRect(box.x+box.w/2-50, box.y+box.h-50, 100, 40);
	SDL_Rect how = SDL_SetLocalRect(box.x+box.w/2-37, box.y+box.h/2-20, 74, 40);
	
	int c = 1;
	int x,y;
	SDL_Color text = {90, 90, 90, 0};
	int isValid=0;
	int refresh = 1;
	SDL_Event event;
	int i;
	int amount = 1;
	int isHow = 0;
	int rounded = 5;
	char buf[8] = "";
	
	if((NULL == main_events) || (NULL == main_events->score))
		return FORCE_CLEAR;
	
	while(c)
	{
		if(refresh)
		{
			
			Window_InteractBackground(box, r, g, b, header1, isValid, valid);
			
			sprintf(buf, "%d", amount);
			if(isHow)
			{
				roundedBoxRGBA(Window->screen, how.x, how.y, 
					how.x+how.w, how.y+how.h, rounded, 180, 220, 255, 255);
				Moteur_WriteText(how.x+how.w/2, how.y+how.h/2, buf, 40,
					FONT_INTERFACE_LIGHT, text,
					TEXT_BLENDED, TEXT_CENTER,
					Window->screen);
			}
			else
			{
				roundedBoxRGBA(Window->screen, how.x, how.y, 
					how.x+how.w, how.y+how.h, rounded, 240, 240, 240, 255);
				Moteur_WriteText(how.x+how.w/2, how.y+how.h/2, buf, 40,
					FONT_INTERFACE_LIGHT, text,
					TEXT_BLENDED, TEXT_CENTER,
					Window->screen);
			}
				
			Moteur_WriteText(box.x + box.w/2, box.y+header1/2, "Ajouts de mesures", header1-5,
					FONT_INTERFACE, text,
					TEXT_BLENDED, TEXT_CENTER,
					Window->screen);

			SDL_Flip(Window->screen);
			refresh = 0;
		}
		SDL_PollEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				c = 0;
				return QUIT;
				break;
					
			case SDL_MOUSEMOTION:
				x = event.motion.x;
				y = event.motion.y;
				
				ButtonRect(x, y, valid, &isValid, &refresh);
				ButtonRect(x, y, how, &isHow, &refresh);
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				x = event.button.x;
				y = event.button.y;

				if(PixelInRect(x, y, valid))
				{
					for(i = 0; i < amount; i++)
					{
						Score_AddEmptyStep(main_events->score);
					}
					return FORCE_SCOREMAJ;
				}
				
				if(PixelInRect(x, y, how))
				{
					switch(event.button.button)
					{
						case SDL_BUTTON_WHEELDOWN:
							if(amount > 1)
							{
								amount--;
								refresh = 1;
							}
							break;
						case SDL_BUTTON_WHEELUP:
							if(amount < 100)
							{
								amount++;
								refresh = 1;
							}
							break;
					}
				}
				break;
				
			case SDL_KEYUP:
				return 0;
		}
		SDL_Delay(2);
	}
	return 0;
}


int Menu_ChooseNew(int *new_vox, Cle **new_cles, int *new_num, int *new_den)
{
	Cle cles[10] = {0};
	int isCle[10] = {0};
	SDL_Rect pos[10];
	int nbr_vox = 2;
	int num = 4, den = 4;
	int header1 = 30;
	int r = 50, g = 50, b = 255;
	int h = Window->height, w = Window->width;
	SDL_Rect box = SDL_SetLocalRect(w/2-((w*0.55)/2), h/2-(h/4), w*0.55, h*0.5);
	SDL_Rect valid = SDL_SetLocalRect(box.x+box.w/2-50, box.y+box.h-50, 100, 40);
	SDL_Rect vox = SDL_SetLocalRect(box.x + 300, box.y+header1+50, 70, 40); 
	SDL_Rect step1 = SDL_SetLocalRect(box.x+200, box.y+box.h-150, 70, 40);
	SDL_Rect step2 = SDL_SetLocalRect(box.x+350, step1.y, step1.w, step1.h);
	int rounded = 5;
	int c = 1;
	int x,y;
	SDL_Color text = {90, 90, 90, 0};
	int isValid=0;
	int refresh = 1;
	int isVox = 0;
	int isStep1 = 0, isStep2 = 0;
	char buf[10];
	int i;
	SDL_Rect cle_base;
	SDL_Event event;
	
	
	SDL_Flip(Window->screen);
	
	for(i = 0; i < 10; i++)
		cles[i] = CLE_SOL;
	
	cles[1] = CLE_FA;
	
	while(c)
	{
		if(refresh)
		{
			
			Window_InteractBackground(box, r, g, b, header1, isValid, valid);
			
			sprintf(buf, "%d", nbr_vox);
			if(isVox)
			{
				roundedBoxRGBA(Window->screen, vox.x, vox.y, 
						vox.x+vox.w, vox.y+vox.h, rounded, 180, 220, 255, 255);
				Moteur_WriteText(vox.x+vox.w/2, vox.y+vox.h/2, buf, 40,
					FONT_INTERFACE_LIGHT, text,
					TEXT_BLENDED, TEXT_CENTER,
					Window->screen);
			}
			else
			{
				roundedBoxRGBA(Window->screen, vox.x, vox.y, 
						vox.x+vox.w, vox.y+vox.h, rounded, 240, 240, 240, 255);
				Moteur_WriteText(vox.x+vox.w/2, vox.y+vox.h/2, buf, 40,
					FONT_INTERFACE_LIGHT, text,
					TEXT_BLENDED, TEXT_CENTER,
					Window->screen);
			}
			
			Moteur_WriteText((step1.x+step2.x)/2+step1.w/2, step1.y+step1.h/2, "/", 60,
					FONT_INTERFACE_LIGHT, text,
					TEXT_BLENDED, TEXT_CENTER,
					Window->screen);
			
			sprintf(buf, "%d", num);
			if(isStep1)
			{
				roundedBoxRGBA(Window->screen, step1.x, step1.y, 
						step1.x+step1.w, step1.y+step1.h, rounded, 180, 220, 255, 255);
				Moteur_WriteText(step1.x+step1.w/2, step1.y+step1.h/2, buf, 40,
					FONT_INTERFACE_LIGHT, text,
					TEXT_BLENDED, TEXT_CENTER,
					Window->screen);
			}
			else
			{
				roundedBoxRGBA(Window->screen, step1.x, step1.y, 
						step1.x+step1.w, step1.y+step1.h, rounded, 240, 240, 240, 255);
				Moteur_WriteText(step1.x+step1.w/2, step1.y+step1.h/2, buf, 40,
					FONT_INTERFACE_LIGHT, text,
					TEXT_BLENDED, TEXT_CENTER,
					Window->screen);
			}
			sprintf(buf, "%d", den);
			if(isStep2)
			{
				roundedBoxRGBA(Window->screen, step2.x, step2.y, 
						step2.x+step2.w, step2.y+step2.h, rounded, 180, 220, 255, 255);
				Moteur_WriteText(step2.x+step2.w/2, step2.y+step2.h/2, buf, 40,
					FONT_INTERFACE_LIGHT, text,
					TEXT_BLENDED, TEXT_CENTER,
					Window->screen);
			}
			else
			{
				roundedBoxRGBA(Window->screen, step2.x, step2.y, 
						step2.x+step2.w, step2.y+step2.h, rounded, 240, 240, 240, 255);
				Moteur_WriteText(step2.x+step2.w/2, step2.y+step2.h/2, buf, 40,
					FONT_INTERFACE_LIGHT, text,
					TEXT_BLENDED, TEXT_CENTER,
					Window->screen);
			}
			
			cle_base.x = box.x+120;
			cle_base.y = box.y+header1+150;
			for(i = 0; i < nbr_vox; i++)
			{
				if(isCle[i])
				{
					roundedBoxRGBA(Window->screen, cle_base.x, cle_base.y, 
						cle_base.x+70, cle_base.y+40, rounded, 180, 220, 255, 255);	
				}
				else
				{
					roundedBoxRGBA(Window->screen, cle_base.x, cle_base.y, 
						cle_base.x+70, cle_base.y+40, rounded, 240, 240, 240, 255);	
				}
				
				pos[i].x = cle_base.x;
				pos[i].y = cle_base.y;
				pos[i].w = 70;
				pos[i].h = 40;
				
				if(cles[i] == CLE_SOL)
					sprintf(buf, "SOL");
				else
					sprintf(buf, "FA");
				Moteur_WriteText(cle_base.x+35, cle_base.y+20, buf, 25,
					FONT_INTERFACE_LIGHT, text,
					TEXT_BLENDED, TEXT_CENTER,
					Window->screen);
				cle_base.x += 90;
				if(cle_base.x+70 > box.x+box.w-80 || (i==4))
				{
					cle_base.x = box.x+120;
					cle_base.y += 60;
				}
				
			}
			
				
			Moteur_WriteText(box.x + box.w/2, box.y+header1/2, "Nouvelle partition", header1-5,
					FONT_INTERFACE, text,
					TEXT_BLENDED, TEXT_CENTER,
					Window->screen);

			Moteur_WriteText(box.x + 20, box.y+header1+50, "Nombre de Voix : ", 30,
					FONT_INTERFACE, text,
					TEXT_BLENDED, TEXT_LEFT,
					Window->screen);
				
			Moteur_WriteText(box.x + 20, box.y+header1+130, "Clés : ", 30,
					FONT_INTERFACE, text,
					TEXT_BLENDED, TEXT_LEFT,
					Window->screen);
			
			Moteur_WriteText(box.x + 20, step1.y, "Mesure : ", 30,
					FONT_INTERFACE, text,
					TEXT_BLENDED, TEXT_LEFT,
					Window->screen);

			SDL_Flip(Window->screen);
			refresh = 0;
		}
		SDL_PollEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				c = 0;
				return QUIT;
				break;
					
			case SDL_MOUSEMOTION:
				x = event.motion.x;
				y = event.motion.y;
				

				ButtonRect(x, y, valid, &isValid, &refresh);
				ButtonRect(x, y, vox, &isVox, &refresh);
				ButtonRect(x, y, step1, &isStep1, &refresh);
				ButtonRect(x, y, step2, &isStep2, &refresh);
				
				for(i = 0; i < nbr_vox; i++)
				{
					if(PixelInRect(x, y, pos[i]))
					{
						if(!isCle[i])
						{
							isCle[i] = 1;
							refresh = 1;
						}
					}
					else
					{
						if(isCle[i])
						{
							isCle[i] = 0;
							refresh = 1;
						}
					}
				}
				
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				x = event.button.x;
				y = event.button.y;
				if(PixelInRect(x, y, vox))
				{
					switch(event.button.button)
					{
						case SDL_BUTTON_WHEELDOWN:
							if(nbr_vox != 1)
							{
								nbr_vox--;
								refresh = 1;
							}
							break;
						case SDL_BUTTON_WHEELUP:
							if(nbr_vox != 10)
							{
								nbr_vox++;
								refresh = 1;
							}
					}
				}
				if(PixelInRect(x, y, step1))
				{
					switch(event.button.button)
					{
						case SDL_BUTTON_WHEELDOWN:
							if(num != 2)
							{
								num--;
								refresh = 1;
							}
							break;
						case SDL_BUTTON_WHEELUP:
							if(num != 5)
							{
								num++;
								refresh = 1;
							}
					}
				}
				if(PixelInRect(x, y, step2))
				{
					switch(event.button.button)
					{
						case SDL_BUTTON_WHEELDOWN:
							if(den != 2)
							{
								den/=2;
								refresh = 1;
							}
							break;
						case SDL_BUTTON_WHEELUP:
							if(den != 8)
							{
								den *= 2;
								refresh = 1;
							}
					}
				}
				for(i = 0; i < nbr_vox; i++)
				{
					if(PixelInRect(x, y, pos[i]))
					{
						switch(event.button.button)
						{
							case SDL_BUTTON_WHEELDOWN:
							case SDL_BUTTON_WHEELUP:
								cles[i] = 1 + (2 - cles[i]);
								refresh = 1;
								break;
						}
					}
				}
				if(PixelInRect(x, y, valid))
				{
					*new_num = num;
					*new_den = den;
					*new_vox = nbr_vox;
					
					*new_cles = (Cle *)malloc(sizeof(Cle) * nbr_vox);
					memtest(*new_cles);
					
					for(i = 0; i < nbr_vox; i++)
						(*new_cles)[i] = cles[i];
					
					return 1;
				}
				break;
				
			case SDL_KEYUP:
				return 0;
		}
		SDL_Delay(2);
	}
	return 0;
}

int Menu_New(void)
{
	Score *new_score = NULL;
	int i,j;
	
	
	int num, den, nbr_vox;
	Cle *cles;
	
	if((NULL == main_events) || (NULL == main_events->score))
		return 0;
	
	
	switch(Menu_ChooseNew(&nbr_vox, &cles, &num, &den))
	{
		case QUIT:
			return QUIT;
		case 0:
			return FORCE_MAJ;
		default: 
			break;
	}
	
	new_score = Score_Alloc();
	
	if(NULL == new_score)
		return 0;
	
	Score_Init(new_score);
	Staff_Init(new_score->lst[0], num, den, cles[0], 0);
	Staff_ChangeArmure(new_score->lst[0], 0, 0);
	for(i = 0; i < 50; i++)
			Score_AddEmptyStep(new_score);
	
	for(j = 1; j < nbr_vox; j++)
	{
		Score_AddEmpty(new_score);
		Staff_ChangeCle(new_score->lst[j], 0, cles[j]);
	}
	
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
			Audio_Pause();
			Score_Free(&(main_events->score));
			main_events->score = new_score;
			
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

int ChangeCleToSol(void)
{
	Select_Node *sn = NULL;	
	if(Select_isEmpty())
		return FORCE_MAJ;
	
	if(main_events->tabselect->last != (sn = Select_GetIterate()) 
	  || sn->val->type != OBJECT_STEP)
	{
		Window_InteractInfo("Veuillez sélectionner une seule mesure", 255, 100, 50);
	}
	
	Staff_ChangeCle(sn->val->staff, 0, CLE_SOL);
	
	return FORCE_SCOREMAJ;
}

int ChangeCleToFa(void)
{
	Select_Node *sn = NULL;	
	if(Select_isEmpty())
		return FORCE_MAJ;
	
	if(main_events->tabselect->last != (sn = Select_GetIterate()) 
	  || sn->val->type != OBJECT_STEP)
	{
		Window_InteractInfo("Veuillez sélectionner une seule mesure", 255, 100, 50);
	}
	
	Staff_ChangeCle(sn->val->staff, 0, CLE_FA);
	
	return FORCE_SCOREMAJ;
}

int ChangeTonality(signed char new)
{
	int i,j;
	Select_Node *sn = NULL;	
	if(Select_isEmpty())
		return FORCE_MAJ;
	
	if(main_events->tabselect->last != (sn = Select_GetIterate()) 
	  || sn->val->type != OBJECT_STEP)
	{
		Window_InteractInfo("Veuillez sélectionner une seule mesure", 255, 100, 50);
	}
		
	for(i = 0; i < main_events->score->n; i++)
	{
		Staff_ChangeArmure(main_events->score->lst[i], sn->val->id_step, new);
		for(j = sn->val->id_step; j< main_events->score->lst[i]->n; j++)
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
	NodeArray_Add(menu->lst->next[0]->next, "Configuration Audio", 0, LEAF, Menu_ConfigAudio);
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
	NodeArray_Add(menu->lst->next[1]->next, "Clés", 0, NODE, NULL);
	NodeArray_Add(menu->lst->next[1]->next->next[5]->next, "Clé de Sol", 1, LEAF, ChangeCleToSol);
	NodeArray_Add(menu->lst->next[1]->next->next[5]->next, "Clé de Fa", 1, LEAF, ChangeCleToFa);
	NodeArray_Add(menu->lst, "Ajouter", 0, NODE, NULL);
	NodeArray_Add(menu->lst->next[2]->next, "Mesure", 0, NODE, menu_no_action);
	NodeArray_Add(menu->lst->next[2]->next->next[0]->next, "Avant la sélection", 1, LEAF, _Ajouter_Mesure_Before);
	NodeArray_Add(menu->lst->next[2]->next->next[0]->next, "Après la sélection", 1, LEAF, _Ajouter_Mesure_After);
	NodeArray_Add(menu->lst->next[2]->next->next[0]->next, "A la fin", 0, LEAF, Menu_AddEmptyStep);
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
	return Window_InteractInfo("Veuillez sélectionner une mesure", 255, 100, 50);
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
				if(!mn->need_select || (mn->need_select && !Select_isEmpty()))
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
	int playingX = 750+45;
	int volumeX = 986;
	Select_Node *sn = NULL;
	
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			x = event.motion.x;
			y = event.motion.y;
			
			if(x >= 866+45 && x <= 906+45 && y >= 39 && y <= 69)
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
			
			if(x >= volumeX && x <= volumeX+15 && y >= dy-20 && y <= dy+20)
			{
				if(!main_events->tools.hover_volume)
				{
					main_events->tools.hover_volume = 1;
					return FORCE_MAJ;
				}
			}
			else
			{
				if(main_events->tools.hover_volume)
				{
					main_events->tools.hover_volume = 0;
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
			
			printf("A\n");
			if(x >= 866+45 && x <= 906+45 && y >= 39 && y <= 69)
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
			printf("B\n");
			if(x >= volumeX && x <= volumeX+15 && y >= dy-20 && y <= dy+20)
			{
				switch(event.button.button)
				{
					case SDL_BUTTON_WHEELDOWN:
						Audio_SetVolume(Audio_GetVolume()-1000);
						return FORCE_MAJ;
					case SDL_BUTTON_WHEELUP:
						Audio_SetVolume(Audio_GetVolume()+1000);
						return FORCE_MAJ;
				}
			}
			printf("C\n");
			if(y<dy-20 || y > dy+20)
				return NONE;
			printf("D\n");
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
				printf("E\n");
			for(i = 0; i < 7; i++)
			{
				if(x >= 60+i*45 && x <= 100+i*45 &&
					y >= dy-20 && y <= dy+20)
				{
					main_events->tools.duration = pow(2, i);
					if(main_events->mode == MODE_EDIT && !Select_isEmpty()
					   && Select_isOne())
					{
						sn = Select_GetIterate();
						if(sn->val->type == OBJECT_NOTE)
						{
							Note *temp = Step_GetNote(sn->val->step, sn->val->id_note);
							rest = temp->rest;
							
							Step_AddNote(sn->val->step,
									sn->val->id_note,
									temp->note,
									temp->flags,
									main_events->tools.duration);
							Step_ChangeRestStatus(sn->val->step,
										sn->val->id_note,
										rest);
							return FORCE_SCOREMAJ;
						}
					}
					return FORCE_MAJ;
				}
			}
			printf("F\n");
			for(i = 0; i < 5; i++)
			{
				if(x >= 410+i*45 && x <= 450+i*45 && 
					y >= dy-20 && y <= dy+20)
				{
					in = 1;
					break;
				}
			}
			printf("G\n");
			for(i = 0; i < 5; i++)
			{
				if(x >= 410+i*45 && x <= 450+i*45 && 
					y >= dy-20 && y <= dy+20)
				{
					goal_alt = i;
				}
				else
					goal_alt = -1;
				none = 0;
				printf("goal_alt = %d\n", goal_alt);
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
						case 4:
							none = InvertBoolean(&(main_events->tools.natural));
							break;
					}
										
					if(main_events->mode == MODE_EDIT && !Select_isEmpty()
					   && Select_isOne())
					{
						sn = Select_GetIterate();
						if(sn->val->type == OBJECT_NOTE)
						{
							Note *temp = Step_GetNote(sn->val->step, sn->val->id_note);
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
									case 4:
										temp->flags |= NOTE_NATURAL;
										break;
								}
							}
							
							#ifdef DEBUG
							printf("Add note real time flags = %X\n", temp->flags);
							#endif
							
							printf("Step_AddNote flags=%x\n", temp->flags);
							
							Step_AddNote(sn->val->step,
									sn->val->id_note,
									temp->note,
									temp->flags,
									main_events->tools.duration);
							Step_ChangeRestStatus(sn->val->step,
										sn->val->id_note,
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
						case 4:
							main_events->tools.natural = 0;
					}
				}
			}
			pointed = 0;
			sauv = main_events->tools.statusdur;
			for(i = 0; i < 2; i++)
			{
				if(x >= 625+45+i*45 && x <= 665+45+i*45 && 
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
				if(main_events->mode == MODE_EDIT && !Select_isEmpty()
				      && Select_isOne())
				{
					sn = Select_GetIterate();
					Note *temp = Step_GetNote(sn->val->step, sn->val->id_note);
					
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
					Step_AddNote(sn->val->step,
							sn->val->id_note,
							temp->note,
							temp->flags,
							main_events->tools.duration);
					Step_ChangeRestStatus(sn->val->step,
								sn->val->id_note,
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
	BlitCenter(LittleImages->DoubleSharp, NULL, Window->screen, &pos);
	
	pos.x += 45;
	
	if(main_events->tools.doubleflat)
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 75, 85, 95, 255);
	else
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 90, 100, 110, 255);
	BlitCenter(LittleImages->DoubleFlat, NULL, Window->screen, &pos);

	pos.x += 45;
	
	if(main_events->tools.natural)
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 75, 85, 95, 255);
	else
		roundedBoxRGBA(Window->screen, pos.x-20, y-20, pos.x+20, y+20, 3, 90, 100, 110, 255);
	BlitCenter(LittleImages->Natural, NULL, Window->screen, &pos);

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
	pos.y += 3;
	pos.x += 35;
			
	pos.x += 40;
	pos.y -= 23;
	roundedBoxRGBA(Window->screen, pos.x, pos.y, pos.x+1, pos.y+46, 7, 90, 100, 110, 255);
	pos.x += 20;
	
	if(main_events->tools.hover_volume)
	{
		boxRGBA(Window->screen, pos.x, pos.y, pos.x+15, pos.y+45, 80, 90, 115, 255);
		boxRGBA(Window->screen, pos.x, pos.y+45-(45*Audio_GetFracVolume()), 
					pos.x+15, pos.y+45, 115, 135, 150, 255);
	}
	else
	{
		boxRGBA(Window->screen, pos.x, pos.y, pos.x+15, pos.y+45, 119, 135, 153, 255);
		boxRGBA(Window->screen, pos.x, pos.y+45-(45*Audio_GetFracVolume()), 
					pos.x+15, pos.y+45, 135, 155, 170, 255);
	}
	
	
	
}




