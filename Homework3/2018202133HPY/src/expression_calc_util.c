#include "expression_calc_util.h"
#include "math.h"
#include "symbols.h"

DOUBLET expr_oper_add(DOUBLET l, DOUBLET r)
{
    return doubleTAdd(l, r);
}

DOUBLET expr_oper_sub(DOUBLET l, DOUBLET r)
{
    return doubleTSub(l, r);
}

DOUBLET expr_oper_mul(DOUBLET l, DOUBLET r)
{
    return doubleTMul(l, r);
}

DOUBLET expr_oper_div(DOUBLET l, DOUBLET r)
{
    return doubleTDiv(l, r);
}

DOUBLET expr_oper_neg(DOUBLET l)
{
    return doubleTNeg(l);
}

DOUBLET expr_symbol(DOUBLET l, unsigned long long id)
{
    double x;
    double t = REINTERPRET_TO_DOUBLE(l);
    switch (id) {
    case SYMBOL_SIN:
        x = sin(t);
        break;
    case SYMBOL_COS:
        x = cos(t);
        break;
    case SYMBOL_TAN:
        x = tan(t);
        break;
    case SYMBOL_LOG:
        x = log(t);
        break;
    case SYMBOL_EXP:
        x = exp(t);
        break;
    case SYMBOL_READ:
        scanf("%lf", &x);
        break;
    default:
        x = 0.0 / 0.0;
    }
    return REINTERPRET_TO_DOUBLET(x);
}

DOUBLET expr_tree_calc(expr_node_t* node)
{
    DOUBLET ret;
    switch (node->type) {
    case EXPR_IMME:
        ret = node->data;
        break;
    case EXPR_ADD:
        ret = expr_oper_add(expr_tree_calc(node->node1), expr_tree_calc(node->node2));
        break;
    case EXPR_SUB:
        ret = expr_oper_sub(expr_tree_calc(node->node1), expr_tree_calc(node->node2));
        break;
    case EXPR_MUL:
        ret = expr_oper_mul(expr_tree_calc(node->node1), expr_tree_calc(node->node2));
        break;
    case EXPR_DIV:
        ret = expr_oper_div(expr_tree_calc(node->node1), expr_tree_calc(node->node2));
        break;
    case EXPR_NEG:
        ret = expr_oper_neg(expr_tree_calc(node->node1));
        break;
    case EXPR_SYMBOL:
        ret = expr_symbol(expr_tree_calc(node->node1), node->symbol_id);
        break;
    default:
        doubleTSetNan(&ret);
        break;
    }

    return ret;
}