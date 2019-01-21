#ifndef _ZIP_H

#define _ZIP_H

#define ZIP_DOUBLE_SCALE 1000.0
#define ZIP_INT_OFFSET 1000000

#define ZIP_INT_BIT_WIDTH 21
#define ZIP_CHAR_BIT_WIDTH 6

int zipfile(const char* input, const char* output);

int unzipfile(const char* input);

#endif
