#define MAXAX 1000000010 

struct RBTree{
	int val, siz, col;
	struct RBTree *left, *right, *p;
};

int min(int x,int y);
int max(int x,int y);

struct RBTree *newnode(int val, int siz, int col, struct RBTree *left, struct RBTree *right, struct RBTree *p);

int isempty();
void outit(struct RBTree *x);

void create(int x);
void destroy();

void update(struct RBTree *x);
void left_rotate(struct RBTree *x);
void right_rotate(struct RBTree *x);

void insert_fixup(struct RBTree *z);
void insert(struct RBTree *z);

void transplant(struct RBTree *u, struct RBTree *v);
struct RBTree *fetch_for_deletion(int v);
struct RBTree *minimum_for_deletion(struct RBTree *x);
void delete_fixup(struct RBTree *x);
void deletion(struct RBTree *z);

void query_kthminimum(int v);
void query_countless(int v);
void query_prev(int v);
void query_next(int v);

