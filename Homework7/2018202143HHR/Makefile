exec: victim input.bin
	./victim
victim: victim.c
	sudo sysctl -w kernel.randomize_va_space=0
	cc -o victim victim.c -fno-stack-protector -g -z execstack
	sudo chown root:root victim
	sudo chmod 4755 victim
shellcode: shellcode.s
	cc -c -o shellcode shellcode.s
	objdump -d shellcode
	objcopy -O binary -j .text shellcode shellcode.bin
	od -An -t x1 shellcode.bin | bash -c 'for x in $$(cat); do echo -n "\x$$x"; done; echo'
shellcode-test: shellcode-test.c
	cc -o shellcode-test shellcode-test.c shellcode.s -fno-stack-protector -g
	sudo chown root:root shellcode-test
	sudo chmod 4755 shellcode-test
	./shellcode-test || true
input-generator: input-generator.c shellcode
	cc -o input-generator input-generator.c -g
input.bin: input-generator
	cp /dev/null input.bin
	./victim | tee buffer_pointer
	./input-generator < buffer_pointer
	rm buffer_pointer
clean:
	rm -f victim shellcode shellcode.bin shellcode-test input.bin input-generator
