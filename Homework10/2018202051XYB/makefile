.PHONY : clean

main : main.o splay.o compression.o
	@gcc -o run main.o splay.o compression.o -lz
main.o : main.c
	@gcc -c main.c -lz
splay.o : splay.c
	@gcc -c splay.c
compression.o : compression.c
	@gcc -c compression.c -lz
clean :
	@rm -f *.o
