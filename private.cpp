#include <stdio.h>

int main() {
    int x = 10;

    #pragma acc parallel firstprivate(x)
    {
        x = x + 5;
        printf("Thread value: %d\n", x);
    }

    printf("Original x: %d\n", x);

    return 0;
}