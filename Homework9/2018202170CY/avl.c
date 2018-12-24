#include "avl.h"
/*struct node
{
	int value, height, size;
	struct node *left, *right;
};*/
int Value(struct node *tree)
{
	if (tree == NULL) return -1;
	return tree -> value;
}
int Size(struct node *tree)
{
	if (tree == NULL) return 0;
	return tree -> size;
}
struct node *newnode(int value, struct node *left, struct node *right)
{
	struct node *new;
	new = (struct node *) malloc( sizeof (struct node) );
	new -> value = value;
	new -> height = 0;
	new -> size = Size(left) + Size(right) + 1;
	new -> left = left;
	new -> right = right;
	return new;
}
int Max(int a, int b)
{
	return a > b ? a : b;
}
int Height(struct node *tree)
{
	if (tree == NULL) return -1;
	return tree -> height;
}
int get_height(struct node *tree)
{
	if (tree == NULL) return -1;
	return Max( Height(tree -> left), Height(tree -> right));
}
void pushup(struct node *tree)
{
	if (tree == NULL) return ;
	tree -> height = get_height(tree);
	tree -> size = Size(tree -> left) + Size(tree -> right) + 1;
}
struct node *left_left_rotate(struct node *roo)
{
	struct node *left;
	left = roo -> left;
	roo -> left = left -> right;
	left -> right = roo;
	roo -> height = get_height(roo);
	left -> height = Max(get_height(left -> left), roo -> height) + 1;
	pushup(roo);
	pushup(left);
	return left;
}
struct node *right_right_rotate(struct node *roo)
{
	struct node *right;
	right = roo -> right;
	roo -> right = right -> left;
	right -> left = roo;
	roo -> height = get_height(roo);
	right -> height = Max(get_height(right -> right), roo -> height) + 1;
	pushup(roo);
	pushup(right);
	return right;
}
struct node *left_right_rotate(struct node *tree)
{
	tree -> left = right_right_rotate(tree -> left);
	return left_left_rotate(tree);
}
struct node *right_left_rotate(struct node *tree)
{
	tree -> right = left_left_rotate(tree -> right);
	return right_right_rotate(tree);
}
struct node *insert(struct node *tree, int value)
{
	if (tree == NULL)
	{
		tree = newnode(value, NULL, NULL);
	}
	else if (value <= tree -> value)
	{
		tree -> left = insert(tree -> left, value);
		if ((get_height(tree -> left) - get_height(tree -> right) )== 2)
		{
			if (value < Value(tree -> left) )
				tree = left_left_rotate(tree);
			else
				tree = left_right_rotate(tree);
		}
	}
	else if (value > tree -> value)
	{
		tree -> right = insert(tree -> right, value);
		if ( (get_height(tree -> right) - get_height(tree -> left) ) == 2)
		{
			if (value > Value(tree -> right) )
				tree = right_right_rotate(tree);
			else
				tree = right_left_rotate(tree);
		}
	}
	pushup(tree);
	return tree;
}
struct node *get_max(struct node *tree)
{
	if (tree -> right == NULL) return tree;
	return get_max(tree -> right);
}
struct node *get_min(struct node *tree)
{
	if (tree -> left == NULL) return tree;
	return get_min(tree -> left);
}
struct node *find(struct node *tree, int value)
{
	if (tree == NULL) return NULL;
	if (tree -> value == value) return tree;
	else if (value < tree -> value) return find(tree -> left, value);
	else return find(tree -> right, value);
}
struct node *_delete(struct node *tree, struct node *d)
{
//	if (tree -> value == 972325001 && d -> value == 972325001) printf("aa\n");
	if (tree == NULL || d == NULL)
		return NULL;
	if (d -> value < tree -> value)
	{
		tree -> left = _delete(tree -> left, d);
		if (get_height(tree -> right) - get_height(tree -> left) == 2)
		{
			struct node *right = tree -> right;
			if (get_height(right -> left) > get_height(right -> right))
				tree = right_left_rotate(tree);
			else
				tree = right_right_rotate(tree);
		}
	}
	else if (d -> value > tree -> value)
	{
		tree -> right = _delete(tree -> right, d);
		if (get_height(tree -> left) - get_height(tree -> right) == 2)
		{
			struct node *left = tree -> left;
			if (get_height(left -> right) > get_height(left -> left))
				tree = left_right_rotate(tree);
			else
				tree = left_left_rotate(tree);
		}
	}
	else
	{
		if ((tree -> left != NULL) && (tree -> right != NULL))
		{
			if (get_height(tree -> left) > get_height(tree -> right))
			{
				struct node *Max_in_left = get_max(tree -> left);
				tree -> value = Max_in_left -> value;
				tree -> left = _delete(tree -> left, Max_in_left);
			}
			else
			{
				struct node *Min_in_right = get_min(tree -> right);
				tree -> value = Min_in_right -> value;
				tree -> right = _delete(tree -> right, Min_in_right);
			}
		}
		else
		{
			struct node *temp = tree;
			tree = (tree -> left != NULL) ? tree -> left : tree -> right;
			free(temp);
		}
	}
	pushup(tree);
	return tree;
}
struct node *delete_value(struct node *root, int value)
{
	struct node *d = find(root, value);
	if (d == NULL) return NULL;
	root = _delete(root, d);
	return root;
}
int k_th_small(struct node *tree, int k)
{
	if (tree == NULL) return -1;
	if (k == Size(tree -> left) + 1) return tree -> value;
	if (k <= Size(tree -> left) ) return k_th_small(tree -> left, k);
	else return k_th_small(tree -> right, k - Size(tree -> left) - 1);
}
int smaller_than_x(struct node *tree, int x)
{
	if (tree == NULL) return 0;
	if (tree -> value >= x) return smaller_than_x(tree -> left, x);
	else return Size(tree -> left) + 1 + smaller_than_x(tree -> right, x);
}
int biggest_one_smaller_than_x(struct node *tree, int x)
{
	if (tree == NULL) return -1;
	if (tree -> value >= x) return biggest_one_smaller_than_x(tree -> left, x);
	else return Max(tree -> value, biggest_one_smaller_than_x(tree -> right, x));
}
int smallest_one_bigger_than_x(struct node *tree, int x)
{
	if (tree == NULL) return -1;
	if (tree -> value <= x) return smallest_one_bigger_than_x(tree -> right, x);
	else
	{
		int v = smallest_one_bigger_than_x(tree -> left, x);
		return v == -1 ? tree -> value : v;
	}
}
