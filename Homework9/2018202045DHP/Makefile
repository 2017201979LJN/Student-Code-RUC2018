exec : main.c RBTree.so mymalloc.o
	cc -o exec main.c mymalloc.o ./RBTree.so
mymalloc.o : mymalloc.c malloc.h
	cc -Wall -DCOMPILETIME -c mymalloc.c
RBTree.so : RBTree.c RBTree.h
	cc -Wall -I. -shared -fPIC -o RBTree.so RBTree.c
clean : 
	rm mymalloc.o RBTree.so exec
