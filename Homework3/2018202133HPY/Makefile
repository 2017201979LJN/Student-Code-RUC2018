all : bin spj

bin : 
	gcc -O2 src/*.c -o double_calculator -lm

spj : 
	gcc -O2 checker/spj.c -o checker/spj.out -lm

clean :
	rm -r double_calculator checker/spj.out tmp/*

test :
	/bin/bash ./test.sh
