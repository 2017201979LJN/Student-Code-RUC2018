#ifndef _MENCI_STACK_H
#define _MENCI_STACK_H

#include <stdbool.h>

typedef struct _stack_node_t {
    void *data;
    struct _stack_node_t *next;
} _stack_node_t;

typedef _stack_node_t *_stack_t;

void _stack_init(_stack_t *stack);
void *_stack_top_(_stack_t *stack);
#define _stack_top(stack, type) ((type)_stack_top_(stack))
void _stack_push_(_stack_t *stack, void *data);
#define _stack_push(stack, data) _stack_push_((stack), (void *)(data))
void _stack_pop(_stack_t *stack);
bool _stack_empty(_stack_t *stack);

#endif // _MENCI_STACK_H
