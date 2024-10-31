#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Status status;
    char *send_buf, *recv_buf;
    int message_size;
    if (argc > 1)
    {
        message_size = atoi(argv[1]);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    send_buf = (char *)malloc(message_size);
    recv_buf = (char *)malloc(message_size);

    for (int i = 0; i < message_size; i++)
    {
        send_buf[i] = rank;
    }

    double start_time = MPI_Wtime();

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;
    printf("%d = %d %d\n", rank, prev, next);

    MPI_Sendrecv(send_buf, message_size, MPI_CHAR, next, 0,
                 recv_buf, message_size, MPI_CHAR, prev, 0,
                 MPI_COMM_WORLD, &status);

    double end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;
    double total_time;
    MPI_Reduce(&elapsed_time, &total_time, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Ring exchange with message size %d B took %f seconds\n", message_size, total_time / size);
    }

    free(send_buf);
    free(recv_buf);
    MPI_Finalize();
    return 0;
}

/*
#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4

mpiexec ./lab1
*/
/*
2*8 B = 0.005639
2*8 KB = 0.006149
2*8 MB= 0.008594
4*4 B = 0.007861
4*4 KB = 0.006562
4*4 MB = 0.010215
*/