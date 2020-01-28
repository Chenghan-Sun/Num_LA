#!/bin/bash

echo "complie MMML code w/o3 optimization"
gcc -o MMML_o3 Matrix_Multiplication_Modified_Loops.c -lm -O3

echo "n = 100"
./MMML_o3 100 3 >> data2_100_o3_opt.csv
echo "n = 200"
./MMML_o3 200 3 >> data2_200_o3_opt.csv
echo "n = 500"
./MMML_o3 500 3 >> data2_500_o3_opt.csv
echo "n = 1000"
./MMML_o3 1000 3 >> data2_1000_o3_opt.csv
echo "n = 2000"
./MMML_o3 2000 3 >> data2_2000_o3_opt.csv
