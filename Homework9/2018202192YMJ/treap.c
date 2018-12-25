#include "treap.h"

static struct Treap_Node *Treap_Root = NULL;

struct Treap_Node *Treap_NewNode(int value)
{
	struct Treap_Node *u = malloc(sizeof(struct Treap_Node));
	u->ch[TREAP_LCH] = u->ch[TREAP_RCH] = NULL;
	u->value = value;
	u->key = rand();
	u->size = 1;
	return u;
}

void Treap_Maintain(struct Treap_Node *u)
{
	u->size = 1;
	if (u->ch[TREAP_LCH] != NULL)
	{
		u->size += u->ch[TREAP_LCH]->size;
	}
	if (u->ch[TREAP_RCH] != NULL)
	{
		u->size += u->ch[TREAP_RCH]->size;
	}
}

void Treap_Rotate(struct Treap_Node **u, bool dir)
{
	struct Treap_Node *v = (*u)->ch[dir];
	(*u)->ch[dir] = v->ch[!dir];
	v->ch[!dir] = *u;
	Treap_Maintain(*u);
	Treap_Maintain(v);
	*u = v;
}

void Treap_Insert_Recursion(struct Treap_Node **u, int value)
{
	if (*u == NULL)
	{
		*u = Treap_NewNode(value);
		return;
	}
	bool dir = value > (*u)->value;
	Treap_Insert_Recursion(&((*u)->ch[dir]), value);
	Treap_Maintain(*u);
	if ((*u)->ch[dir]->key < (*u)->key)
	{
		Treap_Rotate(u, dir);
	}
}

void Treap_Insert(int value)
{
	Treap_Insert_Recursion(&Treap_Root, value);
}

void Treap_Delete_Recursion(struct Treap_Node **u, int value)
{
	if ((*u)->value == value)
	{
		if ((*u)->ch[TREAP_LCH] == NULL)
		{
			struct Treap_Node *tmp = (*u)->ch[TREAP_RCH];
			free(*u);
			*u = tmp;
		}
		else if((*u)->ch[TREAP_RCH] == NULL)
		{
			struct Treap_Node *tmp = (*u)->ch[TREAP_LCH];
			free(*u);
			*u = tmp;
		}
		else
		{
			Treap_Rotate(u, (*u)->ch[TREAP_RCH]->key > (*u)->ch[TREAP_LCH]->key);
			Treap_Delete_Recursion(u, value);
		}
	}
	else
	{
		Treap_Delete_Recursion(&((*u)->ch[value > (*u)->value]), value);
		Treap_Maintain(*u);
	}
}

void Treap_Delete(int value)
{
	// assume that value exists in the Treap
	Treap_Delete_Recursion(&Treap_Root, value);
}

int Treap_Query_Kth_Element(int K)
{
	// assume that K is not larger than the size of the Treap
	for (struct Treap_Node *u = Treap_Root; ; )
	{
		int amt = 0;
		if (u->ch[TREAP_LCH] != NULL) amt = u->ch[TREAP_LCH]->size;
		// amt: number of elements in u->ch[TREAP_LCH] (if exists)
		if (amt >= K)
		{
			u = u->ch[TREAP_LCH];
		}
		else if (amt + 1 == K)
		{
			return u->value;
		}
		else
		{
			K -= amt + 1;
			u = u->ch[TREAP_RCH];
		}
	}
}

int Treap_Query_Rank(int value)
{
	// return the number of elements smaller than value
	int res = 0;
	for (struct Treap_Node *u = Treap_Root; ; )
	{
		if (u == NULL)
		{
			return res;
		}
		if (value > u->value)
		{
			++res;
			if (u->ch[TREAP_LCH] != NULL) res += u->ch[TREAP_LCH]->size;
			u = u->ch[TREAP_RCH];
		}
		else
		{
			u = u->ch[TREAP_LCH];
		}
	}
}

int Treap_Query_Predecessor(int value)
{
	// return the largest element which is smaller than value
	int res = TREAP_NOT_FOUND;
	for (struct Treap_Node *u = Treap_Root; ; )
	{
		if (u == NULL)
		{
			return res;
		}
		if (u->value >= value)
		{
			u = u->ch[TREAP_LCH];
		}
		else
		{
			res = u->value;
			u = u->ch[TREAP_RCH];
		}
	}
}

int Treap_Query_Successor(int value)
{
	// return the smallest element which is larger than value
	int res = TREAP_NOT_FOUND;
	for (struct Treap_Node *u = Treap_Root; ; )
	{
		if (u == NULL)
		{
			return res;
		}
		if (u->value <= value)
		{
			u = u->ch[TREAP_RCH];
		}
		else
		{
			res = u->value;
			u = u->ch[TREAP_LCH];
		}
	}
}

void Treap_Destruct_Recursion(struct Treap_Node *u)
{
	if (u == NULL) return;
	Treap_Destruct_Recursion(u->ch[TREAP_LCH]);
	Treap_Destruct_Recursion(u->ch[TREAP_RCH]);
	free(u);
}

void Treap_Destruct()
{
	// destruct the whole Treap
	Treap_Destruct_Recursion(Treap_Root);
	Treap_Root = NULL;
}
