#include "expr_compiler.h"
#include <assert.h>
#include <stdio.h>

int tot = 0;

void compile_expr(factor_t** facts)
{
    factor_t* t = *facts;
    printf("%d#%d\n", ++tot, (*facts)->type);
    if ((*facts)->type == FACTOR_INT) {
        while ((*facts)->type != FACTOR_OPERATOR || (*facts)->op != ';')
            (*facts)++;
        (*facts)++;
    }
    else {
        assert((*facts)->type == FACTOR_SYMBOL);
        int to_val = (*facts)->sym_id;

        //skip =
        (*facts) += 2;

        factor_t* lfact = (*facts);

        (*facts)++;

        if ((*facts)->op == ';') {
            (*facts) += 2;

            if (lfact->type == FACTOR_SYMBOL) {
                if (lfact->sym_id != to_val)
                    printf("movq %%%%r%d,%%%%r%d\n", lfact->sym_id + 8, to_val + 8);
            }
            else {
                printf("movq $%d,%%%%r%d\n", lfact->var, to_val + 8);
            }
        }
        else {
            char oper = (*facts)->op;

            (*facts)++;

            factor_t* rfact = (*facts);
            (*facts) += 2;

            const char* oper_inst = oper == '+' ? "addq" : "imulq";

            if (lfact->type == FACTOR_SYMBOL) {
                printf("movq %%%%r%d,%%%%rax\n", lfact->sym_id + 8);
            }
            else if (lfact->type == FACTOR_IMME) {
                printf("movq $%d,%%%%rax\n", lfact->var);
            }
            else {
                asm("int3");
            }

            //TODO: do some adjust

            if (rfact->type == FACTOR_SYMBOL) {
                printf("%s %%%%r%d,%%%%rax\n", oper_inst, rfact->sym_id + 8);
            }
            else if (rfact->type == FACTOR_IMME) {
                printf("%s $%d,%%%%rax\n", oper_inst, rfact->var);
            }
            else {
                asm("int3");
            }

            printf("movq %%%%rax,%%%%r%d\n", to_val + 8);
        }
    }
    printf("# %ld\n", *facts - t);
}