#include<stdio.h>
#include<string.h>
#include<memory.h>
#include<stdlib.h>
#include<time.h>

#define STU_MIN 80
#define RAND_ZONE 20
#define NAME_MAX 20
#define GPA_MAX 4.0
#define ACUR 100.0
#define CHAR_SET 26

struct Student{
	char *name;
	double GPA;
	int len;
	struct Student *prev, *next;
};



double Drand(){
	return ((double) (rand() % ((int)(GPA_MAX * ACUR)) + 1)) / ACUR;
}

void swap(struct Student *x, struct Student *y){
	char *name = x -> name;	x -> name = y -> name;	y -> name = name;
	double GPA = x -> GPA;	x -> GPA = y -> GPA;	y -> GPA = GPA;
	int len = x -> len;	x -> len = y -> len;	y -> len = len;
}

void real_slow_sort(struct Student *data){
	struct Student *S = data, *T = data -> prev;
	for(struct Student *i = S; i != T; i = i -> next){
		struct Student *best = i;
		for(struct Student *j = i -> next; j != S; j = j -> next){
			if(best -> GPA < j -> GPA) best = j;
			else if(best -> GPA == j -> GPA && strcmp(j -> name, best -> name) < 0) best = j;
		}
		swap(i, best);
	}
	struct Student *i = S;
	do{
		printf("%s", i -> name);
		for(int j = 1; j <= NAME_MAX + 1 - i -> len; j++)
			printf(" ");
		printf("%.2lf\n", i -> GPA);
		i = i -> next;
	} while(i != S);
}

void delete_s(struct Student *s) {
	printf("%s", s -> name);
	for(int i = 1; i <= NAME_MAX + 1 - s -> len; i++)
		printf(" ");
	printf("%.2lf\n", s -> GPA);
	struct Student *temp1 = s -> prev, *temp2 = s -> next;
	temp1 -> next = temp2;
	temp2 -> prev = temp1;
}

void Joseph_game(struct Student *data, int left){
	int nexv;
	scanf("%d", &nexv);
	struct Student *s = data;
	while(s -> prev != s && s -> next != s){
		delete_s(s);
		left --;
		free(s -> name);
		for(int i = 1; i <= nexv; i++){
			s = s -> next;
			printf("(%s)\n", s -> name);
		}
	}
	printf("AILVE!!! : %s\n", s -> name);
}

int main(){
	int operation;
	struct Student data[STU_MIN + RAND_ZONE + 1];
	freopen("students.out", "w", stdout);
	srand(time(NULL));
	int n = STU_MIN + rand() % RAND_ZONE + 1;
	for(int i = 0; i < n; i++){
		(data + i) -> name = malloc(NAME_MAX);
		(data + i) -> GPA = Drand();
		int nlen = rand() % NAME_MAX + 1;
		(data + i) -> len = nlen;
		*(((data + i) -> name) + 0) = 'A' + rand() % CHAR_SET;
		for(int j = 1; j < nlen; j++)
		*(((data + i) -> name) + j) = 'a' + rand() % CHAR_SET;
		*(((data + i) -> name) + nlen) = 0;
		if(i == 0) 
			(data + i) -> prev = data + n - 1;
		else
			(data + i) -> prev = data + i - 1;
		(data + i) -> prev -> next = data + i;
	}
	/*
	for(int i = 0; i < n; i++){
		printf("%s", (data + i) -> name);
		for(int j = 1; j <= NAME_MAX + 1 - ((data + i) -> len); j++) 
			printf(" ");
		printf("%.2lf ", (data + i) -> GPA);
		printf("prev:%c next:%c\n", *((data + i) -> prev -> name), *((data + i) -> next -> name));
	}
	*/
	scanf("%d", &operation);
	if(operation == 1) real_slow_sort(data);
	else if(operation == 2) Joseph_game(data, n);
	return 0;
}
