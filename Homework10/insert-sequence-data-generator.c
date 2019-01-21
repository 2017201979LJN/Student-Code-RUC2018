#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main() {
    int n, seed;
    assert(scanf("%d %d", &n, &seed) == 2);

    srand(seed);

    printf("%d\n", n);
    for (int i = 0; i < n; i++) {
        double data_double = (double)rand() / RAND_MAX * 1000 * 2 - 1000;
        char data_char = rand() % ('z' - 'a' + 1) + 'a';
        printf("%.3lf %c\n", data_double, data_char);
    }
}
