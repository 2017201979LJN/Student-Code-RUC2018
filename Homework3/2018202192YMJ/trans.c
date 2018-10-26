#include <stdio.h>

#define SEMICOLON ';'
#define NEWLINE '\n'

#define INPUT "data.in"
#define OUTPUT "calc.c"
#define READ_ONLY "r"
#define WRITE_ONLY "w"


int main()
{
	freopen(INPUT, READ_ONLY, stdin);
	freopen(OUTPUT, WRITE_ONLY, stdout);
	printf("#include <stdio.h>\n\n");
	printf("#define SIZE_OF_ARRAY 1\n\n");
	printf("#define OUTPUT \"data.ans\"\n");
	printf("#define WRITE_ONLY \"w\"\n\n\n");
	printf("int main()\n");
	printf("{\n");
	printf("\tfreopen(OUTPUT, WRITE_ONLY, stdout);\n");
	printf("\tdouble arr[SIZE_OF_ARRAY];\n");
	for (int i = 0; ; ++i)
	{
		int c;
		c = getchar();
		if (c == EOF) break;
		printf("\tarr[%d] = ", i);
		putchar(c);
		while ((c = getchar()) != NEWLINE) putchar(c);
		putchar(SEMICOLON);
		putchar(NEWLINE);
	}
	printf("\tfor (int i = 0; i < SIZE_OF_ARRAY; ++i)\n");
	printf("\t\tprintf(\"%%.50le\\n\", arr[i]);\n");
	printf("\treturn 0;\n");
	printf("}\n");
	return 0;
}

