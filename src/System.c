#include "../include/System.h"

extern void _mem(void *ptr, const char *str, const char *file, int line)
{
	if(NULL == ptr)
	{
		printf("Memory Allocation Error variable \"%s\", ", str);
		printf("at \"%s\":%d\n", file, line);
		exit(EXIT_FAILURE);
	}
}

extern int text_color(TerminalColor color)
{
	char strC[7];
	
	strC[0] = 27;
	strC[1] = '[';
	strC[2] = '0' + color / 10;
	strC[3] = '0' + color % 10;
	strC[4] = 'm';
	strC[5] = '\0';
	
	return printf("%s", strC) == 5;
}

extern int colorprintf(TerminalColor color, const char *format, ...)
{
	int r;
	va_list va;
	va_start(va, format);
	text_color(color);
	r = vprintf(format, va);
	text_color(WHITE);
	va_end(va);
	return r;
}

NodeList *NodeList_Alloc(const char *name, void *data, 	void (*Node_Free)(void *))
{
	NodeList *temp = (NodeList *)malloc(sizeof(NodeList));
	memtest(temp);
	
	temp->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
	memtest(temp->name);
	strcpy(temp->name, name);
	
	temp->data = data;
	temp->famous = 1;
	temp->id = 0;
	temp->Node_Free = Node_Free;
	temp->next = NULL;
	
	return temp;
}

void NodeList_Free(NodeList **node)
{
	if(*node != NULL)
	{
		if((*node)->name != NULL)
			free((*node)->name);
			
		if((*node)->Node_Free != NULL && (*node)->data != NULL)
			(*node)->Node_Free(&((*node)->data));
		
		free(*node);
		*node = NULL;
	}
}

void NodeList_FreeAll(NodeList **lst)
{
	if(*lst != NULL)
	{
		NodeList_FreeAll(&((*lst)->next));
		NodeList_Free(lst);
	}
}

GenList *GenList_Alloc(void)
{
	GenList *temp = (GenList *)malloc(sizeof(GenList));
	memtest(temp);
	
	temp->lst = NULL;
	temp->n = 0;
	temp->lst_famous = 1;
	
	return temp;
}

void GenList_Free(GenList **lst)
{
	if(*lst != NULL)
	{
		NodeList_FreeAll(&((*lst)->lst));
		free(*lst);
		*lst = NULL;
	}
}

int NodeList_Add(NodeList **node, NodeList *new)
{
	NodeList *cur = *node;
	memtest(new);
	
	if(NULL == *node)
	{
		*node = new;
		return 1;
	}
	
	if(NULL == cur->next)
	{
		if(new->famous > cur->famous)
		{
			new->next = cur;
			*node = new;
		}
		else
		{
			cur->next = new;
		}
		return 1;
	}
	
	if(cur->famous > new->famous)
		return NodeList_Add(&((*node)->next), new);
	
	new->next = *node;
	*node = new;
	return 1;
}

int GenList_Add(GenList *lst, const char *name, void *data, void (*Node_Free)(void *))
{
	NodeList *node = NULL;
	
	if(NULL == lst || NULL == name || NULL == data || NULL == Node_Free)
		return 0;
	
	node = NodeList_Alloc(name, data, Node_Free);
	memtest(node);
	
	node->id = lst->n;
	node->famous = lst->lst_famous;
	
	NodeList_Add(&(lst->lst), node);
	lst->n++;
	lst->lst_famous++;
	
	return 1;
}

void *NodeList_Get(NodeList **node, const char *name)
{
	NodeList *res = NULL;
	NodeList *temp = NULL;
	
	if(NULL == *node || NULL == name)
		return NULL;
	
	if(!strcmp((*node)->name, name))
	{
		res = (*node)->data;
		(*node)->famous *= 2;
	}
	else
		res = NodeList_Get(&((*node)->next), name);
	
	if((*node)->next != NULL && (*node)->famous < (*node)->next->famous)
	{
		temp = *node;
		*node = (*node)->next;
		temp->next = (*node)->next;
		(*node)->next = temp;
	}
	return res;
}

void *GenList_Get(GenList *lst, const char *name)
{
	if(NULL == lst || NULL == name)
		return NULL;
	
	return NodeList_Get(&(lst->lst), name);
	
}

Point *Point_Alloc(Uint x, Uint y)
{
	Point *temp = (Point *)malloc(sizeof(Point));
	memtest(temp);
	
	temp->x = x;
	temp->y = y;
	
	return temp;
}

void Point_Free(Point **pt)
{
	if(*pt != NULL)
	{
		free(*pt);
		*pt = NULL;
	}	
}

int Dots_Init(Dots **dt)
{
	if(NULL == *dt)
	{
		*dt = (Dots *)malloc(sizeof(Dots));
		memtest(*dt);
		
		(*dt)->tab = NULL;
		(*dt)->n = 0;
	}
	
	if(NULL == (*dt)->tab)
	{
		(*dt)->tab = (Point **)malloc(sizeof(Point *) * 8);
		memtest((*dt)->tab);
		
		(*dt)->n = 0;
	}
	else
	{
		int i;
		for(i = 0; i < (*dt)->n; i++)
			Point_Free(&((*dt)->tab[i]));
		(*dt)->n = 0;
	}
	(*dt)->totalheight = 0;
	
	return 1;
}

void Dots_Free(Dots **dt)
{
	if(*dt != NULL)
	{
		if((*dt)->tab != NULL)
		{
			int i;
			for(i = 0; i < (*dt)->n; i++)
				Point_Free(&((*dt)->tab[i]));
			free((*dt)->tab);
		}
		
		(*dt)->n = 0;
		
		free(*dt);
		*dt = NULL;
	}
}

int Dots_Add(Dots *dt, Uint x, Uint y, int height)
{
	if((NULL == dt) || (NULL == dt->tab))
		return 0;

	
	dt->tab[dt->n] = Point_Alloc(x, y);
	dt->n++;
	dt->totalheight += height;
	
	return 1;
}

int Dots_Length(Dots *dt)
{
	if(dt != NULL)
		return dt->n;
	return 0;
}

double Dots_CalcCoef(Dots *dt)
{
	if(dt->n <= 1)
		return 0.;
	return ((dt->tab[dt->n-1]->y - dt->tab[0]->y) * 1.0) / (dt->tab[dt->n-1]->x - dt->tab[0]->x);
}

double Dots_CalcExtremCoef(Dots *dt)
{
	int i;
	double a = 0.;
	double temp;
	
	for(i = 0; i < dt->n-1; i++)
	{
		temp = ((dt->tab[i+1]->y - dt->tab[i]->y) * 1.0) / (dt->tab[i+1]->x - dt->tab[i]->x);
		if(abs(temp) > abs(a))
			a = temp;
	}
	return a;
}

int Dots_isLinear(Dots *dt)
{
	int i;
	double a = 0;
	double a2 = 0;
	
	if((NULL == dt) || (NULL == dt->tab))
		return 0;

	if(dt->n <= 2)
		return 1;

	a = ((dt->tab[1]->y - dt->tab[0]->y) * 1.0) / (dt->tab[1]->x - dt->tab[0]->x);
	
	for(i = 2; i < dt->n; i++)
	{
		a2 = ((dt->tab[i]->y - dt->tab[0]->y) * 1.0) / (dt->tab[i]->x - dt->tab[0]->x);
		if(a2 != a)
			return 0;
	}
	return 1;
}

int Dots_EvaluateYFromX(Dots *dt, int x)
{
	double a;
	
	a = Dots_CalcCoef(dt);
	x -= dt->tab[0]->x;
	
	printf("Y(%d) = %g (%g * %d + %d)\n", x, a * x + dt->tab[0]->y, a, x, dt->tab[0]->y);
	return a * x + dt->tab[0]->y;
}

int Dots_GetYMin(Dots *dt)
{
	int min = 50000;
	int i;
	
	if((NULL == dt) || (NULL == dt->tab) || (dt->n == 0))
		return 0;

	for(i = 0; i < dt->n; i++)
	{
		if(dt->tab[i]->y < min)
			min = dt->tab[i]->y;
	}
	return min;
}

int Dots_GetYMax(Dots *dt)
{
	int max = 0;
	int i;
	
	if((NULL == dt) || (NULL == dt->tab) || (dt->n == 0))
		return 0;

	for(i = 0; i < dt->n; i++)
	{
		if(dt->tab[i]->y > max)
			max = dt->tab[i]->y;
	}
	return max;
}





