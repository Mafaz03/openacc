#include <stdio.h>

int main() {
    int a[10];

    #pragma acc kernels
    {
        printf("One\n");

        for (int i = 0; i < 10; ++i)
        {
            a[i] = i;
            printf("firstloop %d\n", i);
        }

        printf("Two\n");

        for (int i = 0; i < 10; ++i)
        {
            a[i] *= 2;
            printf("secondloop %d\n", i);
        }

        printf("Three\n");
    }

    return 0;
}