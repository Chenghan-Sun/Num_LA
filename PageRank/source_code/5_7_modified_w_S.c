#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// initialize convergence criterion function
int residual_vector(double*xk1, double*xk2, int n)
{
    double norm_xk1, norm_xk2;
    
    for (int i = 0; i < n; i++)
    {
        norm_xk1 = norm_xk1 + xk1[i]*xk1[i];
        norm_xk2 = norm_xk2 + xk2[i]*xk2[i];
    }
    
    norm_xk1 = sqrt(norm_xk1);
    norm_xk2 = sqrt(norm_xk2);
    
    int i = 0;
    while (i < n)
    {
        if (fabs(xk2[i]/norm_xk2 - xk1[i]/norm_xk1) > 1.0e-6)
        {
            return 0;
        }
        i++;
    }
    return 1;
}

// initialize function prototype
void dgemv_(char* TRANS, int* M, int* N, double* ALPHA, double* A, int* LDA, double* X,
            int* INCX, double* BETA, double* Y, int* INCY);

//main function
int main (int argc, char** argv)
{
    FILE* file;
    int n, i, j, k, x;
    double *A;
    
    file = fopen("../data/data_5_7_n_20.dat", "r");
    fscanf(file, "%d\n", &n);
    
    A = (double*) malloc(n*n*sizeof(double));
    for (k = 0; k < n*n; k++)
    {
        A[k] = 0;
    }
    j = 0;
    while(!feof(file))
    {
        fscanf(file, "%d ", &x);
        i = 0;
        k = 0;
        while (k < x)
        {
            fscanf(file, "%d ", &i);
            A[i-1+n*j] = 1.0/(double)x;
            k++;
        }
        j++;
    }
    
    // initialize shift (details shown in report)
    double shift = -0.25;
    for (i = 0, j = 0; i < n; i++, j++)
    {
        A[i+n*j] = A[i+n*j]-shift;
    }
    fclose(file);
    // print link matrix A
    printf("Shifted Matrix A-σI:\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("%lf ", A[i+n*j]);
        }
        printf("\n");
    }
    
    //power method
    // eigenvalue vectors using initial vector [1,2,3,4]
    double* xk3 = (double*) malloc(n*sizeof(double));
    double* xk4 = (double*) malloc(n*sizeof(double));
    
    // initialize vector [1,2,3,4]
    for (i = 0; i < n; i++)
    {
        xk3[i] = i+1;
    }
    
    // function dgemv inputs
    char TRANS = 'N';
    int INCX = 1;
    int INCY = 1;
    double alpha = 1.0;
    double beta = 0.0;
    
    // power iterations with convergence criterion
    int iter = 0;
    int indicator = 0;
    while (indicator == 0)
    {
        dgemv_(&TRANS, &n, &n, &alpha, A, &n, xk3, &INCX, &beta, xk4, &INCY);
        indicator = residual_vector(xk3, xk4, n);
        int i = 0;
        while (i < n)
        {
            xk3[i] = xk4[i];
            i++;
        }
        iter++;
    }
    printf("Number of iterations: %d\n",iter-1);
    // normalize eigenvector
    double xk3_sum = 0;
    for (i = 0; i < n; i++)
    {
        xk3_sum = xk3_sum + fabs(xk3[i]);
    }
    
    printf("eigenvector of shifted A using initial vector [1,2,3,4]:\n");
    for (i = 0; i < n; i++)
    {
        xk3[i] = xk3[i]/xk3_sum;
        printf("%lf\n", xk3[i]);
    }
    
    // Modified link matrix A with S (M = (1 − m)A + mS)
    // S denote an n × n matrix with all entries 1/n
    double m = 0.15;
    
    // re-define matrix A
    for (i = 0, j = 0; i < n; i++, j++)
    {
        A[i+n*j] = A[i+n*j]+shift;
    }
    
    printf("Matrix M (modified matrix A with S): \n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            A[i+n*j] = A[i+n*j]*(1 - m) + m/n;
            printf("%lf ", A[i+n*j]);
        }
        printf("\n");
    }
    
    // re-initialize vector [1,2,3,4]
    for (i = 0; i < n; i++)
    {
        xk3[i] = i+1;
    }
    
    // re-use power iterations with convergence criterion
    iter = 0;
    indicator = 0;
    while (indicator == 0)
    {
        dgemv_(&TRANS, &n, &n, &alpha, A, &n, xk3, &INCX, &beta, xk4, &INCY);
        indicator = residual_vector(xk3, xk4, n);
        int i = 0;
        while (i < n)
        {
            xk3[i] = xk4[i];
            i++;
        }
        iter++;
    }
    printf("Number of iterations: %d\n",iter-1);
    // normalize eigenvector
    xk3_sum = 0;
    for (i = 0; i < n; i++)
    {
        xk3_sum = xk3_sum + fabs(xk3[i]);
    }
    
    printf("eigenvector of shifted and modified A using initial vector [1,2,3,4]:\n");
    for (i = 0; i < n; i++)
    {
        xk3[i] = xk3[i]/xk3_sum;
        printf("%lf\n", xk3[i]);
    }
}
    
    
    

