/*
  Serwes Bhatta
  To compile: mpicc SerwesBhattaParallelV1.c -o mycode.exe
  To submit to queue: sbatch fronteraParallelSumScriptV1
*/

#include <stdio.h>
#include <mpi.h>

#define ARRAY_SIZE 640000

int main()
{
  int my_rank, comm_sz;
  long long total_sum, local_sum = 0;
  int Location[ARRAY_SIZE];
  int i, chunk_size;
  double start_time, end_time;

  /* Let the system do what it needs to start up MPI */
  MPI_Init(NULL, NULL);

  /* Get my process rank */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /* Find out how many processes are being used */
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  // Determine the size of array that will be assigned to each process
  chunk_size = ARRAY_SIZE / comm_sz;

  if (my_rank == 0)
  {
    // Initialize Summation Array
    long long Summation[comm_sz];

    // Fill the array with numbers
    for (i = 0; i < ARRAY_SIZE; i++)
    {
      Location[i] = i + 1;
    }

    // Record the start time
    start_time = MPI_Wtime();

    // Distribute different array with size of chunk_size to all other processes
    for (i = 1; i < comm_sz; i++)
    {
      int start = i * chunk_size;

      MPI_Send(&Location[start], chunk_size, MPI_INT, i, 0, MPI_COMM_WORLD);
    }

    // Calculate process 0 local sum
    for (i = 0; i < chunk_size; i++)
    {
      local_sum += Location[i];
    }

    // Assign the process 0 local sum to Summation array
    Summation[0] = local_sum;

    // Receive the summation calculated by different processes
    for (i = 1; i < comm_sz; i++)
    {
      MPI_Recv(&Summation[i], 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Calculate the total_sum from the Summation array
    total_sum = 0;
    for (i = 0; i < comm_sz; i++)
    {
      total_sum += Summation[i];
    }

    // Record the end time
    end_time = MPI_Wtime();

    // Print the Sum
    printf("The summation of all number is: %lld\n", total_sum);

    // Print the time taken
    printf("Elapsed time: %f seconds\n", end_time - start_time);
  }
  else
  {
    int temp[chunk_size];

    local_sum = 0;

    // Receive the chunks of array for each process
    MPI_Recv(temp, chunk_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Calculate the sum of the chunk of array received and send the sum to process 0
    for (i = 0; i < chunk_size; i++)
    {
      local_sum += temp[i];
    }
    MPI_Send(&local_sum, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return 0;
}