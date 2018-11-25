#ifndef __MENCI_LINKED_LIST_H
#define __MENCI_LINKED_LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct _list_node_t {
    void *data;
    struct _list_node_t *prev, *next;
} _list_node_t;

typedef struct {
    _list_node_t *first, *last;
} _list_t, *list_t;

typedef struct {
    list_t list;
    _list_node_t *node;
} list_iterator_t;

list_t list_create();
void list_destory(list_t);
size_t list_size(list_t list);
bool list_empty(list_t list);
list_iterator_t list_begin(list_t list);
list_iterator_t list_end(list_t list);
list_iterator_t list_erase(list_iterator_t iterator);
list_iterator_t list_insert(list_iterator_t iterator, void *data);
list_iterator_t list_iterator_prev(list_iterator_t iterator);
list_iterator_t list_iterator_next(list_iterator_t iterator);
void **list_iterator_data(list_iterator_t iterator);
bool list_iterator_equal(list_iterator_t iterator1, list_iterator_t iterator2);

#endif
