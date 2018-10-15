#include <stdio.h>
#include <math.h>

int main() {
    while (!feof(stdin)) {
        double a, b;
        char op[2];
        scanf("%lf %s %lf", &a, op, &b);

        double c;
        if (*op == '+') c = a + b;
        if (*op == '-') c = a - b;
        if (*op == '*') c = a * b;
        if (*op == '/') c = a / b;

        if (isnan(c)) c = fabs(c);

        printf("%.50le %le\n", c, c);
    }
}
