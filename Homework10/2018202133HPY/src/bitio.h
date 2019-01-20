#ifndef _BITIO_H

#define _BITIO_H

#include <stddef.h>

#define BWRITE_PACK_32(stream, x, cnt) ({       \
    unsigned int tmp = __builtin_bswap32(*(x)); \
    bwrite(stream, &tmp, cnt);                  \
})

#define BREAD_PACK_32(stream, x, cnt) ({   \
    unsigned int tmp;                      \
    size_t ret = bread(stream, &tmp, cnt); \
    *(x) = __builtin_bswap32(tmp);         \
    ret;                                   \
})

struct bfile_t;

typedef struct bfile_t BFILE;

BFILE* bfopen(const char* pathname, const char* mode);
int bfclose(BFILE* stream);

size_t bread(BFILE* stream, void* buf, size_t count);
size_t bwrite(BFILE* stream, void* buf, size_t count);

int bfflush(BFILE* stream);

#endif
