#!/bin/bash

echo "complie MMML code w/o2 optimization"
gcc -o MMML_o2 Matrix_Multiplication_Modified_Loops.c -lm -O2

echo "n = 100"
./MMML_o2 100 3 >> data2_100_o2_opt.csv
echo "n = 200"
./MMML_o2 200 3 >> data2_200_o2_opt.csv
echo "n = 500"
./MMML_o2 500 3 >> data2_500_o2_opt.csv
echo "n = 1000"
./MMML_o2 1000 3 >> data2_1000_o2_opt.csv
echo "n = 2000"
./MMML_o2 2000 3 >> data2_2000_o2_opt.csv
