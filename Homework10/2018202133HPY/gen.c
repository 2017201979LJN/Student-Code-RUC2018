#include <stdio.h>
#include <stdlib.h>

double gen_double()
{
    return ((double)rand() / RAND_MAX - 0.5) * 2 * 1e3;
}

int main()
{
    int n;
    scanf("%d", &n);

    srand(n);

    printf("%d\n", n);

    for (int i = 0; i < n; i++) {
        printf("%.3lf %c\n",
            gen_double(),
            ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")[rand() % 52]);
    }
}
