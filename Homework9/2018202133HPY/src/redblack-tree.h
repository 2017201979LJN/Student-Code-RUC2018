#ifndef _REDBLACK_TREE_H

#define _REDBLACK_TREE_H

#include <stddef.h>

#define RBTREE_RED 0
#define RBTREE_BLACK 1

#define RBTREE_NODE_GRANDPARENT(x) ((x)->parent->parent)

#define RBTREE_NODE_UNCLE(x)                        \
    ((RBTREE_NODE_GRANDPARENT(x)->l == (x)->parent) \
            ? RBTREE_NODE_GRANDPARENT(x)->r         \
            : RBTREE_NODE_GRANDPARENT(x)->l)

typedef struct redblack_tree_node {
    struct redblack_tree_node *l, *r, *parent;
    struct redblack_tree_node *prev, *next;
    int color;
    int size;
    char data[];
} redblack_tree_node_t;

void rbtree_node_pushup(redblack_tree_node_t* node);

typedef struct redblack_tree {
    size_t data_size;
    int (*cmp)(const void*, const void*);
    redblack_tree_node_t* root;
    redblack_tree_node_t* begin;
    redblack_tree_node_t* end;
} redblack_tree_t;

void dfs_rbtree_node_int(redblack_tree_node_t* node);

void rbtree_insert_case_1(redblack_tree_t* tree, redblack_tree_node_t* node);
void rbtree_insert_case_2(redblack_tree_t* tree, redblack_tree_node_t* node);
void rbtree_insert_case_3(redblack_tree_t* tree, redblack_tree_node_t* node);
void rbtree_insert_case_4(redblack_tree_t* tree, redblack_tree_node_t* node);
void rbtree_insert_case_5(redblack_tree_t* tree, redblack_tree_node_t* node);

void rbtree_erase_case_1(redblack_tree_t* tree, redblack_tree_node_t* node, redblack_tree_node_t* parent);
void rbtree_erase_case_2(redblack_tree_t* tree, redblack_tree_node_t* node, redblack_tree_node_t* parent);
void rbtree_erase_case_3(redblack_tree_t* tree, redblack_tree_node_t* node, redblack_tree_node_t* parent);
void rbtree_erase_case_4(redblack_tree_t* tree, redblack_tree_node_t* node, redblack_tree_node_t* parent);
void rbtree_erase_case_5(redblack_tree_t* tree, redblack_tree_node_t* node, redblack_tree_node_t* parent);

redblack_tree_node_t* rbtree_insert(redblack_tree_t* tree, void* data);
void rbtree_erase(redblack_tree_t* tree, redblack_tree_node_t* node);
redblack_tree_node_t* rbtree_find(redblack_tree_t* tree, void* data);

redblack_tree_node_t* rbtree_greater(redblack_tree_t* tree, void* data);
redblack_tree_node_t* rbtree_less(redblack_tree_t* tree, void* data);

#endif
