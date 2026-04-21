#include <stdio.h>

int main(){
    int arr[6] = {5, 4, 3, 6, 6, 3};
    int num = 3;

    int flag = -1;

    #pragma acc parallel loop reduction(max:flag)
    for (int i = 0; i < 6; i++){
        if (arr[i] == num)
        {
            flag = i; 
        }
    }

    if (flag != -1)
        printf("Found at index: %d\n", flag);
    else
        printf("Not found\n");

    return 0;
}