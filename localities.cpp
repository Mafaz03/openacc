#include<stdio.h>

# define N 100
# define PROBZERO 33

inline int my_rand(int seed) {
    seed = (1103515245 * seed + 12345);
    return (seed >> 16) & 0x7FFF;
}


int main(){

    int a[N][N];  // citiesX localities
    int x[N];     // sum of infectedpeoplefor all the localitiesof a city
    int y[N];     // numberof localitiesin a city

    #pragma acc parallel
    {
        for (int ii = 0; ii < N; ++ii)
        {
            x[ii] = 0;
            for (int jj = 0; jj < N; ++jj)
            {
                int seed = ii * N + jj;
                int r1 = my_rand(seed);
                int r2 = my_rand(seed + 123);

                a[ii][jj] = r1 % N;
                if (r2 % 100 < PROBZERO) a[ii][jj] = 0;
            }
        }
    }

    #pragma acc parallel collapse(2) reduction
    for (int ii = 0; ii < N; ++ii)
    {
        for (int jj = 0; jj < N; ++jj)
        {
            x[ii] += a[ii][jj];
            if (a[ii][jj] > 0) y[ii]++;
        }
    }

    for (int ii = 0; ii < N; ++ii){
        printf("%.0f ", x[ii] * 100.0 / (y[ii] * N));
        printf("\n");
    }

    return 0;
}