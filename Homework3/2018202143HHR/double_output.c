#include "double_output.h"

#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "defines.h"
#include "bignum.h"
#include "double_helpers.h"

// We need to use 5 ^ [1, -_FP_OUTPUT_EXPONENT_MIN],
//                2 ^ [0, _FP_OUTPUT_EXPONENT_MAX].
_bignum_t _BIGNUM_POW5[-_FP_OUTPUT_EXPONENT_MIN + 1],
          _BIGNUM_POW2[_FP_OUTPUT_EXPONENT_MAX + 1];

bool _fp_output_inited = false;

void _fp_output_init() {
    _fp_output_inited = true;

    _bignum_init(&_BIGNUM_POW5[0], 1);
    _bignum_init(&_BIGNUM_POW2[0], 1);

    for (int i = 1; i <= _FP_OUTPUT_EXPONENT_MAX; i++) {
        _BIGNUM_POW2[i] = _BIGNUM_POW2[i - 1];
        _bignum_mul_uint64(&_BIGNUM_POW2[i], 2);
    }

    for (int i = 1; i <= -_FP_OUTPUT_EXPONENT_MIN; i++) {
        _BIGNUM_POW5[i] = _BIGNUM_POW5[i - 1];
        _bignum_mul_uint64(&_BIGNUM_POW5[i], 5);
    }
}

// Assuming buffer size is enough.
void fp_output(double_t x, int precision, char *buffer) {
    if (!_fp_output_inited) _fp_output_init();

    assert(precision > 0);

    if (x.sign) *buffer++ = '-';

    if (fp_isnan(x)) return (void)strcpy(buffer, "nan");
    if (fp_isinf(x)) return (void)strcpy(buffer, "inf");
    
    // Restore the 1 before point in normal numbers.
    uint64_t fraction = _fp_get_real_fraction(x);
    uint64_t exponent = _fp_get_real_exponent(x);

    // We must convert integer part and decimal part in different ways.
    // So the first step is to determine where the point is.
    // If there isn't exponent, the lowest integer bit is the 
    // _FP_ENCODING_LENGTH_FRACTION-th bit. If we add x to the exponent,
    // in order to prevent the number from changing, we must left-shift the point.
    // Therefore, the position of the lowest integer bit is lower.
    int lowest_integer_bit_pos = _FP_ENCODING_LENGTH_FRACTION - exponent;

    _bignum_t bignum_integer_part;
    _bignum_init(&bignum_integer_part, 0);
    for (int i = 0; lowest_integer_bit_pos + i <= _FP_ENCODING_LENGTH_FRACTION; i++) {
        // The number may be too large.
        if (lowest_integer_bit_pos + i < 0) continue;

        if ((fraction >> (lowest_integer_bit_pos + i)) & 1) {
            _bignum_add_bignum(&bignum_integer_part, &_BIGNUM_POW2[i]);
        }
    }

    char integer_part[_BIGNUM_OUTPUT_BUFFER_SIZE];
    _bignum_output_decimal(&bignum_integer_part, integer_part);

    int highest_decimal_bit_pos = lowest_integer_bit_pos - 1;

    // The original decimal part is like \sum\limits_{i=0}^n 2^{-i}a_i.
    // But in order to prevent decimal calculation, consider that each 2 ^ -i at most
    // one digit more that 2 ^ -(i - 1), we can multiply it with 10. So calculate
    // 5 ^ i instead. Also, to align the digits, we should put a leading 1 on the left
    // the result, and multiple the result with 10 in each cycle.
    _bignum_t bignum_decimal_part;
    _bignum_init(&bignum_decimal_part, 1);
    for (int i = 0; highest_decimal_bit_pos - i >= 0; i++) {
        _bignum_mul_uint64(&bignum_decimal_part, 10);

        // The number may be too small.
        if (highest_decimal_bit_pos - i > _FP_ENCODING_LENGTH_FRACTION) continue;

        if ((fraction >> (highest_decimal_bit_pos - i)) & 1) {
            _bignum_add_bignum(&bignum_decimal_part, &_BIGNUM_POW5[i + 1]);
        }
    }

    char decimal_part[_BIGNUM_OUTPUT_BUFFER_SIZE];
    _bignum_output_decimal(&bignum_decimal_part, decimal_part);
    assert(decimal_part[0] == '1');

    // Now we should round it to the specfied precision.
    int integer_part_length = strlen(integer_part);
    char merged[_BIGNUM_OUTPUT_BUFFER_SIZE * 2];
    strcpy(merged, integer_part);
    strcpy(merged + integer_part_length, decimal_part + 1); // Ignore the leading 1.

    int merged_length = strlen(merged);
    bool carry_overflow = false;
    if (merged_length - integer_part_length > precision) {
        // Check the digit after the position of precision.
        if (merged[integer_part_length + precision] >= '5') {
            // Do the rounding.
            carry_overflow = true;
            for (int i = integer_part_length + precision - 1; i >= 0; i--) {
                if (merged[i] == '9') {
                    merged[i] = '0';
                } else {
                    merged[i]++;
                    carry_overflow = false;
                    break;
                }
            }
        }
    }

    if (carry_overflow) *buffer++ = '1';
    memcpy(buffer, merged, integer_part_length);
    buffer += integer_part_length;
    *buffer++ = '.';
    for (int i = 0, j = integer_part_length; i < precision; i++) {
        if (merged[j]) *buffer++ = merged[j++];
        else *buffer++ = '0';
    }

    *buffer = '\0';

    // if (scientific_notation) {
    //     // Store the number before rounding.
    //     char buffer_before_rounding[precision + 3];

    //     char *p = merged;
    //     bool zero = false;
    //     int exponent = integer_part_length - 1;

    //     // In total we should write precision + 1 digits.
    //     for (int i = 0; i < precision + 1; ) {
    //         // Write a point after the first digit.
    //         if (i == 1) *buffer++ = '.';

    //         char curr = *p ? *p : '0';
    //         if (*p) p++;
    //         else if (!i) {
    //             // We traveled all digits but a non-zero digit is not found.
    //             // So the number is 0.
    //             // Here's a small trick to process 0 as a common number.
    //             *buffer++ = '0';
    //             zero = true;
    //             i++;
    //             continue;
    //         }

    //         // Find the first non-zero digit and then write it to buffer.
    //         // Skip zeros.
    //         if (curr == '0' && !i) {
    //             exponent--;
    //             continue;
    //         }

    //         *buffer++ = curr;
    //         i++;
    //     }

    //     sprintf(buffer, "e%d", exponent);
    // }
}
