#include <stdio.h>
#include "zip.h"

int main()
{
	zip("zip.in", "x.zip");
	decode("x.zip", "zip.out");
	return 0;
}
