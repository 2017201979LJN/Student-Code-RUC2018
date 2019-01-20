#include <stdio.h>
#include <malloc.h>
#include "compress.h"
#include "sbt.h"

void CPS_dfs_son(SBT rt, unsigned char *buf, int *pos, FILE *fp) {
    if (rt->ch[LT] != (&NUL)) (*buf) += (1 << (*pos));
    (*pos)++;
    if (rt->ch[RT] != (&NUL)) (*buf) += (1 << (*pos));
    (*pos)++;
    if ((*pos) == NUM_OF_BYTE) {
        fwrite(buf, 1, sizeof(unsigned char), fp);
        *pos = 0;
        *buf = 0;
    }
    if (rt->ch[LT] != (&NUL)) CPS_dfs_son(rt->ch[LT], buf, pos, fp);
    if (rt->ch[RT] != (&NUL)) CPS_dfs_son(rt->ch[RT], buf, pos, fp);
}
void CPS_encode_son_info(int n, SBT rt, FILE *fp) {
    if (rt == (&NUL)) return;
    unsigned char *buf = malloc(sizeof(unsigned char));
    *buf = 0;
    int *pos = malloc(sizeof(int));
    *pos = 0;
    CPS_dfs_son(rt, buf, pos, fp);
    if (*pos) fwrite(buf, 1, sizeof(unsigned char), fp);
}
void CPS_dfs_node(SBT rt, FILE *fp) {
	int tmp = ((struct data *)rt->key)->d * 1000;
	fwrite(&tmp, 1, sizeof(int), fp);
	fwrite(&((struct data *)rt->key)->c, 1, sizeof(unsigned char), fp);
    if (rt->ch[LT] != (&NUL)) CPS_dfs_node(rt->ch[LT], fp);
    if (rt->ch[RT] != (&NUL)) CPS_dfs_node(rt->ch[RT], fp);
}
void CPS_encode_node_info(int n, SBT rt, FILE *fp) {
    if (rt == (&NUL)) return;
    CPS_dfs_node(rt, fp);
}
void CPS_dfs_build(struct SBTnode **node, int rt, int *lst, unsigned char *buf, int *pos, FILE *fp) {
	if ((*pos) == 0) {
		fread(buf, 1, sizeof(unsigned char), fp);
		*pos = 8;
	}
	int hasl = (*buf) & 0x01, hasr = (*buf) & 0x02;
	(*pos) -= 2;
	(*buf) >>= 2;
	node[rt]->size = 1;
	if (hasl) {
		node[rt]->ch[LT] = node[++(*lst)];
		CPS_dfs_build(node, *lst, lst, buf, pos, fp);
		node[rt]->size += node[rt]->ch[LT]->size;
	}
	if (hasr) {
		node[rt]->ch[RT] = node[++(*lst)];
		CPS_dfs_build(node, *lst, lst, buf, pos, fp);
		node[rt]->size += node[rt]->ch[RT]->size;
	}
}
void CPS_decode(SBT *rt, FILE *fp) {
	int n;
	fread(&n, 1, sizeof(int), fp);
	struct SBTnode **node = malloc(sizeof(struct SBTnode *) * n);
	for (int i = 0; i < n; i++) {
		node[i] = malloc(sizeof(struct SBTnode));
		node[i]->key = malloc(sizeof(struct data));
		node[i]->ch[LT] = node[i] -> ch[RT] = (&NUL);
		int tmp;
		fread(&tmp, 1, sizeof(int), fp);
		((struct data *)node[i]->key)->d = tmp / 1000.0;
		fread(&((struct data *)node[i]->key)->c, 1, sizeof(unsigned char), fp);
	}
	int *lst = malloc(sizeof(int));
	*lst = 0;
	unsigned char *buf = malloc(sizeof(unsigned char));
	int *pos = malloc(sizeof(int));
	*pos = 0;
	CPS_dfs_build(node, 0, lst, buf, pos, fp);
	*rt = node[0];
}
