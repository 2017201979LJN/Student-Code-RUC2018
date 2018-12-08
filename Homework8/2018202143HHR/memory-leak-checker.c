#define _GNU_SOURCE
#include <dlfcn.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static size_t count = 0;

#define ITOA_BUFFER_SIZE  20
#define PRINT_BUFFER_SIZE 100

static void write_dec(char **out, size_t num) {
    char buffer[ITOA_BUFFER_SIZE], *p = buffer;
    do *p++ = num % 10 + '0'; while (num /= 10);
    do *(*out)++ = *--p; while (p != buffer);
}

static void write_hex(char **out, size_t num) {
#define HEX_TABLE "0123456789abcdef"
#define byte2hex(byte) *(uint16_t *)&(char[2]){ HEX_TABLE[(byte) >> 4 & 0xF], \
                                                HEX_TABLE[(byte) & 0xF] }
    for (int i = sizeof(size_t) / sizeof(char) - 1; i >= 0; i--)
        *(*(uint16_t **)out)++ = byte2hex((unsigned char)(num >> (i * 8)));
}

#define write_ptr(out, ptr) (write_str(out, "0x"), write_hex(out, (size_t)(ptr)))

static void write_str(char **out, char *str) {
    while (*str) *(*out)++ = *str++;
}

static void free_message(void *ptr, size_t count) {
    char buffer[PRINT_BUFFER_SIZE], *curr = buffer;
    write_str(&curr, "free(");
    write_ptr(&curr, ptr);
    write_str(&curr, "), ");
    write_dec(&curr, count);
    write_str(&curr, " blocks remain.\n");
    write(STDOUT_FILENO, buffer, curr - buffer);
}

void free(void *ptr) {
    static void (*libc_free)(void *) = NULL;
    if (!libc_free) libc_free = dlsym(RTLD_NEXT, "free");

    libc_free(ptr);

    count--;
    free_message(ptr, count);
}

static void malloc_message(size_t size, void *ptr, size_t count) {
    char buffer[PRINT_BUFFER_SIZE], *curr = buffer;
    write_str(&curr, "malloc(");
    write_dec(&curr, size);
    write_str(&curr, ") = ");
    write_ptr(&curr, ptr);
    write_str(&curr, ", ");
    write_dec(&curr, count);
    write_str(&curr, " blocks remain.\n");
    write(STDOUT_FILENO, buffer, curr - buffer);
}

void *malloc(size_t size) {
    static void *(*libc_malloc)(size_t) = NULL;
    if (!libc_malloc) libc_malloc = dlsym(RTLD_NEXT, "malloc");

    void *ptr = libc_malloc(size);

    count++;
    malloc_message(size, ptr, count);

    return ptr;
}
