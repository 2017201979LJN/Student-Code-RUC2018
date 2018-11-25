#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "double_t_util.h"
#include "expression_tokenizer.h"
#include "symbols.h"

#define DOUBLE_NUM_BEFORE_DOT 1
#define DOUBLE_NUM_AFTER_DOT 3
#define DOUBLE_EXPO_START 4
#define DOUBLE_EXPO_SIGN 6
#define DOUBLE_EXPO_VAL 7

#define NMATCH 10

const char* double_pattern = "^([0-9]*)(\\.?)([0-9]*)(([eE])([+-]?)([0-9]+))?";
const char* oper_pattern   = "^[*/()+-]";
const char* symbol_pattern = "^[A-Za-z0-9][A-Za-z0-9_]*";

regmatch_t pmatch[NMATCH];

regex_t double_reg;
regex_t oper_reg;
regex_t symbol_reg;

int tokenizerInit()
{
    return regcomp(&double_reg, double_pattern, REG_EXTENDED)
           || regcomp(&oper_reg, oper_pattern, REG_EXTENDED)
           || regcomp(&symbol_reg, symbol_pattern, REG_EXTENDED);
}

int isMismatched(regmatch_t pm)
{
    return pm.rm_so >= pm.rm_eo;
}

int checkIfIMME(const char* str)
{
    if (regexec(&double_reg, str, NMATCH, pmatch, 0)) {
        return 0;
    }
    else if (isMismatched(pmatch[DOUBLE_NUM_BEFORE_DOT]) && isMismatched(pmatch[DOUBLE_NUM_AFTER_DOT])) {
        return 0;
    }
    else
        return 1;
}

int checkIfOper(const char* str)
{
    if (regexec(&oper_reg, str, NMATCH, pmatch, 0))
        return 0;
    else
        return 1;
}

int checkIfSymbol(const char* str)
{
    if (regexec(&symbol_reg, str, NMATCH, pmatch, 0))
        return 0;
    else
        return 1;
}

void setIMME(token_t* pt, char* str)
{
    pt->type = TOKEN_IMME;

    char* int_num = pmatch[DOUBLE_NUM_BEFORE_DOT].rm_so < pmatch[DOUBLE_NUM_BEFORE_DOT].rm_eo
                        ? str + pmatch[DOUBLE_NUM_BEFORE_DOT].rm_so
                        : NULL;

    char* frac_num = pmatch[DOUBLE_NUM_AFTER_DOT].rm_so < pmatch[DOUBLE_NUM_AFTER_DOT].rm_eo
                         ? str + pmatch[DOUBLE_NUM_AFTER_DOT].rm_so
                         : NULL;

    signed expo = pmatch[DOUBLE_EXPO_START].rm_so < pmatch[DOUBLE_EXPO_START].rm_eo
                      ? atoi(str + pmatch[DOUBLE_EXPO_START].rm_so + 1)
                      : 0;

    pt->data = float10ToDOUBLET(int_num, frac_num, expo);
}

void setOper(token_t* pt, char* str)
{
    switch (*str) {
    case '+':
        pt->type = TOKEN_OPER_ADD;
        break;
    case '-':
        pt->type = TOKEN_OPER_SUB;
        break;
    case '*':
        pt->type = TOKEN_OPER_MUL;
        break;
    case '/':
        pt->type = TOKEN_OPER_DIV;
        break;
    case '(':
        pt->type = TOKEN_OPER_LBRACE;
        break;
    case ')':
        pt->type = TOKEN_OPER_RBRACE;
        break;
    default:
        pt->type = TOKEN_SYNTAX_ERROR;
        break;
    }
}

void setSymbol(token_t* pt, char* str)
{
    static char name[1024];

    int len  = pmatch[0].rm_eo - pmatch[0].rm_so;
    pt->type = TOKEN_SYMBOL;
    memcpy(name, str, pmatch[0].rm_eo - pmatch[0].rm_so);
    name[len] = 0;

    if (strcmp(name, "sin") == 0) {
        pt->symbol_id = SYMBOL_SIN;
    }
    else if (strcmp(name, "cos") == 0) {
        pt->symbol_id = SYMBOL_COS;
    }
    else if (strcmp(name, "tan") == 0) {
        pt->symbol_id = SYMBOL_TAN;
    }
    else if (strcmp(name, "log") == 0) {
        pt->symbol_id = SYMBOL_LOG;
    }
    else if (strcmp(name, "exp") == 0) {
        pt->symbol_id = SYMBOL_EXP;
    }
    else if (strcmp(name, "read") == 0) {
        pt->symbol_id = SYMBOL_READ;
    }

    //printf("%s#", name);
}

token_t getNextToken(char** const str)
{
    token_t ret;

    while ((**str) && isspace(**str))
        (*str)++;
    if (!(**str)) {
        ret.type = TOKEN_LIST_END;
        return ret;
    }

    if (checkIfIMME(*str)) {
        setIMME(&ret, *str);
        //printf("IMME#%.1le ", ret.data);
    }
    else if (checkIfOper(*str)) {
        setOper(&ret, *str);
        //printf("OPER#%c ", **str);
    }
    else if (checkIfSymbol(*str)) {
        setSymbol(&ret, *str);
        //printf("SYMBOL ");
    }
    else {
        ret.type = TOKEN_SYNTAX_ERROR;
    }
    if (ret.type != TOKEN_SYNTAX_ERROR) {
        *str += pmatch[0].rm_eo - pmatch[0].rm_so;
    }
    return ret;
}
