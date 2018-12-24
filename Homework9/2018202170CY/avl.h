#ifndef AVLTREE
#define AVLTREE

#include <stdio.h>
#include <malloc.h>

struct node
{
	int value, height, size;
	struct node *left, *right;
};
int Value(struct node *tree);
int Size(struct node *tree);
struct node *newnode(int value, struct node *left, struct node *right);
int Max(int a, int b);
int Height(struct node *tree);
int get_height(struct node *tree);
void pushup(struct node *tree);
struct node *left_left_rotate(struct node *roo);
struct node *right_right_rotate(struct node *roo);
struct node *left_right_rotate(struct node *tree);
struct node *right_left_rotate(struct node *tree);
struct node *insert(struct node *tree, int value);
struct node *get_max(struct node *tree);
struct node *get_min(struct node *tree);
struct node *find(struct node *tree, int value);
struct node *_delete(struct node *tree, struct node *d);
struct node *delete_value(struct node *root, int value);
int k_th_small(struct node *tree, int k);
int smaller_than_x(struct node *tree, int x);
int biggest_one_smaller_than_x(struct node *tree, int x);
int smallest_one_bigger_than_x(struct node *tree, int x);

#endif
