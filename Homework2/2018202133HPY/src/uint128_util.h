#ifndef _UINT128_UTIL_H

#define _UINT128_UTIL_H

#include <stdint.h>

#ifndef __INT64_TYPE__
#error ThIs programs needs 64-bit-long integer type to be compiled
#endif

//typedef __uint128_t uint128_t;

typedef struct _uint128_t {
    unsigned long long low;
    unsigned long long high;
} uint128_T;

typedef uint128_T        UINT128;
typedef uint128_T*       LPUINT128;
typedef const uint128_T* LPCUINT128;

#define REINTERPRET_TO_UINT128(x) (*(UINT128*)(void*)&(x))
//#define REINTERPRET_TO_BUILTIN_UINT128(x) (*(uint128_t*)(void*)&(x))

extern const UINT128 UINT128_ZERO;
extern const UINT128 UINT128_ONE;

_Bool uint128IsZero(UINT128 u128);
_Bool uint128IsnZero(UINT128 u128);

UINT128 UINT128Add(UINT128 u128l,
                   UINT128 u128r);

UINT128 UINT128AddNoCarry(UINT128 u128l,
                          UINT128 u128r);

UINT128 UINT128Sub(UINT128 u128l,
                   UINT128 u128r);

UINT128 UINT128SubNoCarry(UINT128 u128l,
                          UINT128 u128r);

int UINT128GetCarry();

UINT128 UINT64Mul(uint64_t u64l,
                  uint64_t u64r);

uint64_t __UINT64DivNoOverflow(UINT128   u128l,
                               uint64_t  u64r,
                               uint64_t* pu64rem);

UINT128 UINT64Div(UINT128   u128l,
                  uint64_t  u64r,
                  uint64_t* pu64rem);

UINT128 UINT128Shr(UINT128 u128l, int r);

UINT128 UINT128Shl(UINT128 u128l, int r);

int UINT128Cmp(UINT128 u128l, UINT128 u128r);

//return number of zeros in the front of x
int UINT128Clz(UINT128 u128x);

#endif