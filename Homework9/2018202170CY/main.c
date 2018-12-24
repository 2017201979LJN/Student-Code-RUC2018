#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "avl.h"


int main()
{
	freopen("avl.in", "r", stdin);
	freopen("avl.out", "w", stdout);

	int n;
	int type, x;
	scanf("%d", &n);
	struct node *root = NULL;
	for (int i = 1; i <= n; i++)
	{
		scanf("%d%d", &type, &x);
		if (type == 0) root = insert(root, x);
		if (type == 1) root = delete_value(root, x);
		if (type == 2) printf("%d\n", k_th_small(root, x) );
		if (type == 3) printf("%d\n", smaller_than_x(root, x) );
		if (type == 4) printf("%d\n", biggest_one_smaller_than_x(root, x) );
		if (type == 5) printf("%d\n", smallest_one_bigger_than_x(root, x) );
	}

	fclose(stdin);
	fclose(stdout);
	return 0;
}
