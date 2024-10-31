#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
double f(double x) {
    return (x*x*x*x)/(0.5*(x*x)+x+6);
}

double integrate(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        double x = a + i * h + 0.5 * h;
        sum += f(x);
    }

    return h * sum;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 0.4;
    double b = 1.5;
    int n = 1000000;

    double local_result, global_result;


    double start_time = MPI_Wtime();

    local_result = integrate(a + rank * (b - a) / size, a + (rank + 1) * (b - a) / size, n / size);

    double end_time = MPI_Wtime();

    MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Result: %f\n", global_result);
        printf("Time: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();

    return 0;
}

/*
1*1 = 0.033680
1*4 = 0.008354
2*4 = 0.004148
3*4 = 0.002764
4*4 = 0.002078
4*5 = 0.001949
4*6 = 0.001390
4*7 = 0.001187
4*8 = 0.001031
 */


/*
4 = 0.008354
8 = 0.004148
12 = 0.002764
16 = 0.002078
20 = 0.001949
24 = 0.001390
28 = 0.001187
32 = 0.001031
*/