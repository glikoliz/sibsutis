#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double f(double x, double y) {
    return x / (y * y);
}

int main(int argc, char *argv[]) {
    int rank, size;
    long long int total_samples = 10000000;
    long long int local_samples;
    long long int i;
    double a_x = 0.0, b_x = 1.0;
    double a_y = 2.0, b_y = 5.0;
    double area = (b_x - a_x) * (b_y - a_y);
    double local_sum = 0.0, global_sum = 0.0;
    double start_time, end_time, total_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        start_time = MPI_Wtime();
    }

    local_samples = total_samples / size;
    if (rank == 0) {
        local_samples += total_samples % size;
    }
    // printf("%d = %lld\n", rank, local_samples);
    unsigned int seed = time(NULL) + rank;

    for (i = 0; i < local_samples; i++) {
        double x = a_x + ((double)rand_r(&seed) / RAND_MAX) * (b_x - a_x);
        double y = a_y + ((double)rand_r(&seed) / RAND_MAX) * (b_y - a_y);
        local_sum += f(x, y);
    }

    double comm_time = MPI_Wtime(); // Начало времени общения
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    comm_time = MPI_Wtime() - comm_time; // Время общения
    if (rank == 0) {
        end_time = MPI_Wtime();
        total_time = end_time - start_time;

        double average = global_sum / total_samples;
        double integral = average * area;
        printf("Приближённое значение интеграла: %lf\n", integral);
        printf("%lf секунд, из них на коммуникацию %lf(%lf%%), процессов: %d\n", total_time, comm_time, (comm_time/total_time)*100, size);
    }

    MPI_Finalize();
    return 0;
}
/*
#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4

mpiexec ./lab2
*/

/*
1 = 0.480420
4 = 0.120141
8 = 0.073394
12 = 0.062262
16 = 0.069940
20 = 0.055537
24 = 0.066139
28 = 0.059058
32 = 0.039939
*/

/*
1 = 4.791998
4 = 1.205938
8 = 0.631860
12 = 0.405665
16 = 0.324204
20 = 0.261053
24 = 0.236736
28 = 0.219813
32 = 0.186085
*/