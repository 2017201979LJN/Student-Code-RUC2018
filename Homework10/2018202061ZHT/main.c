#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "AVL.h"
int n;
void output (struct node *x, FILE *op) {
	if (x == nil) return;
	fwrite (&(x->d.v1), sizeof (int), 1, op);
	char v2 = x->d.v2;
	v2 <<= 2;
	if (x->ch[0] != nil) v2 |= 1;
	if (x->ch[1] != nil) v2 |= 2;
	fwrite (&v2, sizeof (char), 1, op);
	output (x->ch[0], op);
	output (x->ch[1], op);
}
int main() {
	freopen ("tree.in", "r", stdin);
	scanf ("%d", &n);
	nil = newnode ();
	root = nil;
	for (int i = 1; i <= n; i++) {
		double v1;char v2;
		struct data d;
		scanf ("%lf%hhd", &v1, &v2);
		d.v1 = v1; d.v2=v2;
		root = insert (root, d);
	}
	fclose(stdin);
	FILE *op = fopen ("tree.out", "w");
	output (root, op);
	fclose (op);
	return 0;
}
