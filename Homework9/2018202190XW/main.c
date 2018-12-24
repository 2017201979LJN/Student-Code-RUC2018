#include <stdio.h>
#include "sbt.h"

int main() {
	FILE *output = fopen("data.out", "a");
	NUL.ch[LT] = NUL.ch[RT] = &NUL;
	NUL.key = NUL.size = 0;
	SBT root = &NUL;
	int T;
	scanf("%d", &T);
	while (T--) {
		int op, x;
		scanf("%d%d", &op, &x);
		switch (op) {
			case 0 :
				SBT_insert_value(&root, x);
				break;
			case 1 :
				SBT_delete(&root, x);
				break;
			case 2 :
				fprintf(output, "%d\n", SBT_num(root, x));
				break;
			case 3 :
				fprintf(output, "%d\n", SBT_rank(root, x));
				break;
			case 4 :
				fprintf(output, "%d\n", SBT_pred(root, x));
				break;
			case 5 :
				fprintf(output, "%d\n", SBT_succ(root, x));
				break;
			default :
				break;
		}
	}
	SBT_destruct(root);
	fclose(output);
	return 0;
}
