#ifndef TREAP_H_INCLUDED
#define TREAP_H_INCLUDED

#include <stdbool.h>

#define TREAP_NUM_OF_CHILD 2

#define TREAP_LCH 0
#define TREAP_RCH 1

struct Treap_Node
{
	struct Treap_Node *ch[TREAP_NUM_OF_CHILD];
	void* valuep; // maintained value for tree
	int key; // randomly generated key for heap
	int size;
};

struct Treap_Node *Treap_NewNode(void* valuep);

void Treap_Maintain(struct Treap_Node *u);
void Treap_Rotate(struct Treap_Node **u, bool dir);

void Treap_Insert_Recursion(struct Treap_Node **u, void* valuep,
							int (*cmp)(void* a, void* b));
void Treap_Insert(void* valuep, int (*cmp)(void* a, void* b));
void Treap_Delete_Recursion(struct Treap_Node **u, void* valuep,
							int (*cmp)(void* a, void *b));
void Treap_Delete(void* valuep, int (*cmp)(void* a, void* b));

void* Treap_Query_Kth_Element(int K);
int Treap_Query_Rank(void* valuep, int (*cmp)(void* a, void* b));
void* Treap_Query_Predecessor(void* valuep, int (*cmp)(void* a, void* b));
void* Treap_Query_Successor(void* valuep, int (*cmp)(void* a, void* b));

void Treap_Destruct_Recursion(struct Treap_Node *u);
void Treap_Destruct();

#endif // #ifndef TREAP_H_INCLUDED