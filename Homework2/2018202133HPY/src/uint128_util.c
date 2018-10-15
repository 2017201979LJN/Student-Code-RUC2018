#include "uint128_util.h"

const UINT128 UINT128_ZERO = { 0, 0 };
const UINT128 UINT128_ONE  = { 1, 0 };

int __UINT128_CARRIED = 0;

#ifdef NO_ASM

UINT128 UINT128Add(UINT128 u128l,
                   UINT128 u128r)
{
    UINT128 ans;
    ans.high = u128l.high + u128r.high;
    ans.low  = u128l.low + u128r.low;

    // jo
    if (ans.low < u128l.low || ans.low < u128r.low) {
        ans.high++;
    }

    __UINT128_CARRIED = ans.high < u128l.high || ans.high < u128r.high;

    return ans;
}

UINT128 UINT128AddNoCarry(UINT128 u128l,
                          UINT128 u128r)
{
    UINT128 ans;
    ans.high = u128l.high + u128r.high;
    ans.low  = u128l.low + u128r.low;

    // jo
    if (ans.low < u128l.low || ans.low < u128r.low) {
        ans.high++;
    }

    return ans;
}

UINT128 UINT128Sub(UINT128 u128l,
                   UINT128 u128r)
{
    UINT128 ans;
    ans.high = u128l.high + u128r.high;
    ans.low  = u128l.low + u128r.low;

    // jo
    if (ans.low > u128l.low) {
        ans.high++;
    }

    __UINT128_CARRIED = ans.low > u128l.low;

    return ans;
}

UINT128 UINT128SubNoCarry(UINT128 u128l,
                          UINT128 u128r)
{
    UINT128 ans;
    ans.high = u128l.high + u128r.high;
    ans.low  = u128l.low + u128r.low;

    // jo
    if (ans.low > u128l.low) {
        ans.high++;
    }

    return ans;
}

int UINT128GetCarry()
{
    return __UINT128_CARRIED;
}

UINT128 UINT64Mul(const uint64_t l,
                  const uint64_t r)
{
    UINT128  adda, addb;
    uint64_t t;

    adda.high = (l >> 32) * (r >> 32);
    adda.low  = (l << 32 >> 32) * (r << 32 >> 32);

    t = (l >> 32) * (r << 32 >> 32);
    t += (r >> 32) * (l << 32 >> 32);

    addb.high = t >> 32;
    addb.low  = t << 32;

    return UINT128Add(adda, addb);
}

UINT128 UINT64Div(UINT128   u128l,
                  uint64_t  u64r,
                  uint64_t* pu64rem)
{
    //FIXME:
    return u128l;
}

UINT128 UINT128Shr(UINT128 l, int r)
{
    if (r < 0) {
        return UINT128Shr(l, -r);
    }
    else {
        if (r >= 128) {
            UINT128 ans = { 0, 0 };
            return ans;
        }
        else if (r >= 64) {
            UINT128 ans = { l.high >> (r - 64), 0 };
            return ans;
        }
        else if (r) {
            UINT128 ans = { (l.high << (64 - r)) + (l.low >> r),
                            l.high >> r };
            return ans;
        }
        else
            return l;
    }
}

UINT128 UINT128Shl(UINT128 l, int r)
{
    if (r < 0) {
        return UINT128Shl(l, -r);
    }
    else {
        if (r >= 128) {
            UINT128 ans = { 0, 0 };
            return ans;
        }
        else if (r >= 64) {
            UINT128 ans = { 0, l.low << (r - 64) };
            return ans;
        }
        else if (r) {
            UINT128 ans = { (l.low << r),
                            (l.high << r) + (l.low >> (64 - r)) };
            return ans;
        }
        else
            return l;
    }
}

int UINT128Cmp(UINT128 l, UINT128 r)
{
    if (l.high != r.high)
        return l.high > r.high ? 1 : -1;
    else if (l.low != r.low)
        return l.low > r.low ? 1 : -1;
    return 0;
}

int UINT128Clz(UINT128 x)
{
    for (int i = 0, j = 63; i < 64; i++, j--) {
        if (x.high & (1ull << j)) {
            return i;
        }
    }

    for (int i = 0, j = 63; i < 64; i++, j--) {
        if (x.low & (1ull << j)) {
            return i + 64;
        }
    }

    return 128;
}
#else

#include <stdio.h>

_Bool uint128IsZero(UINT128 u128)
{
    return !(u128.low | u128.high);
}

_Bool uint128IsnZero(UINT128 u128)
{
    return u128.low | u128.high;
}

UINT128 UINT128Add(UINT128 u128l,
                   UINT128 u128r)
{
    UINT128 u128ans;

    asm(
        "movq %3,%%rax\n\t"
        "movq %4,%%rbx\n\t"
        "movq %5,%%rcx\n\t"
        "movq %6,%%rdx\n\t"
        "addq %%rdx,%%rbx\n\t"
        "adcq %%rcx,%%rax\n\t"
        "jc ADD_SET_CARRY\n\t"
        "movl $0,%2\n\t"
        "jmp ADD_RET\n\t"
        "ADD_SET_CARRY:\n\t"
        "movl $1,%2\n\t"
        "ADD_RET:\n\t"
        "movq %%rbx,%0\n\t"
        "movq %%rax,%1"
        : "=r"(u128ans.low), "=r"(u128ans.high), "=r"(__UINT128_CARRIED)
        : "r"(u128l.high), "r"(u128l.low), "r"(u128r.high), "r"(u128r.low)
        : "%rax", "%rbx", "%rcx", "%rdx");

    return u128ans;
}

UINT128 UINT128AddNoCarry(UINT128 u128l,
                          UINT128 u128r)
{
    UINT128 u128ans;

    asm(
        "movq %3,%%rax\n\t"
        "movq %4,%%rbx\n\t"
        "movq %5,%%rcx\n\t"
        "movq %6,%%rdx\n\t"
        "addq %%rdx,%%rbx\n\t"
        "adcq %%rcx,%%rax\n\t"
        "movq %%rbx,%0\n\t"
        "movq %%rax,%1"
        : "=r"(u128ans.low), "=r"(u128ans.high), "=r"(__UINT128_CARRIED)
        : "r"(u128l.high), "r"(u128l.low), "r"(u128r.high), "r"(u128r.low)
        : "%rax", "%rbx", "%rcx", "%rdx");

    return u128ans;
}

UINT128 UINT128Sub(UINT128 u128l,
                   UINT128 u128r)
{
    UINT128 u128ans;

    asm(
        "movq %3,%%rax\n\t"
        "movq %4,%%rbx\n\t"
        "movq %5,%%rcx\n\t"
        "movq %6,%%rdx\n\t"
        "subq %%rdx,%%rbx\n\t"
        "sbbq %%rcx,%%rax\n\t"
        "jc SUB_SET_CARRY\n\t"
        "movl $0,%2\n\t"
        "jmp SUB_RET\n\t"
        "SUB_SET_CARRY:\n\t"
        "movl $1,%2\n\t"
        "SUB_RET:\n\t"
        "movq %%rbx,%0\n\t"
        "movq %%rax,%1"
        : "=r"(u128ans.low), "=r"(u128ans.high), "=r"(__UINT128_CARRIED)
        : "r"(u128l.high), "r"(u128l.low), "r"(u128r.high), "r"(u128r.low)
        : "%rax", "%rbx", "%rcx", "%rdx");

    return u128ans;
}

UINT128 UINT128SubNoCarry(UINT128 u128l,
                          UINT128 u128r)
{
    UINT128 u128ans;

    asm(
        "movq %3,%%rax\n\t"
        "movq %4,%%rbx\n\t"
        "movq %5,%%rcx\n\t"
        "movq %6,%%rdx\n\t"
        "subq %%rdx,%%rbx\n\t"
        "sbbq %%rcx,%%rax\n\t"
        "movq %%rbx,%0\n\t"
        "movq %%rax,%1"
        : "=r"(u128ans.low), "=r"(u128ans.high), "=r"(__UINT128_CARRIED)
        : "r"(u128l.high), "r"(u128l.low), "r"(u128r.high), "r"(u128r.low)
        : "%rax", "%rbx", "%rcx", "%rdx");

    return u128ans;
}

int UINT128GetCarry()
{
    return __UINT128_CARRIED;
}

UINT128 UINT64Mul(uint64_t u64l,
                  uint64_t u64r)
{
    UINT128 u128ans;

    asm(
        "movq %2,%%rax\n\t"
        "movq %3,%%rbx\n\t"
        "mulq %%rbx\n\t"
        "movq %%rdx,%1\n\t"
        "movq %%rax,%0\n\t"
        : "=r"(u128ans.low), "=r"(u128ans.high)
        : "r"(u64l), "r"(u64r)
        : "%rax", "%rbx", "%rcx", "%rdx");

    return u128ans;
}

uint64_t __UINT64DivNoOverflow(UINT128   u128l,
                               uint64_t  u64r,
                               uint64_t* pu64rem)
{
    uint64_t u64ans, u64rem;

    asm(
        "movq %2,%%rax\n\t"
        "movq %3,%%rdx\n\t"
        "movq %4,%%rbx\n\t"
        "divq %%rbx\n\t"
        "movq %%rax,%0\n\t"
        "movq %%rdx,%1"
        : "=r"(u64ans), "=r"(u64rem)
        : "r"(u128l.low), "r"(u128l.high), "r"(u64r)
        : "%rax", "%rbx", "%rcx", "%rdx");

    if (pu64rem) {
        *pu64rem = u64rem;
    }

    return u64ans;
}

UINT128 UINT64Div(UINT128   u128l,
                  uint64_t  u64r,
                  uint64_t* pu64rem)
{
    UINT128 ans, numer;
    ans.high = u128l.high / u64r;

    numer.high = u128l.high % u64r;
    numer.low  = u128l.low;

    ans.low = __UINT64DivNoOverflow(numer, u64r, pu64rem);

    return ans;
}

UINT128 UINT128Shr(UINT128 u128l, int r)
{
    if (r < 0) {
        return UINT128Shr(u128l, -r);
    }
    else {
        if (r >= 128) {
            UINT128 ans = { 0, 0 };
            return ans;
        }
        else if (r >= 64) {
            UINT128 ans = { u128l.high >> (r - 64), 0 };
            return ans;
        }
        else if (r) {
            UINT128 ans = { (u128l.high << (64 - r)) + (u128l.low >> r),
                            u128l.high >> r };
            return ans;
        }
        else
            return u128l;
    }
}

UINT128 UINT128Shl(UINT128 u128l, int r)
{
    if (r < 0) {
        return UINT128Shl(u128l, -r);
    }
    else {
        if (r >= 128) {
            UINT128 ans = { 0, 0 };
            return ans;
        }
        else if (r >= 64) {
            UINT128 ans = { 0, u128l.low << (r - 64) };
            return ans;
        }
        else if (r) {
            UINT128 ans = { (u128l.low << r),
                            (u128l.high << r) + (u128l.low >> (64 - r)) };
            return ans;
        }
        else {
            return u128l;
        }
    }
}

int UINT128Cmp(UINT128 u128l, UINT128 u128r)
{
    if (u128l.high != u128r.high)
        return u128l.high > u128r.high ? 1 : -1;
    else if (u128l.low != u128r.low)
        return u128l.low > u128r.low ? 1 : -1;
    return 0;
}

int UINT128Clz(UINT128 u128x)
{
    int ans;
    asm(
        "movq %2,%%rax\n\t"
        "movq %1,%%rbx\n\t"
        "bsrq %%rbx,%%rcx\n\t"
        "jnz UINT128Clz_add_64\n\t"
        "bsrq %%rax,%%rcx\n\t"
        "jnz UINT128Clz_sub\n\t"
        "xor %%rcx,%%rcx\n\t"
        "dec %%rcx\n\t"
        "jmp UINT128Clz_sub\n\t"
        "UINT128Clz_add_64:\n\t"
        "leaq 64(,%%rcx,1),%%rcx\n\t"
        "UINT128Clz_sub:\n\t"
        "negq %%rcx\n\t"
        "addq $127,%%rcx\n\t"
        "movl %%ecx,%0"
        : "=r"(ans)
        : "r"(u128x.high), "r"(u128x.low)
        : "%rax", "%rbx", "%rcx");
    return ans;
}

#endif