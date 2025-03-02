#include "../include/Events.h"

EventData *main_events = NULL;

Area *Area_Alloc(SDL_Rect rect, int nbody, Object_Type type)
{
	Area *temp = (Area *)malloc(sizeof(Area));
	memtest(temp);
	
	temp->rect = rect;
	temp->type = type;
	
	temp->step = NULL;
	temp->id_note = 0;
	temp->nbody = nbody;
	
	temp->next = NULL;
	
	return temp;
}

Area *Area_Set(SDL_Rect rect, int nbody, Object_Type type, ...)
{
	Area *area = NULL;
	va_list va;
	va_start(va, type);
	
	area = Area_Alloc(rect, nbody, type);
	
	switch(type)
	{
		case OBJECT_NOTE:
			area->step = va_arg(va, Step *);
			area->id_note = va_arg(va, int);
			break;			
		case OBJECT_LINE:
		case OBJECT_CLE:
		case OBJECT_STEP:
			area->staff = va_arg(va, Staff*);
			area->id_step = va_arg(va, int);
			area->id_staff = va_arg(va, int);
			break;
		case EVENT_ADDNOTE:
			area->staff = va_arg(va, Staff*);
			area->id_step = va_arg(va, int);
			area->id_note_add = va_arg(va, int);
			break;
		case OBJECT_SIGN:
			area->sign = va_arg(va, Sign **);
			break;
		default:
			return NULL;
	}
	
	return area;
}

void Area_Free(Area **area)
{
	if(*area != NULL)
	{	
		free(*area);
		*area = NULL;
	}
}

void Area_Console(Area *area)
{
	if(NULL == area)
		return;

	printf("Area, \t(%d, %d)  %d<=>  %d/|\\ type=%X next=%p\n", 
			area->rect.x, area->rect.y, area->rect.w, area->rect.h,
			(unsigned int)area->type, (void *)area->next);
	switch(area->type)
	{
		case OBJECT_NOTE:
			printf("\tStep=%p, id_note=%d\n", (void *)area->step, area->id_note);
			break;
		case OBJECT_LINE:
		case OBJECT_CLE:
		case OBJECT_STEP:
			printf("\tStaff=%p, id_step=%d\n", (void *)area->staff, area->id_step);
			break;
		case EVENT_ADDNOTE:
			printf("\tStaff=%p, id_step=%d, id_note=%d\n", (void *)area->staff, area->id_step, area->id_note_add);
			break;
		case OBJECT_SIGN:
			printf("\tSign=%p\n", *(area->sign));
			break;
		default:
			colorprintf(RED, "Type not valid\n");
	}
	
}

Toolbar InitToolbar(void)
{
	Toolbar tools;
	tools.duration = NOIRE;
	tools.sharp = 0;
	tools.doublesharp = 0;
	tools.flat = 0;
	tools.doubleflat = 0;
	tools.statusdur = 0;
	tools.rest = 0;
	tools.hover_button1 = 0;
	tools.hover_button2 = 0;
	tools.hover_button3 = 0;
	tools.hover_tempo = 0;
	tools.hover_volume = 0;
	tools.natural = 0;
	return tools;
}

EventData *EventData_Alloc(void)
{
	EventData *temp = (EventData *)malloc(sizeof(EventData));
	memtest(temp);
	
	temp->lst = NULL;
	temp->n = 0;
	temp->hover = NULL;
	temp->base = NULL;
	temp->mode = MODE_EDIT;
	temp->score = NULL;
	temp->lastArea = NULL;
	
	temp->tools = InitToolbar();
	temp->tabselect = Selection_Alloc();
	temp->ctrl = 0;
	return temp;
}

void EventData_Free(EventData **ed)
{
	if(*ed != NULL)
	{
		Area *sauv2 = (*ed)->hover;
		Area *cur = (*ed)->lst;
		Area *cur2 = NULL;
		while(cur != NULL)
		{
			cur2 = cur->next;
			if(cur != sauv2)
				Area_Free(&cur);
			cur = cur2;
		}
		
		if((*ed)->hover != NULL)
			free((*ed)->hover);
		free(*ed);
		*ed = NULL;
	}
}

int EventData_SetBase(EventData *ed, SDL_Rect *base)
{
	if(NULL == ed)
		return 0;
	ed->base = base;
	return 1;
}

void MainEvents_AssociateScore(Score *score)
{
	if(main_events != NULL)
	{
		main_events->score = score;
	}
}

int EventData_SetZoom(EventData *ed, double zoom)
{
	if(NULL == ed)
		return 0;
	ed->r = zoom;
	return 1;
}

int EventData_Add(EventData *ed, Area *area)
{
	Area **cur = &(ed->lst);
	int goal = area->rect.w * area->rect.h;
	if((NULL == ed) || (NULL == area))
		return 0;
	
	if((NULL == *cur) || (goal <= ((*cur)->rect.w * (*cur)->rect.h)))
	{
		area->next = *cur;
		*cur = area;
		ed->n++;
		return 1;
	}
	
	while((*cur)->next != NULL && goal >= ((*cur)->next->rect.w * (*cur)->next->rect.h))
		cur = &((*cur)->next);
	
	area->next = (*cur)->next;
	(*cur)->next = area;
	
	ed->n++;
	return 1;
}

void Area_FreeRec(Area *area)
{
	if(area != NULL)
	{
		Area_FreeRec(area->next);
		free(area);
	}
}

int EventData_Flush(EventData *ed)
{
	if(NULL == ed)
		return 0;
	Area_FreeRec(ed->lst);
	ed->lst = NULL;
	ed->n = 0;
	ed->hover = NULL;
	Select_Flush();
	return 1;
}

void EventData_Console(EventData *ed)
{
	Area *area = NULL;
	if(NULL == ed)
		return;
	return;
	area = ed->lst;
	
	printf("EventData, n=%d, hover=%p\n",
				ed->n, (void *)ed->hover);
	
	while(area != NULL)
	{
		Area_Console(area);
		area = area->next;
	}
}

int PixelInRect(int x, int y, SDL_Rect rect)
{
	return (x >= rect.x && y >= rect.y && x<=(rect.x + rect.w) && y <= (rect.y + rect.h));
}

Area *Events_GetAreaByPixelAndType(int x, int y, Object_Type type)
{
	Area *cur = NULL;
	int i = 0;
	if(NULL == main_events)
		return NULL;
	cur = main_events->lst;
	while(cur != NULL)
	{
		if((cur->type & type) && PixelInRect(x-SIZE_BODY*cur->nbody, y, cur->rect))
		{
			if(main_events->mode == MODE_ADD)
			{
				if(cur->type == EVENT_ADDNOTE)
					return cur;
			}
			else if(main_events->mode == MODE_EDIT)
			{
				if(cur->type != EVENT_ADDNOTE)
					return cur;
			}
		}
		cur = cur->next;
		i++;
	}
	return NULL;
}

char MouseToNote(Area *area, int y)
{
	int z = y - area->rect.y;
	int i;
	int goal;
	int res = 91;
	int tab[] = {2, 1, 2, 2, 1, 2, 2};
	
	if(area->staff->steps[area->id_step]->cle == CLE_FA)
		z += HEAD_H*6;
	
	y+=HEAD_H*0.25;
	res -= ((int)((z)/(HEAD_H*3.5)))*12;
	goal = ((int)((z)/((int)(HEAD_H*0.5))))%7;
	for(i = 0; i < goal; i++)
		res -= tab[i%7];
	
	#ifdef DEBUG
	printf("return res=%d\n", res);
	#endif
	
	return res;
}

int Events_PollMouse(SDL_Event event)
{
	int x,y;
	Object_Type type = OBJECT_ALL;
	Area *area = NULL;
	
	
	if(NULL == main_events)
		return 0;
		
		
	switch(event.type)
	{
		case SDL_QUIT:
			return QUIT;
			break;
		case SDL_MOUSEMOTION:
			x = event.motion.x;
			y = event.motion.y;
			break;
		case SDL_MOUSEBUTTONDOWN:
			x = event.button.x;
			y = event.button.y;
			break;
		default:
			return 0;
	}
	
	if(main_events->mode == MODE_ADD)
		type = EVENT_ADDNOTE;
	else
		type = OBJECT_NOTE | OBJECT_STEP;
		
		
	area = Events_GetAreaByPixelAndType((int)((x - main_events->base->x) * 1.0 * main_events->r), 
					 (int)((y - main_events->base->y) * 1.0 * main_events->r), type);
	main_events->lastArea = area;				 
					 
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			/*if(main_events->mode == MODE_ADD)
			{
				if(area)
				{
					int tab[] = {0, 2, 4, 5, 7, 9, 11};
					int note = MouseToNote(area, y);
					int dest_y = 0;
					int i;
					double zoom = main_events->r;
					SDL_Surface *temp = NULL;
					
					SDL_Rect pos_temp;
					
					for(i = 0; i < 7; i++)
					{
						if((note % 12) == tab[i])
						{
							dest_y = -i * (HEAD_H / 2) + HEAD_H * 1.5 - (HEAD_H * 3.5) * (note / 12 - 5);
							break;
						}
					}
					temp = shrinkSurface(Images->Note_headBlack, (int)main_events->r, (int)main_events->r);
					pos_temp.x = Window->pos_body->x + (area->rect.x + SIZE_BODY*area->nbody) /zoom + (area->rect.w/2)/zoom-temp->w/2;	
					pos_temp.x -= Images->rot_noteW/zoom;
					pos_temp.y = Window->pos_body->y + area->rect.y/zoom;
					pos_temp.y += ((int)(((y - area->rect.y)/(HEAD_H/2)))*(HEAD_H/2))/zoom;
					pos_temp.y -= Images->rot_noteH/zoom;
					SDL_SetColorKey(temp, SDL_SRCCOLORKEY, SDL_MapRGB(temp->format, 255, 255, 255));
					SDL_BlitSurface(temp, NULL, Window->screen, &pos_temp);
					SDL_Flip(Window->screen);
					return NONE;
				}
			}*/
			if(main_events->mode == MODE_EDIT || main_events->mode == MODE_ADD)
			{
				if(main_events->hover == area)
					return NONE;
				main_events->hover = area;
				return HOVER;
			}
			else
				return NONE;
			break;
		case SDL_MOUSEBUTTONUP:
			return NONE;
		case SDL_MOUSEBUTTONDOWN:
			if((NULL == area) && (x < Window->pos_body->x || y < Window->pos_body->y))
				return NONE;
			if(event.button.button == SDL_BUTTON_WHEELDOWN ||
				event.button.button == SDL_BUTTON_WHEELUP)
				return NONE;
			if(main_events->mode == MODE_EDIT)
			{
				if(area != NULL && area->type == OBJECT_NOTE)
				{
					Note *temp = Step_GetNote(area->step, area->id_note);
					main_events->tools.duration = temp->duration;
					main_events->tools.sharp = temp->flags & NOTE_SHARP;
					main_events->tools.flat = temp->flags & NOTE_FLAT;
					main_events->tools.doublesharp = temp->flags & NOTE_DOUBLESHARP;
					main_events->tools.doubleflat = temp->flags & NOTE_DOUBLEFLAT;
					main_events->tools.natural = temp->flags & NOTE_NATURAL;
					main_events->tools.statusdur = 1*((temp->flags & NOTE_POINTED) != 0) + 2*((temp->flags & NOTE_DOUBLEPOINTED) != 0);
				}
				
				if(area != NULL && area->type == OBJECT_STEP)
				{
					int is = Audio_isPlaying();
					if(is)
						Audio_Pause();
					Audio_GoToStep(area->id_step);
					if(is)
						Audio_Play();
				}
				/*
				if(main_events->select == area)
					return NONE;
				main_events->select = area;
				*/
				if(NULL == area)
				{
					if(!Select_isEmpty())
					{
						Select_Flush();
						
						Select_Debug();
						return SELECT;
					}
					
					Select_Debug();
					return NONE;
				}
				
				
				if(!main_events->ctrl)
				{
					Select_Flush();
					Select_AddFirst(area);
					
					Select_Debug();
					return SELECT;
				}
				if(Select_isIn(area))
				{
					Select_Remove(area);
					
					Select_Debug();
					return SELECT;
				}
				Select_AddFirst(area);
					
				
				Select_Debug();
				return SELECT;
			}
			else
			{
				if((NULL == area))
					return NONE;
				if(area->type == EVENT_ADDNOTE)
				{
					Note_Flags add_flag = NOTE_DEFAULT;
					if(main_events->tools.sharp)
						add_flag |= NOTE_SHARP;
					else if(main_events->tools.flat)
						add_flag |= NOTE_FLAT;
					else if(main_events->tools.doublesharp)
						add_flag |= NOTE_DOUBLESHARP;
					else if(main_events->tools.doubleflat)
						add_flag |= NOTE_DOUBLEFLAT;
					if(main_events->tools.natural)
						add_flag |= NOTE_NATURAL;
					switch(main_events->tools.statusdur)
					{
						case 1:
							add_flag |= NOTE_POINTED;
							break;
						case 2:
							add_flag |= NOTE_DOUBLEPOINTED;
							break;
						default:
							break;
					}
					
					
					Staff_AddNote(area->staff, area->id_step, area->id_note_add, 
						MouseToNote(area, (y-main_events->base->y)*main_events->r), 
						add_flag, main_events->tools.duration);
					
					return FORCE_MAJ;
				}
			}
			break;
		default:
			return 0;
	}
	return 1;
}

int Events_PollKeyboard(SDL_Event event)
{
	Select_Node *sn = NULL;
	int is_maj = 0;
	
	if(NULL == main_events)
		return NONE;
		
	switch(event.type)
	{
		case SDL_QUIT:
			return QUIT;
		case SDL_KEYUP:
			switch(event.key.keysym.sym)
			{
				case SDLK_RCTRL:
				case SDLK_LCTRL:
					main_events->ctrl = 0;
					return NONE;
					break;
				default:
					break;
			}
			break;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
				case SDLK_BACKSPACE:
				case SDLK_DELETE:/*
					if(main_events->select == NULL || main_events->mode != MODE_EDIT)
						return NONE;
					switch(main_events->select->type)
					{
						case OBJECT_NOTE:
							Step_ChangeRestStatus(main_events->select->step, main_events->select->id_note, 1);	
							return FORCE_MAJ;
						case OBJECT_STEP:
							Staff_DeleteStep(main_events->select->staff, main_events->select->id_step);
							return FORCE_MAJ;
						default:
							break;
					}*/
					if(Select_isEmpty() || main_events->mode != MODE_EDIT)
						return NONE;
					
					sn = Select_GetIterate();
					while(sn != NULL)
					{
						switch(sn->val->type)
						{
							case OBJECT_NOTE:
								Step_ChangeRestStatus(sn->val->step,
										sn->val->id_note, 1);
								break;
							case OBJECT_STEP:
								Score_DeleteStep(main_events->score, sn->val->id_step);
								break;
							default:
								break;
						}
						sn = sn->next;
					}
					return FORCE_MAJ;
					
					break;
				case SDLK_d:
					/*if(main_events->select == NULL)
						return NONE;
					switch(main_events->select->type)
					{
						case OBJECT_NOTE:
							if(Step_DiviseRest(main_events->select->step, main_events->select->id_note))
								return FORCE_MAJ;
							else
								return NONE;
						default:
							break;
					}*/
					if(Select_isEmpty())
						return NONE;
					
					sn = Select_GetIterate();
					
					while(sn != NULL)
					{
						switch(sn->val->type)
						{
							case OBJECT_NOTE:
								if(Step_Divise(sn->val->step,
										sn->val->id_note))
									is_maj = 1;
								break;
							default:
								break;
						}
						sn = sn->next;
					}
					
					if(is_maj)
						return FORCE_MAJ;
					
					return NONE;
					
					break;
				case SDLK_a:
					if(main_events->mode == MODE_ADD)
						return NONE;
					main_events->mode = MODE_ADD;
					return FORCE_MAJ;
					break;
				case SDLK_e:
					if(main_events->mode == MODE_EDIT)
						return NONE;
					main_events->mode = MODE_EDIT;
					return FORCE_MAJ;
					break;
				case SDLK_SPACE:
					if(Audio_isPlaying())
						Audio_Pause();
					else
						Audio_Play();
					return FORCE_MAJ;
					
				case SDLK_RCTRL:
				case SDLK_LCTRL:
					main_events->ctrl = 1;
					return NONE;
				default:
					break;
			}
			if(event.key.keysym.sym > SDLK_KP0 && event.key.keysym.sym <= SDLK_KP7)
			{
				main_events->tools.duration = pow(2, event.key.keysym.sym-SDLK_KP0-1);
				return NONE;
			}
	}
	return NONE;
}

Select_Node *SelectNode_Alloc(Area *area)
{
	Select_Node *temp = (Select_Node *)malloc(sizeof(Select_Node));
	memtest(temp);
	
	temp->val = area;
	temp->next = NULL;
	
	return temp;
}

void SelectNode_Free(Select_Node **sn)
{
	if(*sn != NULL)
	{
		free(*sn);
		*sn = NULL;
	}
}

void SelectNode_FreeAll(Select_Node **sn)
{
	if(*sn != NULL)
	{
		SelectNode_FreeAll(&((*sn)->next));
		SelectNode_Free(sn);
	}
}

Selection *Selection_Alloc(void)
{
	Selection *temp = (Selection *)malloc(sizeof(Selection));
	memtest(temp);
	
	temp->first = NULL;
	temp->last = NULL;
	
	return temp;
}

int Selection_AddFirst(Selection *select, Area *area)
{
	Select_Node *new = NULL;
	
	if((NULL == select) || (NULL == area))
		return 0;
	
	new = SelectNode_Alloc(area);
	
	if(NULL == select->first)
	{
		select->first = new;
		select->last = select->first;
		return 1;
	}
	
	new->next = select->first;
	select->first = new;
	
	return 1;
}

int Select_AddFirst(Area *area)
{
	if(main_events != NULL)
		return Selection_AddFirst(main_events->tabselect, area);
	return 0;
}

int Selection_AddLast(Selection *select, Area *area)
{
	Select_Node *new = NULL;
	
	if((NULL == select) || (NULL == area))
		return 0;
	
	new = SelectNode_Alloc(area);
	
	if(NULL == select->last)
	{
		select->last = new;
		select->first = select->last;
		return 1;
	}
	
	select->last->next = new;
	select->last = new;
	
	return 1;
}

int Select_AddLast(Area *area)
{
	if(main_events != NULL)
		return Selection_AddLast(main_events->tabselect, area);
	return 0;
}

void Selection_Flush(Selection *select)
{
	if(select != NULL)
	{
		SelectNode_FreeAll(&(select->first));
		select->last = NULL;
	}
}

void Select_Flush(void)
{
	if(main_events != NULL)
		Selection_Flush(main_events->tabselect);
}

int Selection_Remove(Selection *select, Area *area)
{
	Select_Node *sn = NULL;
	Select_Node *sauv = NULL;
	if((NULL == select) || (NULL == area))
		return 0;

	sn = select->first;
	
	if(select->first->val == area)
	{
		select->first = select->first->next;
		if(select->last->val == area)
			select->last = select->first;
		return 1;
	}
	
	while(sn->next != NULL && sn->next->val != area)
		sn = sn->next;
	
	if(NULL == sn->next)
		return 0;
	
	if(NULL == sn->next->next)
		select->last = sn;
	
	sauv = sn->next;
	sn->next = sn->next->next;
	
	SelectNode_Free(&sauv);
	
	return 1;	
}

int Select_Remove(Area *area)
{
	if(main_events != NULL)
	{
		return Selection_Remove(main_events->tabselect, area);
	}
	return 0;
}

int Selection_isIn(Selection *select, Area *area)
{
	Select_Node *sn = NULL;
	
	if((NULL == select) || (NULL == area))
		return 0;

	sn = select->first;
	
	while(sn != NULL && sn->val != area)
		sn = sn->next;
		
	if(NULL == sn)
		return 0;
		
	return 1;
}

int Select_isIn(Area *area)
{
	if(main_events != NULL)
		return Selection_isIn(main_events->tabselect, area);
	return 0;
}

int Select_isEmpty(void)
{
	if(main_events != NULL)
	{
		if(main_events->tabselect == NULL)
			return 1;
		if(main_events->tabselect->first == NULL)
			return 1;
		return 0;
	}
	return 1;
}

Select_Node *Select_GetIterate(void)
{
	if((NULL == main_events) || (NULL == main_events->tabselect))
		return NULL;
	return main_events->tabselect->first;
}

Select_Node *Select_GetLast(void)
{
	if((NULL == main_events) || (NULL == main_events->tabselect))
		return NULL;
	return main_events->tabselect->last;
}

void Select_Debug(void)
{
	int i;
	Select_Node *sn = NULL;
	
	sn = main_events->tabselect->first;
	i = 0;
	while(sn != NULL)
	{
		i++;
		sn = sn->next;
	}
}

int Select_isOne(void)
{
	if((NULL == main_events) || (NULL == main_events->tabselect))
		return 0;
	if((main_events->tabselect->first == main_events->tabselect->last))
			return 1;
	return 0;
}







