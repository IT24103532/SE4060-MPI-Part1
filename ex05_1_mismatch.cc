// Exercise 5 (part 1): deliberately mismatch source/destination.
// Run with 3 ranks: mpirun -n 3 ./ex5_mismatch
#include <mpi.h>
#include <cstdio>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int message;
    MPI_Status status;

    if (rank == 0) {
        message = 42;
        printf("Rank 0: sending %d to rank 1...\n", message); fflush(stdout);
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Rank 0: MPI_Send returned.\n"); fflush(stdout);
    } else if (rank == 1) {
        printf("Rank 1: waiting for a message FROM RANK 2 (mismatch - rank 0 is the real sender)\n"); fflush(stdout);
        MPI_Recv(&message, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, &status); 
        printf("Rank 1: received %d (this line should never print)\n", message); fflush(stdout);
    } else if (rank == 2) {
        printf("Rank 2: has nothing to send, does nothing.\n"); fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}