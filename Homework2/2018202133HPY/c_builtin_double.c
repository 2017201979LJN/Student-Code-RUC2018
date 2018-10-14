#include <ctype.h>
#include <stdio.h>

int main(void)
{
    char   op, bak;
    double a, b, c = 0;

    while (scanf("%lf", &a) == 1) {

        while (isspace(bak = getchar()))
            ;

        op = bak;

        scanf("%lf", &b);

        switch (op) {
        case '+':
            c = a + b;
            break;
        case '-':
            c = a - b;
            break;
        case '*':
            c = a * b;
            break;
        case '/':
            c = a / b;
            break;
        }

        printf("[%.20le,%le]\n", c, c);
    }
}