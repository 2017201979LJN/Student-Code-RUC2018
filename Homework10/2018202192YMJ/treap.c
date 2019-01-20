#include <stdlib.h>
#include <stdbool.h>

#include "treap.h"

struct Treap_Node *Treap_Root = NULL;

struct Treap_Node *Treap_NewNode(void* valuep)
{
	struct Treap_Node *u = malloc(sizeof(struct Treap_Node));
	u->ch[TREAP_LCH] = u->ch[TREAP_RCH] = NULL;
	u->valuep = valuep;
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

void Treap_Insert_Recursion(struct Treap_Node **u, void* valuep,
							int (*cmp)(void* a, void* b))
{
	if (*u == NULL)
	{
		*u = Treap_NewNode(valuep);
		return;
	}
	bool dir = cmp(valuep, (*u)->valuep) > 0;
	Treap_Insert_Recursion(&((*u)->ch[dir]), valuep, cmp);
	Treap_Maintain(*u);
	if ((*u)->ch[dir]->key < (*u)->key)
	{
		Treap_Rotate(u, dir);
	}
}

void Treap_Insert(void* valuep, int (*cmp)(void* a, void* b))
{
	Treap_Insert_Recursion(&Treap_Root, valuep, cmp);
}

void Treap_Delete_Recursion(struct Treap_Node **u, void* valuep,
							int (*cmp)(void* a, void* b))
{
	int cmp_res = cmp(valuep, (*u)->valuep);
	if (cmp_res == 0)
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
			Treap_Delete_Recursion(u, valuep, cmp);
		}
	}
	else
	{
		Treap_Delete_Recursion(&((*u)->ch[cmp_res > 0]), valuep, cmp);
		Treap_Maintain(*u);
	}
}

void Treap_Delete(void* valuep, int (*cmp)(void* a, void* b))
{
	// assume that value exists in the Treap
	Treap_Delete_Recursion(&Treap_Root, valuep, cmp);
}

void* Treap_Query_Kth_Element(int K)
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
			return u->valuep;
		}
		else
		{
			K -= amt + 1;
			u = u->ch[TREAP_RCH];
		}
	}
}

int Treap_Query_Rank(void* valuep, int (*cmp)(void* a, void* b))
{
	// return the number of elements smaller than value
	int res = 0;
	for (struct Treap_Node *u = Treap_Root; ; )
	{
		if (u == NULL)
		{
			return res;
		}
		if (cmp(valuep, u->valuep) > 0)
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

void* Treap_Query_Predecessor(void* valuep, int (*cmp)(void* a, void* b))
{
	// return the largest element which is smaller than value
	void* res = NULL;
	for (struct Treap_Node *u = Treap_Root; ; )
	{
		if (u == NULL)
		{
			return res;
		}
		if (cmp(u->valuep, valuep) >= 0)
		{
			u = u->ch[TREAP_LCH];
		}
		else
		{
			res = u->valuep;
			u = u->ch[TREAP_RCH];
		}
	}
}

void* Treap_Query_Successor(void* valuep, int (*cmp)(void* a, void* b))
{
	// return the smallest element which is larger than value
	void* res = NULL;
	for (struct Treap_Node *u = Treap_Root; ; )
	{
		if (u == NULL)
		{
			return res;
		}
		if (cmp(u->valuep, valuep) <= 0)
		{
			u = u->ch[TREAP_RCH];
		}
		else
		{
			res = u->valuep;
			u = u->ch[TREAP_LCH];
		}
	}
}

void Treap_Destruct_Recursion(struct Treap_Node *u)
{
	if (u == NULL) return;
	Treap_Destruct_Recursion(u->ch[TREAP_LCH]);
	Treap_Destruct_Recursion(u->ch[TREAP_RCH]);
	free(u->valuep);
	free(u);
}

void Treap_Destruct()
{
	// destruct the whole Treap
	Treap_Destruct_Recursion(Treap_Root);
	Treap_Root = NULL;
}
