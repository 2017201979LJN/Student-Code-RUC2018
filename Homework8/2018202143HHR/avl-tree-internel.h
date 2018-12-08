#ifndef _MENCI_AVL_TREE_INTERNEL_H
#define _MENCI_AVL_TREE_INTERNEL_H

#include "avl-tree.h"

struct _avl_node_t {
    struct _avl_node_t *left_child;
    struct _avl_node_t *right_child;
    struct _avl_node_t *parent;

    // The data field.
    void *data;

    // The height of the subtree with this node as its root.
    int height;

    // The count of nodes in this subtree.
    size_t size;

    // The reference to its tree, to replace the root node when rotated to root.
    struct _avl_tree_t *tree;
};

struct _avl_tree_t {
    // The root node of the tree.
    avl_node_t root;

    // The compare function, to determine the order of nodes' data.
    compare_function_t compare_function;

    // The destruct function, to destruct and free nodes' data.
    destruct_function_t destruct_function;
};

typedef enum {
    LEFT = 0, RIGHT = 1
} _avl_which_child_t;

// Validate and print a subtree, for debugging.
void _avl_debug(avl_node_t node, bool print, int depth);

// Compare two data or node's data, compare by unsigned long if
// compare_function is NULL or call the compare_function if non-NULL.
int _avl_compare_data(compare_function_t compare_function, void *a, void *b);
int _avl_compare_node(avl_node_t a, avl_node_t b);

// Destroy a node or a subtree.
void _avl_destroy_node(avl_node_t node);
void _avl_destroy_subtree(avl_node_t node);

// Recalculate a node's height and size after rotating, rebalancing, insertion
// or deletion.
void _avl_recalculate(avl_node_t node);

// Rotate a node up, to take its parent's place.
// e.g. Left rotate will let one's right-child take its place of subtree root.
void _avl_rotate(avl_node_t node);

// Do a rebalance on a subtree's root level, used when abs(balance factor) = 2.
void _avl_rebalance(avl_node_t subtree_root, avl_node_t new_node);

// Insert a node to a node's subtree.
void _avl_insert_node(avl_node_t *subtree_root,
                      avl_node_t parent,
                      avl_node_t new_node);

typedef enum {
    // Doing a lower_bound binary search means skip all nodes less than specfied data.
    // So skip all nodes that compare(node->data, data) <= -1.
    // For the same reason, doing a upper_bound binary search skips all nodes that
    // compare() <= 0.
    // So skip nodes that compare() <= search_type will do the magic.
    LOWER_BOUND = -1,
    UPPER_BOUND = 0
} _avl_search_type_t;

// Binary Search a data, return found node according to search_type.
avl_node_t _avl_binary_search(avl_node_t root,
                              void *data,
                              _avl_search_type_t search_type);

// Get a node's in-order predecessor or successor.
// direction = LEFT means predecessor, RIGHT means successor.
avl_node_t _avl_neighbour(avl_node_t node, _avl_which_child_t direction);

// Swap two node's position.
void _avl_swap(avl_node_t a, avl_node_t b);

// Delete a node, while maintaining the balance.
void _avl_delete_rebalance(avl_node_t node);
void _avl_delete(avl_node_t node);

int _avl_get_order(avl_node_t node);

#endif // _MENCI_AVL_TREE_INTERNEL_H
