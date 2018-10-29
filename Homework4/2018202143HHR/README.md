# Generate code
```bash
gcc generator.c -o generator -O2
./generator
```

You can pass some arguments to the generator:

```bash
./generator [random_range = 10000] [variable_count = 5] [argument_count = 5] [expression_count = 5 * variable_count]
```

The generated code is like:

```c
long long f(long long a0, long long a1, long long a2, long long a3, long long a4) {
    long long x0 = 4971;
    long long x1 = 534;
    long long x2 = 9448;
    long long x3 = 4646;
    long long x4 = 2096;
    x3 = 3033 + a4;
    x2 = x3 * x0;
    x1 = 2265 + x2;
    x0 = x2 + x3;
    x0 = 5363 * x3;
    x3 = 3195 * x4;
    x2 = 5072 * a4;
    x4 = a0 * a4;
    x2 = a4 + x1;
    x4 = x3 + a2;
    x4 = 8703 + a0;
    x0 = x2 + x2;
    x1 = a4 + a1;
    x0 = 8478 * a3;
    x3 = a0 * x1;
    x3 = 3057 * x2;
    x1 = a3 + x0;
    x0 = 4795 * x2;
    x0 = x3 * a1;
    x0 = 9196 * a3;
    x0 = 4511 * x4;
    x0 = 7040 * a3;
    x3 = 5856 + x0;
    x4 = 4423 * a2;
    x4 = 5588 * x3;
    return x2;
}

#include <stdio.h>

int main() {
    long long a0;
    scanf("%lld", &a0);
    long long a1;
    scanf("%lld", &a1);
    long long a2;
    scanf("%lld", &a2);
    long long a3;
    scanf("%lld", &a3);
    long long a4;
    scanf("%lld", &a4);
    long long res = f(a0, a1, a2, a3, a4);
    printf("%lld\n", res);
    return 0;
}
```

# Compile generated code to ASM
```bash
gcc compiler.c -o compiler -O2
./generator | ./compiler
```

The compiled code (printed to `stdout`) is like:

```asm
movq $4971, %rbx
movq $534, %rcx
movq $9448, %rdx
movq $4646, %rbp
movq $2096, %rsi
movq $3033, %rax
addq %r11, %rax
movq %rax, %rbp
movq %rbp, %rax
imulq %rbx, %rax
movq %rax, %rdx
movq $2265, %rax
addq %rdx, %rax
movq %rax, %rcx
movq %rdx, %rax
addq %rbp, %rax
movq %rax, %rbx
movq $5363, %rax
imulq %rbp, %rax
movq %rax, %rbx
movq $3195, %rax
imulq %rsi, %rax
movq %rax, %rbp
movq $5072, %rax
imulq %r11, %rax
movq %rax, %rdx
movq %rdi, %rax
imulq %r11, %rax
movq %rax, %rsi
movq %r11, %rax
addq %rcx, %rax
movq %rax, %rdx
movq %rbp, %rax
addq %r9, %rax
movq %rax, %rsi
movq $8703, %rax
addq %rdi, %rax
movq %rax, %rsi
movq %rdx, %rax
addq %rdx, %rax
movq %rax, %rbx
movq %r11, %rax
addq %r8, %rax
movq %rax, %rcx
movq $8478, %rax
imulq %r10, %rax
movq %rax, %rbx
movq %rdi, %rax
imulq %rcx, %rax
movq %rax, %rbp
movq $3057, %rax
imulq %rdx, %rax
movq %rax, %rbp
movq %r10, %rax
addq %rbx, %rax
movq %rax, %rcx
movq $4795, %rax
imulq %rdx, %rax
movq %rax, %rbx
movq %rbp, %rax
imulq %r8, %rax
movq %rax, %rbx
movq $9196, %rax
imulq %r10, %rax
movq %rax, %rbx
movq $4511, %rax
imulq %rsi, %rax
movq %rax, %rbx
movq $7040, %rax
imulq %r10, %rax
movq %rax, %rbx
movq $5856, %rax
addq %rbx, %rax
movq %rax, %rbp
movq $4423, %rax
imulq %r9, %rax
movq %rax, %rsi
movq $5588, %rax
imulq %rbp, %rax
movq %rax, %rsi
movq %rdx, %rax
```

Messages that printed to `stderr` can explain which argument/variable is stored in which register. And the return value is stored in `%rax`.

```plain
variable x0 is stored in %rbx
variable x1 is stored in %rcx
variable x2 is stored in %rdx
variable x3 is stored in %rbp
variable x4 is stored in %rsi
argument a0 is stored in %rdi
argument a1 is stored in %r8
argument a2 is stored in %r9
argument a3 is stored in %r10
argument a4 is stored in %r11
```

If you want to get code that can be compiled by GCC (or a GCC-compatible compiler), define `COMPILABLE` when compiling `compiler.c`:

```bash
gcc compiler.c -o compile -DCOMPILABLE
./generator | ./compiler
```

The compiled code (printed to `stdout`) is like:

```c
long long f(long long a0, long long a1, long long a2, long long a3, long long a4) {
    long long return_value;
    asm(
        "movq %1, %%r8\n"
        "movq %2, %%r9\n"
        "movq %3, %%r10\n"
        "movq %4, %%r11\n"
        "movq %5, %%r12\n"
        "movq $4971, %%rbx\n"
        "movq $534, %%rcx\n"
        "movq $9448, %%rdx\n"
        "movq $4646, %%rsi\n"
        "movq $2096, %%rdi\n"
        "movq $3033, %%rax\n"
        "addq %%r12, %%rax\n"
        "movq %%rax, %%rsi\n"
        "movq %%rsi, %%rax\n"
        "imulq %%rbx, %%rax\n"
        "movq %%rax, %%rdx\n"
        "movq $2265, %%rax\n"
        "addq %%rdx, %%rax\n"
        "movq %%rax, %%rcx\n"
        "movq %%rdx, %%rax\n"
        "addq %%rsi, %%rax\n"
        "movq %%rax, %%rbx\n"
        "movq $5363, %%rax\n"
        "imulq %%rsi, %%rax\n"
        "movq %%rax, %%rbx\n"
        "movq $3195, %%rax\n"
        "imulq %%rdi, %%rax\n"
        "movq %%rax, %%rsi\n"
        "movq $5072, %%rax\n"
        "imulq %%r12, %%rax\n"
        "movq %%rax, %%rdx\n"
        "movq %%r8, %%rax\n"
        "imulq %%r12, %%rax\n"
        "movq %%rax, %%rdi\n"
        "movq %%r12, %%rax\n"
        "addq %%rcx, %%rax\n"
        "movq %%rax, %%rdx\n"
        "movq %%rsi, %%rax\n"
        "addq %%r10, %%rax\n"
        "movq %%rax, %%rdi\n"
        "movq $8703, %%rax\n"
        "addq %%r8, %%rax\n"
        "movq %%rax, %%rdi\n"
        "movq %%rdx, %%rax\n"
        "addq %%rdx, %%rax\n"
        "movq %%rax, %%rbx\n"
        "movq %%r12, %%rax\n"
        "addq %%r9, %%rax\n"
        "movq %%rax, %%rcx\n"
        "movq $8478, %%rax\n"
        "imulq %%r11, %%rax\n"
        "movq %%rax, %%rbx\n"
        "movq %%r8, %%rax\n"
        "imulq %%rcx, %%rax\n"
        "movq %%rax, %%rsi\n"
        "movq $3057, %%rax\n"
        "imulq %%rdx, %%rax\n"
        "movq %%rax, %%rsi\n"
        "movq %%r11, %%rax\n"
        "addq %%rbx, %%rax\n"
        "movq %%rax, %%rcx\n"
        "movq $4795, %%rax\n"
        "imulq %%rdx, %%rax\n"
        "movq %%rax, %%rbx\n"
        "movq %%rsi, %%rax\n"
        "imulq %%r9, %%rax\n"
        "movq %%rax, %%rbx\n"
        "movq $9196, %%rax\n"
        "imulq %%r11, %%rax\n"
        "movq %%rax, %%rbx\n"
        "movq $4511, %%rax\n"
        "imulq %%rdi, %%rax\n"
        "movq %%rax, %%rbx\n"
        "movq $7040, %%rax\n"
        "imulq %%r11, %%rax\n"
        "movq %%rax, %%rbx\n"
        "movq $5856, %%rax\n"
        "addq %%rbx, %%rax\n"
        "movq %%rax, %%rsi\n"
        "movq $4423, %%rax\n"
        "imulq %%r10, %%rax\n"
        "movq %%rax, %%rdi\n"
        "movq $5588, %%rax\n"
        "imulq %%rsi, %%rax\n"
        "movq %%rax, %%rdi\n"
        "movq %%rdx, %%rax\n"
        "movq %%rax, %0\n"
    :"=m"(return_value):"m"(a0),"m"(a1),"m"(a2),"m"(a3),"m"(a4):"%rbx","%rcx","%rdx","%rsi","%rdi","%r8","%r9","%r10","%r11","%r12","%rax");
    return return_value;
}

#include <stdio.h>

int main() {
    long long a0;
    scanf("%lld", &a0);
    long long a1;
    scanf("%lld", &a1);
    long long a2;
    scanf("%lld", &a2);
    long long a3;
    scanf("%lld", &a3);
    long long a4;
    scanf("%lld", &a4);
    long long res = f(a0, a1, a2, a3, a4);
    printf("%lld\n", res);
    return 0;
}
```

**Warning**: If you want to test some hand-written code, please make your code the same format and function/argument/variable name as generated code.

# Test

Both generated C code and compiled C code can be compiled and executed and read arguments from `stdin`. You can test them.

You can also run `check.sh` to generator and run random test cases automatically.

`check.sh` will stop on Ctrl-C.

```bash
$ ./check.sh
Compiling...
Successfully compiled!
1 cases passed.
2 cases passed.
3 cases passed.
4 cases passed.
5 cases passed.
6 cases passed.
7 cases passed.
8 cases passed.
9 cases passed.
Cleaning up...
```
