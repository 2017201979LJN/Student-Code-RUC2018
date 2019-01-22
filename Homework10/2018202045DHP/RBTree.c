#include "RBTree.h"
#include <stdio.h>
#include <malloc.h>

//double min(double x,double y){return x > y ? y : x;}
//double max(double x,double y){return x > y ? x : y;}

static struct RBTree nil;
static struct RBTree *root = NULL;

struct RBTree *newnode(double val, char ch, int siz, int col, struct RBTree *left, struct RBTree *right, struct RBTree *p){
	struct RBTree *temp = (struct RBTree *) malloc(sizeof(struct RBTree));
	temp->val = val;
	temp->ch = ch;
	temp->siz = siz;
	temp->col = col;
	temp->left = (left == NULL) ? &nil : left;
	temp->right = (right == NULL) ? &nil : right;
	temp->p = (p == NULL) ? &nil : p;
	return temp;
}

int isempty(){
	return (root == NULL) || (root == &nil);
}

void outit(struct RBTree *x, FILE **fo){
	fprintf(*fo, "val:%.3lf ch:%c col:%d\n", x->val, x->ch, x->col);
	fprintf(*fo, "leftchild:%s\n", x->left == &nil? "NO" : "YES");
	fprintf(*fo, "rightchild:%s\n", x->right == &nil? "NO" : "YES");
	fprintf(*fo, "\n");
	if(x->left != &nil)
		outit(x->left, fo);
	if(x->right != &nil)	
		outit(x->right, fo);
}

void create(double x,char ch){
	nil.val = 0; nil.siz = 0; nil.col = 1;
	nil.left = nil.right = nil.p = &nil;
	root = newnode(x, ch, 1, 1, &nil, &nil, &nil);
}


static void totally_delete(struct RBTree *x){
	if(x == &nil)return;
	totally_delete(x->left);
	totally_delete(x->right);
	free(x);
}

void destroy(){
	totally_delete(root);
}

void update(struct RBTree *x){
	x->siz = (x->left->siz) + (x->right->siz) + 1;
}

void left_rotate(struct RBTree *x){
	struct RBTree *y = x->right;
	x->right = y->left;
	if(y->left != &nil)
		y->left->p = x;
	y->p = x->p;
	if(x->p == &nil)
		root = y;
	else if(x == x->p->left)
		x->p->left= y;
	else x->p->right = y;
	y->left = x;
	x->p = y;
	update(x);update(y);
}

void right_rotate(struct RBTree *x){
	struct RBTree *y = x->left;
	x->left = y->right;
	if(y->right != &nil)
		y->right->p = x;
	y->p = x->p;
	if(x->p == &nil)
		root = y;
	else if(x == x->p->right)
		x->p->right = y;
	else x->p->left= y;
	y->right = x;
	x->p = y;
	update(x);update(y);
}

void insert_fixup(struct RBTree *z){
	struct RBTree *y;
	while(z->p->col == 0){
		if(z->p == z->p->p->left){
			y = z->p->p->right;
			if(y->col == 0){
				z->p->col = 1;
				y->col = 1;
				z->p->p->col = 0;
				z = z->p->p;
			}else{
				
				if(z == z->p->right){
					z = z->p;
					left_rotate(z);
				}
				z->p->col = 1;
				z->p->p->col = 0;
				right_rotate(z->p->p);
			}
		}else {
			y = z->p->p->left;
			if(y->col == 0){
				z->p->col = 1;
				y->col = 1;
				z->p->p->col = 0;
				z = z->p->p;
			}else{
				if(z == z->p->left){
					z = z->p;
					right_rotate(z);
				}
				z->p->col = 1;
				z->p->p->col = 0;
				left_rotate(z->p->p);
			}
		}
	}
	root->col = 1;
}

void insert(struct RBTree *z){
	
	struct RBTree *y = &nil;
	struct RBTree *x = root;
	while (x != &nil){
		y = x;
		x->siz++;
		if(z->val < x->val)
			x = x->left;
		else x = x->right;
	}
	z -> p = y;
	if (y == &nil)
		root = z;
	else if (z->val < y->val)
		y->left = z;
	else y->right = z;
	z->left = z->right = &nil;
	z->col = 0;
	insert_fixup(z);
}

void transplant(struct RBTree *u, struct RBTree *v){
	if(u->p == &nil)
		root = v;
	else if(u == u->p->left)
		u->p->left = v;
	else u->p->right = v;
	v->p = u->p;
//	freeall(v);
}


//these functions are designed for the homework especially
//any constant in these functions are formulated elaborately 

double Abs(double x){
	return x>0?x:-x;
}

void traversal(struct RBTree *x, FILE **fp){
	unsigned int oump;
	oump =((int)Abs (x->val * 10000) + 1) / 10;
	if(x->val < 0) oump |= 1u << 31;
	int chmp;
	if(x->ch >= 'A' && x->ch <= 'Z') chmp = x->ch - 'A';
	else chmp = x->ch - 'a' + 26; 
	oump |= chmp << 24;
	oump |= x->col << 30;
	fwrite(&oump, 4, 1, *fp);
//	if((oump & 0x00ffffff) == 0x007ef269) fprintf(stderr, "%.6lf %d\n", x->val, oump&0x00ffffff);
	if(x->left != &nil) traversal(x->left, fp);
	if(x->right != &nil) traversal(x->right, fp);
}

void compress(FILE *fp, FILE *fo){
	outit(root, &fo);
	traversal(root, &fp);
}

unsigned int resolve(struct RBTree *x, FILE **fp, double llim, double rlim,unsigned int gotva){
	if(gotva == 0xffffffffu) 
		if(fread(&gotva, 4, 1, *fp) == 0)
			return -1;
//	fprintf(stderr, "%x\n", gotva);
//	printf("ha1\n");	
	unsigned int inmp = gotva & 0x80ffffffu;
	int chmp = (gotva & 0x3f000000) >> 24;
	int col = (gotva & 0x40000000) >> 30;	
	char ch = chmp <= 25 ? chmp + 'A' : chmp + 'a' -26;
	double db = inmp % (1u << 31) / 1000.0;
	if(inmp > (1u << 31)) db *= -1;
//	printf("%.3f %c %d %.3f %.3f\n", db, ch, col, llim, rlim);

	if(db > llim && db < rlim && db < x->val){
		x->left = newnode(db, ch, 1, col, &nil, &nil, x);
		gotva = resolve(x->left, fp, llim, x->val, 0xffffffffu);
	}

	if(gotva == 0xffffffffu) 
		if(fread(&gotva, 4, 1, *fp) == 0)
			return -1;
//	fprintf(stderr, "%x\n", gotva);
//	printf("ha2\n");		
	inmp = gotva & 0x80ffffffu;
	chmp = (gotva & 0x3f000000) >> 24;
	col = (gotva & 0x40000000) >> 30;	
	ch = chmp <= 25 ? chmp + 'A' : chmp + 'a' -26;
	db = inmp % (1u << 31) / 1000.0;
	if(inmp > (1u << 31)) db *= -1;
//	printf("%.3f %c %d %.3f %.3f\n", db, ch, col, llim, rlim);

	if(db > llim && db < rlim && db > x->val){
		x->right = newnode(db, ch, 1, col, &nil, &nil, x);
		gotva = resolve(x->right, fp, x->val, rlim, 0xffffffffu);
	}
	update(x);
	return gotva;
}

void decompress(FILE *fp, FILE *fo){
	if(root != NULL) destroy(root);
	int gotva;
	fread(&gotva, 4, 1, fp);
//	fprintf(stderr, "%x\n", gotva);
	//s c hhhhhh vvvvvvvvvvvvvvvvvvvvvvvv	
	unsigned int inmp = gotva & 0x80ffffff;	
	int chmp = (gotva & 0x3f000000u) >> 24;
	char ch = chmp <= 25 ? chmp + 'A' : chmp + 'a' - 26;
	double db = inmp % (1u << 31) / 1000.0;
	if(inmp > (1u << 31)) db *= -1;
//	printf("%f %c\n", db, ch);
	create(db, ch);
	resolve(root, &fp, -10001.0, 10001.0, 0xffffffffu);
	outit(root, &fo);
}

