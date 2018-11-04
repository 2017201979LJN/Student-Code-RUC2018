#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expr_reader.h"

int      symbol_num = 0;
char*    symbol_names[SYMBOL_NUM];
factor_t symbols[SYMBOL_NUM];

factor_t getNextFactor()
{
    static char name[FACTOR_MAX_LENGEH];

    int top = 0;

    factor_t ret;

    char ch = ' ';

    while (isspace(ch))
        ch = getchar();

    if (feof(stdin)) {
        ret.type = FACTOR_EOF;
    }
    else if (isdigit(ch)) { //IMME
        name[top++] = ch;
        while (isdigit(ch = getchar())) {
            name[top++] = ch;
        }
        ungetc(ch, stdin);

        name[top] = '\0';

        ret.type = FACTOR_IMME;
        ret.var  = atoi(name);
    }
    else if (isalpha(ch)) { //symbol || int
        name[top++] = ch;
        while (isalnum(ch = getchar())) {
            name[top++] = ch;
        }
        ungetc(ch, stdin);

        name[top] = '\0';

        if (!strcmp(name, "int")) { //int
            ret.type = FACTOR_INT;
        }
        else {
            ret.type = FACTOR_SYMBOL;

            int i = 0;
            for (i = 0; i < symbol_num; i++) {
                if (!strcmp(symbol_names[i], name)) {
                    break;
                }
            }
            if (i >= symbol_num) {
                assert(i < SYMBOL_NUM);
                symbol_names[i] = malloc(strlen(name) + 1);
                strcpy(symbol_names[i], name);
                symbol_num++;
            }
            ret.sym_id = i;
        }
    }
    else { //operator
        ret.type = FACTOR_OPERATOR;
        ret.op   = ch;
    }

    return ret;
}