gcc dmk.c -o dmk
./dmk
gcc calculate.c -Og -S
gcc calculate.c -Og -c
objdump -d calculate.o > disasm
