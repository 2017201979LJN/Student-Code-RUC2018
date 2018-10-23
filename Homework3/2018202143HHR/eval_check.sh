#!/bin/bash
gcc *.c -o test -O2 -DCHECK

i=0
OPERATORS="+-*/"
RANDOM_RANGE=200
EXPRESSION_LENGTH=50
BRACKET_PROBABILITY=40
while true; do
	# Generate random expressions.
	data="$(($RANDOM%$RANDOM_RANGE)).0"
	((k=$RANDOM%$EXPRESSION_LENGTH+1))
	brackets_count=0
	for ((j=0;j<k;j=j+1)); do
		if ((RANDOM%100<=BRACKET_PROBABILITY)); then
			if ((RANDOM%2==1)); then
				((brackets_count=brackets_count+1))
				data="$data${OPERATORS:$(($RANDOM%4)):1}($(($RANDOM%$RANDOM_RANGE)).0"
			elif ((brackets_count>0)); then
				((brackets_count=brackets_count-1))
				data="$data${OPERATORS:$(($RANDOM%4)):1}$(($RANDOM%$RANDOM_RANGE)).0)"
			else
				data="$data${OPERATORS:$(($RANDOM%4)):1}$(($RANDOM%$RANDOM_RANGE)).0"
			fi
		else
			data="$data${OPERATORS:$(($RANDOM%4)):1}$(($RANDOM%$RANDOM_RANGE)).0"
		fi
	done
	for ((j=0;j<brackets_count;j=j+1)); do
		data="$data)"
	done

	your_output=$(./test <<< $data)
	correct_output=$(echo -e '#include <stdio.h>\n(void)printf("%.10lf\\n", (double)('$data'))' | cling 2>/dev/null | tail -n 1)

	if [ "$correct_output" = "-nan" ]; then
		correct_output="nan"
	fi

	if ! diff <(echo $your_output) <(echo $correct_output) >/dev/null; then
		echo "Failed after $i cases passed."
		echo "Failed case: $data"
		echo "Your output: $your_output"
		echo "Correct output: $correct_output"
		rm test
		exit 1
	fi

	((i=i+1))
	echo "$i cases passed."
done

rm test
