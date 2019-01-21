main : main.o avl.o
	cc -o main main.o avl.o
avl.o : avl.c
	cc -c avl.c
main.o : main.c
	cc -c main.c
solve.o : solve.c
	cc -c solve.c
solve : solve.o avl.o
	cc -o solve solve.o avl.o
datamaker : datamaker.c
	cc -o datamaker datamaker.c
run :
	./datamaker
	./main
	./solve
