#include <stdio.h>
#include <stdlib.h>

#include "treap.h"

#define NUM_OF_BITS 8

extern struct Treap_Node* Treap_Root;

static FILE* input;
static FILE* output_ans;
static FILE* output_compress;

struct my_struct
{
	double d;
	char c;
};

int cmp(void* a, void* b)
{
	struct my_struct *ap = (struct my_struct *)a,
					 *bp = (struct my_struct *)b;
	if (ap -> d != bp -> d)
	{
		return ap -> d < bp -> d ? -1 : 1;
	}
	if (ap -> c != bp -> c)
	{
		return ap -> c < bp -> c ? -1 : 1;
	}
	return 0;
}

void rec(struct Treap_Node* u)
{
	static int mask = 0;
	static int cnt = 0;

	if (u == NULL) return;
	fprintf(output_ans, "%d\n", u->size);
	if (u->ch[TREAP_LCH] != NULL) mask |= 1 << cnt;
	if (u->ch[TREAP_RCH] != NULL) mask |= 1 << (cnt + 1);
	cnt += 2;
	if (cnt == sizeof(int) * NUM_OF_BITS)
	{
		fwrite(&mask, sizeof(int), 1, output_compress);
		cnt = mask = 0;
	}
	rec(u->ch[TREAP_LCH]);
	rec(u->ch[TREAP_RCH]);
	if (u == Treap_Root && cnt > 0)
	{
		fwrite(&mask, sizeof(int), 1, output_compress);
	}
}

void encode(struct Treap_Node* u)
{
	if (u == NULL) return;
	int x;
	char c;
	x = (int)((((struct my_struct*)(u->valuep)) -> d) * 1000);
	c = ((struct my_struct*)(u->valuep)) -> c;
	fwrite(&x, sizeof(int), 1, output_compress);
	fwrite(&c, sizeof(char), 1, output_compress);
	encode(u->ch[TREAP_LCH]);
	encode(u->ch[TREAP_RCH]);
}

int main()
{
	input = fopen("data.in", "r");
	output_compress = fopen("compression.txt", "w");
	output_ans = fopen("data.ans", "w");
	int n;
	fscanf(input, "%d", &n);
	fwrite(&n, sizeof(int), 1, output_compress);
	struct my_struct* tmp;
	for (int i = 0; i < n; ++i)
	{
		double d;
		char c;
		fscanf(input, "%lf %c", &d, &c);
		tmp = (struct my_struct*)malloc(sizeof(struct my_struct));
		tmp -> d = d;
		tmp -> c = c;
		Treap_Insert(tmp, &cmp);
	}
	rec(Treap_Root);
	encode(Treap_Root);
	Treap_Destruct();
	fclose(input);
	fclose(output_compress);
	fclose(output_ans);
	return 0;
}
