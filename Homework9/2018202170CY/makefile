main : main.o libavl.so
	cc -o main main.o ./libavl.so
main.o : main.c
	cc -c main.c
avl.o : avl.c
	cc -shared -fPIC -c avl.c
libavl.so : avl.o
	cc -shared -fPIC -o libavl.so avl.o
intl : my_malloc.o main.c ./libavl.so
	gcc -Wall -DLINKTIME -c my_malloc.c
	gcc -Wall -c main.c
	gcc -Wall -Wl,--wrap,malloc -Wl,--wrap,free -o intl main.o my_malloc.o ./libavl.so
runl : 
	./intl
clean :
	rm main main.o avl.o libavl.so my_malloc.o intl
