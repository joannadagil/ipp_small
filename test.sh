#!/bin/bash

PROG=$1
DIR=$2

for TEST in $DIR/*.in;
do
    #TEST=${f%.in}

    valgrind --error-exitcode=15 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all #-q\
	  ./${PROG} < ${TEST} > ${TEST%.in}2.out 2> ${TEST%.in}2.err

    DIFF1=$(diff ${TEST%.in}2.out ${TEST%.in}.out)
    DIFF2=$(diff ${TEST%.in}2.err ${TEST%.in}.err)
    
    if [[ $DIFF1 == "" || $DIFF2 == "" ]];
    then
      echo -e "$TEST - ok"
    else
      echo -e "$TEST - wrong"
    fi

    rm ${TEST%.in}2.out ${TEST%.in}2.err
done