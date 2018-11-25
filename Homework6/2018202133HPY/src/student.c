#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "student.h"

void *gen_new_student(const char *name, int score)
{
    student_t *student = malloc(sizeof(student_t));

    if (!name)
    {
        student->name = NULL;
    }
    else
    {
        student->name = strcpy(malloc(strlen(name) + 1), name);
    }

    student->score = score;

    fprintf(stderr, "Student %s (%p) has constructed...\n", name ? name : "NONAME", student);

    return student;
}
void destroy_student(void *student)
{
    char *name = ((student_t *)student)->name;
    fprintf(stderr, "Student %s (%p) has destructed...\n", name ? name : "NONAME", student);

    if (((student_t *)student)->name)
        free(((student_t *)student)->name);

    free(student);
}
