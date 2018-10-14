#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t rand64() {
    return (((uint64_t)rand()) << 32) | ((uint64_t)rand());
}

double rand_double() {
    // The first method can generate more edge case while the second
    // method can prevent one number is less/greater than too many times
    // of another.
    if (rand() & 1) {
        union {
            unsigned long long uint64_t_value;
            double double_value;
        } u;
        u.uint64_t_value = rand64();
        return u.double_value;
    } else {
        double a = (double)rand64() * (double)(1ull << 63) + rand64(),
               b = (double)rand64() * (double)(1ull << 63) + rand64();
        int sign = (rand() & 1) ? 1 : -1;
        return a / b * sign;
    }
}

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        printf("%.50le %c %.50le\n", rand_double(), "+-*/"[rand() % 4], rand_double());
    }
}