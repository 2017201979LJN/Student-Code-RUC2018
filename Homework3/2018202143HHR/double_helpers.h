#ifndef _MENCI_DOUBLE_HELPERS_H
#define _MENCI_DOUBLE_HELPERS_H

#include "double.h"

#ifdef DEBUG
void _fp_inspect_print(const char *var_name, const double_t x);
#define fp_inspect(x) _fp_inspect_print(#x, x)
#endif

double_t   _fp_make(base_type_t sign, base_type_t exponent, base_type_t fraction);
double_t   _fp_make_inf(base_type_t sign);
#define    _fp_from_uint64(x) _fp_from_uint128(_uint128_from_uint64(x))
double_t   _fp_from_uint128(_uint128_t x);
int        _fp_compare_abs(const double_t x, const double_t y);
_uint128_t _fp_get_extended_fraction(const double_t x, int target_bit_count, int *shl_amount);
uint64_t   _fp_get_real_fraction(const double_t x);
int        _fp_get_real_exponent(const double_t x);
void       _fp_to_unified_uint128(const double_t x, _uint128_t *ux,
                                  const double_t y, _uint128_t *uy,
                                  int target_larger_bit_count,
                                  int *target_exponent);
void       _fp_parse_and_extend(const double_t x, _uint128_t *ux,
                                int target_bit_count,
                                int *target_exponent);
_uint128_t _fp_unsigned_adaptive_round(const _uint128_t x,
                                       int target_count,
                                       int *shr_amount,
                                       bool *second_carry);
double_t   _fp_from_unified_uint128(_uint128_t real_fraction,
                                    int current_exponent);

#endif // _MENCI_DOUBLE_HELPERS_H
