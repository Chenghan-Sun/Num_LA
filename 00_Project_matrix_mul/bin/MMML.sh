#!/bin/bash

echo "complie MMML code w/o optimization"
gcc -o MMML Matrix_Multiplication_Modified_Loops.c -lm 

echo "n = 100"
./MMML 100 3 >> data2_100_no_opt.csv 
echo "n = 200"
./MMML 200 3 >> data2_200_no_opt.csv
echo "n = 500"
./MMML 500 3 >> data2_500_no_opt.csv
echo "n = 1000"
./MMML 1000 3 >> data2_1000_no_opt.csv
echo "n = 2000"
./MMML 2000 3 >> data2_2000_no_opt.csv
