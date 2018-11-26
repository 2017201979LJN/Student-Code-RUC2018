#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_NUM_OF_STUDENT 1000
#define MAX_NAME_LEN 5
#define MIN_NAME_LEN 3
#define SIZE_OF_LETTER 26
#define MAX_SCORE 101

struct student {
	char *name;
	int score;
	struct student *next, *last;
};

struct student *generate_list(int n);
struct student *generate_student();
void print_list(struct student *head);
void print_student(struct student *a);
void kill_list(struct student *head);
void kill_student(struct student *a);
void josephus(struct student *head, int n);
struct student *sort_list(struct student *head, int n);
int greater(struct student *a, struct student *b);
void swap_student(struct student *a, struct student *b);

int main() {
	srand(time(0));
	
	int num_of_student;
	printf("input num of students: ");
	scanf("%d", &num_of_student);
	struct student *head = generate_list(num_of_student);
	print_list(head);

	printf("input operator(1.kill 2.sort): ");
	int type_of_operator;
	scanf("%d", &type_of_operator);
	
	if (type_of_operator == 1) {
		josephus(head, num_of_student);
	}
	else if (type_of_operator == 2) {
		head = sort_list(head, num_of_student);
		print_list(head);
		kill_list(head);
	}
	return 0;
}
struct student *generate_list(int n) {
	struct student *head = generate_student();
	struct student *current, *last = head;
	for (int i = 1; i < n; i++) {
		current = generate_student();
		last->next = current;
		current->last = last;
		last = current;
	}
	last->next = head;
	head->last = last;
	return head;
}
struct student *generate_student() {
	struct student *a = malloc(sizeof(struct student));
	int name_len = rand() % (MAX_NAME_LEN - MIN_NAME_LEN + 1) + MIN_NAME_LEN;
	a->name = malloc(name_len * sizeof(char));
	for (int i = 0; i < name_len; i++) {
		*(a->name + i) = rand() % SIZE_OF_LETTER + 'a';
	}
	a->score = rand() % MAX_SCORE;
	return a;
}
void print_list(struct student *head) {
	printf("-----------------\n");
	printf("| name  | score |\n");
	printf("|---------------|\n");
	struct student *current = head;
	do {
		print_student(current);
		current = current->next;
	} while (current != head);
	printf("-----------------\n");
}
void print_student(struct student *a) {
	printf("| %-6s| %-6d|\n", a->name, a->score);
}

void kill_list(struct student *head) {
	struct student *current = head->next;
	while (current != head) {
		struct student *tmp = current->next;
		kill_student(current);
		current = tmp;
	}
	kill_student(head);
}
void kill_student(struct student *a) {
	free(a->name);
	if (a->last != NULL && a->next != NULL) {
		a->last->next = a->next;
		a->next->last = a->last;
	}
	free(a);
}
void josephus(struct student *head, int n) {
	int k;
	printf("input k: ");
	scanf("%d", &k);
	struct student *current = head;
	int count;
	for (int i = 1; i < n; i++) {
		printf("round%4d: ", i);
		count = k - 1;
		while (count--) current = current->next;
		printf("%-6swas killed!\n", current->name);
		struct student *tmp = current->next;
		kill_student(current);
		current = tmp;
	}
	printf("final    : %-6swins!\n", current->name);
	kill_student(current);
}
struct student *sort_list(struct student *head, int n) {
	struct student *max, *current = head, *tmp;
	for (int i = 0; i < n - 1; i++) {
		max = current;
		tmp = current->next;
		for (int j = i + 1; j < n; j++) {
			if (greater(tmp, max)) max = tmp;
			tmp = tmp->next;
		}
		swap_student(current, max);
		current = max->next;
	}
	return current->next;
}

int greater(struct student *a, struct student *b) {
	if (a->score > b->score) return 1;
	if (a->score < b->score) return 0;
	if (strcmp(a->name, b->name) < 0) return 1;
	return 0;
}

void swap_student(struct student *a, struct student *b) {
	if (a->next == b) {
		a->last->next = b;
		b->next->last = a;
		a->next = b->next;
		b->last = a->last;
		a->last = b;
		b->next = a;
		return;
	}
	if (a->last == b) {
		a->next->last = b;
		b->last->next = a;
		a->last = b->last;
		b->next = a->next;
		a->next = b;
		b->last = a;
		return;
	}
	a->last->next = b;
	a->next->last = b;
	b->last->next = a;
	b->next->last = a;
	struct student *tmp1, *tmp2;
	tmp1 = a->last;
	tmp2 = a->next;
	a->last = b->last;
	a->next = b->next;
	b->last = tmp1;
	b->next = tmp2;

}
