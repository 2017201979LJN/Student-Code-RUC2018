#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MIN_LEN_OF_NAME 3
#define MAX_LEN_OF_NAME 10

#define MAX_SCORE 100

#define MIN_NUM_OF_STUDENTS 10
#define MAX_NUM_OF_STUDENTS 1000

#define SIZE_OF_ALPHABET 26

#define ASCII_OF_CAP_A 'A'
#define NULL_CHARACTER '\0'

#define MIN_STEP 1
#define MAX_STEP 1000

#define FALSE 0
#define TRUE  1

#define SMALLER -1
#define EQUAL   0
#define LARGER  1

struct my_student
{
	char *name;
	int score;
	struct my_student *prev, *next;
};

typedef struct my_student student;

void init(void);

int generate_rand_int(int min, int max);

void swap_char_ptr(char **a, char **b);
void swap_int(int *a, int *b);

student *generate_student(void);
void swap_student(student *a, student *b);
void remove_student(student *ptr);
void print_student(student *ptr);
int cmp_student(student *a, student *b);

student *generate_list(void);
void remove_list(student *ptr);
void print_list(student *ptr);

void Joseph(student *ptr);
void sort_list(student *ptr);

int main(void)
{
	init();
	int operator;
	printf("input the operator(1 for Joseph, 2 for sort):\n");
	freopen("data.out", "w", stdout);
	scanf("%d", &operator);
	student *ptr = generate_list();
	switch(operator)
	{
	case 1:
		Joseph(ptr);
		break;
	case 2:
		sort_list(ptr);
		print_list(ptr);
		remove_list(ptr);
		break;
	default:
		printf("unknown operator: %d\n", operator);
		break;
	}
}

void init(void)
{
	srand(time(NULL));
}

int generate_rand_int(int min, int max)
{
	return min + rand() % (max - min + 1);
}

void swap_char_ptr(char **a, char **b)
{
	char *t = *a;
	*a = *b;
	*b = t;
}

void swap_int(int *a, int *b)
{
	int c = *a;
	*a = *b;
	*b = c;
}

student *generate_student(void)
{
	student *ptr = malloc(sizeof(student));
	int len_of_name = generate_rand_int(MIN_LEN_OF_NAME, MAX_LEN_OF_NAME);
	ptr -> name = malloc((len_of_name + 1) * sizeof(char));
	for (int i = 0; i < len_of_name; ++i)
	{
		*(ptr -> name + i * sizeof(char)) = ASCII_OF_CAP_A + rand () % SIZE_OF_ALPHABET;
	}
	*(ptr -> name + len_of_name * sizeof(char)) = NULL_CHARACTER;
	ptr -> score = rand() % (MAX_SCORE + 1);
	return ptr;
}

void swap_student(student *a, student *b)
{
	swap_char_ptr(&(a -> name), &(b -> name));
	swap_int(&(a -> score), &(b -> score));
}

void remove_student(student *ptr)
{
	free(ptr -> name);
	if (ptr -> next != ptr && ptr -> prev != ptr)
	{
		ptr -> prev -> next = ptr -> next;
		ptr -> next -> prev = ptr -> prev;
	}
	free(ptr);
}

void print_student(student *ptr)
{
	printf("%-*s %d\n", MAX_LEN_OF_NAME, ptr -> name, ptr -> score);
}

int cmp_student(student *a, student *b)
{
	if (a -> score < b -> score) return SMALLER;
	if (a -> score > b -> score) return LARGER;
	if (strcmp(a -> name, b -> name) == SMALLER) return SMALLER;
	if (strcmp(a -> name, b -> name) == LARGER ) return LARGER;
	return EQUAL;
}

student *generate_list(void)
{
	int num_of_students = generate_rand_int(MIN_NUM_OF_STUDENTS, MAX_NUM_OF_STUDENTS);
	student *head = generate_student();
	student *prev = head;
	student *cur;
	for (int i = 1; i < num_of_students; ++i)
	{
		cur = generate_student();
		cur -> prev = prev;
		prev -> next = cur;
		prev = cur;
	}
	head -> prev = prev;
	prev -> next = head;
	return head;
}

void remove_list(student *ptr)
{
	student *cur = ptr -> next;
	while (cur != ptr)
	{
		student *next = cur -> next;
		remove_student(cur);
		cur = next;
	}
	remove_student(ptr);
}

void print_list(student *ptr)
{
	print_student(ptr);
	student *cur = ptr -> next;
	for (; cur != ptr; cur = cur -> next)
	{
		print_student(cur);
	}
}

void Joseph(student *ptr)
{
	int step = generate_rand_int(MIN_STEP, MAX_STEP);
	while (ptr -> next != ptr)
	{
		for (int i = 0; i < step; ++i)
		{
			ptr = ptr -> next;
		}
		printf("%-*s was killed.\n", MAX_LEN_OF_NAME, ptr -> name);
		student *next = ptr -> next;
		remove_student(ptr);
		ptr = next;
	}
	printf("%-*s survived.\n", MAX_LEN_OF_NAME, ptr -> name);
	remove_student(ptr);
}

void sort_list(student *ptr)
{
	for (int upd = TRUE; upd; )
	{
		upd = FALSE;
		for (student *cur = ptr, *next; ; cur = next)
		{
			next = cur -> next;
			if (next == ptr) break;
			if (cmp_student(cur, next) == LARGER)
			{
				swap_student(cur, next);
				upd = TRUE;
			}
		}
	}
}

