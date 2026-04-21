/* serial code for Cholesky decomposition */
/* make sure that the init function setups a  */
/* symmetric and positive definite matrix  */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <openacc.h>

#define TYPE		float
#define N		5
#define SMALLVALUE	0.001


void initmult(TYPE mat[][N])
{
  double val;

  #pragma acc data copy(mat)
  {
    // Fill symmetric matrix
    #pragma acc parallel loop collapse(2) gang vector
    for (int ii = 0; ii < N; ++ii){
      for (int jj = 0; jj < ii; ++jj){
        val = ((ii+0.1) + (jj*0.1));
        mat[ii][jj] = val;
        mat[jj][ii] = val;
      }
    }

    // Diagonal with reduction (row-wise)
    #pragma acc parallel loop gang
    for (int ii = 0; ii < N; ++ii){
      double row_sum = 0.0;

      #pragma acc loop vector reduction(+:row_sum)
      for (int jj = 0; jj < N; ++jj){
        if (jj != ii) row_sum += fabs(mat[ii][jj]);
      }

      mat[ii][ii] = row_sum + 1.0;
    }
  }
}		
			

void printLower(TYPE a[][N])
{
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
      if (j <= i)
        printf("%.2f ", a[i][j]);
      else
        printf("0.00 ");
    }
    printf("\n");
  }
}

void cholesky(TYPE a[][N])
{
  #pragma acc data copy(a)
  {
    for (int ii = 0; ii < N; ++ii) {
      // Compute lower triangular elements
      for (int jj = 0; jj < ii; ++jj) {
        TYPE sum = 0.0;
        #pragma acc parallel loop reduction(+:sum)
        for (int kk = 0; kk < jj; ++kk) {
          sum += a[ii][kk] * a[jj][kk];
        }
        a[ii][jj] = (a[ii][jj] - sum) /
                    (a[jj][jj] > SMALLVALUE ? a[jj][jj] : 1);
      }
      // Compute diagonal element
      TYPE diag_sum = 0.0;
      #pragma acc parallel loop reduction(+:diag_sum)
      for (int kk = 0; kk < ii; ++kk) {
        diag_sum += a[ii][kk] * a[ii][kk];
      }
      a[ii][ii] = sqrt(a[ii][ii] - diag_sum);
    }
  }
}



int main()
{
  TYPE a[N][N];

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  initmult(a);
  
  printf("Before:\n");
  printMat(a);

  cholesky(a);

  printf("\nAfter:\n");
  printLower(a);

  clock_gettime(CLOCK_MONOTONIC, &end);
  double time_taken = (end.tv_sec - start.tv_sec) +
                      (end.tv_nsec - start.tv_nsec) / 1e9;
  

  printf("\nResult (Lower Triangular L):\n");

  printf("\n\nExecution time: %f seconds\n", time_taken);

  return 0;
}

