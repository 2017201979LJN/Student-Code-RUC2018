#ifndef _MENCI_DOUBLE_H
#define _MENCI_DOUBLE_H

#include <stdbool.h>
#include <stdint.h>

#include "int128.h"
#include "util.h"
#include "defines.h"

typedef uint64_t base_type_t;

typedef struct {
    // On GNU/Linux x86_64 it's little endian, so we declare them
    // in reverse order.

    base_type_t fraction    : _FP_ENCODING_LENGTH_FRACTION;
    base_type_t exponent    : _FP_ENCODING_LENGTH_EXPONENT;
    base_type_t sign        : _FP_ENCODING_LENGTH_SIGN;
} double_t;

// Some special constants
extern const double_t FP_ZERO;
extern const double_t FP_ONE;
extern const double_t FP_NAN;
extern const double_t FP_INF_POSITIVE;
extern const double_t FP_INF_NEGATIVE;

#ifdef DEBUG
void     _fp_inspect_print(const char *var_name, const double_t x);
#define fp_inspect(x) _fp_inspect_print(#x, x)
#endif

double_t _fp_unsigned_add(const double_t x, const double_t y);
double_t _fp_unsigned_sub(const double_t x, const double_t y);
double_t _fp_unsigned_mul(const double_t x, const double_t y);
double_t _fp_unsigned_div(const double_t x, const double_t y);

double   fp_reinterpret_to_double(const double_t x);
double_t fp_reinterpret_from_double(const double x);
double_t fp_reinterpret_from_binary(uint64_t x);

bool     fp_isnan(const double_t x);
bool     fp_isinf(const double_t x);
bool     fp_isnormal(const double_t x);
bool     fp_isdenormal(const double_t x);
bool     fp_iszero(const double_t x);

double_t fp_abs(const double_t x);
double_t fp_add(const double_t x, const double_t y);
double_t fp_sub(const double_t x, const double_t y);
double_t fp_mul(const double_t x, const double_t y);
double_t fp_div(const double_t x, const double_t y);

#endif // _MENCI_DOUBLE_H
