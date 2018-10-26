#ifndef _DOUBLE_T_H

#define _DOUBLE_T_H

#include <stdint.h>

#include "uint128_util.h"

#define VAL_WIDTH 52
#define EXPO_WIDTH 11
#define SIGN_WIDTH 1

#define NUM_OF_BYTES 8

#define REINTERPRET_TO_DOUBLE(x) (*(double*)&(x))
#define REINTERPRET_TO_DOUBLED(x) (*(DOUBLED*)&(x))
#define REINTERPRET_TO_DOUBLET(x) (*(DOUBLET*)&(x))

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

int     doubleDIsNan(DOUBLED l);
int     doubleDIsInf(DOUBLED l);
int     doubleDIsZero(DOUBLED l);
void    doubleDSetInf(DOUBLED* l);
void    doubleDSetNan(DOUBLED* l);
signed  doubleDGetExpo(DOUBLED l);
UINT128 doubleDGetVal(DOUBLED l);

int doubleDSetVal(DOUBLED* pbad,
                  UINT128  v,
                  signed   expo,
                  UINT128* cut_num);

int doubleDCMP(DOUBLED l,
               DOUBLED r);

DOUBLED doubleDAdd(DOUBLED l,
                   DOUBLED r);
DOUBLED doubleDSub(DOUBLED l,
                   DOUBLED r);
DOUBLED doubleDMul(DOUBLED l,
                   DOUBLED r);
DOUBLED doubleDDiv(DOUBLED l,
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
DOUBLET doubleTNeg(DOUBLET l);

#endif
