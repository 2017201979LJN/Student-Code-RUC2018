#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "sbt.h"
#include "compress.h"

void print_size(SBT rt, FILE *fp) {
	fprintf(fp, "%d ", rt->size);
    if (rt->ch[LT] != (&NUL)) print_size(rt->ch[LT], fp);
    if (rt->ch[RT] != (&NUL)) print_size(rt->ch[RT], fp);
}

int main() {
	FILE *input = fopen("data.in", "r");
	NUL.ch[LT] = NUL.ch[RT] = &NUL;
	NUL.key = NULL;
    NUL.size = 0;
	SBT root = &NUL;
	int n;
	fscanf(input, "%d", &n);
	for (int i = 0; i < n; i++) {
		struct data *value = malloc(sizeof(struct data));
		fscanf(input, "%lf%c", &value->d, &value->c);
		while (!islower(value->c)) fscanf(input, "%c", &value->c);
		SBT_insert_value(&root, value);
	}
	FILE *fpsize = fopen("size_pre.txt", "w");
	print_size(root, fpsize);
	fclose(fpsize);
	FILE *fp = fopen("data.bin", "wb");
	fwrite(&n, 1, sizeof(int), fp);
	CPS_encode_node_info(n, root, fp);
	CPS_encode_son_info(n, root, fp);
	fclose(fp);
	SBT_destruct(root);
	root = &NUL;
	fp = fopen("data.bin", "rb");
	CPS_decode(&root, fp);
	fpsize = fopen("size_after.txt", "w");
	print_size(root, fpsize);
	fclose(fpsize);
	fclose(fp);
	return 0;
}
