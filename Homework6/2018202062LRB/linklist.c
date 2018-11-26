#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int LEN = 20;
typedef struct student
{
	struct student *pre, *nxt;
	char *name;
	int score;
} student;

void swap(student *a, student *b)
{
	int t = a->score;
	a->score = b->score;
	b->score = t;
	char *t2 = a->name;
	a->name = b->name;
	b->name = t2;
}

void mergesort(student *base, size_t nitems, size_t size, int (*compar)(const student *a, const student *b))
{
	if (nitems == 1)
        return;
    int mid = nitems >> 1;
    student *midd = base, *end = base;
	for (int i = 1; i <= nitems; i++)
	{
		if (i <= mid)
			midd = midd->nxt;
		end = end->nxt;
	}
	mergesort(base, mid, size, compar);
    mergesort(midd, nitems - mid, size, compar);
    student *p = base;
    while (p != midd)
    {
        if (compar(p, midd) > 0)
        {
            swap(p, midd);
            if (midd->nxt != end)
            {
                for (student *p1 = midd->nxt; p1 != end && compar(p1->pre, p1) > 0; swap(p1, p1->pre), p1 = p1->nxt)
                    ;
            }
        }
        p = p->nxt;
    }
}

int comp(const student *a, const student *b)
{
    if (a->score != b->score)
		return b->score - a->score;
	else
		return strcmp(a->name, b->name);
}

student *create_student(int number_of_student)
{
	student *begin, *p, *pre;
	for (int i = 1; i <= number_of_student; i++)
	{
		p = (student *)malloc(sizeof(student));
		p->name = (char *)malloc(LEN);
		scanf("%s %d", p->name, &(p->score));
		if (i > 1)
		{
			p->pre = pre;
			pre->nxt = p;
		}
		else
			begin = p;
		pre = p;
	}
	begin->pre = p;
	p->nxt = begin;
	return begin;
}

void delete_a_student(student *p)
{
	p->pre->nxt = p->nxt;
	p->nxt->pre = p->pre;
	free(p->name);
	free(p);
}

void josephus(student *p, int number_of_student, int space)
{
	for (int i = 1; i <= number_of_student; i++)
	{
		for (int j = 1; j < space; j++)
			p = p->nxt;
		printf("%s %d\n", p->name, p->score);
		p = p->nxt;
		delete_a_student(p->pre);
	}
}

int main()
{
	int number_of_student;
	scanf("%d", &number_of_student);
	student *begin = create_student(number_of_student);
	int type;
	scanf("%d", &type);
	if (type == 1)
	{
		int space;
		scanf("%d", &space);
		josephus(begin, number_of_student, space);
	}
	else
	{
		mergesort(begin, number_of_student, sizeof(student), comp);
		for (int i = 1; i <= number_of_student; i++)
		{
			printf("%s %d\n", begin->name, begin->score);
			begin = begin->nxt;
			delete_a_student(begin->pre);
		}
	}
	return 0;
}