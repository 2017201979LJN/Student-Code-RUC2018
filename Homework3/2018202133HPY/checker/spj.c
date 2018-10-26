#include <math.h>
#include <stdio.h>

int main(void)
{
    double a, b;
    scanf("%lf%lf", &a, &b);
    if (isnan(a) || isnan(b)) {
        if (isnan(a) && isnan(b)) {
            return 0;
        }
        else {
            return 1;
        }
    }
    if (isinf(a) || isinf(b)) {
        if (a == b)
            return 0;
        else
            return 1;
    }

    double x = fabs(a - b) / fmax(fmin(fabs(a), fabs(b)), 1e-300);

    if (x < 1e-6) {
        return 0;
    }
    else if (x < 1e-3) {
        fprintf(stderr, "[WARNING] FLOATING POINT ERROE DETECTED\n");
        return 0;
    }
    else {
        return 1;
    }
    return 0;
}