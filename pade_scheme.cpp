#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <fstream>


using namespace std;


int main(int argc, char* argv[])
{

    std::ofstream outFile("pade_scheme.dat");

    int points = 15;
    double L = 3.0;
    double h = L / (points - 1);

    vector<vector<double>> A(points, vector<double>(points, 0));
    vector<double> B(points, 0.0);

    A[0][0] = 1;
    A[0][1] = 2;

    A[points - 1][points - 2] = 2;
    A[points - 1][points - 1] = 1;

    double x0 = 0.0;
    double x1 = 1.0 * h;
    double x2 = 2.0 * h;
    double u0 = sin(5.0 * x0);
    double u1 = sin(5.0 * x1);
    double u2 = sin(5.0 * x2);
    B[0] = (1.0 / h) * (-5.0/2.0 * u0 + 2.0 * u1 + 1.0/2.0 * u2);

    double xN   = (points - 1) * h;
    double xNm1 = (points - 2) * h;
    double xNm2 = (points - 3) * h;
    double uN   = sin(5.0 * xN);
    double uNm1 = sin(5.0 * xNm1);
    double uNm2 = sin(5.0 * xNm2);

    B[points - 1] = (1.0 / h) * (5.0/2.0 * uN - 2.0 * uNm1 - 1.0/2.0 * uNm2);
    for (int i = 1; i < points - 1; i++){
        double x_ip = (i + 1) * h;
        double x_im = (i - 1) * h;

        double u_ip = sin(5.0 * x_ip);
        double u_im = sin(5.0 * x_im);

        B[i] = (3.0 / h) * (u_ip - u_im);
    }

    int j = 0;
    for (int i = 1; i < points - 1; i++){
        A[i][j] = 1;
        A[i][j+1] = 4;
        A[i][j+2] = 1;
        j += 1;
    }

    // TDMA solver
    vector<double> a(points, 0.0); // sub-diagonal
    vector<double> b(points, 0.0); // main diagonal
    vector<double> c(points, 0.0); // super-diagonal

    // Extract diagonals from A
    for (int i = 0; i < points; i++) {
        b[i] = A[i][i];
        if (i > 0) a[i] = A[i][i-1];
        if (i < points - 1) c[i] = A[i][i+1];
    }

    // Forward sweep
    for (int i = 1; i < points; i++) {
        double m = a[i] / b[i-1];
        b[i] = b[i] - m * c[i-1];
        B[i] = B[i] - m * B[i-1];
    }

    // Back substitution
    vector<double> x(points, 0.0);
    x[points-1] = B[points-1] / b[points-1];

    for (int i = points - 2; i >= 0; i--) {
        x[i] = (B[i] - c[i] * x[i+1]) / b[i];
    }

    if (outFile.is_open()){
        for (int i = 0; i < points; i++){
            double x_val = i * h;
            printf("%f  |  %f  |  %f\n", x[i], 5.0 * cos(5.0 * x_val), abs(x[i] - 5.0 * cos(5.0 * x_val)));
            outFile << x_val << " "
                    << x[i] << " "
                    << 5.0 * cos(5.0 * x_val) << " "
                    << abs(x[i] - 5.0 * cos(5.0 * x_val))
                    << std::endl;
        }

    }
    printf("saved");
    outFile.close(); 


    return 0;
}