#! /bin/bash
./gen > tmp/test.in
./main zip tmp/test.in tmp/naive.out > tmp/zipoutput
./main unzip tmp/naive.out > tmp/unzipoutput

if diff tmp/zipoutput tmp/unzipoutput ; then
    echo Accepted;
else
    echo WA;
fi
