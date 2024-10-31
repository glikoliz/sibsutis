#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void ring_exchange(int message_size) {
    int rank, size;
    MPI_Status status;
    char *send_buf, *recv_buf;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    send_buf = (char*)malloc(message_size);
    recv_buf = (char*)malloc(message_size);

    for (int i = 0; i < message_size; i++) {
        send_buf[i] = rank;
    }

    double start_time = MPI_Wtime();

    for (int i = 0; i < size; i++) {
        int next = (rank + 1) % size;
        int prev = (rank - 1 + size) % size;
        printf("%d = %d %d,   size = %d\n", rank, prev, next, size);
        fflush(stdout);

        MPI_Sendrecv(send_buf, message_size, MPI_CHAR, next, 0,
                     recv_buf, message_size, MPI_CHAR, prev, 0,
                     MPI_COMM_WORLD, &status);
    }

    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Ring exchange with message size %d B took %f seconds\n", message_size, end_time - start_time);
    }

    free(send_buf);
    free(recv_buf);
}



void broadcast_exchange(int message_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char *sbuf, *rbuf;
    sbuf = (char*)malloc(message_size);
    rbuf = (char*)malloc(message_size);

    if (rank == 0) {
        for (int i = 0; i < message_size; i++) {
            sbuf[i] = rank;
        }
    }

    double start_time = MPI_Wtime();
    MPI_Bcast(sbuf, message_size, MPI_CHAR, 0, MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Broadcast with message size %d B took %f seconds\n", message_size, end_time - start_time);
    }

    free(sbuf);
    free(rbuf);
}
void gather_exchange(int message_size) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char *sbuf = (char*)malloc(message_size);
    char *rbuf = NULL;

    if (rank == 0) {
        rbuf = (char*)malloc(size * message_size);
    }

    for (int i = 0; i < message_size; i++) {
        sbuf[i] = rank;
    }

    double start_time = MPI_Wtime();
    MPI_Gather(sbuf, message_size, MPI_CHAR, rbuf, message_size, MPI_CHAR, 0, MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Gather with message size %d B took %f seconds\n", message_size, end_time - start_time);
    }

    free(sbuf);
    if (rank == 0) free(rbuf);
}
void all_to_all_exchange(int message_size) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char *sbuf = (char*)malloc(message_size);
    char *rbuf = (char*)malloc(size * message_size);

    for (int i = 0; i < message_size; i++) {
        sbuf[i] = rank;
    }

    double start_time = MPI_Wtime();
    MPI_Alltoall(sbuf, message_size, MPI_CHAR, rbuf, message_size, MPI_CHAR, MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("All-to-all with message size %d B took %f seconds\n", message_size, end_time - start_time);
    }

    free(sbuf);
    free(rbuf);
}
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int message_sizes[] = {1, 1024, 1024 * 1024};

    for (int i = 0; i < 3; i++) {
        ring_exchange(message_sizes[i]);
    }

    // for (int i = 0; i < 2; i++) {
    //     broadcast_exchange(message_sizes[i]);
    // }

    // for (int i = 0; i < 2; i++) {
    //     gather_exchange(message_sizes[i]);
    // }

    // all_to_all_exchange(1024);

    MPI_Finalize();
    return 0;
}
