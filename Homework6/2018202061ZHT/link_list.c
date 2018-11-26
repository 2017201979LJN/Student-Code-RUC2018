#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define N 1005
#define MAXN 100005
#define size_of_name 10
int n;
struct student {
	char * name;
	int score;
	struct student *pre, *suf;
};
void init (struct student * s) {
	scanf ("%s", s->name);
	scanf ("%d", &(s->score));
}
void Del (struct student * tode) {
	printf ("%s %d\n", tode->name, tode->score);
	tode->pre->suf = tode->suf;
	tode->suf->pre = tode->pre;
	free (tode->name);
	free (tode);
}
void Josuphus (struct student *first) {
	int K;
	scanf ("%d", &K);
	struct student *cur = first, *tmp;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j < K; j++)
			cur = cur->suf;
		tmp = cur->suf;
		Del (cur);
		cur = tmp;
	}
}
void Swap (struct student * s1, struct student * s2) {
	s1->pre->suf = s2;
	s2->suf->pre = s1;
	s2->pre = s1->pre;
	s1->suf = s2->suf;
	s1->pre = s2;
	s2->suf = s1;
}
void link_sort (struct student *first) {
	struct student * cur, *nxt;
	for (int i = 1; i < n; i++) {
		int flag = 0;
		cur = first;
		for (int j = 1; j < n; j++) {
			nxt = cur->suf;
			if (cur->score < nxt->score || (cur->score == nxt->score && strcmp (cur->name, nxt->name) > 0)) {
				//printf ("%d %d %d %d %d %d\n", cur->score, cur->pre->score, cur->suf->score, nxt->score, nxt->pre->score, nxt->suf->score);
				if (j == 1) first = nxt;
				Swap (cur, nxt);
				flag = 1;
			}
			else cur = cur->suf;
		}
		if (flag == 0) return;
	}
}
int main () {
	struct student *first, *last, *now;
	scanf ("%d", &n);
	first = (struct student *) malloc (sizeof(struct student));
	first->name = (char * )malloc (size_of_name);
	init (first);
	last = first;
	for (int i = 2; i <= n; i++) {
		now = (struct student *) malloc (sizeof(struct student));
		now->name = (char * )malloc (size_of_name);
		//printf ("%p %p\n", s[i]->name, &s[i]->score);
		init (now);
		last->suf = now;
		now->pre = last;
		last = now;
	}
	last->suf = first;
	first->pre = last;
	//Josuphus (first);
	link_sort (first);
	last = last->suf;
	for (int i = 1; i <= n; i++) {
		now = last;
		if (i < n) last = last->suf;
		printf ("%s %d\n", now->name, now->score);
		free (now->name);
		free (now);
	}
	return 0;
}
