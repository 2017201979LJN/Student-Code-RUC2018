#include "avl-tree.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define RAND_RANGE 10000
bool flag[RAND_RANGE];

int compare_function(void *a, void *b) {
    return (long)a - (long)b;
}

void *random_value() {
    int x;
    do x = rand() % RAND_RANGE; while (flag[x]);
    // flag[x] = true;
    return (void *)(long)x;
}

int main() {
    int seed;
    scanf("%d", &seed);
    // seed = 150456915; // n = 7
    // seed = 1544099827;
    srand(seed);
    printf("seed = %d\n", seed);

    avl_tree_t tree = avl_create(compare_function, NULL);

    for (int i = 0; i < 1000; i++) {
        avl_insert(tree, random_value());
        // _avl_debug(tree->root, true, 0);
    }

    // _avl_debug(tree->root, true, 0);

    // memset(flag, 0, sizeof(flag));

    for (int i = 1; i < 10000; i++) {
        void *x = random_value();
        avl_node_t node = avl_find(tree, x);
        if (node) {
            // printf("found %d.\n", (int)(long)x);
            avl_delete_node(node);
            // _avl_debug(tree->root, false, 0);
        }
    }

    // _avl_debug(tree->root, true, 0);

    avl_destroy(tree);
}
