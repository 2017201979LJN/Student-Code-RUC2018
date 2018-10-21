// If we debug this, let it execute the main() in debug.c
#ifndef DEBUG

#include <stdio.h>
#include <string.h>
#include "eval.h"
#include "double_output.h"

int main() {
    const int BUFFER_SIZE = 1e6;
    char s[BUFFER_SIZE];
    while (~scanf(" %[^\n]", s)) {
        double_t result = eval(s);
        char buffer[BUFFER_SIZE];
#ifdef CHECK
        fp_output(result, 10, buffer);
#else
        fp_output(result, 1000, buffer);
        int len = strlen(buffer);
        while (buffer[len - 1] == '0') buffer[--len] = '\0';
        if (buffer[len - 1] == '.') buffer[--len] = '\0';
#endif
        printf("%s\n", buffer);
    }
}

#endif
