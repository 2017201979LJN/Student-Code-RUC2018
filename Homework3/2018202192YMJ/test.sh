#!/bin/bach
gcc evaluation.c -o evaluation
gcc dmk.c -o dmk
gcc trans.c -o trans
while true; do
	./dmk
	./evaluation
	./trans
	gcc calc.c -o calc
	./calc
	if diff data.out data.ans; then
		printf "AC "
	else printf "WA\n"
		exit 0
	fi
done
