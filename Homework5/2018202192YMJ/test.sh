gcc dmk.c -O2 -o dmk
gcc my_qsort.c -O2 -o my_qsort
while true; do
	./dmk
	./my_qsort
	if diff my_qsort.out qsort.out; then
		printf "AC "
	else
		printf "WA\n"
		exit 0
	fi
done
