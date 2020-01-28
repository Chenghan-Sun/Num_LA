#!/bin/bash

echo "complie MMd code"
gcc -o MMd Matrix_Multiplication_dgemm.c -lblas -lm
export OMP_NUM_THREADS=1

echo "n = 100"
./MMd 100 5 >> data3_100.csv

echo "n = 200"
./MMd 200 5 >> data3_200.csv

echo "n = 500"
./MMd 500 5 >> data3_500.csv

echo "n = 1000"
./MMd 1000 5 >> data3_1000.csv

echo "n = 2000"
./MMd 2000 5 >> data3_2000.csv

echo "done"
