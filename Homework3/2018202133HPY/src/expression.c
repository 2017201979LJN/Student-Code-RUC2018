#include "expression.h"
#include "symbols.h"

expr_node_t* newExprNode()
{
    expr_node_t* ret = (expr_node_t*)malloc(sizeof(expr_node_t));

    ret->node1 = NULL;
    ret->node2 = NULL;
    ret->type  = EXPR_IMME;

    return ret;
}

void printExprNode(expr_node_t* pent)
{
    if (pent == NULL)
        return;
    printExprNode(pent->node1);
    printExprNode(pent->node2);
    switch (pent->type) {
    case EXPR_IMME:
        printf("EXPR_IMME#%.1le ", REINTERPRET_TO_DOUBLE(pent->data));
        break;

    case EXPR_ADD:
        printf("EXPR_ADD ");
        break;

    case EXPR_SUB:
        printf("EXPR_SUB ");
        break;

    case EXPR_MUL:
        printf("EXPR_MUL ");
        break;

    case EXPR_DIV:
        printf("EXPR_DIV ");
        break;

    case EXPR_NEG:
        printf("EXPR_NEG ");
        break;

    case EXPR_SYMBOL:
        printf("EXPR_SYMBOL#%llu ", pent->symbol_id);
    }
}

void freeExprTree(expr_node_t* pent)
{
    if (pent->node1 != NULL) {
        freeExprTree(pent->node1);
    }
    if (pent->node2 != NULL) {
        freeExprTree(pent->node2);
    }
    free(pent);
}

expr_node_t* getExactFactor(token_t** tokens);
expr_node_t* getExactTerm(token_t** tokens);
expr_node_t* getExactExpr(token_t** tokens);

expr_node_t* getExactFactor(token_t** tokens)
{
    expr_node_t* ret = NULL;

    token_t* backup = *tokens;

    switch ((*tokens)[0].type) {
    case TOKEN_OPER_ADD:
        (*tokens)++;
        ret = getExactFactor(tokens);
        if (ret == NULL)
            *tokens = backup;
        break;
    case TOKEN_OPER_SUB:
        ret       = newExprNode();
        ret->type = EXPR_NEG;
        (*tokens)++;
        ret->node1 = getExactFactor(tokens);
        if (ret->node1 == NULL) {
            freeExprTree(ret);
            *tokens = backup;
            ret     = NULL;
        }
        break;
    case TOKEN_IMME:
        ret       = newExprNode();
        ret->type = EXPR_IMME;
        ret->data = (*tokens)[0].data;
        (*tokens)++;
        break;
    case TOKEN_OPER_LBRACE:
        (*tokens)++;
        ret = getExactExpr(tokens);
        if (ret == NULL || (*tokens)[0].type != TOKEN_OPER_RBRACE) {
            *tokens = backup;
            if (ret != NULL)
                freeExprTree(ret),
                    ret = NULL;
        }
        else {
            (*tokens)++;
        }
        break;
    case TOKEN_SYMBOL:
        ret            = newExprNode();
        ret->type      = EXPR_SYMBOL;
        ret->symbol_id = (*tokens)[0].symbol_id;
        (*tokens)++;
        ret->node1 = getExactFactor(tokens);

        if (ret->node1 == NULL) {
            *tokens = backup;
            freeExprTree(ret);
            ret = NULL;
        }
        break;
    default:
        break;
    }

    return ret;
}

expr_node_t* getExactTerm(token_t** tokens)
{
    expr_node_t* ret = NULL;

    token_t* backup = *tokens;

    ret = getExactFactor(tokens);

    while ((*tokens)[0].type == TOKEN_OPER_MUL || (*tokens)[0].type == TOKEN_OPER_DIV) {
        token_type_t type = (*tokens)[0].type;
        (*tokens)++;
        expr_node_t* node1 = ret;
        expr_node_t* node2 = getExactFactor(tokens);
        if (node2 == NULL) {
            freeExprTree(ret);
            ret     = NULL;
            *tokens = backup;
            break;
        }
        else {
            ret        = newExprNode();
            ret->node1 = node1;
            ret->node2 = node2;
            ret->type  = type == TOKEN_OPER_MUL ? EXPR_MUL : EXPR_DIV;
        }
    }

    return ret;
}

expr_node_t* getExactExpr(token_t** tokens)
{
    expr_node_t* ret = NULL;

    token_t* backup = *tokens;

    ret = getExactTerm(tokens);

    while ((*tokens)[0].type == TOKEN_OPER_ADD || (*tokens)[0].type == TOKEN_OPER_SUB) {
        token_type_t type = (*tokens)[0].type;
        (*tokens)++;
        expr_node_t* node1 = ret;
        expr_node_t* node2 = getExactTerm(tokens);
        if (node2 == NULL) {
            freeExprTree(ret);
            ret     = NULL;
            *tokens = backup;
            break;
        }
        else {
            ret        = newExprNode();
            ret->node1 = node1;
            ret->node2 = node2;
            ret->type  = type == TOKEN_OPER_ADD ? EXPR_ADD : EXPR_SUB;
        }
    }

    return ret;
}

expr_node_t* createExprTree(token_t** tokens)
{
    return getExactExpr(tokens);
}
