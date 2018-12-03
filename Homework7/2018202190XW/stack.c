#include <stdio.h>
#include <string.h>

void func(char text[]) {
	char buf[32];


	strcpy(buf, text);


}

int main() {
	char text[1024];
	FILE *fp;

	fp = fopen("bad.txt", "r");
	fread(text, sizeof(char), 1024, fp);
	func(text);
	return 0;
}
