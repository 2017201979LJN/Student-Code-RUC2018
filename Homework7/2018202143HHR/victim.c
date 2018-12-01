#include <stdio.h>
#include <string.h>

void function(const char *input) {
	char buffer[512];
	printf("%lu\n", (long *)buffer); // Get buffer address
	strcpy(buffer, input); // Overflow here
}

int main() {
	FILE *f = fopen("input.bin", "rb");
	if (!f) return 0;

	char buffer[1024];
	fgets(buffer, sizeof(buffer), f);
	function(buffer);

	fputs("failed\n", stderr);
	return 0;
}
