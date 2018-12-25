.PHONY : clean

main : main.o splay.o
	@gcc -o main main.o splay.o
main.o : main.c 
	@gcc -c main.c
splay.o : splay.c
	@gcc -c splay.c
clean :
	@rm -f *.o
