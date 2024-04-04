/*
  In Linux:
  Code needed to compile: mpicc point_to_point_practice.c -o mycode.exe
  Code needed to run the program: mpiexec -n 4 ./mycode.exe
*/

#include <mpi.h>
#include <stdio.h>

int main()
{
  int my_rank, comm_size;
  int temp;
  long long total_sum;

  MPI_Init(NULL, NULL);                      /* starts MPI */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);   /* get current process id */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size); /* get number of processes */

  if (my_rank == 0)
  {
    total_sum = 191;

    MPI_Send(&total_sum, 1, MPI_LONG_LONG, my_rank + 1, 0, MPI_COMM_WORLD);
    MPI_Recv(&total_sum, 1, MPI_LONG_LONG, comm_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("The total sum is %lld\n", total_sum);
  }

  else if (my_rank == comm_size - 1)
  {
    temp = my_rank * 111;

    MPI_Recv(&total_sum, 1, MPI_LONG_LONG, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    total_sum += temp;

    MPI_Send(&total_sum, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
  }

  else
  {
    temp = my_rank * 111;

    MPI_Recv(&total_sum, 1, MPI_LONG_LONG, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    total_sum += temp;

    MPI_Send(&total_sum, 1, MPI_LONG_LONG, my_rank + 1, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}
