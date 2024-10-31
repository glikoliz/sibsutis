#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void initialize_matrix_vector(int rank, int size, int m, int n, float **A, float **B) {
    int rows_per_process = m / size;
    *A = (float *)malloc(rows_per_process * n * sizeof(float));
    *B = (float *)malloc(n * sizeof(float));

    for (int i = 0; i < rows_per_process; i++) {
        for (int j = 0; j < n; j++) {
            (*A)[i * n + j] = (float)(rank + 1);
        }
    }

    for (int i = 0; i < n; i++) {
        (*B)[i] = (float)(i + 1);
    }
}

void sgemv(float *A, float *B, float *C, int rows, int n) {
    for (int i = 0; i < rows; i++) {
        C[i] = 0.0f;
        for (int j = 0; j < n; j++) {
            C[i] += A[i * n + j] * B[j];
        }
    }
}

int main(int argc, char **argv) {
    int rank, size;
    int m = 45000; 
    int n = 45000; 
    float *A, *B, *C;
    int rows_per_process;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    rows_per_process = m / size;

    initialize_matrix_vector(rank, size, m, n, &A, &B);
    
    float *local_C = (float *)malloc(rows_per_process * sizeof(float));
    
    double start_time = MPI_Wtime();
    sgemv(A, B, local_C, rows_per_process, n);
    double computation_time = MPI_Wtime() - start_time;

    double comm_start_time = MPI_Wtime();
    if (rank == 0) {
        C = (float *)malloc(m * sizeof(float));
    }

    MPI_Gather(local_C, rows_per_process, MPI_FLOAT, C, rows_per_process, MPI_FLOAT, 0, MPI_COMM_WORLD);
    double comm_time = MPI_Wtime() - comm_start_time;

    if (rank == 0) {
        // printf("Resulting vector C:\n");
        // for (int i = 0; i < m; i++) {
        //     printf("C[%d] = %f\n", i, C[i]);
        // }
        printf("%lf секунд потрачено, из них на коммуникацию %lf(%lf%%), процессов: %d\n", computation_time, comm_time, (comm_time/computation_time)*100, size);
        // printf("Communication time: %f seconds\n", comm_time);
        free(C);
    }

    free(A);
    free(B);
    free(local_C);
    MPI_Finalize();
    return 0;
}
/*
28000
1 = 6.738144
4 = 1.685675
8 = 0.844211
12 = 0.537023
16 = 0.385216
24 = 0.252119
32 = 0.189511
*/
/*
45000
1 = 17.531664
4 = 4.396717
8 = 2.183380
12 = 1.456602
16 = 1.091867
24 = 0.714507
32 = 0.525867
*/ 