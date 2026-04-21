// #include <vector>
#include <iostream>

using namespace std;

double func(int i, int j){
    return i*j*0.3;
}

int main() {
    int rows = 5;
    int cols = 5;

    double matrix1[rows * cols];
    double matrix2[rows * cols];
    double matrix3[rows * cols];
    // vector<double> matrix2(rows * cols);
    // vector<double> matrix3(rows * cols);


    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix1[i*cols + j] = func(i, j);
            matrix2[i*cols + j] = func(i, j);
        }
    }

    #pragma acc data copyin(matrix1[0:rows*cols], matrix2[0:rows*cols]) copyout(matrix3[0:rows*cols])
    {
        #pragma acc parallel loop collapse(2)
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++){
                double sum = 0.0;
                for (int k = 0; k < cols; k++){
                    sum += matrix1[i*cols + k] * matrix2[k*cols + j];
                }
                matrix3[i*cols + j] = sum;
            }
        }
    }

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            cout << matrix3[i*cols + j] << " ";
        }
        cout << endl;
    }

    return 0;
}