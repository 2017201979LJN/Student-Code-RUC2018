#ifndef SBT_H
#define SBT_H

#define LT 0
#define RT 1

struct data {
	double d;
	char c;
};
struct SBTnode {
	struct SBTnode *ch[2];
	int size;
	void *key;
}NUL;
typedef struct SBTnode *SBT;

void SBT_rotate(SBT *rt, int type);
void SBT_maintain(SBT *rt, int flag);
void SBT_insert_node(SBT *rt, struct SBTnode *x);

struct SBTnode *SBT_newnode(void *data);
void SBT_insert_value(SBT *rt, void *data);
struct SBTnode *SBT_get_delete(SBT *rt, void *value);
void SBT_delete(SBT *rt, void *value);
int SBT_rank(SBT rt, void *value);
void *SBT_num(SBT rt, int k);
void *SBT_pred(SBT rt, void *value);
void *SBT_succ(SBT rt, void *value);
void SBT_destruct(SBT rt);

#endif