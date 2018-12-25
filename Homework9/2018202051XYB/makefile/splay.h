#ifndef _SPLAY_H
#define _SPLAY_H

#include<stdlib.h>
#include<stdio.h>

#define INF_POS 2000000000
#define INF_NEG -INF_POS

struct tree_node{
	int size, val;
	struct tree_node *parent, *ch[2];
};

int side(struct tree_node *x);

void update(struct tree_node *x);

void rotate(struct tree_node *x);

void splay(struct tree_node *x, struct tree_node *goal);

struct tree_node *find_kth(struct tree_node *x, int k);

int less_than(struct tree_node *x, int val);

int greater_than(struct tree_node *x, int val);

void insert_node(int val);

void remove_node(int val);

void pretreatment();

void free_memory(struct tree_node *x);

struct tree_node *get_root();

#endif
