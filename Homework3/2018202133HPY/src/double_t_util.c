#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "double_t_util.h"
#include "uint128_util.h"

//FIXME: BUGGY CONST
#define DOUBLE_T_UINT128_MASK 0xF000000000000000ULL
#define DOUBLE_T_UINT128_RESERVE_BIT_WIDTH 4
#define DOUBLE_T_UINT128_HIGEST 0x0800000000000000ULL

#define DOUBLE_T_BUF_LENGTH 4096

#define UINT128MUL10(x) (UINT128Add(UINT128Shl((x), 3), UINT128Shl((x), 1)))

char    double_t_output_buf[DOUBLE_T_BUF_LENGTH];
DOUBLET constants[11];

void doubleUtilInit()
{
#define def(x) constants[x] = float10ToDOUBLET(#x, NULL, 0);
    def(0);
    def(1);
    def(2);
    def(3);
    def(4);
    def(5);
    def(6);
    def(7);
    def(8);
    def(9);
    def(10);
#undef def
}

DOUBLET float10ToDOUBLET(const char* int_part, const char* frac_part, signed expo)
{
    DOUBLET ans;
    memset(&ans, 0, sizeof ans);

    signed    expo_2  = 0;
    uint128_T u128tmp = UINT128_ZERO;

    for (char const* p = int_part; p && isdigit(*p); p++) {
        u128tmp = UINT128Add(UINT128MUL10(u128tmp), UINT128Make(0, *p - '0'));
        while (u128tmp.high & DOUBLE_T_UINT128_MASK) {
            u128tmp = UINT128Shr(u128tmp, 1);
            expo_2 += 1;
        }
    }

    for (char const* p = frac_part; p && isdigit(*p); p++) {
        expo--;
        u128tmp = UINT128Add(UINT128MUL10(u128tmp), UINT128Make(0, *p - '0'));
        while (u128tmp.high & DOUBLE_T_UINT128_MASK) {
            u128tmp = UINT128Shr(u128tmp, 1);
            expo_2 += 1;
        }
    }

    //FIXED: Too slow !
    if (uint128IsZero(u128tmp))
        expo = 0;
    else {
        while (!(u128tmp.high & DOUBLE_T_UINT128_HIGEST)) {
            u128tmp = UINT128Shl(u128tmp, 1);
            expo_2 -= 1;
        }
    }

    while (expo < 0) {
        u128tmp = UINT128Shl(u128tmp, DOUBLE_T_UINT128_RESERVE_BIT_WIDTH);
        u128tmp = UINT64Div(u128tmp, 10, NULL);
        expo_2 -= DOUBLE_T_UINT128_RESERVE_BIT_WIDTH;
        expo++;
        while (u128tmp.high & DOUBLE_T_UINT128_MASK) {
            u128tmp = UINT128Shr(u128tmp, 1);
            expo_2 += 1;
        }
    }
    while (expo > 0) {
        u128tmp = UINT128MUL10(u128tmp);
        u128tmp = UINT128Shr(u128tmp, DOUBLE_T_UINT128_RESERVE_BIT_WIDTH - 1);
        expo_2 += DOUBLE_T_UINT128_RESERVE_BIT_WIDTH - 1;
        expo--;
        while (u128tmp.high & DOUBLE_T_UINT128_MASK) {
            u128tmp = UINT128Shr(u128tmp, 1);
            expo_2 += 1;
        }
    }

    doubleTSetVal(&ans, u128tmp, expo_2 + VAL_WIDTH, NULL);

    return ans;
}

char* DOUBLETToString(DOUBLET bad)
{
    DOUBLED detail = REINTERPRET_TO_DOUBLED(bad);

    if (doubleTIsNan(bad)) {
        strcpy(double_t_output_buf, detail.sign ? "-nan" : "nan");
    }
    else if (doubleTIsInf(bad)) {
        strcpy(double_t_output_buf, detail.sign ? "-inf" : "inf");
    }
    else if (doubleTIsZero(bad)) {
        strcpy(double_t_output_buf, detail.sign ? "-0" : "0");
    }
    else {
        int expo10 = 0;
        int top    = 0;
        if (detail.sign) {
            detail.sign                = 0;
            double_t_output_buf[top++] = '-';
        }
        bad = REINTERPRET_TO_DOUBLET(detail);
        while (doubleTCMP(bad, constants[1]) == -1)
            bad = doubleTMul(bad, constants[10]), expo10--;
        while (doubleTCMP(bad, constants[10]) != -1)
            bad = doubleTDiv(bad, constants[10]), expo10++;
        for (int i = 0; i <= 10; i++) {
            if (doubleTCMP(bad, constants[i]) == -1) {
                bad = doubleTSub(bad, constants[i - 1]);

                double_t_output_buf[top++] = '0' + i - 1;
                break;
            }
        }

        if (!doubleTIsZero(bad)) {
            double_t_output_buf[top++] = '.';
            while (!doubleTIsZero(bad)) {
                bad = doubleTMul(bad, constants[10]);
                for (int i = 0; i <= 10; i++) {
                    if (doubleTCMP(bad, constants[i]) == -1) {
                        bad = doubleTSub(bad, constants[i - 1]);

                        double_t_output_buf[top++] = '0' + i - 1;
                        break;
                    }
                }
            }
        }

        double_t_output_buf[top] = 0;

        if (expo10) {
            double_t_output_buf[top++] = 'e';
            sprintf(double_t_output_buf + top, "%d", expo10);
        }
    }

    return double_t_output_buf;
}
