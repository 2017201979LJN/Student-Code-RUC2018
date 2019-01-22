#include <stdio.h>
#define MAXAX 1000000010 

struct RBTree{
	double val;
	char ch;	
	int siz, col;
	struct RBTree *left, *right, *p;
};

//double min(double x,double y);
//double max(double x,double y);

struct RBTree *newnode(double val, char ch, int siz, int col, struct RBTree *left, struct RBTree *right, struct RBTree *p);

int isempty();
void outit(struct RBTree *x, FILE **fo);

void create(double x, char ch);
void destroy();

void update(struct RBTree *x);
void left_rotate(struct RBTree *x);
void right_rotate(struct RBTree *x);

void insert_fixup(struct RBTree *z);
void insert(struct RBTree *z);

void transplant(struct RBTree *u, struct RBTree *v);

void compress(FILE *fp, FILE *fo);
void decompress(FILE *fp, FILE *fo);
