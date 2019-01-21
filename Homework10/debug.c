#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "avl-tree/avl-tree.h"
#include "avl-tree/avl-tree-internel.h"
#include "bitset/bitset.h"
#include "serialization/serialize.h"
#include "serialization/deserialize.h"
#include "lz77/lz77.h"
#include "huffman/huffman.h"
#include "data-type.h"

int compare_function(void *a, void *b) {
    // Assuming no NaN.
    if (((data_t *)a)->data_double == ((data_t *)b)->data_double) return 0;
    else return ((data_t *)a)->data_double < ((data_t *)b)->data_double ? -1 : 1;
}

void check_equal(avl_node_t a, avl_node_t b) {
    assert((a && b) || (!a && !b));
    if (a && b) {
        assert(memcmp(a->data, b->data, sizeof(double) + sizeof(char)) == 0);
        check_equal(a->left_child, b->left_child);
        check_equal(a->right_child, b->right_child);
    }
}

int main() {
    avl_tree_t tree = avl_create(&compare_function, &free);
    int n, seed;
    assert(scanf("%d %d", &n, &seed) == 2);

    fprintf(stderr, "seed = %d\n", seed);
    srand(seed);

    for (int i = 0; i < n; i++) {
        data_t *data = malloc(sizeof(data_t));

        const int RANGE = 10000;
        data->data_double = (double)rand() / RAND_MAX * RANGE * 2 - RANGE;
        data->data_char = rand() % ('z' - 'a' + 1) + 'a';

        avl_insert(tree, data);
    }

    fputs("Serializing.\n", stderr);
    bitset_t bitset = serialize(tree);

    fprintf(stderr, "Serialized to %d bytes.\n", (int)bitset_sizeof(bitset));

    // void *encoded_data;
    // size_t encoded_length;
    // huffman_encode(bitset->buffer, bitset_buffer_size(bitset), &encoded_data, &encoded_length);

    fprintf(stderr, "Writing %d bytes.\n", (int)bitset_sizeof(bitset));
    fwrite(bitset, bitset_sizeof(bitset), 1, stdout);

    // fputs("LZ77 compressing.\n", stderr);
    // void *lz77_buffer;
    // size_t lz77_size;
    // lz77_compress(bitset, bitset_sizeof(bitset), &lz77_buffer, &lz77_size);

    // fprintf(stderr, "LZ77 compressed to %d bytes.\n", (int)lz77_size);

    // fprintf(stderr, "Writing %d bytes.\n", (int)lz77_size);
    // fwrite(lz77_buffer, lz77_size, 1, stdout);

    // fputs("LZ77 decompressing.\n", stderr);
    // void *lz77_decompress_buffer;
    // size_t lz77_decompress_size;
    // lz77_decompress(lz77_buffer, lz77_size, &lz77_decompress_buffer, &lz77_decompress_size);

    // assert(memcmp(bitset, lz77_decompress_buffer, lz77_decompress_size) == 0);

    // fwrite(bitset, bitset_sizeof(bitset), 1, stdout);
    // fwrite(lz77_decompress_buffer, lz77_decompress_size, 1, stdout);

    // fprintf(stderr, "LZ77 decompressed to %d bytes.\n", (int)lz77_decompress_size);

    // bitset_t bitset2 = (bitset_t)lz77_decompress_buffer;

    avl_tree_t tree2 = avl_create(&compare_function, &free);
    fputs("Deserializing.\n", stderr);
    deserialize(bitset, tree2);

    fputs("Checking.\n", stderr);

    _avl_debug(tree->root, false, 0);
    _avl_debug(tree2->root, false, 0);

    check_equal(tree->root, tree2->root);

    // free(lz77_buffer);
    // bitset_destroy(bitset2);
    bitset_destroy(bitset);
    avl_destroy(tree);
    avl_destroy(tree2);
}
