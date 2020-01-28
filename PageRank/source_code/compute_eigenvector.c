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
    
    file = fopen("../data/data_5_3.dat", "r");
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
    fclose(file);
    
    // print link matrix A
    printf("Matrix A:\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("%lf ", A[i+n*j]);
        }
        printf("\n");
    }
    
    //power method
    double* xk1 = (double*) malloc(n*sizeof(double));
    double* xk2 = (double*) malloc(n*sizeof(double));
    
    int indicator = 0;
    for (i = 0; i < n; i++)
    {
        xk1[i] = 1;
    }
    
    // function dgemv inputs
    char TRANS = 'N';
    int INCX = 1;
    int INCY = 1;
    double alpha = 1.0;
    double beta = 0.0;
    
    // power iterations with convergence criterion
    while (indicator == 0)
    {
        dgemv_(&TRANS, &n, &n, &alpha, A, &n, xk1, &INCX, &beta, xk2, &INCY);
        indicator = residual_vector(xk1, xk2, n);
        int i = 0;
        while (i < n)
        {
            xk1[i] = xk2[i];
            i++;
        }
    }
    
    // normalize eigenvector
    double xk1_sum = 0;
    for (i = 0; i < n; i++)
    {
        xk1_sum = xk1_sum + xk1[i];
    }
    
    printf("Result:\n");
    for (i = 0; i < n; i++)
    {
        xk1[i] = xk1[i]/xk1_sum;
        printf("%lf\n", xk1[i]);
    }
}
