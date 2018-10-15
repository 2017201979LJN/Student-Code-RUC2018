#include <stdio.h>
#include "double.h"

int main() {
    while (!feof(stdin)) {
        double a, b;
        char op[2];
        scanf("%lf %s %lf", &a, op, &b);

        double_t (*calc)(const double_t, const double_t);
        if (*op == '+') calc = fp_add;
        if (*op == '-') calc = fp_sub;
        if (*op == '*') calc = fp_mul;
        if (*op == '/') calc = fp_div;

        double_t ta = fp_reinterpret_from_double(a),
                 tb = fp_reinterpret_from_double(b),
                 tc = calc(ta, tb);

        double c = fp_reinterpret_to_double(tc);

        printf("%.50le %le\n", c, c);
    }
}
