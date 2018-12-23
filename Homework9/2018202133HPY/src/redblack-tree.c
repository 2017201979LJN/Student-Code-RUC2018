#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "redblack-tree.h"

redblack_tree_node_t nil = { &nil, &nil, &nil, &nil, &nil, RBTREE_BLACK, 0, NULL };

void rbtree_node_zig(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    assert(node->parent->l == node);
    redblack_tree_node_t* parent = node->parent;
    parent->l = node->r;
    if (node->r)
        node->r->parent = parent;
    if (parent->parent)
        if (parent->parent->l == parent)
            parent->parent->l = node;
        else
            parent->parent->r = node;
    else
        tree->root = node;
    node->parent = parent->parent;
    parent->parent = node;
    node->r = parent;
    rbtree_node_pushup(parent);
    rbtree_node_pushup(node);
}

/*
inline void Zig(Node *x)
  {
    Node *y = x->fa;
    y->ls = x->rs;
    x->rs->fa = y;
    if(y->fa->ls == y) y->fa->ls = x;
    else if(y->fa->rs == y) y->fa->rs = x;
    x->fa = y->fa;
    y->fa = x; x->rs = y;
    y->Push_Up();
  }
  inline void Zag(Node *x)
  {
    Node *y = x->fa;
    y->rs = x->ls;
    x->ls->fa = y;
    if(y->fa->ls == y) y->fa->ls = x;
    else if(y->fa->rs == y) y->fa->rs = x;
    x->fa = y->fa;
    y->fa = x; x->ls = y;
    y->Push_Up();
  }
*/

void rbtree_node_zag(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    assert(node->parent->r == node);
    redblack_tree_node_t* parent = node->parent;
    parent->r = node->l;
    if (node->l)
        node->l->parent = parent;
    if (parent->parent)
        if (parent->parent->r == parent)
            parent->parent->r = node;
        else
            parent->parent->l = node;
    else
        tree->root = node;
    node->parent = parent->parent;
    parent->parent = node;
    node->l = parent;
    rbtree_node_pushup(parent);
    rbtree_node_pushup(node);
}

/*
inline void Zig(Node *x)
  {
    Node *y = x->fa;
    y->ls = x->rs;
    x->rs->fa = y;
    if(y->fa->ls == y) y->fa->ls = x;
    else if(y->fa->rs == y) y->fa->rs = x;
    x->fa = y->fa;
    y->fa = x; x->rs = y;
    y->Push_Up();
  }
  inline void Zag(Node *x)
  {
    Node *y = x->fa;
    y->rs = x->ls;
    x->ls->fa = y;
    if(y->fa->ls == y) y->fa->ls = x;
    else if(y->fa->rs == y) y->fa->rs = x;
    x->fa = y->fa;
    y->fa = x; x->ls = y;
    y->Push_Up();
  }
*/

void rbtree_node_pushup(redblack_tree_node_t* node)
{
    node->size = 1;
    if (node->l)
        node->size += node->l->size;
    if (node->r)
        node->size += node->r->size;
}

void dfs_rbtree_node_int(redblack_tree_node_t* node)
{
    //printf("%d ", *(int*)(&(node->data)));
    if (node->l) {
        assert(node->l->parent == node);
        dfs_rbtree_node_int(node->l);
    }

    printf("%d ", *(int*)(&(node->data)));

    if (node->r) {
        assert(node->r->parent == node);
        dfs_rbtree_node_int(node->r);
    }
}

redblack_tree_node_t* rbtree_find(redblack_tree_t* tree, void* data)
{
    redblack_tree_node_t* now = tree->root;

    while (now) {
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

redblack_tree_node_t* rbtree_max_node(redblack_tree_node_t* now)
{
    while (now->r)
        now = now->r;
    return now;
}

redblack_tree_node_t* rbtree_min_node(redblack_tree_node_t* now)
{
    while (now->l)
        now = now->l;
    return now;
}

redblack_tree_node_t* rbtree_greater_node(redblack_tree_t* tree, redblack_tree_node_t* now, void* data)
{
    int flag = tree->cmp(now->data, data);

    if (flag <= 0) {
        return now->r ? rbtree_greater_node(tree, now->r, data) : NULL;
        // flag == 0
    } else {
        redblack_tree_node_t* lans = now->l ? rbtree_greater_node(tree, now->l, data) : NULL;
        return lans ? lans : now;
    }
}

redblack_tree_node_t* rbtree_less_node(redblack_tree_t* tree, redblack_tree_node_t* now, void* data)
{
    int flag = tree->cmp(now->data, data);

    if (flag >= 0) {
        return now->l ? rbtree_greater_node(tree, now->l, data) : NULL;
        // flag == 0
    } else {
        redblack_tree_node_t* rans = now->r ? rbtree_greater_node(tree, now->r, data) : NULL;
        return rans ? rans : now;
    }
}

redblack_tree_node_t* rbtree_greater(redblack_tree_t* tree, void* data)
{
    return rbtree_greater_node(tree, tree->root, data);
}

redblack_tree_node_t* rbtree_less(redblack_tree_t* tree, void* data)
{
    return rbtree_less_node(tree, tree->root, data);
}

redblack_tree_node_t* rbtree_insert(redblack_tree_t* tree, void* data)
{
    redblack_tree_node_t** pnow = &(tree->root);
    redblack_tree_node_t* parent = NULL;
    redblack_tree_node_t* node = NULL;
    while (*pnow) {
        int flag = tree->cmp((*pnow)->data, data);
        if (flag < 0) {
            parent = *pnow;
            pnow = &((*pnow)->r);
        } else if (flag == 0) {
            return NULL;
        } else {
            parent = *pnow;
            pnow = &((*pnow)->l);
        }
    }

    node = malloc(sizeof(redblack_tree_node_t) + tree->data_size);
    memset(node, 0, sizeof(redblack_tree_node_t));
    memcpy(node->data, data, tree->data_size);

    node->parent = parent;
    *pnow = node;

    parent = node;
    while (parent) {
        rbtree_node_pushup(parent);
        parent = parent->parent;
    }

    rbtree_insert_case_1(tree, node);

    return node;
}

void rbtree_insert_case_1(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    if (node->parent == NULL) {
        node->color = RBTREE_BLACK;
    } else
        rbtree_insert_case_2(tree, node);
}

void rbtree_insert_case_2(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    if (node->parent->color == RBTREE_BLACK)
        return;
    else
        rbtree_insert_case_3(tree, node);
}

void rbtree_insert_case_3(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    if (RBTREE_NODE_UNCLE(node) != NULL && RBTREE_NODE_UNCLE(node)->color == RBTREE_RED) {
        node->parent->color = RBTREE_BLACK;
        RBTREE_NODE_UNCLE(node)->color = RBTREE_BLACK;
        RBTREE_NODE_GRANDPARENT(node)->color = RBTREE_RED;
        rbtree_insert_case_1(tree, RBTREE_NODE_GRANDPARENT(node));
    } else
        rbtree_insert_case_4(tree, node);
}

void rbtree_insert_case_4(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    if (node == node->parent->r && node->parent == RBTREE_NODE_GRANDPARENT(node)->l) {
        rbtree_node_zag(tree, node);
        node = node->l;
    } else if (node == node->parent->l && node->parent == RBTREE_NODE_GRANDPARENT(node)->r) {
        rbtree_node_zig(tree, node);
        node = node->r;
    }
    rbtree_insert_case_5(tree, node);
}

void rbtree_insert_case_5(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    node->parent->color = RBTREE_BLACK;
    RBTREE_NODE_GRANDPARENT(node)->color = RBTREE_RED;
    if (node == node->parent->l && node->parent == RBTREE_NODE_GRANDPARENT(node)->l) {
        rbtree_node_zig(tree, node->parent);
    } else {
        rbtree_node_zag(tree, node->parent);
    }
}

void rbtree_erase(redblack_tree_t* tree, redblack_tree_node_t* node)
{
    assert(node);

    if (node->l && node->r) {
        redblack_tree_node_t* tmp = rbtree_max_node(node->l);
        memcpy(node->data, tmp->data, tree->data_size);
        node = tmp;
    }

    redblack_tree_node_t* child = node->l ? node->l : node->r;

    if (node->parent) {
        if (node->parent->l == node)
            node->parent->l = child;
        else
            node->parent->r = child;
    } else {
        tree->root = child;
    }
    if (child)
        child->parent = node->parent;

    if (node->color == RBTREE_BLACK) {
        if (child && child->color == RBTREE_RED)
            child->color = RBTREE_BLACK;
        else
            rbtree_erase_case_1(tree, child, node->parent);
    }

    free(node->data);
    free(node);
}

void rbtree_erase_case_1(redblack_tree_t* tree, redblack_tree_node_t* node, redblack_tree_node_t* parent)
{
    if (parent != NULL)
        rbtree_erase_case_2(tree, node, parent);
}

void rbtree_erase_case_2(redblack_tree_t* tree, redblack_tree_node_t* node, redblack_tree_node_t* parent)
{
    redblack_tree_node_t* s = node == parent->l ? parent->r : parent->l;

    if (s && s->color == RBTREE_RED) {
        parent->color = RBTREE_RED;
        s->color = RBTREE_BLACK;
        if (node == parent->l)
            rbtree_node_zag(tree, parent->r);
        else
            rbtree_node_zig(tree, parent->l);
    }
    rbtree_erase_case_3(tree, node, parent);
}

void rbtree_erase_case_3(redblack_tree_t* tree, redblack_tree_node_t* node, redblack_tree_node_t* parent)
{
    redblack_tree_node_t* s = node == parent->l ? parent->r : parent->l;

    if ((parent->color == RBTREE_BLACK) && (s->color == RBTREE_BLACK) && (!s->l || s->l->color == RBTREE_BLACK) && (!s->r || s->r->color == RBTREE_BLACK)) {
        s->color = RBTREE_RED;
        rbtree_erase_case_1(tree, node, parent);
    } else
        rbtree_erase_case_4(tree, node, parent);
}

void rbtree_erase_case_4(redblack_tree_t* tree, redblack_tree_node_t* node, redblack_tree_node_t* parent)
{
    redblack_tree_node_t* s = node == parent->l ? parent->r : parent->l;
    if ((parent->color == RBTREE_RED) && (s->color == RBTREE_BLACK) && (!s->l || s->l->color == RBTREE_BLACK) && (!s->r || s->r->color == RBTREE_BLACK)) {
        s->color = RBTREE_RED;
        parent->color = RBTREE_BLACK;
    } else
        rbtree_erase_case_5(tree, node, parent);
}

void rbtree_erase_case_5(redblack_tree_t* tree, redblack_tree_node_t* node, redblack_tree_node_t* parent)
{
}
