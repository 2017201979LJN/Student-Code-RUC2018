#include<stdlib.h>
#include<stdio.h>
#define INF_POS 2000000000
#define INF_NEG -INF_POS
#define NUM 300100

struct tree_node{
	int size, val;
	struct tree_node *parent, *ch[2];
}*root;

int side(struct tree_node *x){
	if(x->parent == NULL) return 0;
	return x->parent->ch[1] == x;
}
void update(struct tree_node *x){
	x->size = 1;
	if(x->ch[0]) x->size += x->ch[0]->size;
	if(x->ch[1]) x->size += x->ch[1]->size;
}

void rotate(struct tree_node *x){
	struct tree_node *p = x->parent,
					 *g = p ? p->parent : NULL, *son;
	int d = side(x), d1 = side(p);
	son = x->ch[d^1];
	
	if(son != NULL) son->parent = p;
	p->ch[d] = son;
	p->parent = x;
	x->ch[d^1] = p;
	if(g != NULL) g->ch[d1] = x;
	x->parent = g;
	
	update(p);
	update(x);
    if(root == p) root=x;
    
}

void splay(struct tree_node *x, struct tree_node *goal){
	for(; x->parent != goal; rotate(x))
		if(x->parent->parent != goal)
			if(side(x) == side(x->parent)) rotate(x->parent);
			else rotate(x); 
}

struct tree_node *find_kth(struct tree_node *x, int k){
	int tmp;
	if(x->ch[0] == NULL) tmp = 0;
	else tmp = x->ch[0]->size;
	if(k <= tmp) return find_kth(x->ch[0], k);
	if(k == tmp+1) return x;
	if(k > tmp) return find_kth(x->ch[1], k-tmp-1);
}

int less_than(struct tree_node *x, int val){
	if(x == NULL) return 0;
	int ret = 0;
	if(x->val >= val) return less_than(x->ch[0], val);
	
	if(x->ch[0] == NULL) ret = 1;
	else ret = 1 + x->ch[0]->size;
	ret += less_than(x->ch[1], val);
	return ret;
	
}
int greater_than(struct tree_node *x, int val){
	if(x == NULL) return 0;
	int ret = 0;
	if(x->val <= val) return greater_than(x->ch[1],val);
	
	if(x->ch[1] == NULL) ret = 1;
	else ret = 1 + x->ch[1]->size;
	
	ret += greater_than(x->ch[0], val);
	return ret;
}

void insert_node(int val){
	int n_less = less_than(root, val);
	struct tree_node *pre = find_kth(root, n_less),
					 *suc = find_kth(root, n_less+1);
	
	splay(pre, NULL);
	splay(suc, pre);
	

	struct tree_node *x = malloc(sizeof(struct tree_node));
    x->val = val;
	x->parent = suc;
	suc->ch[0] = x;
	x->ch[0] = x->ch[1] = NULL;
	x->size = 1;
	
	update(suc);
	update(pre);	
}

void remove_node(int val){
	int rank = less_than(root, val) + 1;
	struct tree_node *pre = find_kth(root, rank-1),
					 *suc = find_kth(root, rank+1);
	splay(pre, NULL);
	splay(suc, pre);
	
    free(suc->ch[0]);
	suc->ch[0] = NULL;
	update(suc);
	update(pre);
}


void pretreatment(){
	// inf_negetive  inf_positive
	struct tree_node *pre = malloc(sizeof(struct tree_node)),
                     *suc = malloc(sizeof(struct tree_node));
	pre->size = 2;
	pre->val = INF_NEG;
	pre->ch[1] = suc;
	pre->parent = pre->ch[0] = NULL;
	root = pre;

	suc->size = 1;
	suc->val = INF_POS;
	suc->ch[0] = suc->ch[1] = NULL;
	suc->parent = pre;
}	

void free_memory(struct tree_node *x){
    if(x == NULL) return;
    if(x->ch[0]) free_memory(x->ch[0]);
    if(x->ch[1]) free_memory(x->ch[1]);
    free(x);
}
	
int main()
{
    //freopen("data.in","r",stdin);
	//freopen("std.out","w",stdout);
    pretreatment();
	int n,x,i,op, m = 0;
	scanf("%d",&n);
	for(i = 1; i <= n; i++){
		scanf("%d%d", &op, &x);
		if(op == 0) insert_node(x), m++;
		if(op == 1) remove_node(x), m--;
		if(op == 2) printf("%d\n", find_kth(root, x+1)->val); 
		if(op == 3) printf("%d\n", less_than(root, x)-1);
		if(op == 4) {
			int rank = less_than(root, x);
			if(rank == 1) printf("-1\n");
			else{
				struct tree_node *pre = find_kth(root, rank);
				printf("%d\n", pre->val);
			}
		}
		if(op == 5) {
			int rank = m+2 - greater_than(root, x);
			if(rank == m+1) printf("-1\n");
			else{
				struct tree_node *suc = find_kth(root, rank+1);
				printf("%d\n", suc->val);
			}
		}
    }
    free_memory(root);
}

