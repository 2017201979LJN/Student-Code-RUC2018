#include "rdtsc.h"

cycle_t rdtsc()
{
    cycle_t ret = 0;
    __asm__ __volatile__(
        "lfence\n\t"
        "mfence\n\t"
        "rdtsc\n\t"
        "shlq $32,%%rdx\n\t"
        "orq %%rax,%%rdx\n\t"
        "movq %%rdx,%0"
        : "=r"(ret)
        :
        : "%rax", "%rdx");
    return ret;
}
