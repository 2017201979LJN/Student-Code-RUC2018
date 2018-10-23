#ifndef _EXPR_H

#define _EXPR_H

#include <stdio.h>
#include <stdlib.h>

#include "expression_tokenizer.h"

typedef enum {
    EXPR_IMME,
    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_NEG,
    EXPR_SYMBOL
} expr_oper_type_t;

typedef struct ExprNode {
    struct ExprNode *node1, *node2;
    expr_oper_type_t type;
    union {
        DOUBLET            data;
        unsigned long long symbol_id;
    };
} expr_node_t;

void printExprNode(expr_node_t* pent);
void freeExprTree(expr_node_t* pent);

expr_node_t* createExprTree(token_t** tokens);

#endif