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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef NULL

redblack_tree_node_t* rbtree_new_node(redblack_tree_t* tree, size_t data_siz)
{
    redblack_tree_node_t* node = (redblack_tree_node_t*)malloc(sizeof(redblack_tree_node_t) + data_siz);
    node->color = RBTREE_RED;
    node->l = node->r = node->parent = tree->nil;
    node->size = 1;
    return node;
}

void rbtree_node_zig(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    assert(node != tree->nil && node->parent == tree->nil || node->parent->l == node);

    redblack_tree_node_t* grandparent = RBTREE_NODE_GRANDPARENT(node);
    redblack_tree_node_t* parent = node->parent;
    redblack_tree_node_t* s = node->r;

    parent->l = s;

    if (s != tree->nil)
        s->parent = parent;
    node->r = parent;
    parent->parent = node;

    if (tree->root == parent)
        tree->root = node;
    node->parent = grandparent;

    if (grandparent != tree->nil) {
        if (grandparent->l == parent)
            grandparent->l = node;
        else
            grandparent->r = node;
    }

    rbtree_node_pushup(tree, parent);
    rbtree_node_pushup(tree, node);
}

void rbtree_node_zag(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    assert(node != tree->nil && node->parent == tree->nil || node->parent->r == node);

    redblack_tree_node_t* grandparent = RBTREE_NODE_GRANDPARENT(node);
    redblack_tree_node_t* parent = node->parent;
    redblack_tree_node_t* s = node->l;

    parent->r = s;

    if (s != tree->nil)
        s->parent = parent;
    node->l = parent;
    parent->parent = node;

    if (tree->root == parent)
        tree->root = node;
    node->parent = grandparent;

    if (grandparent != tree->nil) {
        if (grandparent->r == parent)
            grandparent->r = node;
        else
            grandparent->l = node;
    }

    rbtree_node_pushup(tree, parent);
    rbtree_node_pushup(tree, node);
}

void rbtree_node_pushup(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    node->size = node != tree->nil;
    node->size += node->l->size;
    node->size += node->r->size;
    if (tree->pushup) {
        tree->pushup(tree, node);
    }
}

void dfs_rbtree_node_int(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    if (node->l != tree->nil) {
        assert(node->l->parent == node);
        dfs_rbtree_node_int(tree, node->l);
    }

    printf("%d ", *(int*)(&(node->data)));

    if (node->r != tree->nil) {
        assert(node->r->parent == node);
        dfs_rbtree_node_int(tree, node->r);
    }
}

redblack_tree_node_t* rbtree_max(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    while (node->r != tree->nil)
        node = node->r;
    return node;
}

redblack_tree_node_t* rbtree_min(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    while (node->l != tree->nil)
        node = node->l;
    return node;
}

redblack_tree_node_t* rbtree_find(redblack_tree_t* tree, void* data)
{
    redblack_tree_node_t* now = tree->root;

    while (now != tree->nil) {
        int flag = tree->cmp(now->data, data);
        if (flag < 0) {
            now = now->r;
        } else if (flag == 0) {
            break;
        } else {
            now = now->l;
        }
    }

    return now;
}

redblack_tree_node_t* rbtree_greater_node(redblack_tree_t* tree, redblack_tree_node_t* now, void* data)
{
    int flag;

    if (now == tree->nil)
        return tree->nil;

    flag = tree->cmp(now->data, data);

    if (flag <= 0) {
        return rbtree_greater_node(tree, now->r, data);
    } else {
        redblack_tree_node_t* ret = rbtree_greater_node(tree, now->l, data);
        if (ret == tree->nil)
            ret = now;
        return ret;
    }
}

redblack_tree_node_t* rbtree_greater(redblack_tree_t* tree, void* data)
{
    return rbtree_greater_node(tree, tree->root, data);
}

redblack_tree_node_t* rbtree_less_node(redblack_tree_t* tree, redblack_tree_node_t* now, void* data)
{
    int flag;

    if (now == tree->nil)
        return tree->nil;

    flag = tree->cmp(now->data, data);

    if (flag >= 0) {
        return rbtree_less_node(tree, now->l, data);
    } else {
        redblack_tree_node_t* ret = rbtree_less_node(tree, now->r, data);
        if (ret == tree->nil)
            ret = now;
        return ret;
    }
}

redblack_tree_node_t* rbtree_less(redblack_tree_t* tree, void* data)
{
    return rbtree_less_node(tree, tree->root, data);
}

redblack_tree_node_t* rbtree_greater_equal_node(redblack_tree_t* tree, redblack_tree_node_t* now, void* data)
{
    int flag;

    if (now == tree->nil)
        return tree->nil;

    flag = tree->cmp(now->data, data);

    if (flag < 0) {
        return rbtree_greater_equal_node(tree, now->r, data);
    } else {
        redblack_tree_node_t* ret = rbtree_greater_equal_node(tree, now->l, data);
        if (ret == tree->nil)
            ret = now;
        return ret;
    }
}

redblack_tree_node_t* rbtree_greater_equal(redblack_tree_t* tree, void* data)
{
    return rbtree_greater_equal_node(tree, tree->root, data);
}

redblack_tree_node_t* rbtree_less_equal_node(redblack_tree_t* tree, redblack_tree_node_t* now, void* data)
{
    int flag;

    if (now == tree->nil)
        return tree->nil;

    flag = tree->cmp(now->data, data);

    if (flag > 0) {
        return rbtree_less_equal_node(tree, now->l, data);
    } else {
        redblack_tree_node_t* ret = rbtree_less_equal_node(tree, now->r, data);
        if (ret == tree->nil)
            ret = now;
        return ret;
    }
}

redblack_tree_node_t* rbtree_less_equal(redblack_tree_t* tree, void* data)
{
    return rbtree_less_equal_node(tree, tree->root, data);
}

void rbtree_insert_case(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    if (node->parent == tree->nil) {
        tree->root = node;
        node->color = RBTREE_BLACK;
        return;
    }
    if (node->parent->color == RBTREE_RED) {
        if (RBTREE_NODE_UNCLE(node)->color == RBTREE_RED) {
            node->parent->color = RBTREE_NODE_UNCLE(node)->color = RBTREE_BLACK;
            RBTREE_NODE_GRANDPARENT(node)->color = RBTREE_RED;
            rbtree_insert_case(tree, RBTREE_NODE_GRANDPARENT(node));
        } else {
            if (node->parent->r == node && RBTREE_NODE_GRANDPARENT(node)->l == node->parent) {
                rbtree_node_zag(tree, node);
                rbtree_node_zig(tree, node);
                node->color = RBTREE_BLACK;
                node->l->color = node->r->color = RBTREE_RED;
            } else if (node->parent->l == node && RBTREE_NODE_GRANDPARENT(node)->r == node->parent) {
                rbtree_node_zig(tree, node);
                rbtree_node_zag(tree, node);
                node->color = RBTREE_BLACK;
                node->l->color = node->r->color = RBTREE_RED;
            } else if (node->parent->l == node && RBTREE_NODE_GRANDPARENT(node)->l == node->parent) {
                node->parent->color = RBTREE_BLACK;
                RBTREE_NODE_GRANDPARENT(node)->color = RBTREE_RED;
                rbtree_node_zig(tree, node->parent);
            } else if (node->parent->r == node && RBTREE_NODE_GRANDPARENT(node)->r == node->parent) {
                node->parent->color = RBTREE_BLACK;
                RBTREE_NODE_GRANDPARENT(node)->color = RBTREE_RED;
                rbtree_node_zag(tree, node->parent);
            }
        }
    }
}

redblack_tree_node_t* rbtree_insert(redblack_tree_t* tree, void* data)
{
    redblack_tree_node_t** ppnow = &(tree->root);
    redblack_tree_node_t* parent = tree->nil;

    while (*ppnow != tree->nil) {
        int flag = tree->cmp((*ppnow)->data, data);
        if (flag < 0) {
            parent = *ppnow;
            ppnow = &((*ppnow)->r);
        } else if (flag == 0) {
            return tree->nil;
        } else {
            parent = *ppnow;
            ppnow = &((*ppnow)->l);
        }
    }

    *ppnow = rbtree_new_node(tree, tree->data_size);
    memcpy((*ppnow)->data, data, tree->data_size);
    (*ppnow)->parent = parent;

    while (parent != tree->nil) {
        rbtree_node_pushup(tree, parent);
        parent = parent->parent;
    }

    parent = *ppnow;

    // while (parent->parent != tree->nil) {
    //     if (parent == parent->parent->l)
    //         rbtree_node_zig(tree, parent);
    //     else
    //         rbtree_node_zag(tree, parent);
    // }

    //rbtree_insert_case(tree, *ppnow);

    return *ppnow;
}

void rbtree_erase(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    assert(node != tree->nil);
    if (node->l != tree->nil && node->r != tree->nil) {
        redblack_tree_node_t* tmp = rbtree_max(tree, node->l);
        memcpy(node->data, tmp->data, tree->data_size);
        node = tmp;
    }

    redblack_tree_node_t* child = (node->r == tree->nil) ? node->l : node->r;

    if (node->parent->l == node)
        node->parent->l = child;
    else
        node->parent->r = child;

    child->parent = node->parent;

    free(node);

    rbtree_node_pushup(tree, child);

    node = child->parent;

    while (node != tree->nil) {
        rbtree_node_pushup(tree, node);
        node = node->parent;
    }

    if (child->parent == tree->nil) {
        tree->root = child;
    }
    /*
    if (node->color == RBTREE_BLACK) {
        if (child->color == RBTREE_RED)
            child->color = RBTREE_BLACK;
        else
            rbtree_erase_case(tree, child);
    }
    */
}

void rbtree_erase_case(redblack_tree_t* tree, redblack_tree_node_t* node)
{
#define sibling(t) (t->parent->l == t ? t->parent->r : t->parent->l)
    if (node->parent == tree->nil) {
        node->color = RBTREE_BLACK;
        return;
    }
    if (sibling(node)->color == RBTREE_RED) {
        node->parent->color = RBTREE_RED;
        sibling(node)->color = RBTREE_BLACK;
        if (node == node->parent->l)
            rbtree_node_zag(tree, sibling(node));
        else
            rbtree_node_zig(tree, sibling(node));
    }
    if (node->parent->color == RBTREE_BLACK && sibling(node)->color == RBTREE_BLACK
        && sibling(node)->l->color == RBTREE_BLACK && sibling(node)->r->color == RBTREE_BLACK) {
        sibling(node)->color = RBTREE_RED;
        rbtree_erase_case(tree, node->parent);
    } else if (node->parent->color == RBTREE_RED && sibling(node)->color == RBTREE_BLACK
        && sibling(node)->l->color == RBTREE_BLACK && sibling(node)->r->color == RBTREE_BLACK) {
        sibling(node)->color = RBTREE_RED;
        node->parent->color = RBTREE_BLACK;
    } else {
        if (sibling(node)->color == RBTREE_BLACK) {
            if (node == node->parent->l && sibling(node)->l->color == RBTREE_RED
                && sibling(node)->r->color == RBTREE_BLACK) {
                sibling(node)->color = RBTREE_RED;
                sibling(node)->l->color = RBTREE_BLACK;
                rbtree_node_zig(tree, sibling(node)->l);
            } else if (node == node->parent->r && sibling(node)->l->color == RBTREE_BLACK
                && sibling(node)->r->color == RBTREE_RED) {
                sibling(node)->color = RBTREE_RED;
                sibling(node)->r->color = RBTREE_BLACK;
                rbtree_node_zag(tree, sibling(node)->r);
            }
        }
        sibling(node)->color = node->parent->color;
        node->parent->color = RBTREE_BLACK;
        if (node == node->parent->l) {
            sibling(node)->r->color = RBTREE_BLACK;
            rbtree_node_zag(tree, sibling(node));
        } else {
            sibling(node)->l->color = RBTREE_BLACK;
            rbtree_node_zig(tree, sibling(node));
        }
    }
}

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        // fprintf(stderr, "[%d] %lld %lld\n", i, x, y);

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
