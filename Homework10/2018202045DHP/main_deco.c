#include "RBTree.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fp = fopen("RBTsaver.bin","r");
	FILE *fo = fopen("RBTshape_decompressed.out","w");
	decompress(fp, fo);
	destroy();
	fclose(fp);
	fclose(fo);
	return 0;
}
