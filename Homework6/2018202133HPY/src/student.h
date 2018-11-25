#ifndef _STUDENT_H

#define _STUDENT_H

typedef struct student
{
    char *name;
    int score;
} student_t;

void *gen_new_student(const char *name, int score);
void destroy_student(void *student);

#endif
