#ifndef SBT_H
#define SBT_H

#include <stdio.h>
#include <malloc.h>

#define LT 0
#define RT 1

struct SBTnode {
    struct SBTnode *ch[2];
    int size, key;
}NUL;
typedef struct SBTnode *SBT;

void SBT_rotate(SBT *rt, int type);
void SBT_maintain(SBT *rt, int flag);
void SBT_insert_node(SBT *rt, struct SBTnode *x);

struct SBTnode *SBT_newnode(int value);
void SBT_insert_value(SBT *rt, int value);
struct SBTnode *SBT_get_delete(SBT *rt, int value);
void SBT_delete(SBT *rt, int value);
int SBT_rank(SBT rt, int value);
int SBT_num(SBT rt, int k);
int SBT_pred(SBT rt, int value);
int SBT_succ(SBT rt, int value);
void SBT_print(SBT rt, int deep);
int max(int x, int y);
int SBT_deep(SBT rt);
void SBT_destruct(SBT rt);

#endif
