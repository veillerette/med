#include "System.h"

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





