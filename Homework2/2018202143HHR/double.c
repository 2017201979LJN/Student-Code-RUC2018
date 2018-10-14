#include "double.h"
#include "double_util.h"

#include <assert.h>

// Some special constants.
#define _FP_ENCODING_EXPONENT_ALL_ONE (~(base_type_t)0) >> (64 - _FP_ENCODING_LENGTH_EXPONENT)
const double_t FP_ZERO = { 0, 0, 0 };
const double_t FP_NAN = { 1, _FP_ENCODING_EXPONENT_ALL_ONE, 0 };
const double_t FP_INF_POSITIVE = { 0, _FP_ENCODING_EXPONENT_ALL_ONE, 0 };
const double_t FP_INF_NEGATIVE = { 0, _FP_ENCODING_EXPONENT_ALL_ONE, 1 };

double_t _fp_unsigned_add(const double_t x, const double_t y) {
    // Because there're normal and denormal doubles, convert them
    // to a unified form first.
    // The add operation may cause overflow of uint64, so use uint128
    // when calculating. Although the overflow of only one bit can
    // be handled easier without a larger ingeger type, uint128 will
    // hugely simplify the future multiple operation.
    _uint128_t u128_x, u128_y;
    int target_exponent;
    
    // Shift to let the highest bit of the larger number is on a specified
    // position. The position is better when higher because there could be
    // more lower position for the smaller number to right shift so that the
    // precision is better.
    // Why not 128 ? Because these two add up may carry a bit, use 127 can
    // prevent it from overflow.
    const int TARGET_LARGER_BIT_COUNT = 127;

    _fp_to_unified_uint128(x, &u128_x, y, &u128_y,
                           TARGET_LARGER_BIT_COUNT,
                           &target_exponent);

    if (!_uint128_to_bool(u128_x)) return y;
    if (!_uint128_to_bool(u128_y)) return x;
    
    _uint128_t u128_result = _uint128_add(u128_x, u128_y);

    return _fp_from_unified_uint128(u128_result, target_exponent);
}

// Assuming x > y
double_t _fp_unsigned_sub(const double_t x, const double_t y) {
    assert(_fp_compare_abs(x, y) == 1);

    _uint128_t u128_x, u128_y;
    int target_exponent;
    
    const int TARGET_LARGER_BIT_COUNT = 127;

    _fp_to_unified_uint128(x, &u128_x, y, &u128_y,
                           TARGET_LARGER_BIT_COUNT,
                           &target_exponent);

    if (!_uint128_to_bool(u128_y)) return x;
    
    _uint128_t u128_result = _uint128_sub(u128_x, u128_y);

    return _fp_from_unified_uint128(u128_result, target_exponent);
}

double_t _fp_unsigned_mul(const double_t x, const double_t y) {
    // In order to make the calculation process easier, we shift them to
    // put the highest 1 to the highest bit of the low 64 bits of a int128.
    const int TARGET_BIT_COUNT = 64;
    
    int ex, ey;
    _uint128_t u128_x, u128_y;
    _fp_parse_and_extend(x, &u128_x, TARGET_BIT_COUNT, &ex);
    _fp_parse_and_extend(y, &u128_y, TARGET_BIT_COUNT, &ey);
    
    _uint128_t u128_result = _uint128_mul(u128_x, u128_y);

    // While calculating, we think the number's fraction is the binary fraction
    // itself, but in fact it's not because we ignored the "hidden" binary point!
    // Thinking like that in addition/subtraction won't cause any error but here will!
    // Consider the fraction right-shift a amount is the original fraction with
    // binary point, so equally we can do something with the exponent.
    // We call that right-shift amount "implicit exponent".
    // Now think about normal numbers - A uint can't contain a binary point, so while
    // storing (in IEEE 754, not while parsing before calculating), we left shifted
    // the fraction to move the first 1 to a specified position, but this step
    // involves the "implicit exponent" which equals to the opposite of the
    // left shift amount -- _FP_ENCODING_LENGTH_FRACTION.
    // What denormal numbers are different from normal numbers is only that the digit
    // before binary point is 0 rather than 1 (and positions are the same!).
    // Now it's easy to explain why directly multiple will cause error -- every
    // fraction is multipled with its "implicit exponent" (and all are the same value)
    // and two fractions' sum/difference is obviously multipled with the
    // "implicit exponent", but the production of two fractions will be multipled with
    // double "implicit exponent". So we should subtrate a "implicit exponent" from
    // the exponent to fix it.
    int implicit_exponent = -_FP_ENCODING_LENGTH_FRACTION;

    // Adding a "implicit exponent" to the current exponent could tell the function
    // to subtrate a "implicit exponent" from the exponent by doing shift.
    return _fp_from_unified_uint128(u128_result, ex + ey + implicit_exponent);
}

double_t _fp_unsigned_div(const double_t x, const double_t y) {
    // 128-bits / (_FP_ENCODING_LENGTH_FRACTION + 1)-bits can preserve the highest
    // precision.
    const int TARGET_BIT_COUNT_X = 128,
              TARGET_BIT_COUNT_Y = _FP_ENCODING_LENGTH_FRACTION + 1;
    
    int ex, ey;
    _uint128_t u128_x, u128_y;
    _fp_parse_and_extend(x, &u128_x, TARGET_BIT_COUNT_X, &ex);
    _fp_parse_and_extend(y, &u128_y, TARGET_BIT_COUNT_Y, &ey);

    _uint128_t u128_result = _uint128_div(u128_x, u128_y);

    // Think about this edge case:
    //             ( calculated )  u128_result = xxxx....1000010      
    //             (    true    )  u128_result = xxxx....1000010.00............001
    //                                                       |     (some zeros)  |
    // And round it do make this bit the lowest bit ---------|                   |
    // Obviously it won't do carry since the lower bits' value is 1/2 and a      |
    // carry won't change this bit from 1 to 0 (it's already 0!).                |
    // But we only did 128-bits / (_FP_ENCODING_LENGTH_FRACTION + 1)-bits. If    |
    // we improve the precision (e.g. left shift divisor 256-bits and do         |
    // 256-bits division), this one may appear in the result --------------------|
    // so in fact the lower bits' value is > 1/2. To let the program know the fact
    // when rounding, we can add a one that will never being carried to the result.
    // The simplest solution is left shift the result by 2 and add 1 to it.
    _uint128_t u128_div_remainder = _uint128_sub(u128_x, _uint128_mul(u128_result, u128_y));
    int extra_shl_amount = 0;
    if (_uint128_to_bool(u128_div_remainder)) {
        extra_shl_amount = 2;
        u128_result = _uint128_add(_uint128_shl(u128_result, 2), _UINT128_ONE);
    }

    // Like the production of two fractions will be multipled with double
    // "implicit exponent", the quotient of two fractions will be multipled with
    // no "implicit exponent". So we should add a "implicit exponent" to the
    // exponent to fix it.
    int implicit_exponent = -_FP_ENCODING_LENGTH_FRACTION;

    // Substrating a "implicit exponent" to the current exponent could tell the
    // function to add a "implicit exponent" to the exponent by doing shift.
    // We did a extra left shift, so we should make the exponent smaller to
    // restore the origin size of the number.
    return _fp_from_unified_uint128(u128_result, ex - ey
                                               - implicit_exponent
                                               - extra_shl_amount);
}

double fp_reinterpret_to_double(const double_t x) {
    union {
        double double_val;
        double_t double_t_val;
    } u;
    u.double_t_val = x;
    return u.double_val;
}

double_t fp_reinterpret_from_double(double x) {
    union {
        double double_val;
        double_t double_t_val;
    } u;
    u.double_val = x;
    return u.double_t_val;
}

bool fp_isnan(const double_t x) {
    return x.exponent == _FP_EXPONENT_ALL_ONE && x.fraction;
}

bool fp_isinf(const double_t x) {
    return x.exponent == _FP_EXPONENT_ALL_ONE && !x.fraction;
}

bool fp_isnormal(const double_t x) {
    return x.exponent && !fp_isinf(x) && !fp_isnan(x);
}

bool fp_isdenormal(const double_t x) {
    return !x.exponent && x.fraction && !fp_isinf(x) && !fp_isnan(x);
}

bool fp_iszero(const double_t x) {
    return !x.exponent && !x.fraction;
}

double_t fp_minus(const double_t x) {
    double_t result = x;
    result.sign ^= 1;
    return result;
}

double_t fp_abs(const double_t x) {
    double_t result = x;
    result.sign = 0;
    return result;
}

double_t fp_add(const double_t x, const double_t y) {
    if (fp_isnan(x) || fp_isnan(y)) return FP_NAN;
	// 0 - 0 will be converted to 0 + -0 and results in -0.
	if (fp_iszero(x) && fp_iszero(y)) return FP_ZERO;
    if (fp_iszero(x)) return y;
    if (fp_iszero(y)) return x;
    if (fp_isinf(x) && fp_isinf(y)) {
        // If the sign of x and y are the same, result is
        // the origin inf. Otherwise it's NaN.
        return (x.sign ^ y.sign) ? FP_NAN : x;
    }
    if (fp_isinf(x)) return x;
    if (fp_isinf(y)) return y;

    if (x.sign ^ y.sign) {
        // If the sign of x and y are different, do substrate: l - r.
        double_t l, r;
        int compare_result = _fp_compare_abs(x, y);
        if (compare_result == 0) return FP_ZERO;
        else if (compare_result == 1) l = x, r = y;
        else if (compare_result == -1) l = y, r = x;
        // Without the extra assert(false), Clang will report the following warnings:
        // variable 'l' is used uninitialized whenever 'if' condition is false [-Wsometimes-uninitialized]
        // variable 'r' is used uninitialized whenever 'if' condition is false [-Wsometimes-uninitialized]
        else assert(false);

        double_t result = _fp_unsigned_sub(fp_abs(l), fp_abs(r));

        if (compare_result == 1) result.sign = x.sign;
        else result.sign = x.sign ^ 1;

        return result;
    } else {
        double_t result = _fp_unsigned_add(fp_abs(x), fp_abs(y));
        result.sign = x.sign;
        return result;
    }
}

double_t fp_sub(const double_t x, const double_t y) {
    return fp_add(x, fp_minus(y));
}

double_t fp_mul(const double_t x, const double_t y) {
    if (fp_isnan(x) || fp_isnan(y)) return FP_NAN;
    if (fp_isinf(x) && fp_isinf(y)) return _fp_make_inf(x.sign ^ y.sign);
    // inf * x = { nan (when x = 0), inf (otherwise) }
    if (fp_isinf(x)) return fp_iszero(y) ? FP_NAN : _fp_make_inf(x.sign ^ y.sign);
    if (fp_isinf(y)) return fp_iszero(x) ? FP_NAN : _fp_make_inf(x.sign ^ y.sign);
    // non-inf/nan * 0 = 0
    if (fp_iszero(x) || fp_iszero(y)) return FP_ZERO;

    double_t result = _fp_unsigned_mul(fp_abs(x), fp_abs(y));
    result.sign = x.sign ^ y.sign;

    return result;
}

double_t fp_div(const double_t x, const double_t y) {
    if (fp_isnan(x) || fp_isnan(y)) return FP_NAN;
    if (fp_isinf(x) && fp_isinf(y)) return FP_NAN;
    if (fp_iszero(x) && fp_iszero(y)) return FP_NAN;
    if (fp_iszero(x) || fp_isinf(y)) return FP_ZERO;
    // non-zero / zero = infinity
    if (fp_isinf(x) || fp_iszero(y)) return _fp_make_inf(x.sign);

    double_t result = _fp_unsigned_div(fp_abs(x), fp_abs(y));
    result.sign = x.sign ^ y.sign;

    return result;
}
