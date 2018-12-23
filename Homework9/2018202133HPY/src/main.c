#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "redblack-tree.h"

int cmp(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}

int main(void)
{
    redblack_tree_t tree;
    memset(&tree, 0, sizeof tree);

    tree.data_size = sizeof(int);
    tree.cmp = cmp;

    int a = 0;

    for (int i = 0; i < 10000; i++) {
        a = i & 1 ? i : -i;
        rbtree_insert(&tree, &a);
    }

    for (int i = 500; i < 1000; i++) {
        a = i & 1 ? i : -i;
        rbtree_erase(&tree, rbtree_find(&tree, &a));
    }

    // for (int i = 500; i < 1000; i++) {
    //     a = i & 1 ? i : -i;
    //     rbtree_insert(&tree, &a);
    // }

    dfs_rbtree_node_int(tree.root);

    printf("%d\n", tree.root->size);

    a = -681;
    printf("%d\n", *(int*)(rbtree_greater(&tree, &a)->data));

    a = -498;
    printf("%d\n", *(int*)(rbtree_find(&tree, &a)->data));

    return 0;
}
