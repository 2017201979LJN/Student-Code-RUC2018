#include <stdio.h>
#include <stdlib.h>

#define GEN16 ((long long)(rand()) & 0xffff)

union double_bits {
    double             d;
    unsigned long long ull;
};

double gen_double()
{
    union double_bits db;
    db.ull = (GEN16) | (GEN16 << 16) | (GEN16 << 32) | (GEN16 << 48);
    return db.d;
}

char gen_op()
{
    static char ops[] = "+-*/";
    return ops[rand() & 3];
}

int main(int argc, char** argv)
{
    srand(atoi(argv[1]));

    int num = atoi(argv[2]);

    for (int i = 1; i <= num; i++) {
        printf("%.8le %c %.8le\n", gen_double(), gen_op(), gen_double());
    }

    return 0;
}