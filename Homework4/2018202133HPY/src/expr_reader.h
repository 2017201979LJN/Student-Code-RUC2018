#ifndef _EXPR_READER_H

#define _EXPR_READER_H

#define SYMBOL_NUM 5
#define FACTOR_MAX_LENGEH 1024

typedef enum {
    FACTOR_EOF,
    FACTOR_SYMBOL,
    FACTOR_IMME,
    FACTOR_OPERATOR,
    FACTOR_INT
} factor_type_t;

typedef struct {
    factor_type_t type;
    union {
        int  var;
        char op;
        int  sym_id;
    };
} factor_t;

extern char*    symbol_names[SYMBOL_NUM];
extern factor_t symbols[SYMBOL_NUM];

factor_t getNextFactor();

#endif