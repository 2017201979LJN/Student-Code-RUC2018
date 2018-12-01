#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define N 1005
int n,x[5];
char s[N];
int opt (char c) {
	if (c == '+') return 1;
	return 2;
}
int pos (long x) {
	return (x - 5) * 8;
}
int main () {
	freopen ("opt.in", "r", stdin);
	freopen ("opt2.out" ,"w" ,stdout);
	while (scanf (" %[^\n]", s) != EOF) {
		n = strlen (s);
		int ty = 0;
		for (int i = 0; i < n; i++) {
			if (s[i] == '{') {
				ty = 1; break;
			}
			if (s[i] == '}') {
				ty = 2; break;
			}
			if (s[i] == 'r' && s[i+1] == 'e') {
				ty = 3; break;
			}
		}
		if (ty == 0) {
			long x1, x2, x3, op;
			int t2, t3;
			int i = 0;
			while (i < n && s[i] == ' ') i++;
			i++;
			x1 = s[i] - '0';
			i++;
			while (i < n && s[i] == ' ') i++;
			i++;
			while (i < n && s[i] == ' ') i++;
			
			if (s[i] >= '0' && s[i] <= '9') {
				t2 = 1;
				x2 = s[i] - '0';
				i++;
				while (s[i] >= '0' && s[i] <= '9') {
					x2 = x2 * 10 + s[i] - '0';
					i++;
				}
			}
			else {
				i++;
				t2 = 2;
				x2 = s[i] - '0';
				i++;
			}
			while (i < n && s[i] == ' ') i++;
			op = opt (s[i]);
			i++;
			while (i < n && s[i] == ' ') i++;
			if (s[i] >= '0' && s[i] <= '9') {
				t3 = 1;
				x3 = s[i] - '0';
				i++;
				while (s[i] >= '0' && s[i] <= '9') {
					x3 = x3 * 10 + s[i] - '0';
					i++;
				}
			}
			else {
				i++;
				t3 = 2;
				x3 = s[i] - '0';
				i++;
			}
			if (t2 == 2){
				if (t3 == 2) {
					printf ("movq %d(%rsp) %rdx\n", pos (x2));
					printf ("movq %d(%rsp) %rax\n", pos (x3));
					if (op == 1) printf ("addq ");
					else printf ("imulq ");
					puts ("%rdx %rax");
					printf ("movq %rax %d(%rsp)\n", pos (x1));
				}
				else {
					printf ("movq %d(%rsp) %rax\n", pos (x2));
					if (op == 1) printf ("addq ");
					else printf ("imulq ");
					printf ("$%lld %rax\n", x3);
					printf ("movq %rax %d(%rsp)\n", pos (x1));
				}
			}
			else {
				printf ("movq %d(%rsp) %rax\n", pos (x3));
				if (op == 1) printf ("addq ");
				else printf ("imulq ");
				printf ("$%lld %rax\n", x2);
				printf ("movq %rax %d(%rsp)\n", pos (x1));
			}
		}
		if (ty == 1) {
			puts ("calc:");
			puts ("pushq %rbp");
			puts ("movq %rsp %rbp");
		}
		if (ty == 3) {
			int i = 0;
			while (i < n && s[i] != 'n') i++;
			i++;
			while (i < n && s[i] == ' ') i++;
			i++;
			int xi = s[i] - '0';
			printf ("movq %d(%rsp) %rax\n", pos (xi));
			puts ("popq %rbp");
			puts ("ret");
		}
	}
	return 0;
}
