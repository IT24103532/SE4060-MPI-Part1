// Exercise 5 (part 2): message2.cc rewritten to use Buffered Send (MPI_Bsend).
#include <mpi.h>
#include <cstdio>
#include <cstdlib>

#define NUM_MESSAGES 5

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int message;
    MPI_Status status;

    if (rank == 0) {
        int buf_size = NUM_MESSAGES * (sizeof(int) + MPI_BSEND_OVERHEAD);
        char* buffer = (char*)malloc(buf_size);
        MPI_Buffer_attach(buffer, buf_size);

        for (int i = 0; i < NUM_MESSAGES; i++) {
            message = i * 10;
            MPI_Bsend(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            printf("Rank 0 sent: %d\n", message);
        }

        MPI_Buffer_detach(&buffer, &buf_size);
        free(buffer);
    } else if (rank == 1) {
        for (int i = 0; i < NUM_MESSAGES; i++) {
            MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            printf("Rank 1 received: %d\n", message);
        }
    }

    MPI_Finalize();
    return 0;
}
