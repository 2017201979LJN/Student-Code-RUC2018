#include<stdio.h>
#include<stdlib.h>

int main() {
	system("gcc generator.c -o gen");
	system("./gen");
	system("gcc test.c -o test");
	system("gcc translator.c -o translator");
	system("./translator");
	system("gcc final.s -o final");
	return 0;
}
	
