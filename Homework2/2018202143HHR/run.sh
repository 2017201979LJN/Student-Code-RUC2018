#!/bin/bash -e

DATA_COUNT=10000

mkdir -p build
gcc -Wno-unused-result generator.c -o build/generator -O2
gcc -Wno-unused-result double.c double_util.c int128.c util.c main.c -o build/double -O2
gcc -Wno-unused-result double_sys.c -o build/double_sys -O2

mkdir -p /tmp/double_Menci

build/generator <<< "$DATA_COUNT" > /tmp/double_Menci/data.in
build/double < /tmp/double_Menci/data.in > /tmp/double_Menci/data.out
build/double_sys < /tmp/double_Menci/data.in > /tmp/double_Menci/data_sys.out
diff /tmp/data.out /tmp/double_Menci/data_sys.out | grep '^>' | wc -l

rm -r /tmp/double_Menci
rm -r build