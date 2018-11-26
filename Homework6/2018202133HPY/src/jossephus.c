#include <stdio.h>
#include <assert.h>

#include "list.h"
#include "student.h"

list_node_t *next_node(list_t *list, list_node_t *node)
{
    if (list->begin == list->end)
    {
        return list->begin;
    }
    node = node->next;
    if (node == list->end)
        node = node->next;
    return node;
}

int main(void)
{
    int num, step;
    char buffer[12];

    scanf("%d%d", &num, &step);

    list_t list;
    init_list(&list);

    for (int i = 1; i <= num; i++)
    {
        sprintf(buffer, "ID#%d", i);
        list_insert_node(&list, list.end, new_list_node(gen_new_student(buffer, i), destroy_student));
    }

    list_node_t *now = list.begin, *tmp;
    while (list.size)
    {
        tmp = next_node(&list, now);
        list_drop_node(&list, now);
        for (int i = 0; i < step; i++)
            tmp = next_node(&list, tmp);
        now = tmp;
    }

    destroy_list(&list);
}
