#include "linked_list.h"
#include <stdlib.h>
#include <assert.h>

list_t list_create() {
    list_t list = malloc(sizeof(_list_t));
    list->first = list->last = NULL;
    return list;
}

void list_destory(list_t list) {
    free(list);
}

size_t list_size(list_t list) {
    size_t size = 0;
    for (_list_node_t *node = list->first;
         node;
         node = node->next)
        size++;

    return size;
}

bool list_empty(list_t list) {
    return list_iterator_equal(list_begin(list), list_end(list));
}

list_iterator_t list_begin(list_t list) {
    return (list_iterator_t){list, list->first};
}

list_iterator_t list_end(list_t list) {
    return (list_iterator_t){list, NULL};
}

list_iterator_t list_erase(list_iterator_t iterator) {
    if (iterator.node == iterator.list->first) {
        iterator.list->first = iterator.node->next;
    }

    if (iterator.node == iterator.list->last) {
        iterator.list->last = iterator.node->prev;
    }

    list_iterator_t result = list_iterator_next(iterator);

    _list_node_t *node = iterator.node;
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    free(node);

    if (node == iterator.list->first) {
        iterator.list->first = result.node;
    }

    return result;
}

list_iterator_t list_insert(list_iterator_t iterator, void *data) {
    _list_node_t *node = iterator.node;

    _list_node_t *new_node = malloc(sizeof(_list_node_t));
    new_node->data = data;
    new_node->next = node;
    new_node->prev = node ? node->prev : iterator.list->last;
    if (new_node->prev) new_node->prev->next = new_node;
    if (new_node->next) new_node->next->prev = new_node;

    if (list_iterator_equal(iterator, list_begin(iterator.list))) {
        iterator.list->first = new_node;
    }

    if (list_iterator_equal(iterator, list_end(iterator.list))) {
        iterator.list->last = new_node;
    }

    return (list_iterator_t){iterator.list, new_node};
}

list_iterator_t list_iterator_prev(list_iterator_t iterator) {
    _list_node_t *node = iterator.node;
    if (!node) {
        return (list_iterator_t){iterator.list, iterator.list->last};
    }
    assert(node->prev); // Assert it's not begin.
    return (list_iterator_t){iterator.list, node->prev};
}

list_iterator_t list_iterator_next(list_iterator_t iterator) {
    _list_node_t *node = iterator.node;
    assert(node); // Assert it's not end.
    return (list_iterator_t){iterator.list, node->next};
}

void **list_iterator_data(list_iterator_t iterator) {
    _list_node_t *node = iterator.node;
    assert(node); // Assert it's not end.
    return &node->data;
}

bool list_iterator_equal(list_iterator_t iterator1, list_iterator_t iterator2) {
    if (iterator1.list != iterator2.list) return false;
    if (iterator1.node != iterator2.node) return false;
    return true;
}
