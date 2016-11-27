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

Menu_Node *MenuNode_Alloc(const char *name, Node_Type type, int (*f)(void))
{
	Menu_Node *temp = (Menu_Node *)malloc(sizeof(Menu_Node));
	memtest(temp);
	
	temp->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
	memtest(temp->name);
	strcpy(temp->name, name);
	
	temp->type = type;
	switch(type)
	{
		case NODE:
			temp->next = NodeArray_Alloc(20);
			break;
		case LEAF:
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

int menu_no_action(void)
{
	printf("menu_no_action()\n");
	return 1;
}






