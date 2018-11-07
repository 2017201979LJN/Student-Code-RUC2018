#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

char* regs[] = {
    "",
    "rdi",
    "rsi",
    "rdx",
    "rcx",
    "r8"
};

void eatUntil(char* s)
{
    static char buf[MAX_BUFFER_SIZE];
    while (scanf("%s", buf) == 1 && strcmp(buf, s))
        ;
}

void doFileBegin()
{
    fputs(".global QAQ\n", stdout);
}

void doFuncBegin()
{
    fputs(
        "QAQ:\n\t"
        "pushq %rbp\n\t"
        "movq %rsp, %rbp\n\t",
        stdout);
}

void doFuncEnd()
{
    fputs(
        "movq %rdi, %rax\n\t"
        "xorq %rsi, %rax\n\t"
        "xorq %rdx, %rax\n\t"
        "xorq %rcx, %rax\n\t"
        "xorq %r8, %rax\n\t"
        "popq %rbp\n\t"
        "ret\n\t",
        stdout);
}

int isIMME(char* s)
{
    return isdigit(s[0]);
}

void addIMME_REG(long long x, int src, int dst)
{
    fprintf(stdout,
            "leaq %lld(%%%s),%%%s\n\t",
            x,
            regs[src],
            regs[dst]);
}

void mulIMME_REG(long long lx, int src, int dst)
{
    assert((long long)(int)lx == lx);
    int x = (int)lx;

    fprintf(stdout,
            "imulq $%d,%%%s,%%%s\n\t",
            x,
            regs[src],
            regs[dst]);
}

void loadIMME(long long x, int dst)
{
    fprintf(stdout,
            "movq $%lld,%%%s\n\t",
            x,
            regs[dst]);
}

void loadREG(int id, int dst)
{
    fprintf(stdout,
            "movq %%%s,%%%s\n\t",
            regs[id],
            regs[dst]);
}

void addREG(int id, int dst)
{
    fprintf(stdout,
            "addq %%%s,%%%s\n\t",
            regs[id],
            regs[dst]);
}

void mulREG(int id, int dst)
{
    fprintf(stdout,
            "imulq %%%s,%%%s\n\t",
            regs[id],
            regs[dst]);
}

int main(void)
{
    char* dst  = malloc(MAX_BUFFER_SIZE);
    char* src1 = malloc(MAX_BUFFER_SIZE);
    char* op   = malloc(MAX_BUFFER_SIZE);
    char* src2 = malloc(MAX_BUFFER_SIZE);

    doFileBegin();
    for (;;) {
        if (scanf("%s", dst) != 1)
            break;
        else if (!strcmp(dst, "long")) {
            eatUntil("{");
            doFuncBegin();
        }
        else if (!strcmp(dst, "return")) {
            eatUntil("}");
            doFuncEnd();
        }
        else {
            int dst_id = atoi(dst + 1);
            scanf("%*s%s%s%s", src1, op, src2);
            printf("# %s = %s %s %s\n\t", dst, src1, op, src2);
            if (isIMME(src1) || isIMME(src2)) {
                long long IMME;
                int       reg_id;

                if (isIMME(src1))
                    IMME = atoll(src1), reg_id = atoi(src2 + 1);
                else
                    reg_id = atoi(src1 + 1), IMME = atoll(src2);

                if (op[0] == '+')
                    addIMME_REG(IMME, reg_id, dst_id);
                else
                    mulIMME_REG(IMME, reg_id, dst_id);
            }
            else {
                loadREG(atoi(src1 + 1), dst_id);
                if (op[0] == '+') {
                    addREG(atoi(src2 + 1), dst_id);
                }
                else if (op[0] == '*') {
                    mulREG(atoi(src2 + 1), dst_id);
                }
                else
                    assert(0);
            }
        }
    }

    free(dst);
    free(src1);
    free(op);
    free(src2);

    return 0;
}
