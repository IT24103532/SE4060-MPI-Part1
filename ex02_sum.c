// Exercise 2: Parallel sum of 1 to 10,000,000 using MPI (explicit Send/Recv)
#include <cstdio>
#include <cstdlib>
#include <mpi.h>

#define N 10000000LL   
#define SUM_TAG 1

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Split [1, N] into `size` contiguous chunks, one per process.
    long long chunk  = N / size;
    long long start  = rank * chunk + 1;
    long long end    = (rank == size - 1) ? N : (rank + 1) * chunk; 

    double t_start = MPI_Wtime();

    long long local_sum = 0;
    for (long long i = start; i <= end; i++) {
        local_sum += i;
    }

    if (rank == 0) {
        long long total_sum = local_sum;  
        long long recv_val;
        MPI_Status status;
        for (int src = 1; src < size; src++) {
            MPI_Recv(&recv_val, 1, MPI_LONG_LONG, src, SUM_TAG, MPI_COMM_WORLD, &status);
            total_sum += recv_val;
        }
        double t_end = MPI_Wtime();
        printf("Sum 1..%lld = %lld\n", (long long)N, total_sum);
        printf("Processes: %d  Time: %f seconds\n", size, t_end - t_start);
    } else {
        MPI_Send(&local_sum, 1, MPI_LONG_LONG, 0, SUM_TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}