#include <stdio.h>
#include <stdlib.h>

#define LT 0
#define RT 1

struct SBTnode {
    struct SBTnode *ch[2];
    int size, key;
}NUL={&NUL, &NUL, 0, 0};
typedef struct SBTnode *SBT;
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
struct SBTnode *SBT_newnode(int value) {
    struct SBTnode *ret = malloc(sizeof(struct SBTnode));
    ret->ch[LT] = ret->ch[RT] = &NUL;
    ret->key = value;
    ret->size = 1;
    return ret;
}
void SBT_insert_node(SBT *rt, struct SBTnode *x) {
    if ((*rt) == (&NUL)) {
        *rt = x;
        return;
    }
    (*rt)->size++;
    SBT_insert_node(&((*rt)->ch[x->key > (*rt)->key]), x);
    SBT_maintain(rt, x->key > (*rt)->key);
}
void SBT_insert_value(SBT *rt, int value) {
    struct SBTnode *x = SBT_newnode(value);
    SBT_insert_node(rt, x);
}
struct SBTnode *SBT_get_delete(SBT *rt, int value) {
    if ((*rt) == &NUL) return &NUL;
    (*rt)->size--;
    if ((*rt)->key == value || (*rt)->ch[value > (*rt)->key] == &NUL) {
        struct SBTnode *todel;
        if ((*rt)->ch[LT] == &NUL || (*rt)->ch[RT] == &NUL) {
            todel = (*rt);
            *rt = (*rt)->ch[(*rt)->ch[RT] != &NUL];
        }
        else {
            todel = SBT_get_delete(&((*rt)->ch[1]),  value - 1);
            (*rt)->key = todel->key;
        }
        return todel;
    }
    else return SBT_get_delete(&((*rt)->ch[value > (*rt)->key]), value);
}
void SBT_delete(SBT *rt, int value) {
    struct SBTnode *x = SBT_get_delete(rt, value);
    free(x);
}
int SBT_rank(SBT rt, int value) {
    int ans = 0;
    while (rt != &NUL) {
        if (value > rt->key) {
            ans += rt->ch[LT]->size + 1;
            rt = rt->ch[RT];
        }
        else rt = rt->ch[LT];
    }
    return ans;
}
int SBT_num(SBT rt, int k) {
    while (1) {
        if (rt->ch[LT]->size + 1 == k) return rt->key;
        if (rt->ch[LT]->size >= k) rt = rt->ch[LT];
        else {
            k -= rt->ch[LT]->size + 1;
            rt = rt->ch[RT];
        }
    }
}
int SBT_pred(SBT rt, int value) {
    int ans = -1;
    while (rt != &NUL) {
        if (value > rt->key) {
            ans = rt->key;
            rt = rt->ch[RT];
        }
        else rt = rt->ch[LT];
    }
    return ans;
}
int SBT_succ(SBT rt, int value) {
    int ans = -1;
    while (rt != &NUL) {
        if (value < rt->key) {
            ans = rt->key;
            rt = rt->ch[LT];
        }
        else rt = rt->ch[RT];
    }
    return ans;
}
void SBT_print(SBT rt, int deep) {
    if (rt->ch[LT] != &NUL) SBT_print(rt->ch[LT], deep + 1);
    printf("(%d, %d) ", deep, rt->key);
    if (rt->ch[RT] != &NUL) SBT_print(rt->ch[RT], deep + 1);
}
int max(int x, int y) {
    if (x > y) return x;
    return y;
}
int SBT_deep(SBT rt) {
    int ret = 1;
    if (rt->ch[LT] != &NUL) ret = max(ret, SBT_deep(rt->ch[LT]) + 1);
    if (rt->ch[RT] != &NUL) ret = max(ret, SBT_deep(rt->ch[RT]) + 1);
    return ret;
}
void SBT_destruct(SBT rt) {
    if (rt->ch[LT] != &NUL) SBT_destruct(rt->ch[LT]);
    if (rt->ch[RT] != &NUL) SBT_destruct(rt->ch[RT]);
    free(rt);
}
int main() {
    SBT root = &NUL;
    int T;
    scanf("%d", &T);
    while (T--) {
        int op, x;
        scanf("%d%d", &op, &x);
        switch (op) {
            case 0 :
                SBT_insert_value(&root, x);
                break;
            case 1 :
                SBT_delete(&root, x);
                break;
            case 2 :
                printf("%d\n", SBT_num(root, x));
                break;
            case 3 :
                printf("%d\n", SBT_rank(root, x));
                break;
            case 4 :
                printf("%d\n", SBT_pred(root, x));
                break;
            case 5 :
                printf("%d\n", SBT_succ(root, x));
                break;
            default :
                break;
        }
    }
    SBT_destruct(root);
    return 0;
}
