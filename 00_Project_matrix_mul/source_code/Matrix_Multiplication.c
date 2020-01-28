//
//  Matrix_Multiplication.c
//  ECS230_HW3_(1)
//
//  Created by Chenghan Sun on 11/11/18.
//  Copyright © 2018 Chenghan Sun. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

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

// main function
int main(int argc, char** argv)
{
    // initialize matrix size
    int n = atoi(argv[1]);
    
    // initialize runs of matrix multiplication
    int t = atoi(argv[2]);
    
    int i,j,k,d; // initialize matrix array indicies
    long long tm; //set timer
    
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
    
    //initialize timer related variables
    double unit_convert = 1.0e9, cpu_speed = 4.0e9;
    
    //repeat A*B t times
    for (d = 1; d <= t; d++)
    {
        //start timing
        tm = readTSC();
        
        //indicating each runs
        printf("run: %i\n", d);
        
        //performing C = A*B
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                for (k = 0; k < n; k++)
                {
                    C[i+n*j] += A[i+n*k]*B[k+n*j];
                }
            }
        }
        
        //update timer
        tm = readTSC() - tm;
        
        // performance indicators
        double Gflops = 2.0*(double)n*(double)n*(double)n / unit_convert; //2*o(n^3)
        double t_real = tm / (cpu_speed);
        double GFlopsPerSec = Gflops / t_real;
        
        //print timing information
        printf("t_cpu: %lld clocks\n", tm);
        printf("floating_point_performance: %15.6e\n", GFlopsPerSec);
        
    }
}
