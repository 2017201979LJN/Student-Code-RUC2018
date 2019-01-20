#include <stdio.h>
#include <malloc.h>
#include "sbt.h"

int cmp(void *a, void *b) {
	if (((struct data *)a)->d == ((struct data *)b)->d) return 0;
	return (((struct data *)a)->d < ((struct data *)b)->d) ? -1 : 1;
}

void SBT_rotate(SBT *rt, int type) {
	struct SBTnode *x = *rt;
	struct SBTnode *tmp = x->ch[!type];
	x->ch[!type] = tmp->ch[type];
	tmp->ch[type] = x;
	tmp->size = x->size;
	x->size = x->ch[LT]->size + x->ch[RT]->size + 1;
	*rt = tmp;
}
void SBT_maintain(SBT *rt, int flag) {
	if ((*rt)->ch[flag]->ch[flag]->size > (*rt)->ch[!flag]->size) {
		SBT_rotate(rt, !flag);
	}
	else if ((*rt)->ch[flag]->ch[!flag] ->size > (*rt)->ch[!flag]->size) {
		SBT_rotate(&((*rt)->ch[flag]), flag);
		SBT_rotate(rt, !flag);
	}
	else return;
	SBT_maintain(&((*rt)->ch[LT]), LT);
	SBT_maintain(&((*rt)->ch[RT]), RT);
	SBT_maintain(rt, LT);
	SBT_maintain(rt, RT);
}
void SBT_insert_node(SBT *rt, struct SBTnode *x) {
	if ((*rt) == (&NUL)) {
		*rt = x;
		return;
	}
	(*rt)->size++;
	int dir = (cmp(x->key, (*rt)->key) != -1) ? 1 : 0;
	SBT_insert_node(&((*rt)->ch[dir]), x);
	SBT_maintain(rt, dir);
}
struct SBTnode *SBT_newnode(void *data) {
	struct SBTnode *ret = malloc(sizeof(struct SBTnode));
	ret->ch[LT] = ret->ch[RT] = &NUL;
	ret->key = data;
	ret->size = 1;
	return ret;
}
struct SBTnode *SBT_get_delete(SBT *rt, void *value) {
	if ((*rt) == &NUL) return &NUL;
	(*rt)->size--;
	int tmp = cmp(value, (*rt)->key);
	if (tmp == 0) {
		struct SBTnode *todel;
		if ((*rt)->ch[LT] == &NUL || (*rt)->ch[RT] == &NUL) {
			todel = (*rt);
			*rt = (*rt)->ch[(*rt)->ch[RT] != &NUL];
		}
		else {
			SBT *lst = &((*rt)->ch[RT]);
			todel = (*rt)->ch[RT];
			while (todel->ch[LT] != (&NUL)) {
				lst = &(todel->ch[LT]);
				todel->size--;
				todel = todel->ch[LT];
			}
			struct data *tmp = (*rt)->key;
			(*rt)->key = todel->key;
			todel->key = tmp;
			todel = SBT_get_delete(lst, value);
		}
		return todel;
	}
	else return SBT_get_delete(&((*rt)->ch[tmp > 0]), value);
}
void SBT_delete(SBT *rt, void *value) {
	struct SBTnode *x = SBT_get_delete(rt, value);
	free(x->key);
	free(x);
}
void SBT_insert_value(SBT *rt, void *value) {
	struct SBTnode *x = SBT_newnode(value);
	SBT_insert_node(rt, x);
}
int SBT_rank(SBT rt, void *value) {
	int ans = 0;
	while (rt != &NUL) {
		int tmp = cmp(value, rt->key);
		if (tmp == 1) {
			ans += rt->ch[LT]->size + 1;
			rt = rt->ch[RT];
		}
		else rt = rt->ch[LT];
	}
	return ans;
}
void *SBT_num(SBT rt, int k) {
	while (1) {
		if (rt->ch[LT]->size + 1 == k) return rt->key;
		if (rt->ch[LT]->size >= k) rt = rt->ch[LT];
		else {
			k -= rt->ch[LT]->size + 1;
			rt = rt->ch[RT];
		}
	}
}
void *SBT_pred(SBT rt, void *value) {
	void *ans = NULL;
	while (rt != &NUL) {
		int tmp = cmp(value, rt->key);
		if (tmp == 1) {
			ans = rt->key;
			rt = rt->ch[RT];
		}
		else rt = rt->ch[LT];
	}
	return ans;
}
void *SBT_succ(SBT rt, void *value) {
	void *ans = NULL;
	while (rt != &NUL) {
		int tmp = cmp(value, rt->key);
		if (tmp == -1) {
			ans = rt->key;
			rt = rt->ch[LT];
		}
		else rt = rt->ch[RT];
	}
	return ans;
}
void SBT_destruct(SBT rt) {
	if (rt->ch[LT] != &NUL) SBT_destruct(rt->ch[LT]);
	if (rt->ch[RT] != &NUL) SBT_destruct(rt->ch[RT]);
	free(rt->key);
	free(rt);
}