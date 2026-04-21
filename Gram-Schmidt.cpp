#include <iostream>
#include <cmath>

using namespace std;

void Gram_Schmidt(double A[5][3], double Q[5][3], double R[3][3]) {
    int M = 5;
    int N = 3;

    // Copy A -> Q
    #pragma acc parallel loop collapse(2)
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            Q[i][j] = A[i][j];
        }
    }

    // Initialize R = 0
    #pragma acc parallel loop collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            R[i][j] = 0.0;
        }
    }

    // Modified Gram-Schmidt
    for (int i = 0; i < N; i++) {

        // norm of column i
        double norm = 0.0;
        #pragma acc parallel loop reduction(+:norm)
        for (int k = 0; k < M; k++) {
            norm += Q[k][i] * Q[k][i];
        }
        norm = sqrt(norm);

        R[i][i] = norm;

        // Normalize column i
        #pragma acc parallel loop
        for (int k = 0; k < M; k++) {
            Q[k][i] /= norm;
        }

        // Orthogonalize remaining columns
        for (int j = i + 1; j < N; j++) {

            // Compute dot product
            double dot = 0.0;
            #pragma acc parallel loop reduction(+:dot)
            for (int k = 0; k < M; k++) {
                dot += Q[k][j] * Q[k][i];
            }

            R[i][j] = dot;

            // Subtract projection
            #pragma acc parallel loop
            for (int k = 0; k < M; k++) {
                Q[k][j] = Q[k][j] - (dot * Q[k][i]);
            }
        }
    }
}

int main(){
    double A[5][3] = {

        {1, 1, 1},
        {1, 0, 2},
        {1, 3, 1},
        {1, 2, 0},
        {1, 1, 3}

    };

    double Q[5][3];
    double R[3][3];

    Gram_Schmidt(A, Q, R);

    cout << "Q:\n";

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            cout << Q[i][j] << " ";

        }

        cout << "\n";

    }

    cout << "\nR:\n";

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << R[i][j] << " ";

        }
        cout << "\n";

    }
    return 0;
}