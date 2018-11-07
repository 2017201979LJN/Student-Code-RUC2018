#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
    srand(atoi(argv[1]));
    printf("long long QAQ(long long x1, long long x2, long long x3, long long x4, long long x5)\n{\n");
    for (int i = 0; i < 25; i++) {
        printf("    x%d = x%d %c ", rand() % 5 + 1, rand() % 5 + 1, rand() & 1 ? '+' : '*');
        if (rand() & 1) {
            printf("x%d", rand() % 5 + 1);
        }
        else {
            printf("%d", rand());
        }
        puts(";");
    }
    printf("    return x1 ^ x2 ^ x3 ^ x4 ^ x5;\n}\n\n");
    return 0;
}
