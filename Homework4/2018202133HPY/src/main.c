#include <stdio.h>
#include <stdlib.h>

long long QAQ(long long x1, long long x2, long long x3, long long x4, long long x5);

int main(int argc, char** argv)
{
    srand(atoi(argv[1]));
    printf("%lld\n", rand(), rand(), rand(), rand(), rand());
    return 0;
}
