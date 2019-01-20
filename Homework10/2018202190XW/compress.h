#include "sbt.h"
#define NUM_OF_BYTE 8

void CPS_dfs_son(SBT rt, unsigned char *buf, int *pos, FILE *fp);
void CPS_dfs_node(SBT rt, FILE *fp);
void CPS_encode_son_info(int n, SBT rt, FILE *fp);
void CPS_encode_node_info(int n, SBT rt, FILE *fp);
void CPS_dfs_build(struct SBTnode **node, int rt, int *lst, unsigned char *buf, int *pos, FILE *fp);
void CPS_decode(SBT *rt, FILE *fp);
