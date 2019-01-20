#ifndef AVLTREE
#define AVLTREE

#include <stdio.h>
#include <stdlib.h>

struct Data
{
	double value1;
	char value2;
};
struct node
{
	int height, size;
	struct Data *data;
	struct node *left, *right;
};
double Value(struct node *tree);
int Size(struct node *tree);
struct node *newnode(struct Data data, struct node *left, struct node *right);
int Max(int a, int b);
int Height(struct node *tree);
int get_height(struct node *tree);
void pushup(struct node *tree);
struct node *left_left_rotate(struct node *roo);
struct node *right_right_rotate(struct node *roo);
struct node *left_right_rotate(struct node *tree);
struct node *right_left_rotate(struct node *tree);
struct node *insert(struct node *tree, struct Data data);
struct node *get_max(struct node *tree);
struct node *get_min(struct node *tree);
struct node *find(struct node *tree, double value);
struct node *_delete(struct node *tree, struct node *d);
struct node *delete_value(struct node *root, double value);
int k_th_small(struct node *tree, int k);
int smaller_than_x(struct node *tree, double x);
double biggest_one_smaller_than_x(struct node *tree, double x);
double smallest_one_bigger_than_x(struct node *tree, double x);

#endif
