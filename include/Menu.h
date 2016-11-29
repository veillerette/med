#ifndef __HEAD_MENU__
#define __HEAD_MENU__


#include <stdio.h>
#include <stdlib.h>

#include "Window2.h"
#include "Events.h"

#define MENU_FONT "media/Garamond.ttf"
#define MENU_SIZETEXT 18
#define FORCE_CLEAR 20

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
	SDL_Rect pos;
	Node_Type type;
	union
	{
		Node_Array *next;
		struct
		{
			int (*f)(void);
			int need_select;
		};
	};
};
typedef struct Menu Menu;
struct Menu
{
	Node_Array *lst;
	TTF_Font *font;
	Menu_Node *select;
	Menu_Node *hover;
};

Node_Array *NodeArray_Alloc(int max);

int NodeArray_AddElem(Node_Array *na, Menu_Node *elem);

int NodeArray_Add(Node_Array *na, const char *name, int need_select, Node_Type type, int (*f)(void));

Menu_Node *MenuNode_Alloc(const char *name, int need_select, Node_Type type, int (*f)(void));

Menu *Menu_Alloc(void);

void NodeArray_Free(Node_Array **lst);

void MenuNode_Free(Menu_Node **node);

void Menu_Free(Menu **menu);

int menu_no_action(void);

Menu *Menu_Create(void);

void Menu_AffBaseOne(TTF_Font *font, Menu_Node *node, int *x, int *y, int status, int dir, int max);

void Menu_AffExtendOne(TTF_Font *font, Menu *menu, Menu_Node *node, int *x, int *y);

void Menu_Aff(Menu *menu, int *x, int *y);

void Menu_Console(Node_Array *lst, int tab);

int Menu_PollMouse(Menu *menu, SDL_Event event);
#endif
