#include "serialize.h"

#include <limits.h>
#include <stdlib.h>
#include <assert.h>

#include "../bitset/bitset.h"
#include "../avl-tree/avl-tree-internel.h"
#include "../huffman/huffman.h"
#include "../data-type.h"

void _serialize_subtree(avl_node_t root,
                        bitstream_t bitstream,
                        double **p_double,
                        char **p_char) {
    assert(root);

    if (root->left_child) {
        bitstream_write(bitstream, 1);
        _serialize_subtree(root->left_child, bitstream, p_double, p_char);
    } else {
        bitstream_write(bitstream, 0);
    }

    data_t *data = avl_get_data(root);
    *(*p_double)++ = data->data_double;
    *(*p_char)++ = data->data_char;

    if (root->right_child) {
        bitstream_write(bitstream, 1);
        _serialize_subtree(root->right_child, bitstream, p_double, p_char);
    } else {
        bitstream_write(bitstream, 0);
    }
}

bitset_t serialize(avl_tree_t tree) {
    size_t tree_size = avl_size(tree);

    // It's not easy to calculate huffman encoded data's size.
    const size_t ENOUGH_SIZE = 1024UL * 1024 * 10 * 8;
    bitset_t bitset = bitset_create(ENOUGH_SIZE);
    bitstream_t bitstream = bitstream_create(bitset, 0);
    
    bitstream_write_data(bitstream, &tree_size, sizeof(size_t));

    double *array_double = malloc(sizeof(double) * tree_size), *p_double = array_double;
    char *array_char = malloc(sizeof(char) * tree_size), *p_char = array_char;

    // Use DFS sequence to serialize the tree structure.
    // 0: No such child, 1: Enter a child. After boths childs return to parent.
    _serialize_subtree(tree->root, bitstream, &p_double, &p_char);

    for (size_t i = 0; i < tree_size; i++) {
        assert(array_char[i] >= 'a' && array_char[i] <= 'z');
        int tmp = array_char[i] - 'a';

        // Use 5 bit to store a alpha.
        for (size_t j = 0; j < 5; j++) {
            bitstream_write(bitstream, !!(tmp & (1 << j)));
        }
    }

    void *encoded_double;
    size_t encoded_double_length;
    huffman_encode(array_double, sizeof(double) * tree_size, &encoded_double, &encoded_double_length);
    bitstream_write_data(bitstream, &encoded_double_length, sizeof(size_t));
    bitstream_write_data(bitstream, encoded_double, encoded_double_length);

    bitstream_close(bitstream);
    bitstream_destroy(bitstream);

    free(encoded_double);
    free(array_double);
    free(array_char);

    return bitset;
}
