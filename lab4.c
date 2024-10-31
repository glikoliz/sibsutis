#define EPS 0.001
#define PI 3.14159265358979323846
#define NELEMS(x) (sizeof((x)) / sizeof((x)[0]))
#define IND(i, j) ((i) * (nx + 2) + (j))
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

int get_block_size(int n, int rank, int nprocs) {
    int s = n / nprocs;
    if (n % nprocs > rank) s++;
    return s;
}

int get_sum_of_prev_blocks(int n, int rank, int nprocs) {
    int rem = n % nprocs;
    return n / nprocs * rank + ((rank >= rem) ? rem : rank);
}

int main(int argc, char *argv[]) {
    int commsize, rank;
    MPI_Init(&argc, &argv);
    double ttotal = -MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm cartcomm;
    int dims[2] = {0, 0}, periodic[2] = {0, 0};
    MPI_Dims_create(commsize, 2, dims);
    int px = dims[0];
    int py = dims[1];

    if (px < 2 || py < 2) {
        fprintf(stderr, "Invalid number of processes\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periodic, 0, &cartcomm);
    int coords[2];
    MPI_Cart_coords(cartcomm, rank, 2, coords);
    int rankx = coords[0];
    int ranky = coords[1];
    int rows, cols;

    if (rank == 0) {
        rows = (argc > 1) ? atoi(argv[1]) : py * 5;
        cols = (argc > 2) ? atoi(argv[2]) : px * 5;
        if (rows < py) {
            fprintf(stderr, "Number of rows %d less then number of py processes %d\n", rows, py);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        if (cols < px) {
            fprintf(stderr, "Number of cols %d less then number of px processes %d\n", cols, px);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        int args[2] = {rows, cols};
        MPI_Bcast(&args, NELEMS(args), MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        int args[2];
        MPI_Bcast(&args, NELEMS(args), MPI_INT, 0, MPI_COMM_WORLD);
        rows = args[0];
        cols = args[1];
    }

    int ny = get_block_size(rows, ranky, py);
    int nx = get_block_size(cols, rankx, px);
    double *local_grid = calloc((ny + 2) * (nx + 2), sizeof(*local_grid));
    double *local_newgrid = calloc((ny + 2) * (nx + 2), sizeof(*local_newgrid));
    double dx = 1.0 / (cols - 1.0);
    int sj = get_sum_of_prev_blocks(cols, rankx, px);

    if (ranky == 0) {
        for (int j = 1; j <= nx; j++) {
            double x = dx * (sj + j - 1);
            int ind = IND(0, j);
            local_newgrid[ind] = local_grid[ind] = sin(PI * x);
        }
    }
    if (ranky == py - 1) {
        for (int j = 1; j <= nx; j++) {
            double x = dx * (sj + j - 1);
            int ind = IND(ny + 1, j);
            local_newgrid[ind] = local_grid[ind] = sin(PI * x) * exp(-PI);
        }
    }

    int left, right, top, bottom;
    MPI_Cart_shift(cartcomm, 0, 1, &left, &right);
    MPI_Cart_shift(cartcomm, 1, 1, &top, &bottom);
    MPI_Datatype col;
    MPI_Type_vector(ny, 1, nx + 2, MPI_DOUBLE, &col);
    MPI_Type_commit(&col);
    MPI_Datatype row;
    MPI_Type_contiguous(nx, MPI_DOUBLE, &row);
    MPI_Type_commit(&row);
    MPI_Request reqs[8];
    double thalo = 0;
    double treduce = 0;
    int niters = 0;

    while(1) {
        niters++;
        for (int i = 1; i <= ny; i++) {
            for (int j = 1; j <= nx; j++) {
                local_newgrid[IND(i, j)] =
                    (local_grid[IND(i - 1, j)] + local_grid[IND(i + 1, j)] +
                     local_grid[IND(i, j - 1)] + local_grid[IND(i, j + 1)]) *
                    0.25;
            }
        }

        double maxdiff = 0;
        for (int i = 1; i <= ny; i++) {
            for (int j = 1; j <= nx; j++) {
                int ind = IND(i, j);
                maxdiff = fmax(maxdiff, fabs(local_grid[ind] - local_newgrid[ind]));
            }
        }

        double *p = local_grid;
        local_grid = local_newgrid;
        local_newgrid = p;
        MPI_Status statuses[8];
        treduce -= MPI_Wtime();
        MPI_Allreduce(MPI_IN_PLACE, &maxdiff, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        treduce += MPI_Wtime();
        if (maxdiff < EPS)
            break;

        thalo -= MPI_Wtime();
        MPI_Irecv(&local_grid[IND(0, 1)], 1, row, top, 0, cartcomm, &reqs[0]);
        MPI_Irecv(&local_grid[IND(ny + 1, 1)], 1, row, bottom, 0, cartcomm, &reqs[1]);
        MPI_Irecv(&local_grid[IND(1, 0)], 1, col, left, 0, cartcomm, &reqs[2]);
        MPI_Irecv(&local_grid[IND(1, nx + 1)], 1, col, right, 0, cartcomm, &reqs[3]);
        MPI_Isend(&local_grid[IND(1, 1)], 1, row, top, 0, cartcomm, &reqs[4]);
        MPI_Isend(&local_grid[IND(ny, 1)], 1, row, bottom, 0, cartcomm, &reqs[5]);
        MPI_Isend(&local_grid[IND(1, 1)], 1, col, left, 0, cartcomm, &reqs[6]);
        MPI_Isend(&local_grid[IND(1, nx)], 1, col, right, 0, cartcomm, &reqs[7]);
        MPI_Waitall(8, reqs, statuses);
        thalo += MPI_Wtime();
    }

    MPI_Type_free(&row);
    MPI_Type_free(&col);
    ttotal += MPI_Wtime();

    // printf("Process %d grid values:\n", rank);
    // for (int i = 1; i <= ny; i++) {
    //     for (int j = 1; j <= nx; j++)
    //         printf("%f ", local_grid[IND(i, j)]);
    //     printf("\n");
    // }

    free(local_newgrid);
    free(local_grid);

    double prof[3] = {ttotal, treduce, thalo};
    if (rank == 0) {
        MPI_Reduce(MPI_IN_PLACE, prof, NELEMS(prof), MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        printf("# procs %d : grid %d %d : niters %d : total time %.6f : mpi time %.6f : allred %.6f : halo %.6f\n",
               commsize, rows, cols, niters, prof[0], prof[1] + prof[2], prof[1], prof[2]);
    } else {
        MPI_Reduce(prof, NULL, NELEMS(prof), MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    return 0;
}

/*
1000
8 = 0.670935  ||  0.012560
16 = 0.423948  ||  0.081855
32 = 0.298602  ||  0.101264
10000
8 = 69.687708  ||  3.249612
16 = 35.627734  ||  2.502366
32 = 18.910215  ||  2.369910
*/