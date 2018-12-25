leak_checker: splay leak_checker.so
	LD_PRELOAD=./leak_checker.so ./splay
splay: splay.so main.c
	gcc -o splay main.c ./splay.so
leak_checker.so: leak_checker.c
	gcc -Wall -DLEAK_CHECKER -shared -fPIC -o leak_checker.so leak_checker.c -ldl
splay.so:
	gcc -shared -fPIC -o splay.so splay.c
clean:
	rm -f splay.so leak_checker.so
