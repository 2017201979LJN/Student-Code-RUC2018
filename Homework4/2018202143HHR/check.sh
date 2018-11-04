#!/bin/bash -e

clean() {
    echo -e '\b\bCleaning up...'
    rm prog_origin prog_asm generator compiler
}

trap '{ clean; exit 0; }' INT

echo 'Compiling...'

gcc -O2 -Wno-unused-result generator.c -o generator
gcc -O2 -Wno-unused-result compiler.c -o compiler -DCOMPILABLE

echo 'Successfully compiled!'

RANDOM_RANGE=100000000
MAX_ARGUMENT_COUNT=5
MAX_VARIABLE_COUNT=5

while true; do
    ((CASE_COUNT = CASE_COUNT + 1))

    ARGUMENT_COUNT=$((RANDOM % MAX_ARGUMENT_COUNT + 1))
    VARIABLE_COUNT=$((RANDOM % MAX_VARIABLE_COUNT + 1))

    INPUT_DATA=""
    for ((i = 0; i < ARGUMENT_COUNT; i = i + 1)); do
        INPUT_DATA="$INPUT_DATA$((RANDOM%RANDOM_RANGE)) "
    done

    CODE_ORIGIN=$(./generator $RANDOM_RANGE $VARIABLE_COUNT $ARGUMENT_COUNT)
    CODE_ASM=$(./compiler <<< $CODE_ORIGIN 2>/dev/null)

    gcc -O0 -Wno-unused-result -x c -o prog_origin - <<< $CODE_ORIGIN
    gcc -O0 -Wno-unused-result -x c -o prog_asm - <<< $CODE_ASM

    CORRECT_OUTPUT=$(./prog_origin <<< $INPUT_DATA)
    YOUR_OUTPUT=$(./prog_asm <<< $INPUT_DATA)

    if ! diff <(echo "$CORRECT_OUTPUT") <(echo "$YOUR_OUTPUT"); then
		echo "Failed after $CASE_COUNT cases passed."
		echo "Failed code:"
        echo "$CODE_ORIGIN"
		echo "Failed code:"
        echo "$CODE_ASM"
		echo "Failed data: $INPUT_DATA"
		echo "   Your output: $YOUR_OUTPUT"
		echo "Correct output: $CORRECT_OUTPUT"
        clear
        exit 1
    fi

    echo "$CASE_COUNT cases passed."
done
