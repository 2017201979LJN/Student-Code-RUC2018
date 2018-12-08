#ifndef _MENCI_AVL_TREE_H
#define _MENCI_AVL_TREE_H

#include <stdbool.h>
#include <stddef.h>

typedef int (*compare_function_t)(void *, void *);
typedef void (*destruct_function_t)(void *);
typedef struct _avl_node_t *avl_node_t;
typedef struct _avl_tree_t *avl_tree_t;

avl_tree_t avl_create(compare_function_t compare_function, destruct_function_t destruct_function);
void avl_destroy(avl_tree_t tree);
size_t avl_size(avl_tree_t tree);
avl_node_t avl_maximum_node(avl_tree_t tree);
avl_node_t avl_minimum_node(avl_tree_t tree);
avl_node_t avl_insert(avl_tree_t tree, void *data);
void *avl_get_data(avl_node_t node);
avl_node_t avl_find(avl_tree_t tree, void *data);
avl_node_t avl_find_by_order(avl_tree_t tree, int order);
int avl_get_order(avl_tree_t tree, void *data);
int avl_get_order_of_node(avl_node_t node);
void avl_delete_node(avl_node_t node);
bool avl_delete_data(avl_tree_t tree, void *data);

avl_node_t avl_lower_bound(avl_tree_t tree, void *data);
avl_node_t avl_upper_bound(avl_tree_t tree, void *data);
avl_node_t avl_node_predecessor(avl_node_t node);
avl_node_t avl_node_successor(avl_node_t node);

#endif // _MENCI_AVL_TREE_H
