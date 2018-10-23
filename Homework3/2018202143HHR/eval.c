#include "eval.h"

#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#include "double.h"
#include "double_input.h"
#include "stack.h"

#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wenum-compare"
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"

typedef struct {
    enum {
        OPERATOR,
        NUMBER
    } type;

    union {
        double_t number;
        char operator;
    } data;
} _node_t;

int _eval_operator_priority(char operator) {
    switch (operator) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    default:
        return -1;
    }
}

_node_t *_eval_new_node(_node_t value) {
    _node_t *p = malloc(sizeof(_node_t));
    *p = value;
    return p;
}

double_t eval(const char *expression) {
    const char *p = expression;

    enum {
        OPERATOR,
        LEFT_BRACKET,
        RIGHT_BRACKET,
        NUMBER
    } last_type = LEFT_BRACKET; // Imagine there's a left bracket.

    _stack_t stack_expression,
             stack_symbol;
    _stack_init(&stack_expression);
    _stack_init(&stack_symbol);

#define _make_node(t, union_value) _eval_new_node((_node_t){ .type = t, .data = union_value })
#define _make_node_operator(op) _make_node(OPERATOR, { .operator = op })
#define _make_node_number(num) _make_node(NUMBER, { .number = num })

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        switch (last_type) {
        case OPERATOR:
        case LEFT_BRACKET:
            // Only number or left bracket can follow a operator or left bracket.
            if (*p == '(') {
                _stack_push(&stack_symbol, '(');
                p++;

                last_type = LEFT_BRACKET;
            } else {
                int len;
                double_t number;
                assert(fp_input(p, &number, &len));
                _stack_push(&stack_expression, _make_node_number(number));
                p += len;

                last_type = NUMBER;
            }
            break;
        case NUMBER:
        case RIGHT_BRACKET:
            // Only operator or right bracket can follow a number or right bracket.
            if (*p == ')') {
                // A level is finished processing.
                char ch;
                do {
                    if (_stack_empty(&stack_symbol)) break;
                    ch = _stack_top(&stack_symbol, char);
                    if (ch != '(') {
                        _stack_push(&stack_expression,
                                    _make_node_operator(_stack_top(&stack_symbol, char)));
                    }
                    _stack_pop(&stack_symbol);
                } while (ch != '(');

                p++;

                last_type = RIGHT_BRACKET;
            } else {
                assert(_eval_operator_priority(*p) != -1);
                int curr_priority = _eval_operator_priority(*p);
                // Pop operators until a lower-or-equal priority opeartor.
                for (char ch;
                     !_stack_empty(&stack_symbol)
                     && _eval_operator_priority(ch = _stack_top(&stack_symbol, char))
                        >= curr_priority; ) {
                    assert(_eval_operator_priority(ch) != -1);
                    _stack_push(&stack_expression,
                                _make_node_operator(_stack_top(&stack_symbol, char)));
                    _stack_pop(&stack_symbol);
                };
                _stack_push(&stack_symbol, *p);

                p++;

                last_type = OPERATOR;
            }
            break;
        default:
            assert(false);
            break;
        }
    }

    // Imagine there's a right bracket.
    while (!_stack_empty(&stack_symbol)) {
        char operator = _stack_top(&stack_symbol, char);
        _stack_pop(&stack_symbol);
        _stack_push(&stack_expression, _make_node_operator(operator));
    }

    // Now let's reverse the expression stack to get the Reverse Polish notation.
    _stack_t stack_polish;
    _stack_init(&stack_polish);
    while (!_stack_empty(&stack_expression)) {
        _stack_push(&stack_polish, _stack_top(&stack_expression, _node_t *));
        _stack_pop(&stack_expression);
    }

    // Evaluate the Reverse Polish notation expression.
    _stack_t stack_number;
    _stack_init(&stack_number);
    while (!_stack_empty(&stack_polish)) {
        _node_t *node = _stack_top(&stack_polish, _node_t *);
        _stack_pop(&stack_polish);

        if (node->type == NUMBER) {
            double_t *number = malloc(sizeof(double_t));
            *number = node->data.number;
            _stack_push(&stack_number, number);
        } else {
            double_t *number1, *number2;
            number2 = _stack_top(&stack_number, double_t *);
            _stack_pop(&stack_number);
            number1 = _stack_top(&stack_number, double_t *);
            _stack_pop(&stack_number);

            double_t *result = malloc(sizeof(double_t));
            switch (node->data.operator) {
            case '+': *result = fp_add(*number1, *number2); break;
            case '-': *result = fp_sub(*number1, *number2); break;
            case '*': *result = fp_mul(*number1, *number2); break;
            case '/': *result = fp_div(*number1, *number2); break;
            default: assert(false);
            }

            free(number1);
            free(number2);

            _stack_push(&stack_number, result);
        }

        free(node);
    }

    double_t *result_ptr = _stack_top(&stack_number, double_t *);
    double_t result = *result_ptr;
    free(result_ptr);
    _stack_pop(&stack_number);
    
    assert(_stack_empty(&stack_number));
    assert(_stack_empty(&stack_polish));
    assert(_stack_empty(&stack_expression));
    assert(_stack_empty(&stack_symbol));

    return result;
}
