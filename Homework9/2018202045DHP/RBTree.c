#include "RBTree.h"
#include <stdio.h>
#include <malloc.h>

int min(int x,int y){return x > y ? y : x;}
int max(int x,int y){return x > y ? x : y;}

static struct RBTree nil;
static struct RBTree *root = NULL;

struct RBTree *newnode(int val, int siz, int col, struct RBTree *left, struct RBTree *right, struct RBTree *p){
	struct RBTree *temp = (struct RBTree *) malloc(sizeof(struct RBTree));
	temp->val = val;
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

void outit(struct RBTree *x){
	if(x == &nil) return;
	struct RBTree * y;
	printf("%s %d size:%d :\n", x->col == 0 ? "red" : "black", x->val, x->siz);
	
	printf("\t left  child :");
	y = x->left;
	if(y == &nil) printf("nil\n");
	else printf("%s %d\n", y->col == 0 ? "red" : "black", y->val);
	
	printf("\t right child :");
	y = x->right;
	if(y == &nil) printf("nil\n");
	else printf("%s %d\n", y->col == 0 ? "red" : "black", y->val);
	outit(x->left);
	outit(x->right);

}

void create(int x){
	nil.val = 0; nil.siz = 0; nil.col = 1;
	nil.left = nil.right = nil.p = &nil;
	root = newnode(x, 1, 1, &nil, &nil, &nil);
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

struct RBTree *fetch_for_deletion(int v){
	struct RBTree *x = root;
	x->siz--;
	while(x->val != v){
		if(v < x->val) x = x->left;
		else x = x->right;
		x->siz--;
	}return x;
}

struct RBTree *minimum_for_deletion(struct RBTree *x){
	while(x->left != &nil){
		x->siz--;
		x = x->left;
	}
	return x;
}

void delete_fixup(struct RBTree *x){
	while(x != root && x->col == 1){
		if(x == x->p->left){
			struct RBTree *w = x->p->right;
			if(w->col == 0){
				w->col = 1;
				x->p->col = 0;
				left_rotate(x->p);
				w = x->p->right;
			}
			if(w->left->col == 1 && w->right->col == 1){
				w->col = 0;
				x = x->p;
			}else{
				if(w->right->col == 1){
					w->left->col = 1;
					w->col = 0;
					right_rotate(w);
					w = x->p->right;
				}w->col = x->p->col;
				x->p->col = 1;
				w->right->col = 1;
				left_rotate(x->p);
				x = root;
			}
		}else{
			struct RBTree *w = x->p->left;
			if(w->col == 0){
				w->col = 1;
				x->p->col = 0;
				right_rotate(x->p);
				w = x->p->left;
			}
			if(w->right->col == 1 && w->left->col == 1){
				w->col = 0;
				x = x->p;
			}else{
				if(w->left->col == 1){
					w->right->col = 1;
					w->col = 0;
					left_rotate(w);
					w = x->p->left;
				}w->col = x->p->col;
				x->p->col = 1;
				w->left->col = 1;
				right_rotate(x->p);
				x = root;
			}
		}
	}x->col = 1;
}

void deletion(struct RBTree *z){
	struct RBTree *y = z, *x;
	int y_original_col = y->col;
	if(z->left == &nil){
		x = z->right;
		transplant(z, z->right);
//		update(x);
//		free(z);
	}else if(z->right == &nil){
		x = z->left;
		transplant(z, z->left);
//		update(x);
//		free(z);
	}else{
		y = minimum_for_deletion(z->right);
		y_original_col = y->col;
		x = y->right;
		if(y->p == z)
			x->p = y;
		else {
			transplant(y,y->right);
			y->right = z->right;
			y->right->p = y;
		}transplant(z,y);		
		y->left = z->left;
		y->left->p = y;
		y->col = z->col;
		update(y);
//		free(z);
}
//	outit(root);
	if(y_original_col == 1)
		delete_fixup(x);
	free(z);
}

void query_kthminimum(int v){
	struct RBTree *x = root;
	while(1){
		if(v <= x->left->siz) x = x->left;
		else if(v == x->left->siz + 1){
			printf("%d\n", x->val);
			return;
		}else{
			v -= x->left->siz+1;
			x = x->right;
		}
	}
}

void query_countless(int v){
	int ans=0;
	struct RBTree *x = root;
	while(x != &nil){
		if(x->val >= v) x = x->left;
		else{
			ans += x->left->siz+1;
			x = x->right;
		}
	}printf("%d\n",ans);
}

void query_prev(int v){
	int ans = -1;
	struct RBTree *x = root;
	while(x != &nil){
		if(x->val >= v) x = x->left;
		else{
			ans = max(ans, x->val);
			x = x->right;
		}
	}printf("%d\n", ans == -1 ? -1 : ans);
}

void query_next(int v){
	int ans = MAXAX;
	struct RBTree *x = root;
	while(x != &nil){
		if(x->val <= v) x = x->right;
		else{
			ans = min(ans, x->val);
			x = x->left;
		}
	}printf("%d\n", ans == MAXAX ? -1 : ans);
}

