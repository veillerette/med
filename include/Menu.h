#ifndef __HEAD_MENU__
#define __HEAD_MENU__


#include <stdio.h>
#include <stdlib.h>

#include "../include/Window.h"

#define MENU_FONT "media/Garamond-light"
#define MENU_SIZETEXT 15

typedef enum
{
	NODE, LEAF
} Node_Type;

typedef struct Node_Array Node_Array;
typedef struct Menu_Node Menu_Node;

struct Node_Array
{
	Menu_Node **next;
	int n;
	int max;
};


struct Menu_Node
{
	char *name;
	Node_Type type;
	union
	{
		Node_Array *next;
		int (*f)(void);
	};
};
typedef struct Menu Menu;
struct Menu
{
	Node_Array *lst;
	TTF_Font *font;
};

Node_Array *NodeArray_Alloc(int max);

Menu_Node *MenuNode_Alloc(const char *name, Node_Type type, int (*f)(void));

Menu *Menu_Alloc(void);

void NodeArray_Free(Node_Array **lst);

void MenuNode_Free(Menu_Node **node);

void Menu_Free(Menu **menu);

int menu_no_action(void);

#endif
