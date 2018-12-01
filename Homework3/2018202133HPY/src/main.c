#include <stdio.h>
#include <string.h>

#include "double_t_util.h"
#include "expression.h"
#include "expression_calc_util.h"
#include "expression_tokenizer.h"

int      num = 0;
token_t  tokens[1024];
token_t* begin = tokens;

void Init()
{
    tokenizerInit();
    doubleUtilInit();
}

int main(void)
{
    Init();
    char  str[10240];
    char* p = str;
    fgets(str, sizeof str, stdin);

    while (*p) {
        if ((tokens[num++] = getNextToken(&p)).type == TOKEN_SYNTAX_ERROR) {
            puts("SYNTAX ERROR ON TOKENIZING");
            exit(0);
        }
    }

    expr_node_t* node = createExprTree(&begin);

    if (node == NULL || begin->type != TOKEN_LIST_END) {
        puts("SYNTAX ERROR ON PARSING");
        exit(0);
    }

    DOUBLET ans = expr_tree_calc(node);

    printf("%s\n", DOUBLETToString(ans));

    freeExprTree(node);

    return 0;
}