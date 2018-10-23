#include <stdio.h>

#include "double_t.h"
#include "uint128_util.h"

/*
 *  double_t operations begin
 */

int mind(int l, int r)
{
    return l < r ? l : r;
}

int __doubleDIsNan(DOUBLED l)
{
    return l.expo == EXPO_SPEC && l.val != VAL_INF;
}

int __doubleDIsInf(DOUBLED l)
{
    return l.expo == EXPO_SPEC && l.val == VAL_INF;
}

int __doubleDIsZero(DOUBLED l)
{
    return l.expo == 0 && l.val == 0;
}

void __doubleDSetInf(DOUBLED* l)
{
    l->expo = EXPO_SPEC;
    l->val  = 0;
}

void __doubleDSetNan(DOUBLED* l)
{
    l->expo = EXPO_SPEC;
    l->val  = 1;
}

signed __doubleDGetExpo(DOUBLED l)
{
    if (l.expo) {
        return (signed)l.expo - EXPO_BIAS;
    }
    else {
        return (signed)l.expo - (EXPO_BIAS - 1);
    }
}

UINT128 __doubleDGetVal(DOUBLED l)
{
    UINT128 t;

    t.high = 0;
    t.low  = l.val;
    t      = UINT128Shl(t, CALC_SHIFT_WIDTH);

    //+1* if normalized
    if (l.expo) {
        t.low = t.low | CALC_TOP_BIT_HALF;
    }

    return t;
}

//returns cut width
int __doubleDSetVal(DOUBLED* pbad,
                    UINT128  u128v,
                    signed   expo,
                    UINT128* pu128cutNum)
{
    UINT128 u128toCut;

    expo += EXPO_BIAS;

    int num = UINT128Clz(u128v);

    int shlnum = 0, shrnum = 0;

    if (num > (CALC_UNIT_WIDTH - VAL_WIDTH - 1)) {
        int tmp = mind(expo - 1, num - (CALC_UNIT_WIDTH - VAL_WIDTH - 1));
        num -= tmp;
        expo -= tmp;
        shlnum += num;
    }

    /*
    while (num > (CALC_UNIT_WIDTH - VAL_WIDTH - 1) && expo > 1) {
        num--;
        expo--;
        shlnum++;
    }
    */

    u128v = UINT128Shl(u128v, shlnum);

    if (num < (CALC_UNIT_WIDTH - VAL_WIDTH - 1)) {
        shrnum = CALC_UNIT_WIDTH - VAL_WIDTH - 1 - num;
        expo   = expo + shrnum;
        num    = CALC_UNIT_WIDTH - VAL_WIDTH - 1;
    }

    if (expo < 1) {
        shrnum += 1 - expo;
        expo = 1;
        num += 1 - expo;
    }

    if (expo > EXPO_MAX) {
        __doubleDSetInf(pbad);
        if (pu128cutNum) {
            pu128cutNum->low  = 0;
            pu128cutNum->high = 0;
        }
        return 0;
    }

    if (shrnum) {
        u128toCut = UINT128Shr(UINT128Shl(u128v,
                                          CALC_UNIT_WIDTH - shrnum),
                               CALC_UNIT_WIDTH - shrnum);
        if (pu128cutNum) {
            *pu128cutNum = u128toCut;
        }
        u128v = UINT128Shr(u128v, shrnum);
    }

    if (!(u128v.low >> VAL_WIDTH)) {
        expo--;
    }

    pbad->val  = u128v.low;
    pbad->expo = expo;

    //rounding
    //DONE
    if (shrnum > 0 && shrnum <= 128) {
        UINT128 u128toCmp;

        u128toCmp.low  = 1;
        u128toCmp.high = 0;

        u128toCmp = UINT128Shl(u128toCmp, shrnum - 1);

        switch (UINT128Cmp(u128toCut, u128toCmp)) {
        case 0:
            if (!(pbad->val & 1))
                break;
        case 1:
            pbad->val++;
            if (!pbad->val)
                pbad->expo++;
            if (pbad->expo == EXPO_SPEC)
                pbad->val = 0;
            break;
        default:
            break;
        }
    }
    return shrnum;
}

int __doubleDCMP(DOUBLED l,
                 DOUBLED r)
{
    if (l.sign) {
        l.expo = ~l.expo;
        l.val  = ~l.val;
    }
    l.sign ^= 1;

    if (r.sign) {
        r.expo = ~r.expo;
        r.val  = ~r.val;
    }
    r.sign ^= 1;

    if (*(uint64_t*)&l < *(uint64_t*)&r) {
        return -1;
    }
    else if (*(uint64_t*)&l == *(uint64_t*)&r) {
        return 0;
    }
    else
        return 1;
}

DOUBLED __doubleDAdd(DOUBLED l,
                     DOUBLED r)
{
    if (__doubleDIsInf(l) || __doubleDIsInf(r)) {
        if (__doubleDIsInf(r) && (l.sign ^ r.sign)) {
            __doubleDSetNan(&l);
        }
        return l;
    }

    if (__doubleDIsNan(l) || __doubleDIsNan(r)) {
        __doubleDSetNan(&l);
        return l;
    }

    if (l.sign ^ r.sign) {
        r.sign ^= 1;
        return __doubleDSub(l, r);
    }

    DOUBLED ret;

    ret.sign = l.sign;

    l.sign = r.sign = 0;

    if (__doubleDCMP(l, r) == -1) {
        DOUBLED tmp;

        tmp = l;
        l   = r;
        r   = tmp;
    }

    UINT128 lv = __doubleDGetVal(l),
            rv = __doubleDGetVal(r);

    int shlnum = UINT128Clz(lv) - 1;

    lv = UINT128Shl(lv, shlnum);
    rv = UINT128Shl(rv, shlnum);

    rv = UINT128Shr(rv, __doubleDGetExpo(l) - __doubleDGetExpo(r));
    lv = UINT128AddNoCarry(lv, rv);

    __doubleDSetVal(&ret, lv, __doubleDGetExpo(l) - shlnum - CALC_SHIFT_WIDTH, NULL);

    return ret;
}

DOUBLED __doubleDSub(DOUBLED l,
                     DOUBLED r)
{
    if (__doubleDIsInf(l) || __doubleDIsInf(r)) {
        if (__doubleDIsInf(r) && !(l.sign ^ r.sign)) {
            __doubleDSetNan(&l);
        }
        return l;
    }

    if (__doubleDIsNan(l) || __doubleDIsNan(r)) {
        __doubleDSetNan(&l);
        return l;
    }

    if (l.sign ^ r.sign) {
        r.sign ^= 1;
        return __doubleDAdd(l, r);
    }

    DOUBLED ret;

    ret.sign = l.sign;

    l.sign = r.sign = 0;

    if (__doubleDCMP(l, r) == -1) {
        DOUBLED tmp;

        tmp = l;
        l   = r;
        r   = tmp;

        ret.sign ^= 1;
    }

    UINT128 lv = __doubleDGetVal(l),
            rv = __doubleDGetVal(r);

    int shlnum = UINT128Clz(lv) - 1;

    lv = UINT128Shl(lv, shlnum);
    rv = UINT128Shl(rv, shlnum);

    rv = UINT128Shr(rv, __doubleDGetExpo(l) - __doubleDGetExpo(r));
    lv = UINT128Sub(lv, rv);

    if (UINT128GetCarry()) {
        lv.low  = ~lv.low;
        lv.high = ~lv.high;
        UINT128Add(lv, UINT128_ONE);
        ret.sign ^= 1;
    }

    __doubleDSetVal(&ret, lv, __doubleDGetExpo(l) - shlnum - CALC_SHIFT_WIDTH, NULL);

    return ret;
}

DOUBLED __doubleDMul(DOUBLED l,
                     DOUBLED r)
{
    DOUBLED ret;

    ret.sign = l.sign ^ r.sign;

    //TODO: L'Hopital

    if (__doubleDIsNan(l) || __doubleDIsNan(r)) {
        __doubleDSetNan(&ret);
        return ret;
    }

    if (__doubleDIsInf(l) || __doubleDIsInf(r)) {
        if (__doubleDIsZero(l) || __doubleDIsZero(r)) {
            __doubleDSetNan(&ret);
            return ret;
        }
        else {
            __doubleDSetInf(&ret);
            return ret;
        }
    }

    signed expo = __doubleDGetExpo(l) + __doubleDGetExpo(r);

    UINT128 lv = UINT64Mul(__doubleDGetVal(l).low,
                           __doubleDGetVal(r).low);

    //DONE shl;

    __doubleDSetVal(&ret, lv, expo - CALC_SHIFT_WIDTH - (CALC_UNIT_WIDTH_HALF - 1), NULL);

    return ret;
}

DOUBLED __doubleDDiv(DOUBLED l,
                     DOUBLED r)
{
    DOUBLED ret;

    ret.sign = l.sign ^ r.sign;

    //TODO: L'Hopital

    if (__doubleDIsNan(l) || __doubleDIsNan(r)) {
        __doubleDSetNan(&ret);
        return ret;
    }

    if (__doubleDIsZero(r)) {
        if (__doubleDIsZero(l)) {
            __doubleDSetNan(&ret);
            return ret;
        }
        else {
            __doubleDSetInf(&ret);
            return ret;
        }
    }

    if (__doubleDIsInf(l)) {
        __doubleDSetInf(&ret);
        return ret;
    }

    if (__doubleDIsInf(r)) {
        ret.expo = ret.val = 0;
        return ret;
    }

    signed expo = __doubleDGetExpo(l) - __doubleDGetExpo(r);

    //DONE : 非规约 精度损失

    //DONE : 无穷精度损失

    UINT128  lv = __doubleDGetVal(l);
    uint64_t rv = __doubleDGetVal(r).low;
    uint64_t rem;

    while (lv.low && !(lv.low & CALC_TOP_BIT_HALF)) {
        lv.low = lv.low << 1;
        expo--;
    }

    while (rv && !(rv & CALC_TOP_BIT_HALF)) {
        rv = rv << 1;
        expo++;
    }

    lv = UINT128Shl(lv, (CALC_UNIT_WIDTH_HALF - 1));

    lv = UINT64Div(lv, rv, &rem);

    lv = UINT128Shl(lv, 2);
    if (rem) {
        lv = UINT128Add(lv, UINT128_ONE);
    }

    __doubleDSetVal(&ret, lv, expo - CALC_SHIFT_WIDTH - 2, NULL);

    return ret;
}

int doubleTIsNan(DOUBLET l)
{
    return __doubleDIsNan(REINTERPRET_TO_DOUBLED(l));
}

int doubleTIsInf(DOUBLET l)
{
    return __doubleDIsInf(REINTERPRET_TO_DOUBLED(l));
}
int doubleTIsZero(DOUBLET l)
{
    return __doubleDIsZero(REINTERPRET_TO_DOUBLED(l));
}
void doubleTSetInf(DOUBLET* l)
{
    return __doubleDSetInf(&REINTERPRET_TO_DOUBLED(*l));
}
void doubleTSetNan(DOUBLET* l)
{
    return __doubleDSetNan(&REINTERPRET_TO_DOUBLED(*l));
}
signed doubleTGetExpo(DOUBLET l)
{
    return __doubleDGetExpo(REINTERPRET_TO_DOUBLED(l));
}
UINT128 doubleTGetVal(DOUBLET l)
{
    return __doubleDGetVal(REINTERPRET_TO_DOUBLED(l));
}

int doubleTSetVal(DOUBLET* pbad,
                  UINT128  v,
                  signed   expo,
                  UINT128* cut_num)
{
    return __doubleDSetVal(&REINTERPRET_TO_DOUBLED(*pbad),
                           v,
                           expo,
                           cut_num);
}

int doubleTCMP(DOUBLET l,
               DOUBLET r)
{
    return __doubleDCMP(REINTERPRET_TO_DOUBLED(l), REINTERPRET_TO_DOUBLED(r));
}

DOUBLET doubleTAdd(DOUBLET l,
                   DOUBLET r)
{
    DOUBLED ans = __doubleDAdd(REINTERPRET_TO_DOUBLED(l), REINTERPRET_TO_DOUBLED(r));
    return REINTERPRET_TO_DOUBLET(ans);
}
DOUBLET doubleTSub(DOUBLET l,
                   DOUBLET r)
{
    DOUBLED ans = __doubleDSub(REINTERPRET_TO_DOUBLED(l), REINTERPRET_TO_DOUBLED(r));
    return REINTERPRET_TO_DOUBLET(ans);
}
DOUBLET doubleTMul(DOUBLET l,
                   DOUBLET r)
{
    DOUBLED ans = __doubleDMul(REINTERPRET_TO_DOUBLED(l), REINTERPRET_TO_DOUBLED(r));
    return REINTERPRET_TO_DOUBLET(ans);
}
DOUBLET doubleTDiv(DOUBLET l,
                   DOUBLET r)
{
    DOUBLED ans = __doubleDDiv(REINTERPRET_TO_DOUBLED(l), REINTERPRET_TO_DOUBLED(r));
    return REINTERPRET_TO_DOUBLET(ans);
}
