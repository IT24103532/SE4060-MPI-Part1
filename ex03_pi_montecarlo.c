// Exercise 3: Estimate Pi with the Monte Carlo method using MPI (explicit Send/Recv)
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

#define TOTAL_POINTS 10000000LL
#define PI_TAG 2

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long local_points = TOTAL_POINTS / size;
    if (rank == size - 1) local_points += TOTAL_POINTS % size; 

    
    unsigned int seed = (unsigned int)time(NULL) + (unsigned int)rank * 7919u;
    srand(seed);

    double t_start = MPI_Wtime();

    long long local_hits = 0;
    for (long long i = 0; i < local_points; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) local_hits++;
    }

    if (rank == 0) {
        long long total_hits = local_hits;
        long long recv_val;
        MPI_Status status;
        for (int src = 1; src < size; src++) {
            MPI_Recv(&recv_val, 1, MPI_LONG_LONG, src, PI_TAG, MPI_COMM_WORLD, &status);
            total_hits += recv_val;
        }
        double pi_estimate = 4.0 * (double)total_hits / (double)TOTAL_POINTS;
        double t_end = MPI_Wtime();
        printf("Estimated Pi = %f\n", pi_estimate);
        printf("Processes: %d  Time: %f seconds\n", size, t_end - t_start);
    } else {
        MPI_Send(&local_hits, 1, MPI_LONG_LONG, 0, PI_TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
