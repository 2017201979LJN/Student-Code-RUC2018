#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define len_of_name 20
#define num_of_used num_of_people + 10

struct List
{
	char *name;
	int score;
	struct List *front, *next;
};
void insert_(struct List *p, char *name, int score)
{
	struct List *q = (struct List *)malloc( sizeof( struct List ) );
	q -> name = name;
	q -> score = score;
	p -> next -> front = q;
	q -> next = p -> next;
	p -> next = q;
	q -> front = p;
}
struct List *creat_list(int num_of_people)
{
	if (num_of_people == 0) return NULL;
	char *name = (char *)malloc( sizeof( char ) * (len_of_name) );
	int score;
	scanf("%s%d", name, &score);
	struct List *p = (struct List *)malloc( sizeof( struct List ) );
	p -> name = name;
	p -> score = score;
	p -> front = p;
	p -> next = p;
	for (int i = 2; i <= num_of_people; i++)
	{
		name = (char *)malloc( sizeof( char ) * (len_of_name) );
		scanf("%s%d", name, &score);
		insert_(p, name, score);
		p = p -> next;
	}
	return p -> next;
}
void delete_(struct List *p)
{
	printf("%s %d\n", p -> name, p -> score);
	if (p -> next == p)
	{
		free(p);
		return ;
	}
	p -> front -> next = p -> next;
	p -> next -> front = p -> front;
	free(p);
	return ;
}
void kill_people(struct List *head, int k)
{
	struct List *p = head;
	while (1)
	{
		for (int i = 1; i <= k; i++)
		{
			p = p -> next;
		}
		p = p -> next;
		int flag = 0;
		if (p -> next == p)
		{
			flag = 1;
		}
		delete_(p -> front);
		if (flag) break;
	}
}
int compare(struct List *a, struct List *b)
{
	if (a -> score != b -> score) return a -> score < b -> score;
	return strcmp(a -> name, b -> name);
}
void connect(struct List *a, struct List *b)
{
	a -> next = b;
	b -> front = a;
}
void print_out(struct List *head, int num_of_people)
{
	for (int i = 1; i <= num_of_people; i++)
	{
		printf("%s %d\n", head -> name, head -> score);
		head = head -> next;
	}
}
void swap(struct List *a, struct List *b)
{
	struct List *f1 = a -> front, *f2 = b -> front, *n1 = a -> next, *n2 = b -> next;
	if (a -> next == b)
	{
		f1 -> next = b;
		n2 -> front = a;
		a -> next = n2;
		a -> front = b;
		b -> next = a;
		b -> front = f1;
		return ;
	}
	else if (a -> front == b)
	{
		a -> next = b;
		a -> front = f2;
		b -> next = n1;
		b -> front = a;
		f2 -> next = a;
		n2 -> front = b;
		return ;
	}
	else
	{
		a -> next = n2;
		a -> front = f2;
		n2 -> front = a;
		f2 -> next = a;
		b -> next = n1;
		b -> front = f1;
		f1 -> next = b;
		n1 -> front = b;
		return ;
	}
}
void swap_int(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
int equal(struct List *a, struct List *b)
{
	return a -> score == b -> score && a -> name == b -> name;
}
struct List *sort(struct List *head, int num_of_people)
{
	if (num_of_people == 0) return NULL;
	char used[num_of_used];
	memset(used, 0, sizeof(used) );
	int new_list[num_of_people + 1], id[num_of_people + 1];
	struct List *old_list[num_of_people + 1];
	struct List *l = head;
	for (int i = 1; i <= num_of_people; i++)
	{
		old_list[i] = l;
		l = l -> next;
	}
	for (int i = 1; i <= num_of_people; i++)
	{
		struct List *mini = NULL, *p = head;
		int pl;
		for (int j = 1; j <= num_of_people; j++)
		{
			if (!used[j])
			{
				if (mini == NULL || compare(mini, p) == 1)
					mini = p, pl = j;
			}
			p = p -> next;
		}
		used[pl] = 1;
		new_list[i] = pl;
		id[pl] = i;
	}
	for (int i = 1; i <= num_of_people; i++)
	{
		int pl = id[i];
		int front = new_list[id[i] == 1 ? num_of_people : (id[i] - 1)];
		int next = new_list[id[i] == num_of_people ? 1 : (id[i] + 1)];
		old_list[i] -> front = old_list[ front ];
		old_list[i] -> next = old_list[ next ];
		if (id[i] == 1) head = old_list[i];
	}
	return head;
}

int main()
{
	freopen("data.txt", "r", stdin);
	freopen("answer.txt", "w", stdout);
	int num_of_people, k;
	scanf("%d%d", &num_of_people, &k);
	struct List *head = creat_list(num_of_people);
	head = sort(head, num_of_people);
	print_out(head, num_of_people);
//	kill_people(head, k);
	fclose(stdin);
	return 0;
}
