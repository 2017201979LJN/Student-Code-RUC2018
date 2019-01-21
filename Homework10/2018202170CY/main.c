#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "avl.h"

void out(struct node *tree, FILE *op)
{
	if (tree == NULL) return ;
	int dd = (int)tree -> data -> value1;
	fwrite(&dd, sizeof(int), 1, op);
	fwrite(&(tree -> data -> value2), sizeof(char), 1, op);
	char a = (tree -> left != NULL) * 2;
	a |= (tree -> right != NULL) & 1;
	fwrite(&a, sizeof(char), 1, op);
	out(tree -> left, op);
	out(tree -> right, op);
	return ;
}
void debug(struct node *tree)
{
	if (tree == NULL) return ;
	printf("%lf %d ", tree -> data -> value1, tree -> data -> value2);
	printf("%d %d ", tree -> height, tree -> size);
	printf("%d %d\n", tree -> left != NULL, tree -> right != NULL);
	debug(tree -> left);
	debug(tree -> right);
}
int main()
{
	freopen("data.in", "r", stdin);
	int n;
	scanf("%d", &n);
	struct Data data;
	struct node *root = NULL;
	for(int i = 1; i <= n; i++)
	{
		scanf("%lf %hhd", &data.value1, &data.value2);
		data.value1 *= 1000;
		root = insert(root, data);
	}
	fclose(stdin);
	//debug(root);
	//return 0;
	FILE *op = fopen("result.txt", "w");
	out(root, op);
	fclose(op);
}
