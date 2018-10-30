#! /bin/bash

for i in {1..100} ; do

    node checker/generator.js > tmp/input 2> tmp/output
    ./double_calculator < tmp/input >> tmp/output
    if ./checker/spj.out < tmp/output ; then
        :
    else
        echo -e '\033[1;4;31mWrong Answer\033[0m'
        exit
    fi
    echo "$i done"
done

echo -e '\033[5;32;45mAccepted\033[0m'
