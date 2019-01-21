#include "avl.h"

double Value(struct node *tree)
{
	if (tree == NULL) return -1;
	return tree -> data -> value1;
}
int Size(struct node *tree)
{
	if (tree == NULL) return 0;
	return tree -> size;
}
struct node *newnode(struct Data data, struct node *left, struct node *right)
{
	struct node *new;
	struct Data *newdata;
	new = (struct node *) malloc( sizeof (struct node) );
	newdata = (struct Data *) malloc( sizeof (struct Data) );
	newdata -> value1 = data.value1;
	newdata -> value2 = data.value2;
	new -> data = newdata;
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
struct node *insert(struct node *tree, struct Data data)
{
	if (tree == NULL)
	{
		tree = newnode(data, NULL, NULL);
	}
	else if (data.value1 <= tree -> data -> value1)
	{
		tree -> left = insert(tree -> left, data);
		if ((get_height(tree -> left) - get_height(tree -> right) )== 2)
		{
			if (data.value1 < Value(tree -> left) )
				tree = left_left_rotate(tree);
			else
				tree = left_right_rotate(tree);
		}
	}
	else if (data.value1 > tree -> data -> value1)
	{
		tree -> right = insert(tree -> right, data);
		if ( (get_height(tree -> right) - get_height(tree -> left) ) == 2)
		{
			if (data.value1 > Value(tree -> right) )
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
struct node *find(struct node *tree, double value)
{
	if (tree == NULL) return NULL;
	if (tree -> data -> value1 == value) return tree;
	else if (value < tree -> data -> value1) return find(tree -> left, value);
	else return find(tree -> right, value);
}
struct node *_delete(struct node *tree, struct node *d)
{
	if (tree == NULL || d == NULL)
		return NULL;
	if (d -> data -> value1 < tree -> data -> value1)
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
	else if (d -> data -> value1 > tree -> data -> value1)
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
				tree -> data -> value1 = Max_in_left -> data -> value1;
				tree -> left = _delete(tree -> left, Max_in_left);
			}
			else
			{
				struct node *Min_in_right = get_min(tree -> right);
				tree -> data -> value1 = Min_in_right -> data -> value1;
				tree -> right = _delete(tree -> right, Min_in_right);
			}
		}
		else
		{
			struct node *temp = tree;
			tree = (tree -> left != NULL) ? tree -> left : tree -> right;
			free(temp -> data);
			free(temp);
		}
	}
	pushup(tree);
	return tree;
}
struct node *delete_value(struct node *root, double value)
{
	struct node *d = find(root, value);
	if (d == NULL) return NULL;
	root = _delete(root, d);
	return root;
}
int k_th_small(struct node *tree, int k)
{
	if (tree == NULL) return -1;
	if (k == Size(tree -> left) + 1) return tree -> data -> value1;
	if (k <= Size(tree -> left) ) return k_th_small(tree -> left, k);
	else return k_th_small(tree -> right, k - Size(tree -> left) - 1);
}
int smaller_than_x(struct node *tree, double x)
{
	if (tree == NULL) return 0;
	if (tree -> data -> value1 >= x) return smaller_than_x(tree -> left, x);
	else return Size(tree -> left) + 1 + smaller_than_x(tree -> right, x);
}
double _Max(double a, double b)
{
	return a < b ? b : a;
}
double biggest_one_smaller_than_x(struct node *tree, double x)
{
	if (tree == NULL) return -1;
	if (tree -> data -> value1 >= x) return biggest_one_smaller_than_x(tree -> left, x);
	else return _Max(tree -> data -> value1, biggest_one_smaller_than_x(tree -> right, x));
}
double smallest_one_bigger_than_x(struct node *tree, double x)
{
	if (tree == NULL) return -1;
	if (tree -> data -> value1 <= x) return smallest_one_bigger_than_x(tree -> right, x);
	else
	{
		int v = smallest_one_bigger_than_x(tree -> left, x);
		return v == -1 ? tree -> data -> value1 : v;
	}
}
