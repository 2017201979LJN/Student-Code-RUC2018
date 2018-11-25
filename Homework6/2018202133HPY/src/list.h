#ifndef _LIST_H

#define _LIST_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef void destroy_list_data_func_t(void *);

typedef struct list_node
{
    struct list_node *prev, *next;

    void *data;
    destroy_list_data_func_t *data_destroyer;

} list_node_t;

list_node_t *init_list_node(list_node_t *new_node, void *data, destroy_list_data_func_t *des);
list_node_t *new_list_node(void *data, destroy_list_data_func_t *des);
void destroy_list_node(list_node_t *node);
void delete_list_node(list_node_t *node);

typedef struct list
{
    int size;
    list_node_t *begin, *end;
} list_t;

list_t *init_list(list_t *lst);
void list_insert_node(list_t *list, list_node_t *pos, list_node_t *node);
void list_drop_node(list_t *list, list_node_t *pos);
void swap_node(list_node_t *l, list_node_t *r);
void destroy_list(list_t *list);
#endif
