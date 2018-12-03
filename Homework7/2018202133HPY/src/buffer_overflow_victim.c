#include <stdio.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 128
#endif

int main(void) {
    char buffer[BUFFER_SIZE];
    printf("%p\n", buffer);
    gets(buffer);
    return 0;
}

