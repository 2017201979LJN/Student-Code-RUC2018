#include "bitstream.h"

#include <limits.h>
#include <string.h>
#include <stdlib.h>
// #include <stdio.h>

bitstream_t bitstream_create(bitset_t bitset, size_t pos) {
    bitstream_t bitstream = malloc(sizeof(*bitstream));
    bitstream->bitset = bitset;
    bitstream->pos = pos;
    return bitstream;
}

void bitstream_destroy(bitstream_t bitstream) {
    free(bitstream);
}

void bitstream_write(bitstream_t bitstream, bool value) {
    bitset_set(bitstream->bitset, bitstream->pos++, value);
}

void bitstream_write_data(bitstream_t bitstream, void *data, size_t length) {
    bitstream->pos += (CHAR_BIT - bitstream->pos % CHAR_BIT) % CHAR_BIT; // Ailgn
    char *buffer = (char *)bitstream->bitset->buffer;
    memcpy(buffer + bitstream->pos / CHAR_BIT, data, length);
    bitstream->pos += length * CHAR_BIT;

    // fprintf(stderr, "after write data of length = %d, pos = %d\n", (int)length, (int)bitstream->pos);
}

bool bitstream_read(bitstream_t bitstream) {
    return bitset_get(bitstream->bitset, bitstream->pos++);
}

void bitstream_read_data(bitstream_t bitstream, void *result, size_t length) {
    bitstream->pos += (CHAR_BIT - bitstream->pos % CHAR_BIT) % CHAR_BIT; // Align
    char *buffer = (char *)bitstream->bitset->buffer;
    memcpy(result, buffer + bitstream->pos / CHAR_BIT, length);
    bitstream->pos += length * CHAR_BIT;
}

// Close a output stream and set the size to current size.
void bitstream_close(bitstream_t bitstream) {
    bitstream->bitset->n = bitstream->pos + 1;
}
