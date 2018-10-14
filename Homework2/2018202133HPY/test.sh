#! /bin/bash

./gen $RANDOM 10000 > tmp/data.in

./double_t_test < tmp/data.in > tmp/sample.out
./c_builtin_double < tmp/data.in > tmp/ans.out

sed -i 's/-nan/nan/g' tmp/sample.out
sed -i 's/-nan/nan/g' tmp/ans.out

echo

echo `wc -l < tmp/ans.out` lines readed / `wc -l < tmp/sample.out` lines expected
echo `diff -b tmp/sample.out tmp/ans.out | grep '>' | wc -l` lines different

if diff -b tmp/sample.out tmp/ans.out ; then
    echo -e '\e[5;32;45mAccepted\e[0m'
else
    echo -e '\e[1;4;31mWrong Answer\e[0m'
fi
