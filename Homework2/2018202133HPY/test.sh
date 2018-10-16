#! /bin/bash

./gen $RANDOM 100000 > tmp/data.in

time ./double_t_test < tmp/data.in > tmp/sample.out
./c_builtin_double < tmp/data.in > tmp/ans.out

sed -i 's/-nan/nan/g' tmp/sample.out
sed -i 's/-nan/nan/g' tmp/ans.out

echo

echo `wc -l < tmp/ans.out` lines readed / `wc -l < tmp/sample.out` lines expected
echo `diff -b tmp/sample.out tmp/ans.out | grep '>' | wc -l` lines different

if diff -b tmp/sample.out tmp/ans.out ; then
    echo -e '\033[5;32;45mAccepted\033[0m'
else
    echo -e '\033[1;4;31mWrong Answer\033[0m'
fi
