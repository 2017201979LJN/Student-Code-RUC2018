#include "RBTree.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
	int n;
	double x;
	char ch;
	FILE *in = fopen("data.in","r");
	FILE *fp = fopen("RBTsaver.bin","w");
	FILE *fo = fopen("RBTshape.out","w");
	fscanf(in, "%d", &n);
	for(int i = 1;i <= n; i++){
		fscanf(in, "%lf %c", &x, &ch);
		if(i == 1) create(x, ch);
		else insert(newnode(x, ch, 1, 0, NULL, NULL, NULL));
	}
	compress(fp, fo);
	destroy();
	fclose(in);
	fclose(fp);
	fclose(fo);
	return 0;
}
