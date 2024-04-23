#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    int total_points = 1000000;
    int points_inside_circle = 0;
    double x, y, pi_estimate;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(rank); // Seed the random number generator

    for (int i = 0; i < total_points / size; i++) {
        x = (double) rand() / RAND_MAX;
        y = (double) rand() / RAND_MAX;
        if (x*x + y*y <= 1.0) {
            points_inside_circle++;
        }
    }

    int total_inside;
    MPI_Reduce(&points_inside_circle, &total_inside, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        pi_estimate = 4.0 * total_inside / total_points;
        printf("Estimated value of Pi: %f\n", pi_estimate);
    }

    MPI_Finalize();

    return 0;
}