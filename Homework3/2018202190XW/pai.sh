#!/bin/bash 
gcc -o dmk dmk.c
gcc -o a double_calc.c
gcc -o check check.c
while true; do
	./dmk > data.in
	./check > tmp.c
	gcc -o tmp tmp.c 2> compile
	if [ -s "compile" ]; then
		continue
	fi
	./tmp > c.out
	./a > my.out
	if diff my.out c.out; then 
		printf "AC " 
	else printf "Wa\n" 
		exit 0 
	fi 
done
