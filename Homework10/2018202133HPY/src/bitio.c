#include "bitio.h"

#include <stdio.h>
#include <stdlib.h>

struct bfile_t {
    FILE* stream;
    _Bool input;
    unsigned char buffer;
    int len;
};

BFILE* bfopen(const char* pathname, const char* mode)
{
    BFILE* stream = malloc(sizeof(BFILE));

    if (!stream) {
        return NULL;
    }

    stream->stream = fopen(pathname, mode);
    stream->len = 0;

    while (*mode) {
        if (*mode == 'r') {
            stream->input = 1;
        }

        if (*mode == 'w') {
            stream->input = 0;
        }

        mode++;
    }

    if (!stream->stream) {
        free(stream);
    }

    return stream;
}

int bfclose(BFILE* stream)
{
    if (!stream->input) {
        if (bfflush(stream)) {
            free(stream);
            return EOF;
        }
    }

    int err = fclose(stream->stream);
    free(stream);

    return err;
}

int bfflush(BFILE* stream)
{
    if (!stream->input) {
        if (stream->len) {
            stream->buffer &= ~(0xFFu >> (stream->len));
            if (fwrite(&stream->buffer, 1, 1, stream->stream) != 1)
                return EOF;

            stream->buffer = 0;
            stream->len = 0;
        }
    }

    return 0;
}

size_t bread(BFILE* stream, void* buf, size_t count)
{
    if (!stream->input)
        return 0;

    for (int i = 0, offset = 0; i < count; i++) {
        if (stream->len == 0) {
            if (fread(&stream->buffer, 1, 1, stream->stream) != 1) {
                return i;
            }
            stream->len = 8;
        }

        if (stream->buffer >> 7) {
            *(unsigned char*)buf = *(unsigned char*)buf | (1u << (8 - offset - 1));
        } else {
            *(unsigned char*)buf = *(unsigned char*)buf & ~(1u << (8 - offset - 1));
        }

        stream->buffer <<= 1;
        stream->len--;

        offset++;
        if (offset == 8) {
            buf++;
            offset = 0;
        }
    }

    return count;
}

size_t bwrite(BFILE* stream, void* buf, size_t count)
{
    if (stream->input)
        return 0;

    for (int i = 0, offset = 0; i < count; i++) {

        if (*(unsigned char*)buf & (1u << (8 - offset - 1))) {
            stream->buffer = stream->buffer | (0x80u >> stream->len);
        } else {
            stream->buffer = stream->buffer & ~(0x80u >> stream->len);
        }
        stream->len++;

        if (stream->len == 8) {
            if (fwrite(&stream->buffer, 1, 1, stream->stream) != 1) {
                return i;
            }
            stream->len = 0;
        }

        offset++;
        if (offset == 8) {
            buf++;
            offset = 0;
        }
    }

    return count;
}
