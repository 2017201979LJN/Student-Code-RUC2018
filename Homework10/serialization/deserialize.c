#include "deserialize.h"

#include <limits.h>
#include <stdlib.h>
#include <assert.h>

#include "../bitset/bitset.h"
#include "../avl-tree/avl-tree-internel.h"
#include "../huffman/huffman.h"
#include "../data-type.h"

avl_node_t _deserialize_subtree(avl_tree_t tree,
                                bitstream_t bitstream,
                                bool **p_structure,
                                double **p_double,
                                char **p_char) {
    avl_node_t root = malloc(sizeof(*root));
    root->tree = tree;

    if (*(*p_structure)++) {
        root->left_child = _deserialize_subtree(tree, bitstream, p_structure, p_double, p_char);
        root->left_child->parent = root;
    } else {
        root->left_child = NULL;
    }

    data_t *data = malloc(sizeof(data_t));
    data->data_double = *(*p_double)++;
    data->data_char = *(*p_char)++;
    root->data = data;

    if (*(*p_structure)++) {
        root->right_child = _deserialize_subtree(tree, bitstream, p_structure, p_double, p_char);
        root->right_child->parent = root;
    } else {
        root->right_child = NULL;
    }

    _avl_recalculate(root);

    return root;
}

void deserialize(bitset_t bitset, avl_tree_t tree) {
    bitstream_t bitstream = bitstream_create(bitset, 0);

    // Use DFS sequence to serialize the tree structure.
    // 0: No such child, 1: Enter a child. After boths childs return to parent.
    // Format: [meta: tree size, size_t]
    //         [structure: 2 * tree size bits]
    //         [doubles: Huffman encoded data]
    //         [chars: 5 * tree size bits]
    size_t tree_size;
    bitstream_read_data(bitstream, &tree_size, sizeof(size_t));

    bool *array_structure = malloc(sizeof(bool) * tree_size * 2), *p_structure = array_structure;
    for (int i = 0; i < tree_size * 2; i++) {
        array_structure[i] = bitstream_read(bitstream);
    }

    char *array_char = malloc(sizeof(char) * tree_size), *p_char = array_char;
    for (int i = 0; i < tree_size; i++) {
        int tmp = 0;

        // Use 5 bit to store a alpha.
        for (size_t j = 0; j < 5; j++) {
            tmp |= bitstream_read(bitstream) << j;
        }

        array_char[i] = tmp + 'a';
    }

    size_t encoded_double_length;
    bitstream_read_data(bitstream, &encoded_double_length, sizeof(size_t));
    void *encoded_double = malloc(encoded_double_length);
    bitstream_read_data(bitstream, encoded_double, encoded_double_length);

    double *array_double;
    size_t decoded_double_length;
    huffman_decode(encoded_double, (void **)&array_double, &decoded_double_length);
    assert(decoded_double_length == tree_size * sizeof(double));

    double *p_double = array_double;
    tree->root = _deserialize_subtree(tree, bitstream, &p_structure, &p_double, &p_char);

    bitstream_destroy(bitstream);

    free(encoded_double);
    free(array_double);
    free(array_char);
    free(array_structure);
}
