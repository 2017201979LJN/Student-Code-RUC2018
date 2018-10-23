#ifndef _MENCI_EVAL_H
#define _MENCI_EVAL_H

#include "double_input.h"

int _eval_operator_priority(char operator);
double_t eval(const char *expression);

#endif // _MENCI_EVAL_H
