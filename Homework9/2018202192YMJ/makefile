objects = prog main.o treap.o libtreap.so\
		  prog_intc main_intc.o treap_intc.o my_stdlib_intc.o\
		  prog_intl my_stdlib_intl.o\
		  prog_intr my_stdlib_intr.so\
		  check

prog : main.o libtreap.so
		gcc -o prog main.o ./libtreap.so
prog_intc : main_intc.o treap_intc.o my_stdlib_intc.o
		gcc -I. -o prog_intc main_intc.o treap_intc.o my_stdlib_intc.o
prog_intl : main.o treap.o my_stdlib_intl.o
		gcc -Wl,--wrap,malloc -Wl,--wrap,free\
		-o prog_intl main.o treap.o my_stdlib_intl.o
prog_intr : my_stdlib_intr.so main.c treap.c treap.h
		gcc -o prog_intr main.c treap.c
check : check.c
		gcc -o check check.c

main.o : main.c treap.h
		gcc -c main.c
libtreap.so : treap.o
		gcc -shared -o libtreap.so treap.o
treap.o : treap.c treap.h
		gcc -shared -fpic -c treap.c
my_stdlib_intc.o : my_stdlib.c
		gcc -o my_stdlib_intc.o -DCOMPILETIME -c my_stdlib.c
main_intc.o : main.c treap.h
		gcc -I. -o main_intc.o -c main.c
treap_intc.o : treap.c treap.h
		gcc -I. -o treap_intc.o -c treap.c
my_stdlib_intl.o : my_stdlib.c
		gcc -o my_stdlib_intl.o -DLINKTIME -c my_stdlib.c
my_stdlib_intr.so: my_stdlib.c
		gcc -DRUNTIME -shared -fpic -o my_stdlib_intr.so my_stdlib.c -ldl

clean :
		rm $(objects)
