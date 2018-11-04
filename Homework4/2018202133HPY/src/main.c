#include <stdio.h>

#include "expr_compiler.h"
#include "expr_reader.h"

int      top = 0;
factor_t factors[1024];

int main(void)
{
    for (;;) {
        factors[top] = getNextFactor();

        printf("%d#%d ", factors[top].type, (int)factors[top].op);

        if (factors[top].type == FACTOR_EOF)
            break;

        top++;
    }

    puts("");
    factor_t* p = factors;

    while (p->type != FACTOR_EOF) {
        compile_expr(&p);
    }
    return 0;
}