#include <stdio.h>
#include <assert.h>
#include <string.h>

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

int node_leq(list_node_t *l, list_node_t *r)
{
    if (((student_t *)l->data)->score != ((student_t *)r->data)->score)
    {
        return ((student_t *)l->data)->score < ((student_t *)r->data)->score;
    }
    else
    {
        return strcmp(((student_t *)l->data)->name, ((student_t *)r->data)->name) == -1;
    }
}

int main(void)
{
    int num;
    char buffer[4096];
    int score;

    scanf("%d", &num);

    list_t list;
    init_list(&list);

    for (int i = 1; i <= num; i++)
    {
        scanf("%s%d", buffer, &score);
        list_insert_node(&list, list.end, new_list_node(gen_new_student(buffer, score), destroy_student));
    }

    for (list_node_t *start = list.begin; start != list.end; start = start->next)
    {
        for (list_node_t *now = start; now->next != list.end; now = now->next)
        {
            if (node_leq(now->next, now))
            {
                swap_node(now, now->next);
            }
        }
    }

    for (list_node_t *start = list.begin; start != list.end; start = start->next)
    {
        printf("[%s,%d] ", ((student_t *)start->data)->name, ((student_t *)start->data)->score);
    }

    printf("\n");

    destroy_list(&list);
}
