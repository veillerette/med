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

NodeList *NodeList_Alloc(const char *name, void *data, 	void (*Node_Free)(void *);)
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
			temp->Node_Free(&((*node)->data));
		
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



