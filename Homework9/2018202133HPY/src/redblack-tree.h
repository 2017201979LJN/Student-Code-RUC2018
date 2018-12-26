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

#define CONSTRUCT_RBTREE(type, cmp, pushup_func)                          \
    ({                                                                    \
        redblack_tree_t tree;                                             \
        tree.data_size = sizeof(type);                                    \
        tree.cmp = cmp;                                                   \
        tree.nil = malloc(sizeof(redblack_tree_node_t) + tree.data_size); \
        tree.nil->l = tree.nil;                                           \
        tree.nil->r = tree.nil;                                           \
        tree.nil->parent = tree.nil;                                      \
        tree.nil->color = RBTREE_BLACK;                                   \
        tree.nil->size = 0;                                               \
        tree.root = tree.nil;                                             \
        tree.pushup = pushup_func;                                        \
        tree;                                                             \
    })

typedef struct redblack_tree_node {
    struct redblack_tree_node *l, *r, *parent;
    int color;
    int size;
    char data[];
} redblack_tree_node_t;

typedef struct redblack_tree {
    size_t data_size;
    int (*cmp)(const void*, const void*);
    void (*pushup)(struct redblack_tree*, redblack_tree_node_t*);
    redblack_tree_node_t *root, *nil;
} redblack_tree_t;

void rbtree_node_pushup(redblack_tree_t* tree, redblack_tree_node_t* node);

void dfs_rbtree_node_int(redblack_tree_t* tree, redblack_tree_node_t* node);

void rbtree_insert_case(redblack_tree_t* tree, redblack_tree_node_t* node);

void rbtree_erase_case(redblack_tree_t* tree, redblack_tree_node_t* node);

redblack_tree_node_t* rbtree_insert(redblack_tree_t* tree, void* data);
void rbtree_erase(redblack_tree_t* tree, redblack_tree_node_t* node);
redblack_tree_node_t* rbtree_find(redblack_tree_t* tree, void* data);

redblack_tree_node_t* rbtree_greater(redblack_tree_t* tree, void* data);
redblack_tree_node_t* rbtree_less(redblack_tree_t* tree, void* data);
redblack_tree_node_t* rbtree_greater_equal(redblack_tree_t* tree, void* data);
redblack_tree_node_t* rbtree_less_equal(redblack_tree_t* tree, void* data);

#endif
