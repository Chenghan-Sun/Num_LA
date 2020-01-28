//
//  ECS_230_HW4.c
//  
//
//  Created by Chenghan Sun on 11/20/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// loading prototypes of functions needed in this assignment
void dgemm_(char* TRANSA, char* TRANSB, int* M, int* N, int* K, double* ALPHA, double* A,
            int* LDA, double* B, int* LDB, double* BETA, double* C, int* LDC);

void dgemv_(char* TRANS, int* M, int* N, double* ALPHA, double* A, int* LDA, double* X,
            int* INCX, double* BETA, double* Y, int* INCY);

void dpotrf_(char *uplo, int* n, double* a, int* lda, int* info);

void dtrsm_(char* SIDE, char* UPLO, char* TRANSA, char* DIAG, int* M, int* N, double* ALPHA,
            double* A, int* LDA, double* B, int* LDB);

// BLAS and LAPACK inputs (arguments in functions above)
char TRANSA;
char TRANSB;
char TRANS;
double ALPHA;
double BETA;
int M;
int N;
int K;
int LDA;
int LDB;
int LDC;
int INCX;
int INCY;
char SIDE;
char DIAG;

int main(int argc, char** argv)
{
    // read the degree of the polynomial to be fitted from the first
    // argument on the command line
    int d = atoi(argv[1])+1;
    int n; //number of pairs of data points
    FILE *file; // pointer to the file object containing the input data
    double* x = (double*) malloc(10*sizeof(double));
    double* y = (double*) malloc(10*sizeof(double));
    int h = 0;
    
    // read the data points(x,y) from file data.dat
    file = fopen("../data/data.dat", "r");
    fscanf(file, "%i\n", &n);
    while(!feof(file))
    {
        fscanf(file, "%lf %lf\n", x+h, y+h);
        h++;
    }
    fclose(file);
    
    // write all data in a new file
    file = fopen("degree3.dat", "a"); // change writting file name
    fprintf(file, "for n = %i\n", d-1);
    
    // solve normal equation X'Xb = X'Y
    // Compute A = X'X
    double* X = (double*) malloc(n*d*sizeof(double));
    double* A = (double*) malloc(d*d*sizeof(double));
    int i, j;
    fprintf(file, "Matrix X:\n");
    for(j = 0; j < d; j++)
    {
        for(i = 0; i < n; i++)
        {
            X[i+n*j] = pow(x[i], j);
            // print matrix X
            fprintf(file, "%6.4e ", X[i+n*j]);
        }
        fprintf(file, "\n");
    }
    
    // specify values of arguments in dgemm
    TRANSA = 'T';
    TRANSB = 'N';
    ALPHA = 1.0;
    BETA = 0.0;
    M = d;
    N = d;
    K = n;
    LDA = n;
    LDB = n;
    LDC = d;
    // solve X'X
    dgemm_(&TRANSA, &TRANSB, &M, &N, &K, &ALPHA, X, &LDA, X, &LDB, &BETA, A, &LDC);
    
    // print Matrix A
    fprintf(file, "Resulting Matrix of dgemm: A:\n");
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            fprintf(file, "%6.4e", A[i + d*j]);
        }
        fprintf(file, "\n");
    }
    
    // Compute C = X'Y and specify values of arguments in dgemv
    double* C = (double*) malloc(d*sizeof(double));
    char TRANS = 'T';
    M = n;
    INCX = 1;
    INCY = 1;
    dgemv_(&TRANS, &M, &N, &ALPHA, X, &M, y, &INCX, &BETA, C, &INCY);
    
    // print matrix C
    fprintf(file, "Resulting Matrix of dgemv: C:\n");
    for (int m = 0; m < d; m++)
    {
        fprintf(file, "%6.4e", C[m]);
    }
    fprintf(file, "\n");
    
    // Compute cholesky factorization A = LL'
    // specify values of arguments in dpotrf
    char UPLO = 'L';
    int ORD = d;
    int LDD = d;
    int INFO;
    dpotrf_(&UPLO, &ORD, A, &LDD, &INFO);

    // compute matrix L and print L
    fprintf(file, "Resulting Matrix of dpotrf: L:\n");
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            if (i >= j)
            {
                fprintf(file, "%5.3e", A[i+j*d]);
            }
            else
            {
               fprintf(file, "%5.3e ", 0.0);
            }
        }
        fprintf(file, "\n");
    }
    
    // Compute u in Lu = C
    // specify values of arguments in dtrsm
    SIDE = 'L';
    TRANSA = 'N';
    DIAG = 'N';
    M = d;
    N = 1;
    ALPHA = 1.0;
    LDA = d;
    LDB = d;
    dtrsm_(&SIDE, &UPLO, &TRANSA, &DIAG, &M, &N, &ALPHA, A, &LDA, C, &LDB);
    
    // Compute vector b in L'b = u
    // specify values of arguments in dtrsm
    TRANSA = 'T';
    dtrsm_(&SIDE, &UPLO, &TRANSA, &DIAG, &M, &N, &ALPHA, A, &LDA, C, &LDB);
    
    //saving vector b (coefficients of the polynomial fit) to another file
    fprintf(file, "Resulting Matrix of dtrsm: LS Parameters:\n");
    for (int k = 0; k < d; k++)
    {
        fprintf(file, "%lf ", C[k]);
    }
    fprintf(file, "\n");
    fclose(file);
    
}
