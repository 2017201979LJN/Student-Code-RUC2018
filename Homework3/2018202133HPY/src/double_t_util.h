#ifndef _DOUBLE_T_UTIL_H

#define _DOUBLE_T_UTIL_H

#include "double_t.h"

void    doubleUtilInit();
DOUBLET float10ToDOUBLET(const char* int_part, const char* frac_part, signed expo);
char*   DOUBLETToString(DOUBLET bad);

#endif