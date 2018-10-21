#ifndef _MENCI_POW10_TABLE_H
#define _MENCI_POW10_TABLE_H

#include <stdint.h>

// [1e-323, 1e308]
#define _POW10_TABLE_MIN -323
#define _POW10_TABLE_MAX 308

extern const uint64_t _POW10_TABLE[_POW10_TABLE_MAX - _POW10_TABLE_MIN + 1];

#endif // _MENCI_POW10_TABLE_H
