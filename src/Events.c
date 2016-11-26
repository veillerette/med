#include "../include/Events.h"

EventData *main_events = NULL;

Area *Area_Alloc(SDL_Rect rect, Object_Type type)
{
	Area *temp = (Area *)malloc(sizeof(Area));
	memtest(temp);
	
	temp->rect = rect;
	temp->type = type;
	
	temp->step = NULL;
	temp->id_note = 0;
	
	temp->next = NULL;
	
	return temp;
}

Area *Area_Set(SDL_Rect rect, Object_Type type, ...)
{
	Area *area = NULL;
	va_list va;
	va_start(va, type);
	
	area = Area_Alloc(rect, type);
	
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
		case OBJECT_SIGN:
			printf("\tSign=%p\n", *(area->sign));
			break;
		default:
			colorprintf(RED, "Type not valid\n");
	}
	
}

EventData *EventData_Alloc(void)
{
	EventData *temp = (EventData *)malloc(sizeof(EventData));
	memtest(temp);
	
	temp->lst = NULL;
	temp->n = 0;
	temp->select = NULL;
	temp->hover = NULL;
	temp->base = NULL;
	
	return temp;
}

void EventData_Free(EventData **ed)
{
	if(*ed != NULL)
	{
		Area *sauv = (*ed)->select;
		Area *sauv2 = (*ed)->hover;
		Area *cur = (*ed)->lst;
		Area *cur2 = NULL;
		while(cur != NULL)
		{
			cur2 = cur->next;
			if(cur != sauv && cur != sauv2)
				Area_Free(&cur);
			cur = cur2;
		}
		
		if((*ed)->select != NULL)
			free((*ed)->select);
		if((*ed)->hover != NULL)
			free((*ed)->hover);
		if((*ed)->base != NULL)
			free((*ed)->base);
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

int EventData_Flush(EventData *ed)
{
	Area **cur = NULL;
	Area **sauv = NULL;
	if(NULL == ed)
		return 0;
	cur = &ed->lst;
	while(cur != NULL)
	{
		sauv = cur;
		if((*cur)->next != NULL)
			cur = &((*cur)->next);
		else
			cur = NULL;
		Area_Free(sauv);
	}
	ed->lst = NULL;
	ed->n = 0;
	ed->hover = NULL;
	ed->select = NULL;
	return 1;
}

void EventData_Console(EventData *ed)
{
	Area *area = NULL;
	if(NULL == ed)
		return;
	area = ed->lst;
	
	printf("EventData, n=%d, select=%p, hover=%p\n",
				ed->n, (void *)ed->select, (void *)ed->hover);
	
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
		if((cur->type & type) && PixelInRect(x, y, cur->rect))
			return cur;
		cur = cur->next;
		i++;
	}
	return NULL;
}

int Events_PollMouse(SDL_Event event)
{
	int x,y;
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
	area = Events_GetAreaByPixelAndType((int)((x - main_events->base->x) * 1.0 * main_events->r), 
					 (int)((y - main_events->base->y) * 1.0 * main_events->r), OBJECT_ALL);
	switch(event.type)
	{
		case SDL_MOUSEMOTION:
			if(main_events->hover == area)
				return NONE;
			main_events->hover = area;
			return HOVER;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(main_events->select == area)
				return NONE;
			main_events->select = area;
			if(area != NULL)
				printf("select new area\n");
			return SELECT;
			break;
		default:
			return 0;
	}
	return 1;
}

int Events_PollKeyboard(SDL_Event event)
{
	if(NULL == main_events)
		return NONE;
	switch(event.type)
	{
		case SDL_QUIT:
			return QUIT;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
				case SDLK_BACKSPACE:
				case SDLK_DELETE:
					if(main_events->select == NULL)
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
					}
				case SDLK_d:
					if(main_events->select == NULL)
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
					}
				default:
					break;
			}
	}
	return NONE;
}








