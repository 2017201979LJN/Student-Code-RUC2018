#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zip.h"

int tree_data_cmp(const tree_data_t* l, const tree_data_t* r)
{
    if (l->d < r->d) {
        return -1;
    } else if (l->d > r->d) {
        return 1;
    } else
        return l->c - r->c;
}

void tree_insert_node(tree_node_t** ppnode, tree_data_t data)
{
    if (*ppnode == NULL) {
        *ppnode = malloc(sizeof(tree_node_t));
        (*ppnode)->ls = (*ppnode)->rs = 0;
        memcpy(&((*ppnode)->data), &data, sizeof(tree_data_t));
        (*ppnode)->siz = 1;

        return;
    } else {
        int cmpres = tree_data_cmp(&((*ppnode)->data), &data);

        if (cmpres < 0) {
            tree_insert_node(&((*ppnode)->rs), data);
        } else if (cmpres == 0)
            return;
        else {
            tree_insert_node(&((*ppnode)->ls), data);
        }

        (*ppnode)->siz = 1;

        if ((*ppnode)->ls)
            (*ppnode)->siz += (*ppnode)->ls->siz;
        if ((*ppnode)->rs)
            (*ppnode)->siz += (*ppnode)->rs->siz;
    }
}

int tree_size_hash(tree_node_t* pnode)
{
    if (pnode == NULL)
        return 0;

    int ret = 1;

    printf("%.3lf %c\n", (pnode->data.d - ZIP_INT_OFFSET) / ZIP_DOUBLE_SCALE, pnode->data.c);

    ret += tree_size_hash(pnode->ls);

    ret *= 100007;
    ret %= 19260817;

    ret += tree_size_hash(pnode->rs);

    ret %= 19260817;

    return ret;
}
