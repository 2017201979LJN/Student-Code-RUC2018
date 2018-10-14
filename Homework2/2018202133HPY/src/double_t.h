#ifndef _DOUBLE_T_H

#define _DOUBLE_T_H

#include <stdint.h>

#include "uint128_util.h"

#define VAL_WIDTH 52
#define EXPO_WIDTH 11
#define SIGN_WIDTH 1

#define EXPO_BIAS ((1ll << (EXPO_WIDTH - 1)) - 1)
#define EXPO_SPEC ((1ll << EXPO_WIDTH) - 1)
//#define EXPO_MAX ((1ll << EXPO_WIDTH) - 2 - EXPO_BIAS)
#define EXPO_MAX ((1ll << EXPO_WIDTH) - 2)

#define CALC_UNIT_WIDTH 128
#define CALC_UNIT_WIDTH_HALF (CALC_UNIT_WIDTH >> 1)
#define CALC_TOP_BIT_HALF (1ull << (CALC_UNIT_WIDTH_HALF - 1))
#define CALC_SHIFT_WIDTH (CALC_UNIT_WIDTH_HALF - VAL_WIDTH - 1)
#define CALC_SHIFT_MASK(x) ((1 << (x)) - 1)
#define CALC_SHIFT_HALF(x) (1 << ((x)-1))

#define VAL_INF 0

#define REINTERPRET_TO_DOUBLE(x) (*(double*)&(x))
#define REINTERPRET_TO_DOUBLED(x) (*(DOUBLED*)&(x))
#define REINTERPRET_TO_DOUBLET(x) (*(DOUBLET*)&(x))

#define NUM_OF_BYTES 8

typedef struct double_t {
    unsigned char bytes[NUM_OF_BYTES];
} DOUBLET;

typedef struct double_detail {
    uint64_t val : VAL_WIDTH;
    uint64_t expo : EXPO_WIDTH;
    uint64_t sign : SIGN_WIDTH;
} DOUBLED;

/*
 *UINT128 declear
 */

int     __doubleDIsNan(DOUBLED l);
int     __doubleDIsInf(DOUBLED l);
int     __doubleDIsZero(DOUBLED l);
void    __doubleDSetInf(DOUBLED* l);
void    __doubleDSetNan(DOUBLED* l);
signed  __doubleDGetExpo(DOUBLED l);
UINT128 __doubleDGetVal(DOUBLED l);

int __doubleDSetVal(DOUBLED* pbad,
                    UINT128  v,
                    signed   expo,
                    UINT128* cut_num);

int __doubleDCMP(DOUBLED l,
                 DOUBLED r);

DOUBLED __doubleDAdd(DOUBLED l,
                     DOUBLED r);
DOUBLED __doubleDSub(DOUBLED l,
                     DOUBLED r);
DOUBLED __doubleDMul(DOUBLED l,
                     DOUBLED r);
DOUBLED __doubleDDiv(DOUBLED l,
                     DOUBLED r);

int     doubleTIsNan(DOUBLET l);
int     doubleTIsInf(DOUBLET l);
int     doubleTIsZero(DOUBLET l);
void    doubleTSetInf(DOUBLET* l);
void    doubleTSetNan(DOUBLET* l);
signed  doubleTGetExpo(DOUBLET l);
UINT128 doubleTGetVal(DOUBLET l);

int doubleTSetVal(DOUBLET* pbad,
                  UINT128  v,
                  signed   expo,
                  UINT128* cut_num);

int doubleTCMP(DOUBLET l,
               DOUBLET r);

DOUBLET doubleTAdd(DOUBLET l,
                   DOUBLET r);
DOUBLET doubleTSub(DOUBLET l,
                   DOUBLET r);
DOUBLET doubleTMul(DOUBLET l,
                   DOUBLET r);
DOUBLET doubleTDiv(DOUBLET l,
                   DOUBLET r);

#endif
