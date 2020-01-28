#!/bin/bash 

echo "complie MM code"
gcc -o MM Matrix_Multiplication.c -lm 

mkdir data 

echo "n = 100"
./MM 100 5 >> data1_100.csv

echo "n = 200"
./MM 200 5 >> data1_200.csv

echo "n = 500"
./MM 500 5 >> data1_500.csv

echo "n = 1000"
./MM 1000 5 >> data1_1000.csv

echo "n = 2000"
./MM 2000 5 >> data1_2000.csv
