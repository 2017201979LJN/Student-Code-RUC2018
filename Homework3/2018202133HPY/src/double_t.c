#include <stdio.h>

#include "double_t.h"
#include "uint128_util.h"

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

/*
 *  double_t operations begin
 */

int mind(int l, int r)
{
    return l < r ? l : r;
}

int doubleDIsNan(DOUBLED l)
{
    return l.expo == EXPO_SPEC && l.val != VAL_INF;
}

int doubleDIsInf(DOUBLED l)
{
    return l.expo == EXPO_SPEC && l.val == VAL_INF;
}

int doubleDIsZero(DOUBLED l)
{
    return l.expo == 0 && l.val == 0;
}

void doubleDSetInf(DOUBLED* l)
{
    l->expo = EXPO_SPEC;
    l->val  = 0;
}

void doubleDSetNan(DOUBLED* l)
{
    l->expo = EXPO_SPEC;
    l->val  = 1;
}

signed doubleDGetExpo(DOUBLED l)
{
    if (l.expo) {
        return (signed)l.expo - EXPO_BIAS;
    }
    else {
        return (signed)l.expo - (EXPO_BIAS - 1);
    }
}

UINT128 doubleDGetVal(DOUBLED l)
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
int doubleDSetVal(DOUBLED* pbad,
                  UINT128  u128v,
                  signed   expo,
                  UINT128* pu128cutNum)
{
    UINT128 u128_to_cut;

    expo += EXPO_BIAS;

    int num = UINT128Clz(u128v);

    if (num == 128)
        expo = 0;

    int shlnum = 0, shrnum = 0;

    if (num > (CALC_UNIT_WIDTH - VAL_WIDTH - 1)) {
        int tmp = mind(expo - 1, num - (CALC_UNIT_WIDTH - VAL_WIDTH - 1));
        num -= tmp;
        expo -= tmp;
        shlnum += num;
    }

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
        doubleDSetInf(pbad);
        if (pu128cutNum) {
            pu128cutNum->low  = 0;
            pu128cutNum->high = 0;
        }
        return 0;
    }

    if (shrnum) {
        u128_to_cut = UINT128Shr(UINT128Shl(u128v,
                                            CALC_UNIT_WIDTH - shrnum),
                                 CALC_UNIT_WIDTH - shrnum);
        if (pu128cutNum) {
            *pu128cutNum = u128_to_cut;
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

        switch (UINT128Cmp(u128_to_cut, u128toCmp)) {
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

int doubleDCMP(DOUBLED l,
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

DOUBLED doubleDAdd(DOUBLED l,
                   DOUBLED r)
{
    if (doubleDIsInf(l) || doubleDIsInf(r)) {
        if (doubleDIsInf(l) && doubleDIsInf(r)) {
            if (l.sign ^ r.sign) {}
        }
        else if (doubleDIsInf(r))
            l = r;
        return l;
    }

    if (doubleDIsNan(l) || doubleDIsNan(r)) {
        doubleDSetNan(&l);
        return l;
    }

    if (l.sign ^ r.sign) {
        r.sign ^= 1;
        return doubleDSub(l, r);
    }

    DOUBLED ret;

    ret.sign = l.sign;

    l.sign = r.sign = 0;

    if (doubleDCMP(l, r) == -1) {
        DOUBLED tmp;

        tmp = l;
        l   = r;
        r   = tmp;
    }

    UINT128 lv = doubleDGetVal(l),
            rv = doubleDGetVal(r);

    int shlnum = UINT128Clz(lv) - 1;

    lv = UINT128Shl(lv, shlnum);
    rv = UINT128Shl(rv, shlnum);

    rv = UINT128Shr(rv, doubleDGetExpo(l) - doubleDGetExpo(r));
    lv = UINT128AddNoCarry(lv, rv);

    doubleDSetVal(&ret, lv, doubleDGetExpo(l) - shlnum - CALC_SHIFT_WIDTH, NULL);

    return ret;
}

DOUBLED doubleDSub(DOUBLED l,
                   DOUBLED r)
{
    if (doubleDIsInf(l) || doubleDIsInf(r)) {
        if (doubleDIsInf(l) && doubleDIsInf(r)) {
            if (!(l.sign ^ r.sign)) {
                doubleDSetNan(&l);
            }
        }
        else if (doubleDIsInf(r)) {
            l = r;
            l.sign ^= 1;
        }
        return l;
    }

    if (doubleDIsNan(l) || doubleDIsNan(r)) {
        doubleDSetNan(&l);
        return l;
    }

    if (l.sign ^ r.sign) {
        r.sign ^= 1;
        return doubleDAdd(l, r);
    }

    DOUBLED ret;

    ret.sign = l.sign;

    l.sign = r.sign = 0;

    if (doubleDCMP(l, r) == -1) {
        DOUBLED tmp;

        tmp = l;
        l   = r;
        r   = tmp;

        ret.sign ^= 1;
    }

    UINT128 lv = doubleDGetVal(l),
            rv = doubleDGetVal(r);

    int shlnum = UINT128Clz(lv) - 1;

    lv = UINT128Shl(lv, shlnum);
    rv = UINT128Shl(rv, shlnum);

    rv = UINT128Shr(rv, doubleDGetExpo(l) - doubleDGetExpo(r));
    lv = UINT128Sub(lv, rv);

    if (UINT128GetCarry()) {
        lv.low  = ~lv.low;
        lv.high = ~lv.high;
        UINT128Add(lv, UINT128_ONE);
        ret.sign ^= 1;
    }

    doubleDSetVal(&ret, lv, doubleDGetExpo(l) - shlnum - CALC_SHIFT_WIDTH, NULL);

    return ret;
}

DOUBLED doubleDMul(DOUBLED l,
                   DOUBLED r)
{
    DOUBLED ret;

    ret.sign = l.sign ^ r.sign;

    //TODO: L'Hopital

    if (doubleDIsNan(l) || doubleDIsNan(r)) {
        doubleDSetNan(&ret);
        return ret;
    }

    if (doubleDIsInf(l) || doubleDIsInf(r)) {
        if (doubleDIsZero(l) || doubleDIsZero(r)) {
            doubleDSetNan(&ret);
            return ret;
        }
        else {
            doubleDSetInf(&ret);
            return ret;
        }
    }

    signed expo = doubleDGetExpo(l) + doubleDGetExpo(r);

    UINT128 lv = UINT64Mul(doubleDGetVal(l).low,
                           doubleDGetVal(r).low);

    //DONE shl;

    doubleDSetVal(&ret, lv, expo - CALC_SHIFT_WIDTH - (CALC_UNIT_WIDTH_HALF - 1), NULL);

    return ret;
}

DOUBLED doubleDDiv(DOUBLED l,
                   DOUBLED r)
{
    DOUBLED ret;

    ret.sign = l.sign ^ r.sign;

    //TODO: L'Hopital

    if (doubleDIsNan(l) || doubleDIsNan(r)) {
        doubleDSetNan(&ret);
        return ret;
    }

    if (doubleDIsZero(r)) {
        if (doubleDIsZero(l)) {
            doubleDSetNan(&ret);
            return ret;
        }
        else {
            doubleDSetInf(&ret);
            return ret;
        }
    }

    if (doubleDIsInf(l)) {
        doubleDSetInf(&ret);
        return ret;
    }

    if (doubleDIsInf(r)) {
        ret.expo = ret.val = 0;
        return ret;
    }

    signed expo = doubleDGetExpo(l) - doubleDGetExpo(r);

    //DONE : 非规约 精度损失

    //DONE : 无穷精度损失

    UINT128  lv = doubleDGetVal(l);
    uint64_t rv = doubleDGetVal(r).low;
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

    doubleDSetVal(&ret, lv, expo - CALC_SHIFT_WIDTH - 2, NULL);

    return ret;
}

int doubleTIsNan(DOUBLET l)
{
    return doubleDIsNan(REINTERPRET_TO_DOUBLED(l));
}

int doubleTIsInf(DOUBLET l)
{
    return doubleDIsInf(REINTERPRET_TO_DOUBLED(l));
}
int doubleTIsZero(DOUBLET l)
{
    return doubleDIsZero(REINTERPRET_TO_DOUBLED(l));
}
void doubleTSetInf(DOUBLET* l)
{
    return doubleDSetInf(&REINTERPRET_TO_DOUBLED(*l));
}
void doubleTSetNan(DOUBLET* l)
{
    return doubleDSetNan(&REINTERPRET_TO_DOUBLED(*l));
}
signed doubleTGetExpo(DOUBLET l)
{
    return doubleDGetExpo(REINTERPRET_TO_DOUBLED(l));
}
UINT128 doubleTGetVal(DOUBLET l)
{
    return doubleDGetVal(REINTERPRET_TO_DOUBLED(l));
}

int doubleTSetVal(DOUBLET* pbad,
                  UINT128  v,
                  signed   expo,
                  UINT128* cut_num)
{
    return doubleDSetVal(&REINTERPRET_TO_DOUBLED(*pbad),
                         v,
                         expo,
                         cut_num);
}

int doubleTCMP(DOUBLET l,
               DOUBLET r)
{
    return doubleDCMP(REINTERPRET_TO_DOUBLED(l), REINTERPRET_TO_DOUBLED(r));
}

DOUBLET doubleTAdd(DOUBLET l,
                   DOUBLET r)
{
    DOUBLED ans = doubleDAdd(REINTERPRET_TO_DOUBLED(l), REINTERPRET_TO_DOUBLED(r));
    return REINTERPRET_TO_DOUBLET(ans);
}
DOUBLET doubleTSub(DOUBLET l,
                   DOUBLET r)
{
    DOUBLED ans = doubleDSub(REINTERPRET_TO_DOUBLED(l), REINTERPRET_TO_DOUBLED(r));
    return REINTERPRET_TO_DOUBLET(ans);
}
DOUBLET doubleTMul(DOUBLET l,
                   DOUBLET r)
{
    DOUBLED ans = doubleDMul(REINTERPRET_TO_DOUBLED(l), REINTERPRET_TO_DOUBLED(r));
    return REINTERPRET_TO_DOUBLET(ans);
}
DOUBLET doubleTDiv(DOUBLET l,
                   DOUBLET r)
{
    DOUBLED ans = doubleDDiv(REINTERPRET_TO_DOUBLED(l), REINTERPRET_TO_DOUBLED(r));
    return REINTERPRET_TO_DOUBLET(ans);
}
DOUBLET doubleTNeg(DOUBLET l)
{
    DOUBLED ans = REINTERPRET_TO_DOUBLED(l);
    ans.sign ^= 1;
    return REINTERPRET_TO_DOUBLET(ans);
}