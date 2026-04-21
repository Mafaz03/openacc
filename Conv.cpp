#include <iostream>
#include <cmath>
#include <openacc.h>

using namespace std;

int main(){
    int N = 8;
    int k = 5;

    int F[5] = {3, 4, 5, 4, 3};
    int A[8];
    int B[8] = {0};  

    // Input: 0 1 2 3 4 5 6 7
    for (int i = 0; i < N; i++){
        A[i] = i;
    }

    int passes = 4;   

    #pragma acc data copyin(A[0:N], F[0:k]) copyout(B[0:N])
    {
        #pragma acc parallel loop
        for (int i = 0; i < passes; i++){
            int sum = 0;

            #pragma acc loop reduction(+:sum)
            for (int j = 0; j < k; j++){
                sum += A[i + j] * F[j];  
            }

            B[i] = sum;
        }
    }

    for (int i = 0; i < N; i++){
        printf("%d ", B[i]);
    }
}
