#include "RBTree.h"
#include <stdio.h>

int main(){
	freopen("107.in","r",stdin);
	freopen("107.out","w",stdout);
	int T,id,v;
	scanf("%d", &T);
	while(T--){
		scanf("%d %d",&id,&v);
		if(isempty()){
			if(id == 3 || id == 4 || id == 5) printf("-1\n");
			if(id == 0) create(v);
		}else{
			if(id == 0) {
				insert(newnode(v, 1, 0, NULL, NULL, NULL));
//				outit(root);printf("\n");
			}
			if(id == 1) {
				deletion(fetch_for_deletion(v));
//				outit(root);printf("\n");
			}
			if(id == 2) query_kthminimum(v);
			if(id == 3) query_countless(v);
			if(id == 4) query_prev(v);
			if(id == 5) query_next(v);
//			outit(root);
		}
		//if(T % 10000 == 0)outit(root);
		//printf("\n");
	}destroy();
	return 0;
}

