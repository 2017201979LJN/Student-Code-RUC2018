#include "list.h"

list_node_t *init_list_node(list_node_t *new_node, void *data, destroy_list_data_func_t *des)
{
    assert(new_node);

    new_node->prev = NULL;
    new_node->next = NULL;

    new_node->data = data;
    new_node->data_destroyer = des;

    return new_node;
}

list_node_t *new_list_node(void *data, destroy_list_data_func_t *des)
{
    return init_list_node((list_node_t *)malloc(sizeof(list_node_t)), data, des);
}

void destroy_list_node(list_node_t *node)
{
    assert(node);

    if (node->data_destroyer)
        node->data_destroyer(node->data);
}

void delete_list_node(list_node_t *node)
{
    assert(node);
    destroy_list_node(node);
    free(node);
}

list_t *init_list(list_t *lst)
{

    lst->size = 0;
    lst->begin = lst->end = new_list_node(NULL, NULL);
    lst->begin->prev = lst->begin->next = lst->begin;

    return lst;
}

void list_insert_node(list_t *list, list_node_t *pos, list_node_t *node)
{
    list->size++;

    pos->prev->next = node;
    node->prev = pos->prev;

    pos->prev = node;
    node->next = pos;

    if (pos == list->begin)
        list->begin = node;
}

void list_drop_node(list_t *list, list_node_t *pos)
{
    assert(pos != list->end);

    list->size--;

    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;

    if (list->begin == pos)
        list->begin = pos->next;

    delete_list_node(pos);
}

void swap_node(list_node_t *l, list_node_t *r)
{
    void *data = l->data;
    l->data = r->data;
    r->data = data;

    destroy_list_data_func_t *func = l->data_destroyer;
    l->data_destroyer = r->data_destroyer;
    r->data_destroyer = func;
}

void destroy_list(list_t *list)
{
    list_node_t *now = list->begin, *tmp;
    while (now != list->end)
    {
        tmp = now->next;
        delete_list_node(now);
        now = tmp;
    }
    delete_list_node(now);
}
