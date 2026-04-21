#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int ngangs= 1;

    if (argc == 2)
        ngangs= atoi(argv[1]);
    else
    {
        printf(" Run the programas ./a.out 10");
        return 1;
    }

    printf("\n ngangs= %d", ngangs);

    #pragma acc parallel num_gangs(ngangs)
    {
        printf(" Helloworld \n");
        printf(" Bye world \n");
    }

    printf("Host \n");

    #pragma acc parallel num_gangs(ngangs/2)
    printf("Second pragma\n");

    return 0;
}