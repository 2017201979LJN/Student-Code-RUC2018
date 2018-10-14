#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "double_t.h"
#include "uint128_util.h"

#define TEST_DOUBLE_OPERATOR(op, func)                     \
    void test_double_operator_##func(                      \
        double v1,                                         \
        double v2)                                         \
    {                                                      \
        struct double_detail a, b;                         \
        double               c, d;                         \
        REINTERPRET_TO_DOUBLE(a) = v1;                     \
        REINTERPRET_TO_DOUBLE(b) = v2;                     \
                                                           \
        REINTERPRET_TO_DOUBLED(c) = __doubleD##func(a, b); \
                                                           \
        d = v1 op v2;                                      \
                                                           \
        printf("%lf " #func                                \
               " %lf: %le %le %le\n",                      \
               v1,                                         \
               v2,                                         \
               c,                                          \
               d,                                          \
               (c - d) / (c + d));                         \
        return;                                            \
    }

TEST_DOUBLE_OPERATOR(*, Mul)
TEST_DOUBLE_OPERATOR(/, Div)

#if 0
void test_UINT128Shl(uint128_t l, int r)
{
    uint128_t a;
    uint128_t b;

    REINTERPRET_TO_UINT128(a) = UINT128Shl(REINTERPRET_TO_UINT128(l), r);

    b = l << r;

    printf("%d << %d : %d %d \n",
           (int)(l % 19260817),
           r,
           (int)(a % 19260817),
           (int)(b % 19260817));
}

void test_UINT128Shr(uint128_t l, int r)
{
    uint128_t a;
    uint128_t b;

    REINTERPRET_TO_UINT128(a) = UINT128Shr(REINTERPRET_TO_UINT128(l), r);

    b = l >> r;

    printf("%d >> %d : %d %d \n",
           (int)(l % 19260817),
           r,
           (int)(a % 19260817),
           (int)(b % 19260817));
}
*/

double gen_double()
{
    double ans = 0;
    for (int i = 0; i < 64; i++) {
        *(long long*)&ans += ((long long)(rand() & 1)) << i;
    }

    return ans;
}
#endif

#ifdef USE_MAIN
int main(void)
{
    char   op, bak;
    double l, r;

    DOUBLET c;

    memset(&c, 0, sizeof c);

    while (scanf("%lf", &l) == 1) {

        while (isspace(bak = getchar()))
            ;

        op = bak;

        scanf("%lf", &r);

        switch (op) {
        case '+':
            c = doubleTAdd(REINTERPRET_TO_DOUBLET(l), REINTERPRET_TO_DOUBLET(r));
            break;
        case '-':
            c = doubleTSub(REINTERPRET_TO_DOUBLET(l), REINTERPRET_TO_DOUBLET(r));
            break;
        case '*':
            c = doubleTMul(REINTERPRET_TO_DOUBLET(l), REINTERPRET_TO_DOUBLET(r));
            break;
        case '/':
            c = doubleTDiv(REINTERPRET_TO_DOUBLET(l), REINTERPRET_TO_DOUBLET(r));
            break;
        }

        printf("[%.20le,%le]\n", REINTERPRET_TO_DOUBLE(c), REINTERPRET_TO_DOUBLE(c));
    }
}
#endif

double benchmark_calc(double a, double b, char c)
{
    DOUBLET ans;
    switch (c) {
    case '+':
        ans = doubleTAdd(REINTERPRET_TO_DOUBLET(a), REINTERPRET_TO_DOUBLET(b));
        break;
    case '-':
        ans = doubleTSub(REINTERPRET_TO_DOUBLET(a), REINTERPRET_TO_DOUBLET(b));
        break;
    case '*':
        ans = doubleTMul(REINTERPRET_TO_DOUBLET(a), REINTERPRET_TO_DOUBLET(b));
        break;
    case '/':
        ans = doubleTDiv(REINTERPRET_TO_DOUBLET(a), REINTERPRET_TO_DOUBLET(b));
        break;
    default:
        asm("int3");
    }
    return REINTERPRET_TO_DOUBLE(ans);
}