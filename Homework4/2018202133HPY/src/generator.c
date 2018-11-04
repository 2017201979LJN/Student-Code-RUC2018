#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* generateVarName()
{
    static char names[1024];

    const int var_name_len = 5;

    for (int i = 0; i < var_name_len; i++) {
        names[i] = 'a' + rand() % 26;
    }

    names[var_name_len] = '\0';

    return names;
}

int generateOperator()
{
    return (rand() & 1) ? '+' : '*';
}

const char* generateFactor(char** names, int var_num)
{
    static char factor[1024];

    if (rand() & 1) {
        sprintf(factor, "%d", (int)rand());
    }
    else {
        sprintf(factor, "%s", names[rand() % var_num]);
    }

    return factor;
}

const char* generateExpr(char** names, int var_num)
{
    static char expr[4096];
    static char fact1[1024], fact2[1024];

    if (rand() & 1) {
        sprintf(expr, "%s = %s;", names[rand() % var_num], generateFactor(names, var_num));
    }
    else {
        strcpy(fact1, generateFactor(names, var_num));
        strcpy(fact2, generateFactor(names, var_num));
        sprintf(expr,
                "%s = %s %c %s;",
                names[rand() % var_num],
                fact1,
                generateOperator(),
                fact2);
    }

    return expr;
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        printf("Usage: %s <var_num> <expr_num>\n", argv[0]);
        exit(1);
    }

    int   var_num = atoi(argv[1]), expr_num = atoi(argv[2]);
    char* names[5];

    for (int i = 0; i < var_num; i++)
        names[i] = malloc(1024);

    for (int i = 0; i < var_num; i++)
        strcpy(names[i], generateVarName());

    printf("int %s = 0", names[0]);
    for (int i = 1; i < var_num; i++) {
        printf(", %s = 0", names[i]);
    }
    printf(";\n");

    for (int i = 0; i < expr_num; i++) {
        printf("%s\n", generateExpr(names, var_num));
    }

    for (int i = 0; i < var_num; i++)
        free(names[i]);

    return 0;
}