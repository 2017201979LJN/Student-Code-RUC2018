#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STR_LEN 100

typedef struct link_list
{
    int score;
    char *name;
    struct link_list *pre,*next;
} link_list;

link_list *init_link_list (int mem_num)
{
    size_t mem_size = sizeof(link_list);
    link_list *last, *head, *tail;
    for (int i = 0; i < mem_num; i++) {
        link_list *newp = (link_list *)malloc(mem_size);
        if (i) {
            last->next = newp;
            newp->pre = last;
        }
        else
            head = newp;
        newp->name = (char *)malloc(STR_LEN);
        scanf("%s", newp->name);
        scanf("%d", &newp->score);      
        last = newp;
        tail = newp;
    }
    head->pre = tail;
    tail->next = head;
    return head;
}

void joseph (link_list *head, int mem_num)
{
    int step_len;
    scanf("%d", &step_len);
    link_list *p = head;
    while(mem_num--) {
        for(int i = 1; i < step_len ; i++, p = p->next);
        p->pre->next = p->next;
        p->next->pre = p->pre;
        printf("%s %d\n", p->name, p->score);
        link_list *nex = p->next;
        free(p->name);
        free(p);
        p = nex;
    }
    head = NULL;
}

int cmp (link_list *v1, link_list *v2)
{
    if(v1->score == v2->score)
        return strcmp(v1->name, v2->name);
    return v1->score - v2->score;
}

void swap (link_list *v1, link_list *v2) 
{
    int temp_score = v1->score;
    char *temp_name = v1->name;
    v1->score = v2->score;
    v1->name = v2->name;
    v2->score = temp_score;
    v2->name = temp_name;
}

void bubble_sort (link_list *head, int mem_num)
{
    link_list *p1, *p2;
    int i, j;
    for (i = 1, p1 = head->next; i < mem_num; i++, p1 = p1->next) 
        for (j = i, p2 = p1; j > 0 && cmp(p2, p2->pre) < 0; j--, p2 = p2->pre)
            swap(p2->pre, p2);
}

void print_link_list (link_list *head)
{
    for(link_list *i = head; ; i = i->next) {
        printf("%s ", i->name);
        printf("%d\n", i->score);
        if(i->next == head)
            break;
    }
}

int main()
{
    int n;
    scanf ("%d", &n);
    link_list *head = init_link_list(n);
    bubble_sort(head, n);
    print_link_list(head);
    joseph(head, n);
    if (head != NULL)
        head->pre->next = NULL;
    while (head != NULL) {
        link_list *nex = head->next;
        free(head->name);
        free(head);
        head = nex;
    }
    return 0;
}
