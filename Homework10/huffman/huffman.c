#include "huffman.h"

#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#include "../int256/int256.h"

#define _HUFFMAN_WORD_COUNT 256

typedef struct {
    uint8_t word;
    size_t weight;
} _huffman_word_t;

typedef struct _huffman_tree_t {
    uint8_t word;
    struct _huffman_tree_t *child[2];
} _huffman_tree_t;

typedef struct {
    _huffman_tree_t *tree_node;
    uint32_t weight;
} _huffman_build_node_t;

int _huffman_build_sort_compare(const void *a, const void *b) {
    int value1 = ((const _huffman_build_node_t *)a)->weight,
        value2 = ((const _huffman_build_node_t *)b)->weight;
    return value1 - value2;
}

_huffman_tree_t *_huffman_build_tree(_huffman_word_t *words, size_t word_count) {
    _huffman_build_node_t nodes[word_count];
    for (size_t i = 0; i < word_count; i++) {
        nodes[i].weight = words[i].weight;
        nodes[i].tree_node = malloc(sizeof(_huffman_tree_t));
        nodes[i].tree_node->word = words[i].word;
        nodes[i].tree_node->child[0] = nodes[i].tree_node->child[1] = NULL;
    }

    for (int i = 0; i < word_count - 1; i++) {
        qsort(nodes + i, word_count - i, sizeof(_huffman_build_node_t), _huffman_build_sort_compare);

        _huffman_tree_t *new_node = malloc(sizeof(_huffman_tree_t));
        new_node->child[0] = nodes[i].tree_node;
        new_node->child[1] = nodes[i + 1].tree_node;

        nodes[i + 1].weight += nodes[i].weight;
        nodes[i + 1].tree_node = new_node;
    }

    return nodes[word_count - 1].tree_node;
}

void _huffman_destroy_tree(_huffman_tree_t *tree) {
    if (tree->child[0]) _huffman_destroy_tree(tree->child[0]);
    if (tree->child[1]) _huffman_destroy_tree(tree->child[1]);
    free(tree);
}

typedef struct {
    _uint256_t code;
    size_t code_length;
} _huffman_encode_node_t;

void _huffman_dfs(_huffman_tree_t *root,
                  _uint256_t code,
                  size_t code_length,
                  _huffman_encode_node_t *nodes) {
    assert(code_length < _HUFFMAN_WORD_COUNT);

    if (root->child[0]) {
        _huffman_dfs(root->child[0],
                     _uint256_or(_uint256_shl(code, 1), _UINT256_ZERO),
                     code_length + 1,
                     nodes);
    }

    if (root->child[1]) {
        _huffman_dfs(root->child[1],
                     _uint256_or(_uint256_shl(code, 1), _UINT256_ONE),
                     code_length + 1,
                     nodes);
    }

    if (!root->child[0] && !root->child[1]) {
        nodes[root->word].code = code;
        nodes[root->word].code_length = code_length;
    }
}

void huffman_encode(void *data,
                    size_t data_length,
                    void **buffer,
                    size_t *encoded_length) {
    _huffman_word_t words[_HUFFMAN_WORD_COUNT];
    for (size_t i = 0; i < _HUFFMAN_WORD_COUNT; i++) {
        words[i].word = i;
        words[i].weight = 0;
    }

    for (size_t i = 0; i < data_length; i++) words[((uint8_t *)data)[i]].weight++;

    _huffman_tree_t *root = _huffman_build_tree(words, _HUFFMAN_WORD_COUNT);

    _huffman_encode_node_t nodes[_HUFFMAN_WORD_COUNT];
    _huffman_dfs(root, _UINT256_ZERO, 0, nodes);

    // Calculate buffer size (in bit).
    size_t size = (sizeof(_huffman_word_t) * _HUFFMAN_WORD_COUNT + sizeof(size_t) * 2) * CHAR_BIT;
    for (size_t i = 0; i < _HUFFMAN_WORD_COUNT; i++) size += nodes[i].code_length * words[i].weight;
    bitset_t bitset = bitset_create(size);
    bitstream_t bitstream = bitstream_create(bitset, 0);

    // Write words.
    bitstream_write_data(bitstream, words, sizeof(_huffman_word_t) * _HUFFMAN_WORD_COUNT);

    // Write encoded data.
    bitstream_write_data(bitstream, &data_length, sizeof(size_t));
    for (size_t i = 0; i < data_length; i++) {
        _huffman_encode_node_t *node = &nodes[((uint8_t *)data)[i]];
        for (size_t j = 0; j < node->code_length; j++) {
            bitstream_write(bitstream,
                            _uint256_to_bool(_uint256_and(node->code,
                                                          _uint256_shl(_UINT256_ONE,
                                                                       node->code_length - j - 1))));
        }
    }

    bitstream_destroy(bitstream);
    _huffman_destroy_tree(root);

    *encoded_length = bitset_sizeof(bitset);
    *buffer = bitset;
}

void huffman_decode(void *encoded_data,
                    void **buffer,
                    size_t *decoded_length) {
    bitstream_t bitstream = bitstream_create(encoded_data, 0);

    // Read words.
    _huffman_word_t words[_HUFFMAN_WORD_COUNT];
    bitstream_read_data(bitstream, words, sizeof(_huffman_word_t) * _HUFFMAN_WORD_COUNT);

    _huffman_tree_t *root = _huffman_build_tree(words, _HUFFMAN_WORD_COUNT);

    size_t data_length;
    bitstream_read_data(bitstream, &data_length, sizeof(size_t));
    uint8_t *data = malloc(sizeof(uint8_t) * data_length);
    for (size_t i = 0; i < data_length; i++) {
        _huffman_tree_t *tree_node = root;
        while (tree_node->child[0] || tree_node->child[1])
            tree_node = tree_node->child[bitstream_read(bitstream)];
        
        data[i] = tree_node->word;
    }

    _huffman_destroy_tree(root);
    bitstream_destroy(bitstream);

    *decoded_length = data_length;
    *buffer = data;
}
