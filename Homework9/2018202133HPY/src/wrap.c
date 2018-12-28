#include <stdio.h>

static size_t num = 0;

void *__real_malloc(size_t size);
void __real_free(void *ptr);

void *__wrap_malloc(size_t size)
{
    num++;
    return __real_malloc(size);
}

void __wrap_free(void *ptr)
{
    num--;
    __real_free(ptr);
}

size_t getSiz()
{
    return num;
}
