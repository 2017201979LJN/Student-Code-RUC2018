#include "avl-tree.h"

#include <stdlib.h>
#include <assert.h>

#include "avl-tree-internel.h"

avl_tree_t avl_create(compare_function_t compare_function, destruct_function_t destruct_function) {
    avl_tree_t tree = malloc(sizeof(struct _avl_tree_t));
    tree->root = NULL;
    tree->compare_function = compare_function;
    tree->destruct_function = destruct_function;
    return tree;
}

void avl_destroy(avl_tree_t tree) {
    assert(tree);
    
    if (tree->root) _avl_destroy_subtree(tree->root);
    free(tree);
}

size_t avl_size(avl_tree_t tree) {
    assert(tree);
    return tree->root ? tree->root->size : 0;
}

avl_node_t avl_maximum_node(avl_tree_t tree) {
    assert(tree);

    avl_node_t node = tree->root;
    if (!node) return NULL;

    while (node->right_child) node = node->right_child;
    return node;
}

avl_node_t avl_minimum_node(avl_tree_t tree) {
    assert(tree);

    avl_node_t node = tree->root;
    if (!node) return NULL;

    while (node->left_child) node = node->left_child;
    return node;
}

avl_node_t avl_insert(avl_tree_t tree, void *data) {
    assert(tree);

    avl_node_t new_node = malloc(sizeof(struct _avl_node_t));
    new_node->left_child = new_node->right_child = new_node->parent = NULL;
    new_node->data = data;
    new_node->height = 1;
    new_node->size = 1;
    new_node->tree = tree;
    
    _avl_insert_node(&tree->root, NULL, new_node);

    return new_node;
}

void *avl_get_data(avl_node_t node) {
    return node->data;
}

avl_node_t avl_find(avl_tree_t tree, void *data) {
    assert(tree);

    if (!tree->root) return NULL;

    avl_node_t node = avl_lower_bound(tree, data);
    if (node && node->data == data) return node;

    return NULL;
}

avl_node_t avl_find_by_order(avl_tree_t tree, int order) {
    assert(tree);
    
    avl_node_t node = tree->root;

    // How many nodes should be skipped from the leftist of node's subtree.
    int count = order - 1;
    while (1) {
        if (!node) {
            // Not found - order isn't in [1, tree->root->size].
            return NULL;
        }

        int left_count = node->left_child ? node->left_child->size : 0;
        if (count < left_count) {
            // Skip part of the left subtree, not the whole left subtree.
            // So the node to find must be in the left subtree.
            node = node->left_child;
        } else if (count > left_count) {
            // The whole left subtree should be skipped, and node itself should
            // be skipped, too.
            // So skip them and find the node in the right subtree.
            count -= left_count + 1;
            node = node->right_child;
        } else {
            // The node to skip is exactly the whole left subtree.
            // So current node is what we're finding.
            return node;
        }
    }
}

int avl_get_order(avl_tree_t tree, void *data) {
    assert(tree);

    avl_node_t node = avl_find(tree, data);
    if (!node) return -1;
    return avl_get_order_of_node(node);
}

int avl_get_order_of_node(avl_node_t node) {
    assert(node);

    return _avl_get_order(node);
}

void avl_delete_node(avl_node_t node) {
    assert(node);

    _avl_delete(node);
}

bool avl_delete_data(avl_tree_t tree, void *data) {
    assert(tree);

    avl_node_t node = avl_find(tree, data);
    if (!node) return false;

    avl_delete_node(node);
    return true;
}

avl_node_t avl_lower_bound(avl_tree_t tree, void *data) {
    assert(tree);
    return _avl_binary_search(tree->root, data, LOWER_BOUND);
}

avl_node_t avl_upper_bound(avl_tree_t tree, void *data) {
    assert(tree);
    return _avl_binary_search(tree->root, data, UPPER_BOUND);
}

avl_node_t avl_node_predecessor(avl_node_t node) {
    assert(node);
    return _avl_neighbour(node, LEFT);
}

avl_node_t avl_node_successor(avl_node_t node) {
    assert(node);
    return _avl_neighbour(node, RIGHT);
}
