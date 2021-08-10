#!/bin/bash

PROG=$1
DIR=$2

iterator=1;

for f in ${DIR}/*.in
do
    TEST=${f%.in}

    valgrind --error-exitcode=15 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all -q\
	  ${PROG} <$f >${f%.in}2.out 2>${f%.in}2.err
    
    if [diff ${f%.in}2.out ${f%.in}.out || diff ${f%.in}2.err ${f%.in}.err]
    then
      echo -e "$TEST - ok"
    else
      echo -e "$TEST - wrong"
    fi
    
    rm ${f%.in}2.out ${f%.in}2.err
done


<<COMMENT1
if (($# != 2))
then
  # >&2 redirects standard output to standard error
  echo "Usage: $0 <amount of numbers generated> <range size>." >&2
  # exit with error code 1 to indicate an error
  exit 1
elif (($2 < 1))
then
  echo "Range must contain at least one number." >&2
  exit 1
fi

AMOUNT=$1
RANGE=$2

for ((i = 0; i < AMOUNT; ++i))
do
  # RANDOM jest słabym generatorem - daje wartości z przedziału 0 do 32767.
  # Lepszą losowość uzyskamy, korzystając z /dev/urandom. Jeśli potrzebujemy
  # naprawdę dobrej losowości, należy korzystać z /dev/random.

  # echo $((RANDOM % RANGE))
  echo $((`od -vAn -N4 -tu4 < /dev/random` % RANGE))
done
COMMENT1
