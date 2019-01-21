#ifndef _SPLAY_H
#define _SPLAY_H

#define INF_POS 20000.000
#define INF_NEG -INF_POS


struct tree_node{
    int size; 
    double val;
    char c;
	struct tree_node *parent, *ch[2];
}*root;

int side(struct tree_node *x);

void update(struct tree_node *x);

void rotate(struct tree_node *x);

void splay(struct tree_node *x, struct tree_node *goal);

struct tree_node *find_kth(struct tree_node *x, int k);

int less_than(struct tree_node *x, double val);

int greater_than(struct tree_node *x, double val);

void insert_node(double val, char c);

void remove_node(double val);

void initialize_empty_tree();

void free_memory(struct tree_node *x);

void destruct_tree();

struct tree_node *getroot();

void check_dfs(struct tree_node *x);

#endif
