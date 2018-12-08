#include "avl-tree-internel.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define _avl_which_child(node) \
    ((node) == (node)->parent->left_child ? LEFT : RIGHT)
#define _avl_get_child(node, which) \
    (*((which) == LEFT ? (&(node)->left_child) : (&(node)->right_child)))
#define _avl_subtree_height(node) ((node) ? ((node)->height) : 0)

void _avl_debug(avl_node_t node, bool print, int depth) {
    if (!node) return;

    if (print && !depth) puts("-------------------");

    int right_height = 0;
    size_t right_size = 0;
    if (node->right_child) {
        assert(node->right_child->parent == node);
        _avl_debug(node->right_child, print, depth + 1);
        right_height = node->right_child->height;
        right_size = node->right_child->size;
    }

    if (print) {
        for (int i = 0; i < depth; i++) fputs("   ", stdout);
        printf("%lu\n", (unsigned long)node->data);
    }

    int left_height = 0;
    size_t left_size = 0;
    if (node->left_child) {
        assert(node->left_child->parent == node);
        _avl_debug(node->left_child, print, depth + 1);
        left_height = node->left_child->height;
        left_size = node->left_child->size;
    }


    if (print && !depth) puts("-------------------");

    int max = left_height < right_height ? right_height : left_height;
    assert(node->height == max + 1);
    assert(node->size == left_size + right_size + 1);
    assert(abs(left_height - right_height) < 2);
}

int _avl_compare_data(compare_function_t compare_function, void *a, void *b) {
    if (a == b) return 0;
    if (compare_function) {
        return compare_function(a, b);
    }

    return (unsigned long)a < (unsigned long)b ? -1 : 1;
}

int _avl_compare_node(avl_node_t a, avl_node_t b) {
    assert(a && b && a->tree == b->tree);
    if (a == b) return 0;
    return _avl_compare_data(a->tree->compare_function, a->data, b->data);
}

void _avl_destroy_node(avl_node_t node) {
    if (node->tree->destruct_function) {
        node->tree->destruct_function(node->data);
    }

    free(node);
}

void _avl_destroy_subtree(avl_node_t node) {
    if (node->left_child) _avl_destroy_subtree(node->left_child);
    if (node->right_child) _avl_destroy_subtree(node->right_child);
    _avl_destroy_node(node);
}

void _avl_recalculate(avl_node_t node) {
    assert(node);

    int max_height = 0;
    node->size = 1;

    if (node->left_child) {
        if (node->left_child->height > max_height)
            max_height = node->left_child->height;
        node->size += node->left_child->size;
    }
    if (node->right_child) {
        if (node->right_child->height > max_height)
            max_height = node->right_child->height;
        node->size += node->right_child->size;
    }
    
    node->height = max_height + 1;
}

void _avl_rotate(avl_node_t node) {
    assert(node && node->parent);

    avl_node_t old_parent = node->parent;

    _avl_which_child_t which = _avl_which_child(node), another = !which;

    // Connect self to old parent's parent.
    node->parent = old_parent->parent;
    if (old_parent->parent)
        _avl_get_child(old_parent->parent, _avl_which_child(old_parent)) = node;
    
    // Connect self's another child to old parent
    if (_avl_get_child(node, another)) {
        _avl_get_child(node, another)->parent = old_parent;
    }
    _avl_get_child(old_parent, which) = _avl_get_child(node, another);

    // Connect old parent to self
    old_parent->parent = node;
    _avl_get_child(node, another) = old_parent;

    // Notice that the ancestors of old_parent's height are NOT recalculated.
    _avl_recalculate(old_parent);
    _avl_recalculate(node);

    // Check if rotated to root.
    if (!node->parent) {
        node->tree->root = node;
    }
}

void _avl_insert_node(avl_node_t *subtree_root,
                      avl_node_t parent,
                      avl_node_t new_node) {
    if (!*subtree_root) {
        new_node->parent = parent;
        *subtree_root = new_node;
        return;
    }

    avl_node_t current = *subtree_root;

    if (_avl_compare_node(new_node, current) <= 0) {
        // new data <= current node's data
        _avl_insert_node(&current->left_child, current, new_node);

        // Check balance.
        if (_avl_subtree_height(current->left_child) - 
            _avl_subtree_height(current->right_child) == 2) {
            // The balance has been broken, rebalance it.
            // Check which child of left-child it's inserted to.
            if (_avl_compare_node(new_node, current->left_child) <= 0) {
                // The Left-Left case.
                _avl_rotate(current->left_child);
            } else {
                // The Left-Right case.
                _avl_rotate(current->left_child->right_child);
                _avl_rotate(current->left_child);
            }
        }
    } else {
        // new data > current node's data
        _avl_insert_node(&current->right_child, current, new_node);

        // Check balance.
        if (_avl_subtree_height(current->right_child) - 
            _avl_subtree_height(current->left_child) == 2) {
            // The balance has been broken, rebalance it.
            // Check which child of right-child it's inserted to.
            if (_avl_compare_node(new_node, current->right_child) <= 0) {
                // The Right-Left case.
                _avl_rotate(current->right_child->left_child);
                _avl_rotate(current->right_child);
            } else {
                // The Right-Right case.
                _avl_rotate(current->right_child);
            }
        }
    }

    _avl_recalculate(*subtree_root);
}

avl_node_t _avl_binary_search(avl_node_t root,
                              void *data,
                              _avl_search_type_t search_type) {
    if (!root) return NULL;

    int compare_result = _avl_compare_data(root->tree->compare_function,
                                           root->data,
                                           data);

    // Since we should find the first satisfied node, search in the left
    // subtree if satisfied.
    bool satisfied = compare_result > search_type;
    if (satisfied) {
        avl_node_t left_subtree_result = _avl_binary_search(root->left_child,
                                                            data,
                                                            search_type);
        // If a more left satisfied node was found, return it.
        if (left_subtree_result) return left_subtree_result;
        else return root;
    } else {
        return _avl_binary_search(root->right_child,
                                  data,
                                  search_type);
    }
}

avl_node_t _avl_neighbour(avl_node_t node, _avl_which_child_t direction) {
    assert(node);

    // Imagine a traversal on the tree. We are on the node now and willing to
    // move to the in-order (or reversed in-order) next node.
    // e.g. find one's successor, with direction = RIGHT.
    if (_avl_get_child(node, direction)) {
        // If RIGHT subtree, find in the subtree.
        // Find the LEFT-most descendant of node's RIGHT subtree.
        node = _avl_get_child(node, direction);
        while (_avl_get_child(node, !direction)) {
            node = _avl_get_child(node, !direction);
        }
        return node;
    } else {
        // The RIGHT subtree doesn't exist, backtraces will occur.
        if (!node->parent) {
            // Backtrace reaches root. No successor found!
            return NULL;
        }

        while (_avl_which_child(node) == direction) {
            // If node is its parent's RIGHT child, after backtracing to parent,
            // another backtrace will occur to parent's parent.
            if (!node->parent->parent) {
                // Backtrace reaches root. No successor found!
                return NULL;
            }

            node = node->parent;
        }

        // If node is its parent's LEFT child, the next to be printed in the
        // traversal is just its parent!
        return node->parent;
    }
}

void _avl_swap(avl_node_t a, avl_node_t b) {
    assert(a && b);

#define _avl_reconnect(parent_node, child, which) {              \
        if (parent_node) {                                       \
            _avl_get_child((parent_node), (which)) = (child);    \
        } else {                                                 \
            assert(child);                                       \
            (child)->tree->root = (child);                       \
        }                                                        \
        if (child) (child)->parent = (parent_node);              \
    }

    // If one is another's parent, assume a is b's parent.
    if (a->parent == b) {
        _avl_swap(b, a);
    }

    _avl_which_child_t which_a = a->parent ? _avl_which_child(a) : LEFT,
                       which_b = b->parent ? _avl_which_child(b) : LEFT;

    if (a == b->parent) {
        // If a is b's parent.
        avl_node_t c1 = _avl_get_child(b, which_b),
                   c2 = _avl_get_child(b, !which_b),
                   c3 = _avl_get_child(a, !which_b);
        
        _avl_reconnect(a->parent, b, which_a);

        _avl_reconnect(a, c1, which_b);
        _avl_reconnect(a, c2, !which_b);

        _avl_reconnect(b, a, which_b);
        _avl_reconnect(b, c3, !which_b);
    } else {
        avl_node_t a_l = a->left_child, a_r = a->right_child,
                   b_l = b->left_child, b_r = b->right_child;

        avl_node_t a_p = a->parent, b_p = b->parent;
        _avl_reconnect(a_p, b, which_a);
        _avl_reconnect(b_p, a, which_b);

        _avl_reconnect(a, b_l, LEFT);
        _avl_reconnect(a, b_r, RIGHT);

        _avl_reconnect(b, a_l, LEFT);
        _avl_reconnect(b, a_r, RIGHT);
    }

#undef _avl_reconnect
}

void _avl_delete_rebalance(avl_node_t node) {
    if (!node) return;

    if (_avl_subtree_height(node->left_child) - 
        _avl_subtree_height(node->right_child) == 2) {
        // L - R = 2

        if (_avl_subtree_height(node->left_child->left_child) >=
            _avl_subtree_height(node->left_child->right_child)) {
            // The Left-Left case.
            _avl_rotate(node->left_child);
        } else {
            // The Left-Right case.
            _avl_rotate(node->left_child->right_child);
            _avl_rotate(node->left_child);
        }
    } else if (_avl_subtree_height(node->right_child) - 
               _avl_subtree_height(node->left_child) == 2) {
        // R - L = 2

        if (_avl_subtree_height(node->right_child->left_child) <=
            _avl_subtree_height(node->right_child->right_child)) {
            // The Right-Right case.
            _avl_rotate(node->right_child);
        } else {
            // The Right-Left case.
            _avl_rotate(node->right_child->left_child);
            _avl_rotate(node->right_child);
        }
    }

    _avl_recalculate(node);
    _avl_delete_rebalance(node->parent);
}

void _avl_delete(avl_node_t node) {
    assert(node);

    while (node->left_child || node->right_child) {
        // Both children exist.
        // In this case its in-order predecessor or successor must be its descendant.
        // Find it's in-order predecessor or successor to take its place.
        avl_node_t neighbour = _avl_neighbour(node, node->left_child ? LEFT : RIGHT);

        // Swap the node and its neighbour.
        _avl_swap(node, neighbour);
    }

    // Now the node has no child, delete it.
    if (!node->parent) {
        node->tree->root = NULL;
    } else {
        _avl_get_child(node->parent, _avl_which_child(node)) = NULL;
    }

    // Since deleting a node breaks the balance, rebalance it.
    _avl_delete_rebalance(node->parent);

    _avl_destroy_node(node);
}

int _avl_get_order(avl_node_t node) {
    assert(node);
    
    int count = node->left_child ? node->left_child->size : 0;
    while (node->parent) {
        if (_avl_which_child(node) == RIGHT) {
            // If node is a RIGHT child, the parent and parent's LEFT subtree
            // is lesser than node, so add them to the count.
            if (node->parent->left_child) {
                count += node->parent->left_child->size;
            }
            count++;
        }

        node = node->parent;
    }

    return count + 1;
}
