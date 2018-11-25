#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "linked_list.h"

typedef struct {
    char *name;
    int score;
} student_t;

void trim(char *s) {
    char *p = s;
    while (p && isspace(*p)) p++;
    size_t len = strlen(p);
    memmove(s, p, len + 1);
    while (len && isspace(s[len - 1])) len--;
    s[len] = '\0';
}

void scan_student(student_t *new_student) {
    size_t temp;
    assert(getdelim(&new_student->name, &temp, ' ', stdin) > 0);

    size_t name_len = strlen(new_student->name);
    assert(new_student->name[name_len - 1] == ' ');

    trim(new_student->name);
    name_len = strlen(new_student->name);

    assert(scanf("%d", &new_student->score) == 1);
}

void print_student(student_t *student) {
    printf("%s %d\n", student->name, student->score);
}

void destruct_student(student_t *student) {
    free(student->name);
    free(student);
}

void play_joseph(list_t list, int n, int k) {
    list_iterator_t it = list_begin(list);
    for (int i = 1; i <= n; i++) {
        student_t *student = *(student_t **)list_iterator_data(it);
        print_student(student);
        destruct_student(student);
        it = list_erase(it);
        if (list_iterator_equal(it, list_end(list))) it = list_begin(list);

        // Find the next to delete.
        if (i == n) break;
        for (int j = 0; j < k; j++) {
            it = list_iterator_next(it);
            if (list_iterator_equal(it, list_end(list))) it = list_begin(list);
        }
    }
}

int compare(student_t *a, student_t *b) {
    if (a->score != b->score) return b->score - a->score;
    return strcmp(a->name, b->name);
}

void sort_list(list_t list) {
    for (list_iterator_t it1 = list_begin(list);
         !list_iterator_equal(list_iterator_next(it1), list_end(list));
         it1 = list_iterator_next(it1)) {
        // Select the least in [it1, end).
        list_iterator_t least_it = it1;
        for (list_iterator_t it2 = list_iterator_next(it1);
             !list_iterator_equal(it2, list_end(list));
             it2 = list_iterator_next(it2)) {
            student_t *current = *(student_t **)list_iterator_data(it2);
            if (compare(current, *(student_t **)list_iterator_data(least_it)) < 0) {
                least_it = it2;
            }
        }

        // Swap current with the least.
        if (!list_iterator_equal(least_it, it1)) {
            void *temp = *list_iterator_data(least_it);
            *list_iterator_data(least_it) = *list_iterator_data(it1);
            *list_iterator_data(it1) = temp;
        }
    }
}

int main() {
    int n, sort, joseph;
    assert(scanf("%d %d %d", &n, &sort, &joseph) == 3);
    assert(n >= 0);
    assert(sort == 0 || sort == 1);
    assert(joseph >= 0);

    list_t list = list_create();
    for (int i = 0; i < n; i++) {
        student_t *new_student = malloc(sizeof(new_student));
        scan_student(new_student);
        list_insert(list_end(list), new_student);
    }

    if (sort) {
        sort_list(list);
    }

    if (joseph) {
        assert(n > 0);
        play_joseph(list, n, joseph);
    } else {
        while (!list_empty(list)) {
            list_iterator_t begin = list_begin(list);
            student_t *student = *(student_t **)list_iterator_data(begin);
            print_student(student);
            destruct_student(student);
            list_erase(begin);
        }
    }

    list_destory(list);

    return 0;
}
