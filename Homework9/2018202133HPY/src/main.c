#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "redblack-tree.h"

typedef struct {
    long long val;
    int num;
    int size;
} data_t;

void pushup(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    assert(node == tree->nil || ((data_t*)(node->data))->num != 0);
    ((data_t*)(node->data))->size = ((data_t*)(node->data))->num + ((data_t*)(node->l->data))->size + ((data_t*)(node->r->data))->size;
}

int cmp(const void* a, const void* b)
{
    return ((data_t*)a)->val - ((data_t*)b)->val;
}

void Add(long long x, redblack_tree_t* tree)
{
    data_t data = { .val = x, .num = 1, .size = 1 };

    redblack_tree_node_t* node;

    if ((node = rbtree_find(tree, &data)) == tree->nil) {
        rbtree_insert(tree, &data);
    } else {
        ((data_t*)(node->data))->num++;

        while (node != tree->nil) {
            rbtree_node_pushup(tree, node);
            node = node->parent;
        }
    }
}

void Del(long long x, redblack_tree_t* tree)
{
    data_t data = { .val = x, .num = 1, .size = 1 };

    redblack_tree_node_t *node, *tmp;

    node = rbtree_find(tree, &data);

    if (((data_t*)(node->data))->num > 1) {

        ((data_t*)(node->data))->num--;

        tmp = node;
        while (tmp != tree->nil) {
            rbtree_node_pushup(tree, tmp);
            tmp = tmp->parent;
        }
    } else {
        rbtree_erase(tree, node);
    }
}

int check_tree_size(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    if (node == tree->nil)
        return 1;
    assert(node == tree->nil || node->size == node->l->size + node->r->size + 1);

    return check_tree_size(tree, node->l) && check_tree_size(tree, node->r);
}

int check_tree_data(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    if (node == tree->nil)
        return 1;
    assert(((data_t*)(node->data))->size == ((data_t*)(node->data))->num + ((data_t*)(node->l->data))->size + ((data_t*)(node->r->data))->size);

    return check_tree_data(tree, node->l) && check_tree_data(tree, node->r);
}

long long Kth(int k, redblack_tree_t* tree)
{
    redblack_tree_node_t* now = tree->root;

    for (;;) {
        if (k <= ((data_t*)(now->l->data))->size) {
            now = now->l;
        } else {
            k -= ((data_t*)(now->l->data))->size;
            if (k <= ((data_t*)(now->data))->num) {
                return ((data_t*)now->data)->val;
            } else {
                k -= ((data_t*)(now->data))->num;
                now = now->r;
            }
        }
    }
}

int getLessThanNumNode(long long x, redblack_tree_node_t* node, redblack_tree_t* tree)
{
    if (node == tree->nil)
        return 0;

    int ret = 0;
    data_t data = { .val = x, .num = 1, .size = 1 };
    int flag = tree->cmp(node->data, &data);

    if (flag > 0) {
        ret = getLessThanNumNode(x, node->l, tree);
    } else if (flag < 0) {
        ret = ((data_t*)node->l->data)->size + ((data_t*)node->data)->num;
        ret += getLessThanNumNode(x, node->r, tree);
    } else {
        ret = ((data_t*)node->l->data)->size;
    }

    return ret;
}

int getLessThanNum(long long x, redblack_tree_t* tree)
{
    return getLessThanNumNode(x, tree->root, tree);
}

int getGreaterThanNumNode(long long x, redblack_tree_node_t* node, redblack_tree_t* tree)
{
    if (node == tree->nil)
        return 0;

    int ret = 0;
    data_t data = { .val = x, .num = 1, .size = 1 };
    int flag = tree->cmp(node->data, &data);

    if (flag < 0) {
        ret = getGreaterThanNumNode(x, node->r, tree);
    } else if (flag > 0) {
        ret = ((data_t*)node->r->data)->size + ((data_t*)node->data)->num;
        ret += getGreaterThanNumNode(x, node->l, tree);
    } else {
        ret = ((data_t*)node->r->data)->size;
    }

    return ret;
}

int getGreaterThanNum(long long x, redblack_tree_t* tree)
{
    return getGreaterThanNumNode(x, tree->root, tree);
}

long long getMaxLessThan(long long x, redblack_tree_t* tree)
{
    data_t data = { .val = x, .num = 1, .size = 1 };
    redblack_tree_node_t* node = rbtree_less(tree, &data);

    return node == tree->nil ? -1 : ((data_t*)(node->data))->val;
}

long long getMinGreaterThan(long long x, redblack_tree_t* tree)
{
    data_t data = { .val = x, .num = 1, .size = 1 };
    redblack_tree_node_t* node = rbtree_greater(tree, &data);

    return node == tree->nil ? -1 : ((data_t*)(node->data))->val;
}

void dfs_rbtree_node_data_t(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    if (node->l != tree->nil) {
        assert(node->l->parent == node);
        dfs_rbtree_node_data_t(tree, node->l);
    }

    printf("(%d %lld %d)\n", ((data_t*)(node->data))->num, ((data_t*)(node->data))->val, ((data_t*)(node->data))->size);

    if (node->r != tree->nil) {
        assert(node->r->parent == node);
        dfs_rbtree_node_data_t(tree, node->r);
    }
}

int main(void)
{
    redblack_tree_t tree = CONSTRUCT_RBTREE(data_t, cmp, pushup);

    ((data_t*)(tree.nil->data))->size = 0;
    ((data_t*)(tree.nil->data))->val = 0;

    long long n, x, y;

    scanf("%lld", &n);

    for (int i = 1; i <= n; i++) {
        scanf("%lld %lld", &x, &y);

        check_tree_size(&tree, tree.root);
        check_tree_data(&tree, tree.root);

        fprintf(stderr, "[%d] %lld %lld\n", i, x, y);

        switch (x) {
        case 0:
            Add(y, &tree);
            break;
        case 1:
            Del(y, &tree);
            break;
        case 2:
            printf("%lld\n", Kth(y, &tree));
            break;
        case 3:
            printf("%d\n", getLessThanNum(y, &tree));
            break;
        case 4:
            printf("%lld\n", getMaxLessThan(y, &tree));
            break;
        case 5:
            printf("%lld\n", getMinGreaterThan(y, &tree));
            break;
        }
    }

    return 0;
}
