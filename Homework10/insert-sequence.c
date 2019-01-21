#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>

#include "bitset/bitset.h"
#include "bitset/bitstream.h"

// #define HUFFMAN

#include "huffman/huffman.h"
#ifdef HUFFMAN
#endif

void zip(FILE *fin, FILE *fout) {
    size_t n;
    assert(fscanf(fin, "%zu", &n) == 1);

    // Doubles are in (-1000.000, 1000.000) and have 3 decimals.
    // Convert them to x * 1000, which is in (-1000000, 1000000), store as 20 bit and 1 sign bit.
    // Use 5 bit to store a alpha.
    // So one group of data is 26 bits -- NOT friendly for huffman encoding.
    // Consider value of highest 2 bits of both data is not average, try huffman encode them.
    bitset_t bitset = bitset_create((20 + 3) * n);
    bitstream_t bitstream = bitstream_create(bitset, 0);

    bitset_t bitset2 = bitset_create((2 + 2) * n);
    bitstream_t bitstream2 = bitstream_create(bitset2, 0);

    for (size_t i = 0; i < n; i++) {
        double data_double;
        char data_char;
        assert(fscanf(fin, "%lf %c", &data_double, &data_char) == 2);

        // Encode double data.
        assert(data_double >= -1000 && data_double <= 1000);
        uint32_t integer = rint(fabs(data_double) * 1000);
        bitstream_write(bitstream2, integer & (1U << 20));
        bitstream_write(bitstream2, integer & (1U << 19));
        for (size_t i = 2; i < 21; i++) {
            bitstream_write(bitstream, integer & (1U << (21 - i - 1)));
        }
        bitstream_write(bitstream, signbit(data_double));

        // Encode char data.
        assert(data_char >= 'a' && data_char <= 'z');
        uint32_t tmp = data_char - 'a';

        // Use 5 bit to store a alpha.
        bitstream_write(bitstream2, tmp & (1U << 4));
        bitstream_write(bitstream2, tmp & (1U << 3));
        for (size_t j = 2; j < 5; j++) {
            bitstream_write(bitstream, tmp & (1U << (5 - j - 1)));
        }
    }

    // Write n.
    fwrite(&n, sizeof(size_t), 1, fout);

    // Hufman encode and write bitset2 (both's first 2 bits).
    void *encoded_buffer2;
    size_t encoded_length2;
    huffman_encode(bitset2, bitset_sizeof(bitset2), &encoded_buffer2, &encoded_length2);
    fwrite(&encoded_length2, sizeof(size_t), 1, fout);
    fwrite(encoded_buffer2, 1, encoded_length2, fout);
    free(encoded_buffer2);

#ifdef HUFFMAN
    void *encoded_buffer;
    size_t encoded_length;
    huffman_encode(bitset, bitset_sizeof(bitset), &encoded_buffer, &encoded_length);
    fwrite(encoded_buffer, 1, encoded_length, fout);
    free(encoded_buffer);
#else
    fwrite(bitset, 1, bitset_sizeof(bitset), fout);
#endif

    bitstream_destroy(bitstream2);
    bitstream_destroy(bitstream);
    bitset_destroy(bitset2);
    bitset_destroy(bitset);
}

void unzip(FILE *fin, FILE *fout) {
    fseek(fin, 0, SEEK_END);
    size_t file_size = ftell(fin);
    rewind(fin);

    size_t n;
    assert(fread(&n, sizeof(size_t), 1, fin) == 1);

    size_t encoded_length2;
    assert(fread(&encoded_length2, sizeof(size_t), 1, fin) == 1);
    void *encoded_buffer2 = malloc(encoded_length2);
    assert(fread(encoded_buffer2, 1, encoded_length2, fin) == encoded_length2);
    bitset_t bitset2;
    size_t decoded_length2;
    huffman_decode(encoded_buffer2, (void **)&bitset2, &decoded_length2);
    assert(bitset2->n == (2 + 2) * n);

    size_t remain_size = file_size - 2 * sizeof(size_t) - encoded_length2;

#ifdef HUFFMAN
    void *encoded_buffer = malloc(remain_size);
    assert(fread(encoded_buffer, 1, remain_size, fin) == remain_size);

    bitset_t bitset;
    size_t decoded_length;
    huffman_decode(encoded_buffer, (void **)&bitset, &decoded_length);
#else
    bitset_t bitset = malloc(remain_size);
    assert(fread(bitset, 1, remain_size, fin) == remain_size);
#endif

    assert(n * (20 + 3) == bitset->n);
    fprintf(fout, "%zu\n", n);

    bitstream_t bitstream = bitstream_create(bitset, 0);
    bitstream_t bitstream2 = bitstream_create(bitset2, 0);

    for (size_t i = 0; i < n; i++) {
        // Decode double data.
        uint32_t integer = 0;
        integer = (integer << 1) | bitstream_read(bitstream2);
        integer = (integer << 1) | bitstream_read(bitstream2);
        for (size_t i = 2; i < 21; i++) {
            integer = (integer << 1) | bitstream_read(bitstream);
        }
        assert(integer < 1000000);
        bool sign = bitstream_read(bitstream);
        
        double data_double = (sign ? -1.0 : 1.0) * (double)integer / 1000;

        // Decode char data.
        int tmp = 0;
        tmp = (tmp << 1) | bitstream_read(bitstream2);
        tmp = (tmp << 1) | bitstream_read(bitstream2);
        for (size_t j = 2; j < 5; j++) {
            tmp = (tmp << 1) | bitstream_read(bitstream);
        }

        char data_char = tmp + 'a';

        // Write decoded data.
        fprintf(fout, "%.3lf %c\n", data_double, data_char);
    }

#ifdef HUFFMAN
    free(encoded_buffer);
#endif

    free(encoded_buffer2);
    bitstream_destroy(bitstream2);
    bitstream_destroy(bitstream);
    bitset_destroy(bitset2);
    bitset_destroy(bitset);
}

int main(int argc, char *argv[]) {
    assert(argc == 4);

    FILE *fin = fopen(argv[2], "r");
    assert(fin);

    FILE *fout = fopen(argv[3], "w");
    assert(fout);

    if (strcmp(argv[1], "zip") == 0) {
        zip(fin, fout);
    } else if (strcmp(argv[1], "unzip") == 0) {
        unzip(fin, fout);
    } else assert(false);

    fclose(fin);
    fclose(fout);
}
