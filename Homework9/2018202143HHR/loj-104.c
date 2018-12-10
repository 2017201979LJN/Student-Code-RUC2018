#include "avl-tree.h"

#include <stdio.h>
#include <assert.h>

#define data_type_from_int(x) ((void *)(long)(x))
#define data_type_to_int(x)   ((int)(long)(x))

int compare(void *a, void *b) {
    return data_type_to_int(a) - data_type_to_int(b);
}

int main() {
    avl_tree_t tree = avl_create(&compare, NULL);

    int n;
    assert(scanf("%d", &n) == 1);
    assert(n >= 1 && n <= (int)1e5);

    for (int i = 0; i < n; i++) {
        int option, x;
        assert(scanf("%d %d", &option, &x) == 2);
        assert(option >= 1 && option <= 6);

        if (option == 1) {
            avl_insert(tree, data_type_from_int(x));
        } else if (option == 2) {
            avl_delete_data(tree, data_type_from_int(x));
        } else if (option == 3) {
            printf("%d\n", avl_get_order(tree, data_type_from_int(x)));
        } else if (option == 4) {
            avl_node_t node = avl_find_by_order(tree, x);
            assert(node);
            printf("%d\n", data_type_to_int(avl_get_data(node)));
        } else if (option == 5) {
            avl_node_t node = avl_lower_bound(tree, data_type_from_int(x)),
                       predecessor = node ? avl_node_predecessor(node) : avl_maximum_node(tree);
            printf("%d\n", data_type_to_int(avl_get_data(predecessor)));
        } else if (option == 6) {
            avl_node_t successor = avl_upper_bound(tree, data_type_from_int(x));
            assert(successor);
            printf("%d\n", data_type_to_int(avl_get_data(successor)));
        }
    }

    avl_destroy(tree);
}
