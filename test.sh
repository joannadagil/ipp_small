#!/bin/bash

PROG=$1
DIR=$2

for f in ${DIR}/*.in
do
    TEST=${f%.in}

    valgrind --error-exitcode=15 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all -q\
	  ${PROG} <$f >${f%.in}2.out 2>${f%.in}2.err
    
    if diff ${f%.in}2.out ${f%.in}.out || diff ${f%.in}2.err ${f%.in}.err
    then
      echo -e "$TEST - ok"
    else
      echo -e "$TEST - wrong"
    fi

    rm ${f%.in}2.out ${f%.in}2.err
done