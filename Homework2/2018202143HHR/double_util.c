#include "double_util.h"
#include "util.h"

#include <assert.h>

#ifdef DEBUG
#include <stdio.h>

void _fp_inspect_print(const char *var_name, const double_t x) {
    // Format:
    // var-name = +/-
    // (value) [reinterpreted-as-double %lf] [reinterpreted-as-double %le]
    // exponent-binary (exponent-decimal => real-exponent-decimal)
    // fraction-binary (fraction-decimal)

    const char *type = "ERROR";
    if (fp_isnan(x)) type = "not a number";
    else if (fp_isinf(x)) type = "infinity";
    else if (fp_isnormal(x)) type = "normal";
    else if (fp_isdenormal(x)) type = "denormal";
    else if (fp_iszero(x)) type = "zero";

    fprintf(stderr, "%s = %c %s\n(%s) [%lf] [%.20le]\n",
                                          var_name,
                                          x.sign ? '-' : '+',
                                          type,
                                          "not implemented",
                                          fp_reinterpret_to_double(x),
                                          fp_reinterpret_to_double(x));
    // Indent to make it easier to read.
    for (int i = _FP_ENCODING_LENGTH_EXPONENT; i < _FP_ENCODING_LENGTH_FRACTION; i++)
        fputc(' ', stderr);

    for (int i = _FP_ENCODING_LENGTH_EXPONENT - 1; i >= 0; i--)
        putc(!!(x.exponent & (1ull << i)) + '0', stderr);
    fprintf(stderr, " (%llu => %lld)\n", (unsigned long long)x.exponent,
                                         (long long)x.exponent - _FP_BIAS + fp_isdenormal(x));

    for (int i = _FP_ENCODING_LENGTH_FRACTION - 1; i >= 0; i--)
        putc(!!(x.fraction & (1ull << i)) + '0', stderr);
    fprintf(stderr, " (%llu)\n\n", (unsigned long long)x.fraction);
}
#endif

double_t _fp_make(base_type_t sign, base_type_t exponent, base_type_t fraction) {
    double_t result;
    result.sign = sign;
    result.exponent = exponent;
    result.fraction = fraction;

    assert(result.sign == sign);
    assert(result.exponent == exponent);
    assert(result.fraction == fraction);
    return result;
}

double_t _fp_make_inf(base_type_t sign) {
    double_t result = FP_INF_POSITIVE;
    result.sign = sign;
    return result;
}

int _fp_compare_abs(const double_t x, const double_t y) {
    assert(!fp_isinf(x));
    assert(!fp_isinf(y));
    assert(!fp_isnan(x));
    assert(!fp_isnan(y));

    double_t abs_x = fp_abs(x), abs_y = fp_abs(y);

    // NOTE: With -O2 in GCC, *(uint64_t *)&abs_x will result in the warning:
    //       dereferencing type-punned pointer will break strict-aliasing rules [-Wstrict-aliasing]

    union {
        uint64_t uint64_t_val;
        double_t double_t_val;
    } union_x, union_y;

    union_x.double_t_val = abs_x;
    union_y.double_t_val = abs_y;

    if (union_x.uint64_t_val == union_y.uint64_t_val) return 0;
    else return union_x.uint64_t_val < union_y.uint64_t_val ? -1 : 1;
}

// Shift the origin fraction to make full use of the target bits.
_uint128_t _fp_get_extended_fraction(const double_t x, int target_bit_count, int *shl_amount) {
    assert(target_bit_count > _FP_ENCODING_LENGTH_FRACTION);

    _uint128_t tmp = _uint128_from_uint64(x.fraction);

    if (fp_isnormal(x)) {
        // Preserve the highest bit to restore the true fraction's first bit.
        *shl_amount = target_bit_count - _FP_ENCODING_LENGTH_FRACTION - 1;
        tmp = _uint128_shl(tmp, *shl_amount);

        tmp = _uint128_or(tmp,
                          _uint128_shl(_UINT128_ONE, target_bit_count - 1));
        return tmp;
    } else {
        *shl_amount = target_bit_count - (128 - _uint128_count_left_zeros(tmp));
        return _uint128_shl(tmp, *shl_amount);
    }
}

int _fp_get_real_exponent(const double_t x) {
    int res = x.exponent;
    return res - _FP_BIAS + fp_isdenormal(x);
}

// For add and sub, we should make the exponent of two number the same value.
void _fp_to_unified_uint128(const double_t x, _uint128_t *ux,
                            const double_t y, _uint128_t *uy,
                            int target_larger_bit_count,
                            int *target_exponent) {
    int shl_amount_x, shl_amount_y;
    *ux = _fp_get_extended_fraction(x, target_larger_bit_count, &shl_amount_x);
    *uy = _fp_get_extended_fraction(y, target_larger_bit_count, &shl_amount_y);
    
    int ex = _fp_get_real_exponent(x), ey = _fp_get_real_exponent(y);

    // When we did left shift, the number became larger, so exponent should be smaller
    // to restore the original size of the number.
    ex -= shl_amount_x;
    ey -= shl_amount_y;

    // Right shift another to align with the larger number.
    *target_exponent = max(ex, ey);

    // The right shift amount may larger than 128.
    *ux = _uint128_shr(*ux, *target_exponent - ex);
    *uy = _uint128_shr(*uy, *target_exponent - ey);
}

void _fp_parse_and_extend(const double_t x, _uint128_t *ux,
                          int target_bit_count,
                          int *target_exponent) {
    int shl_amount;
    *ux = _fp_get_extended_fraction(x, target_bit_count, &shl_amount);

    // When we did left shift, the number became larger, so exponent should be smaller
    // to restore the original size of the number.
    *target_exponent = _fp_get_real_exponent(x) - shl_amount;
}

// Auto round to speciafied bits, also returns shift amount of the progress.
_uint128_t _fp_unsigned_adaptive_round(const _uint128_t x,
                                       int target_count,
                                       int *shr_amount,
                                       bool *second_carry) {
    int left_zero_count = _uint128_count_left_zeros(x);
    int current_count = 128 - left_zero_count;

    *second_carry = false;

    if (target_count == current_count) return x;
    else if (target_count > current_count) {
        // In this case we need to left shift it.
        // Notice now we don't round it.

        *shr_amount = current_count - target_count;
        return _uint128_shl(x, -*shr_amount);
    } else {
        // The lowest current_count - target_count bits will be rounded.
        int low_count = current_count - target_count;

        // x >> (128 - low_count) << (128 - low_count)
        _uint128_t low_bits = _uint128_shr(_uint128_shl(x, 128 - low_count),
                                           128 - low_count);

        bool carry = false;
        // Is low_bits (interpret as floating-point number) >= 1/2?
        // low_bits & (1 << (low_count - 1))
        if (_uint128_to_bool(_uint128_and(low_bits,
                                          _uint128_shl(_UINT128_ONE, low_count - 1)))) {
            // Is low_bits (interpret as floating-point number) > 1/2?
            // low_bits & ~(1 << (low_count - 1))
            if (_uint128_to_bool(_uint128_and(low_bits,
                                              _uint128_not(_uint128_shl(_UINT128_ONE, low_count - 1))))) {
                carry = true;
            }
            // If do carry can make the result a even number, do the carry.
            // x & (1 << low_count)
            else if (_uint128_to_bool(_uint128_and(x, _uint128_shl(_UINT128_ONE, low_count)))) {
                carry = true;
            }
        }

        *shr_amount = low_count;

        if (!carry) return _uint128_shr(x, *shr_amount);
        else {
            _uint128_t tmp = _uint128_shr(x, *shr_amount);
            assert(!_uint128_to_bool(_uint128_shr(tmp, target_count)));

            // The target count may be zero, to make sure some nearly underflowed numbers can be rounded
            // to the smallest non-zero number.
            if (target_count)
                assert(_uint128_to_bool(_uint128_shr(tmp, target_count - 1)));

            tmp = _uint128_add(tmp, _uint128_from_uint64(1));

            // If it's all ones before carry, it'll cause the second carry.
            // But we don't need to do another rounding because in this case
            // the right most bit is always zero.
            if (target_count > 0 && _uint128_to_bool(_uint128_shr(tmp, target_count))) {
                assert(!_uint128_to_bool(_uint128_and(tmp, _UINT128_ONE)));
                tmp = _uint128_shr(tmp, 1);
                (*shr_amount)++;
                *second_carry = true;
            }

            return tmp;
        }
    }
}

// current_exponent: real exponent after calculating
//                   equals to the exponent before calculating in add/sub
double_t _fp_from_unified_uint128(_uint128_t real_fraction,
                                  int current_exponent) {
    assert(_uint128_to_bool(real_fraction));

    // Firstly, calculate the exponent if directly shift to (fraction length + 1) bits.
    // When converting the origin number, we shifted them to make sure the highest 1 is
    // on the highest position of the lowest some bits of a int128.
    int current_bit_count = 128 - _uint128_count_left_zeros(real_fraction);

    // When we did left shift, the bit count became more, the exponent became less.
    // Now we want to shift back, so we should add the exponent.
    // Because doing carry can enlarge the number, the real exponent may be larger
    // than estimated value.
    int estimated_exponent = current_exponent
                           + (current_bit_count - (_FP_ENCODING_LENGTH_FRACTION + 1));

    int biased_estimated_exponent = estimated_exponent + _FP_BIAS;
    
    // The smallest biased exponent for normal numbers is 1. 
    if (biased_estimated_exponent < 1) {
        // It may be denormal.
        // Why 'may be'? Beacuse rounding may make the number larger.

        // Still, when we did left shift, the bit count became more, the exponent became less.
        // So if we want to enlarge the exponent, we should make bit count less
        int target_bit_count = current_bit_count - ((1 - _FP_BIAS) - current_exponent);

        // Oops, underflowed!
        if (target_bit_count < 0) return FP_ZERO;

        int shr_amount;
        bool second_carry;
        _uint128_t rounded = _fp_unsigned_adaptive_round(real_fraction,
                                                         target_bit_count,
                                                         &shr_amount,
                                                         &second_carry);

        int new_exponent = current_exponent + shr_amount;
        assert(new_exponent + _FP_BIAS >= 1 && new_exponent + _FP_BIAS <= 2);

        // If fraction is like xxxx111...111, rounding will do a seoncd carry and make the
        // content bits one more.
        if (second_carry) {
            // If it's _FP_ENCODING_LENGTH_FRACTION bits after second carry, it'll be a
            // normal number.
            // rounded & (1 << (_FP_ENCODING_LENGTH_FRACTION - 1))
            if (_uint128_to_bool(_uint128_and(rounded,
                                              _uint128_shl(_UINT128_ONE,
                                                           _FP_ENCODING_LENGTH_FRACTION - 1)))) {
                assert(!_uint128_to_bool(_uint128_xor(rounded, _uint128_shl(_UINT128_ONE, target_bit_count - 1))));
                return _fp_make(0, 1, 0);
            }
            // Otherwise treat the second carry as a normal carry and don't shift back
            // to make the exponent = 1 - _FP_BIAS
            else {
                shr_amount--;
                rounded = _uint128_shl(rounded, 1);
            }
        }

        return _fp_make(0, 0, _uint128_get_low64(rounded));
    } else {
        // It's normal.
        int shr_amount;
        bool second_carry;
        _uint128_t rounded = _fp_unsigned_adaptive_round(real_fraction,
                                                         _FP_ENCODING_LENGTH_FRACTION + 1,
                                                         &shr_amount,
                                                         &second_carry);

        int new_exponent = current_exponent + shr_amount;
        int biased_new_exponent = new_exponent + _FP_BIAS;
        if (biased_new_exponent > _FP_EXPONENT_MAX) {
            // Infinity.
            return FP_INF_POSITIVE;
        }

        uint64_t new_fraction = _uint128_get_low64(rounded);
        assert(new_fraction & (1ull << _FP_ENCODING_LENGTH_FRACTION));
        uint64_t final_fraction = new_fraction ^ (1ull << _FP_ENCODING_LENGTH_FRACTION);

        return _fp_make(0, biased_new_exponent, final_fraction);
    }
}
