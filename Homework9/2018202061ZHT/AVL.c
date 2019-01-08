#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define N 300005
#define inf 2e9
#define Max(x,y) ((x)>(y)?(x):(y))
#define Min(x,y) ((x)<(y)?(x):(y))
int Q,k,x;
	struct node{
		int value,num,size,height;
		struct node *ch[2];
	}*nil,*root;
	struct node *newnode(){
		struct node *x;
		x=malloc(sizeof(struct node ));
		x->value=x->num=x->size=x->height=0;
		x->ch[0]=x->ch[1]=nil;
		return x;
	}
	void updata(struct node *x){
		x->size=x->ch[0]->size+x->ch[1]->size+x->num;
		x->height=Max(x->ch[0]->height,x->ch[1]->height)+1;
	}
	struct node * rotate(struct node *x,int f){
		struct node *y=x->ch[!f];
		x->ch[!f]=y->ch[f];
		y->ch[f]=x;
		updata(x);updata(y);
		return y;
	}
	struct node * rebalance(struct node *x,int f){
		struct node *p=x->ch[f];
		if(x->ch[!f]->height+1<p->height){
			if(p->ch[!f]->height>p->ch[f]->height)
				p=rotate(p,f);
			x->ch[f]=p;
			x=rotate(x,!f);
		}
		else updata(x);
		return x;
	}
	struct node * insert(struct node *x,int v){
		if(x==nil){
			x=newnode();
			x->value=v;
			x->num=x->size=1;
			x->height=1;
			return x;
		}
		if(x->value==v){
			x->num++;
			x->size++;
			return x;
		}
		int f=(v>x->value);
		x->ch[f]=insert(x->ch[f],v);
		x=rebalance(x,f);
			//printf("%p\n",x);
			//printf("%d %d %d %d\n",x->value,x->num,x->size,x->height);
		return x;
	}
	struct node * delet(struct node *x,int v){
		if(x==nil) return nil;
		//printf("%d %d %d %d %d\n",v,x->value,x->num,x->size,x->height);
		if(x->value==v){
			//puts("aaa");
			if(x->num>1){
				x->num--;
				x->size--;
			}
			else{
				struct node *y=x;
				if(x->ch[0]==nil){
					if(x->ch[1]==nil){
						x=nil;
						free(y);
					}
					else{
						x=x->ch[1];
						free(y);
					}
				}
				else{
					if(x->ch[1]==nil){
						x=x->ch[0];
						free(y);
					}
					else{
						//puts("orzwwx");
						if(x->ch[0]->height>x->ch[1]->height)
							x=rotate(x,1);
						else x=rotate(x,0);
						x=delet(x,v);
					}
				}
			}
			return x;
		}
		else{
			int f=(v>x->value);
			x->ch[f]=delet(x->ch[f],v);
			x=rebalance(x,!f);
			return x;
		}
	}
	int ask_k(struct node *x,int k){
		//printf("%d %d\n",x->value,x->size);
		if(x==nil) return 0;
		if(k<=x->ch[0]->size)
			return ask_k(x->ch[0],k);
		k-=x->ch[0]->size;
		//printf("k=%d\n",k);
		if(k<=x->num)
			return x->value;
		//printf("k=%d\n",k);
		k-=x->num;
		return ask_k(x->ch[1],k);
	}
	int ask_num(struct node *x,int v){
		if(x==nil) return 0;
		if(v<=x->value)
			return ask_num(x->ch[0],v);
		else
			return ask_num(x->ch[1],v)+x->ch[0]->size+x->num;
	}
	int ask_pre(struct node *x,int v){
		if(x==nil) return -1;
		if(v<=x->value)
			return ask_pre(x->ch[0],v);
		else
			return Max(x->value,ask_pre(x->ch[1],v));
	}
	int ask_suf(struct node *x,int v){
		if(x==nil) return inf;
		if(v<x->value)
			return Min(x->value,ask_suf(x->ch[0],v));
		else
			return ask_suf(x->ch[1],v);
	}
	void recycle(struct node *x){
		if(x==nil) return;
		recycle(x->ch[0]);
		recycle(x->ch[1]);
		free(x);
	}
	void debug(struct node *x){
		if(x==nil) return ;
		debug(x->ch[0]);
		printf("%d %d %d %d\n",x->value,x->num,x->size,x->height);
		debug(x->ch[1]);
	}
	void chushi(){
		nil=newnode();
		root=nil;
	}
	void Recycle(){
		recycle(root);
		free(nil);
	}
	void Insert(int x){
		root=insert(root,x);
	}
	void Delet(int x){
		root=delet(root,x);
	}
	int Ask_K(int x){
		return ask_k(root,x);
	}
	int Ask_Num(int x){
		return ask_num(root,x);
	}
	int Ask_Pre(int x){
		return ask_pre(root,x);
	}
	int Ask_Suf(int x){
		int ans=ask_suf(root,x);
		if(ans==inf) return -1;
		return ans;
	}
	void Debug(){
		debug(root);
	}
int main(){
	scanf("%d",&Q);//printf("%d\n",sizeof(struct node));
	chushi();//printf("%p\n",nil);
	while(Q--){
		scanf("%d%d",&k,&x);
		if(k==0) Insert(x);
		if(k==1) Delet(x);
		if(k==2) printf("%d\n",Ask_K(x));
		if(k==3) printf("%d\n",Ask_Num(x));
		if(k==4) printf("%d\n",Ask_Pre(x));
		if(k==5) printf("%d\n",Ask_Suf(x));
		//puts("DEBUG:");
		//Debug();
	}
	//Debug();
	Recycle();
	return 0;
}
