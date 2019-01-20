#ifndef _TREE_H

#define _TREE_H

struct tree_data {
    int d;
    char c;
};

typedef struct tree_data tree_data_t;

struct tree_node {
    struct tree_node *ls, *rs;
    int siz;
    tree_data_t data;
};

typedef struct tree_node tree_node_t;

void tree_insert_node(tree_node_t** ppnode, tree_data_t data);

int tree_size_hash(tree_node_t* pnode);

#endif
