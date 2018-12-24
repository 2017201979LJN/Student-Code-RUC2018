#include "treap.h"

static FILE *output;

int main()
{
	int n;
	scanf("%d", &n);
	output = fopen("data.out", "w");
	for (int ope, x; n--; )
	{
		scanf("%d %d", &ope, &x);
		switch (ope)
		{
		case 0:
			Treap_Insert(x);
			break;
		case 1:
			Treap_Delete(x);
			break;
		case 2:
			fprintf(output, "%d\n", Treap_Query_Kth_Element(x));
			break;
		case 3:
			fprintf(output, "%d\n", Treap_Query_Rank(x));
			break;
		case 4:
			fprintf(output, "%d\n", Treap_Query_Predecessor(x));
			break;
		case 5:
			fprintf(output, "%d\n", Treap_Query_Successor(x));
			break;
		default:
			fprintf(stderr, "error: unknown operator %d\n", ope);
			break;
		}
	}
	Treap_Destruct();
	fclose(output);
	return 0;
}
