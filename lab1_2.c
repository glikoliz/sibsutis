#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int message_size = 1024*1024;
    char *sbuf, *rbuf;
    sbuf = (char*)malloc(message_size);
    rbuf = (char*)malloc(message_size);

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();
    MPI_Bcast(sbuf, message_size, MPI_CHAR, 0, MPI_COMM_WORLD);
    double end_time = MPI_Wtime();
    printf("Broadcast on %d with message size %d B took %f seconds\n", rank, message_size, end_time - start_time);
    free(sbuf);
    free(rbuf);
    MPI_Finalize();
    return 0;
}