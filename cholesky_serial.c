/* serial code for Cholesky decomposition */
/* make sure that the init function setups a  */
/* symmetric and positive definite matrix  */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define TYPE		float
#define N		5
#define SMALLVALUE	0.001


void initmult(TYPE mat[][N])
{
  double val;
  // Fill symmetric matrix
  for (int ii = 0; ii < N; ++ii){
    for (int jj = 0; jj < ii; ++jj){
      val = ((ii+0.1) + (jj*0.1));
      mat[ii][jj] = mat[jj][ii] = val;
    }
  }
  
  // diagnol
  for (int ii = 0; ii < N; ++ii){
    double row_sum = 0.0;
    for (int jj = 0; jj < N; ++jj){
      if (jj != ii) row_sum += fabs(mat[ii][jj]);
    }
    mat[ii][ii] = row_sum + 1.0;
  }
}		
			
void printMat(TYPE a[][N])
{
  for (int ii = 0; ii < N; ++ii)
    {
      for (int jj = 0; jj < N; ++jj)
	printf("%.2f ", a[ii][jj]);
      printf("\n");
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
  for (int ii = 0; ii < N; ++ii) {
    for (int jj = 0; jj < ii; ++jj) {
      for (int kk = 0; kk < jj; ++kk)
	a[ii][jj] += -a[ii][kk] * a[jj][kk];
      a[ii][jj] /= (a[jj][jj] > SMALLVALUE ? a[jj][jj] : 1);
    }
    for (int kk = 0; kk < ii; ++kk)
      a[ii][ii] += -a[ii][kk] * a[ii][kk];
    a[ii][ii] = sqrt(a[ii][ii]);
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
