#include "splay.h"

int main()
{
    freopen("data.in","r",stdin);
	freopen("data.out","w",stdout);
    pretreatment();
	int n,x,i,op, m = 0;
	scanf("%d",&n);
	for(i = 1; i <= n; i++){
		scanf("%d%d", &op, &x);
		if(op == 0) insert_node(x), m++;
		if(op == 1) remove_node(x), m--;
		if(op == 2) printf("%d\n", find_kth(get_root(), x+1)->val); 
		if(op == 3) printf("%d\n", less_than(get_root(), x)-1);
		if(op == 4) {
			int rank = less_than(get_root(), x);
			if(rank == 1) printf("-1\n");
			else{
				struct tree_node *pre = find_kth(get_root(), rank);
				printf("%d\n", pre->val);
			}
		}
		if(op == 5) {
			int rank = m+2 - greater_than(get_root(), x);
			if(rank == m+1) printf("-1\n");
			else{
				struct tree_node *suc = find_kth(get_root(), rank+1);
				printf("%d\n", suc->val);
			}
		}
    }
    free_memory(get_root());
}
