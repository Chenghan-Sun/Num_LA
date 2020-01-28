//
//  Matrix_Multiplication_dgemm.c
//  ECS230_HW3_(c)
//
//  Created by Chenghan Sun on 11/11/18.
//  Copyright © 2018 Chenghan Sun. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "sys/time.h"
#include "math.h"

volatile double gtod(void)
{
    static struct timeval tv;
    static struct timezone tz;
    gettimeofday(&tv,&tz);
    return tv.tv_sec + 1.e-6*tv.tv_usec;
}

long long readTSC(void)
{
    // read the time stamp counter on Intel x86 chips
    // from timing2.c provided
    union { long long complete; unsigned int part[2]; } ticks;
    __asm__ ("rdtsc; mov %%eax,%0;mov %%edx,%1"
             : "=mr" (ticks.part[0]),
             "=mr" (ticks.part[1])
             : /* no inputs */
             : "eax", "edx");
    return ticks.complete;
}

//initialize dgemm fcn
void dgemm_(char* TRANSA, char* TRANSB, int* M, int* N, int* K, double* ALPHA, double* A,
            int* LDA, double* B, int* LDB, double* BETA, double* C, int* LDC);

// main function
int main(int argc, char** argv)
{
    // initialize matrix size
    int n = atoi(argv[1]);
    
    // initialize runs of matrix multiplication
    int t = atoi(argv[2]);
    int i,j,d; // initialize matrix array indicies
    long long tm; //set timer
    double tod, t_cpu, t_real;
    
    // dynamic memory allocation for the matrices
    double* A = (double*) malloc((n*n)*sizeof(double));
    double* B = (double*) malloc((n*n)*sizeof(double));
    double* C = (double*) malloc((n*n)*sizeof(double));
    
    // make arbitrary full matrices A and B with 1.0's
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            A[i+j*n] = 1.0;
            B[i+j*n] = 1.0;
        }
    }
    
    // define dgemm function arguments
    double alpha = 1.0;
    double beta = 0.0;
    char TRANSA = 'N';
    char TRANSB = 'N';
    
    //initialize timer related variables
    double unit_convert = 1.0e9, cpu_speed = 4.0e9;
    
    //using dgemm fcn
    for (d = 1; d <= t; d++)
    {
        //start timing
        tm = readTSC();
        tod = gtod();
        
        //indicating each runs
        printf("run: %i\n", d);
        
        dgemm_(&TRANSA, &TRANSB, &n, &n, &n, &alpha, A, &n, B, &n, &beta, C, &n);
        
        //update timer
        tm = readTSC() - tm;
        t_real = gtod() - tod;
        
        //print timing information
        printf("%lld\n", tm);
        printf("%f\n", t_real);
    }
}
