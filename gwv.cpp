#include <stdio.h>

int main() {
    int arr[8] = {1,2,3,4,5,6,7,8};

    #pragma acc parallel loop num_gangs(2) num_workers(2) vector_length(2)
    for (int i = 0; i < 8; i++) {
        arr[i] = arr[i] * 2;
    }

    for (int i = 0; i < 8; i++) {
        printf("%d ", arr[i]);
    }

    return 0;
}