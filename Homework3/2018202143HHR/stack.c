#include "stack.h"

#include <stdlib.h>
#include <assert.h>

void _stack_init(_stack_t *stack) {
    *stack = NULL;
}

void *_stack_top_(_stack_t *stack) {
    assert(*stack);
    return (*stack)->data;
}

void _stack_pop(_stack_t *stack) {
    assert(*stack);

    _stack_node_t *next = (*stack)->next;
    free(*stack);
    *stack = next;
}

void _stack_push_(_stack_t *stack, void *data) {
    _stack_node_t *new = malloc(sizeof(_stack_node_t));
    new->next = *stack;
    new->data = data;
    *stack = new;
}

bool _stack_empty(_stack_t *stack) {
    return !*stack;
}
