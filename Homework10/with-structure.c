#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "avl-tree/avl-tree.h"
#include "avl-tree/avl-tree-internel.h"
#include "bitset/bitset.h"
#include "serialization/serialize.h"
#include "serialization/deserialize.h"
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

        const int RANGE = 20;
        data->data_double = (double)rand() / RAND_MAX * RANGE * 2 - RANGE;
        data->data_char = rand() % ('z' - 'a' + 1) + 'a';

        avl_insert(tree, data);
    }

    fputs("Serializing.\n", stderr);
    bitset_t bitset = serialize(tree);

    fprintf(stderr, "Serialized to %d bytes.\n", (int)bitset_sizeof(bitset));

    fprintf(stderr, "Writing %d bytes.\n", (int)bitset_sizeof(bitset));
    fwrite(bitset, bitset_sizeof(bitset), 1, stdout);

    avl_tree_t tree2 = avl_create(&compare_function, &free);
    fputs("Deserializing.\n", stderr);
    deserialize(bitset, tree2);

    fputs("Checking.\n", stderr);

    _avl_debug(tree->root, false, 0);
    _avl_debug(tree2->root, false, 0);

    check_equal(tree->root, tree2->root);

    bitset_destroy(bitset);
    avl_destroy(tree);
    avl_destroy(tree2);
}
