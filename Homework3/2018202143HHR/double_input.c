#include "double_input.h"

#include <ctype.h>
#include <assert.h>
#include <string.h>

#include "double.h"
#include "double_helpers.h"
#include "pow10_table.h"

const char *_FP_STRING_INF = "inf";
const char *_FP_STRING_INF_POSITIVE = "+inf";
const char *_FP_STRING_INF_NEGATIVE = "-inf";
const char *_FP_STRING_NAN = "nan";
const char *_FP_STRING_NAN_POSITIVE = "+nan";
const char *_FP_STRING_NAN_NEGATIVE = "-nan";

bool fp_input(const char *s, double_t *result, int *len) {
#define _map_result(str, res)                    \
        if (!strncasecmp(s, str, strlen(str))) { \
            *result = res;                       \
            *len = strlen(str);                  \
            return true;                         \
        }

    _map_result(_FP_STRING_INF, FP_INF_POSITIVE);
    _map_result(_FP_STRING_INF_POSITIVE, FP_INF_POSITIVE);
    _map_result(_FP_STRING_INF_NEGATIVE, FP_INF_NEGATIVE);
    _map_result(_FP_STRING_NAN, FP_NAN);
    _map_result(_FP_STRING_NAN_POSITIVE, FP_NAN);
    _map_result(_FP_STRING_NAN_NEGATIVE, FP_NAN); // -nan is meaningless.

#undef _map_result

    // [+/-][integer].integer[e[+/-]integer]
    // [+/-]integer[.integer][e[+/-]integer]

    enum {
        SIGN,
        DIGITS_BEFORE_POINT,
        POINT,
        DIGITS_AFTER_POINT,
        EXPONENT_SYMBOL,
        EXPONENT_SIGN,
        EXPONENT,
        END
    } state;

    // Determine the initial state.
    if (*s == '+' || *s == '-') state = SIGN;
    else if (isdigit(*s)) state = DIGITS_BEFORE_POINT;
    else if (*s == '.') state = POINT;
    else return false;

    int sign = 0,           // 0 -> '+', 1 -> '-'.
        exponent_sign = 1,  // 1 -> '+', -1 -> '-'.
        extra_exponent = 0; // The exponent converted from the decimal number.
    uint32_t exponent = 0;  // The input exponent.

    // The result is number * (10 ^ (exponent + extra_exponent)).
    _uint128_t number = _uint128_from_uint64(0);

    // The current position of s, will be used to calculate the length after the for loop.
    const char *p = s;

    for (; state != END; p++) {
        char curr = *p,
             next = *(p + 1);
        
        assert(curr);
            
        switch (state) {
        case SIGN:
            assert(curr == '+' || curr == '-');

            sign = (curr == '-');
            
            if (isdigit(next)) state = DIGITS_BEFORE_POINT;
            else if (next == '.') state = POINT;
            else return false;

            break;
        case DIGITS_BEFORE_POINT:
            assert(isdigit(curr));

            // Prevent number from overflow.
            // If number >= _UINT128_MAX / 10
            if (_uint128_compare(number,
                                 _uint128_div(_UINT128_MAX,
                                              _uint128_from_uint64(10))) != -1) {
                extra_exponent++;
            } else {
                number = _uint128_add(_uint128_mul(number, _uint128_from_uint64(10)),
                                      _uint128_from_uint64(curr - '0'));
            }
            
            if (isdigit(next));
            else if (next == '.') state = POINT;
            else if (next == 'e' || next == 'E') state = EXPONENT_SYMBOL;
            else state = END;

            break;
        case POINT:
            assert(curr == '.');

            if (isdigit(next)) state = DIGITS_AFTER_POINT;
            else if (next == 'e' || next == 'E') state = EXPONENT_SYMBOL;
            else state = END;

            break;
        case DIGITS_AFTER_POINT:
            assert(isdigit(curr));
            
            // Prevent number from overflow.
            // If number >= _UINT128_MAX / 10
            if (_uint128_compare(number,
                                 _uint128_div(_UINT128_MAX,
                                              _uint128_from_uint64(10))) == -1) {
                extra_exponent--;
                number = _uint128_add(_uint128_mul(number, _uint128_from_uint64(10)),
                                      _uint128_from_uint64(curr - '0'));
            }

            if (isdigit(next));
            else if (next == 'e' || next == 'E') state = EXPONENT_SYMBOL;
            else state = END;

            break;
        case EXPONENT_SYMBOL:
            assert(curr == 'e' || curr == 'E');

            if (next == '+' || next == '-') state = EXPONENT_SIGN;
            else if (isdigit(next)) state = EXPONENT;
            else return false;

            break;
        case EXPONENT_SIGN:
            assert(curr == '+' || curr == '-');

            exponent_sign = (curr == '-') ? -1 : 1;

            if (isdigit(next)) state = EXPONENT;
            else return false;

            break;
        case EXPONENT:
            assert(isdigit(curr));

            if (exponent * 10 + (curr - '0') > exponent) {
                // Here, it's already infinity.
                exponent = exponent * 10 + (curr - '0');
            }

            if (isdigit(next));
            else state = END;

            break;
        default:
            assert(false);
            break;
        }
    }

    *len = p - s;

    int64_t final_exponent = (int64_t)exponent * exponent_sign + extra_exponent,
            overflowed_exponent = 0;
    
    if (final_exponent > _POW10_TABLE_MAX) {
        overflowed_exponent = final_exponent - _POW10_TABLE_MAX;
        final_exponent = _POW10_TABLE_MAX;
    } else if (final_exponent < _POW10_TABLE_MIN) {
        overflowed_exponent = _POW10_TABLE_MIN - final_exponent;
        final_exponent = _POW10_TABLE_MIN;
    }

    // Calculate 10 ^ overflowed_exponent.
    // log2(overflowed_exponent) operations will be performed.
    uint64_t abs_overflowed_exponent = overflowed_exponent > 0
                                     ? overflowed_exponent 
                                     : -overflowed_exponent;
    double_t tmp = _fp_from_uint64(10), exponent_factor = FP_ONE;
    for (uint64_t n = abs_overflowed_exponent; n; n >>= 1) {
        if (n & 1) exponent_factor = fp_mul(exponent_factor, tmp);
        tmp = fp_mul(tmp, tmp);
    }

    *result = _fp_from_uint128(number);
    if (overflowed_exponent)
        *result = (overflowed_exponent > 0 ? fp_mul : fp_div)(*result, exponent_factor);
    
    double_t pow10_final_exponent = *(double_t *)
                                    &_POW10_TABLE[final_exponent - _POW10_TABLE_MIN];
    *result = fp_mul(*result, pow10_final_exponent);
    
    result->sign = sign;
    return true;
}
