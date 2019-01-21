.PHONY : clean

main : main.o splay.o compression.o
	@gcc data_gen.c -o data_gen
	./data_gen
	rm ./data_gen
	bash ./res/load_libs.sh
	@gcc -g -o run main.o splay.o compression.o ./libz.a
	# @gcc -g -o run main.o splay.o compression.o -lz
main.o : main.c
	@gcc -g -c main.c
splay.o : splay.c
	@gcc -g -c splay.c
compression.o : compression.c
	@gcc -g -c compression.c
clean :
	@rm -f *.o
	@rm -f libz.a libz.so data.in run
