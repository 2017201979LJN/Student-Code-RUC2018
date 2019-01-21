#include "lz77.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t count;

    uint16_t *distance;

    // To save space, store length & literal in the same array.
    // If one position is literal, it's in [0, UCHAR_MAX].
    // Otherwise length_literal[i] - (UCHAR_MAX + 1) is length.
    uint16_t *length_literal;
} _lz77_t;

typedef struct {
    size_t count;
    size_t origin_size;
    char arrays[0];
} _lz77_buffer_t;

void _lz77_string_search(char *window,
                         char *next,
                         size_t next_length,
                         char **matched_position,
                         size_t *matched_length) {
    *matched_length = 0;
    for (char *p = window; p < next; p++) {
        size_t length = 0;

        // Overlap is allowed.
        while (length < LZ77_MAX_LENGTH && length < next_length && p[length] == next[length])
            length++;

        if (length > LZ77_MIN_LENGTH && length >= *matched_length) {
            *matched_position = p;
            *matched_length = length;
        }
    }
}

void lz77_compress(void *data, size_t size, void **buffer, size_t *result_size) {
    _lz77_t lz77;
    lz77.count = 0;
    lz77.distance = malloc(sizeof(uint16_t) * size);
    lz77.length_literal = malloc(sizeof(uint16_t) * size);

    // [window, next) is the current sliding window.
    char *window = data, *next = data, *end = data + size;
    size_t i = 0;
    while (next < end) {
        // Right-shift the sliding window.
        if (next - window > LZ77_WINDOW_SIZE) window = next - LZ77_WINDOW_SIZE;

        char *matched_position;
        size_t matched_length;
        _lz77_string_search(window, next, end - next, &matched_position, &matched_length);
        if (matched_length) {
            // Matched: distance + length
            lz77.distance[i] = next - matched_position - 1;
            lz77.length_literal[i] = matched_length + (UCHAR_MAX + 1);

            next += matched_length;
        } else {
            // Not matched: literal
            lz77.length_literal[i] = (unsigned char)*next;

            next++;
        }

        i++, lz77.count++;
    }

    // Output binary data.
    size_t array_size = sizeof(uint16_t) * lz77.count;
    *result_size = sizeof(size_t) * 2 + array_size * 2;
    *buffer = malloc(*result_size);
    
    _lz77_buffer_t *p = *buffer;
    p->count = lz77.count;
    p->origin_size = size;
    memcpy(p->arrays, lz77.distance, array_size);
    memcpy(p->arrays + array_size, lz77.length_literal, array_size);
}

void lz77_decompress(void *data, size_t size, void **buffer, size_t *result_size) {
    _lz77_buffer_t *p = data;

    _lz77_t lz77;
    lz77.count = p->count;
    size_t array_size = sizeof(uint16_t) * lz77.count;
    lz77.distance = (uint16_t *)p->arrays;
    lz77.length_literal = (uint16_t *)(p->arrays + array_size);

    *result_size = p->origin_size;
    *buffer = malloc(*result_size);

    char *curr = *buffer;
    for (size_t i = 0; i < lz77.count; i++) {
        if (lz77.length_literal[i] <= UCHAR_MAX) {
            // literal
            *curr++ = lz77.length_literal[i];
        } else {
            // distance + length
            // Maybe overlap, so don't use memcpy().
            char *start = curr - lz77.distance[i] - 1;
            size_t length = lz77.length_literal[i] - (UCHAR_MAX + 1);
            for (size_t j = 0; j < length; j++)
                *curr++ = start[j];
        }
    }
}
