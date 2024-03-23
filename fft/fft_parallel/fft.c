#include <stdio.h>
#include <math.h>
#include <mpi.h>

void InitializeSamples(float x_real[], float x_imaginary[], int n);

int main()
{
  int comm_sz, my_rank;
  int n = 1024, local_k, k;
  int dest_process, source;
  float x_real[n], x_imaginary[n];
  double X_real[n], X_imaginary[n];
  double odd_real, odd_imaginary, expo_real, expo_imaginary, angle, sin_value, cos_value;

  InitializeSamples(x_real, x_imaginary, n);

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  local_k = n / comm_sz;

  double even_real[local_k], even_imaginary[local_k], second_half_real[local_k], second_half_imaginary[local_k];
  double local_X_real[local_k], local_X_imaginary[local_k];

  if (my_rank < comm_sz / 2)
  {
    for (int i = 0; i < local_k; i++)
    {
      local_X_real[i] = 0;
      local_X_imaginary[i] = 0;

      even_real[i] = 0;
      odd_real = 0;
      even_imaginary[i] = 0;
      odd_imaginary = 0;

      k = my_rank * local_k + i;

      for (int m = 0; m < (n / 2); m++)
      {
        angle = (4 * M_PI * m * k) / n;

        sin_value = sin(angle);
        cos_value = cos(angle);

        even_real[i] += (x_real[2 * m] * cos_value) + (x_imaginary[2 * m] * (-sin_value) * -1);
        even_imaginary[i] += (x_real[2 * m] * (-sin_value)) + (x_imaginary[2 * m] * cos_value);

        odd_real += (x_real[2 * m + 1] * cos_value) + (x_imaginary[2 * m + 1] * (-sin_value) * -1);
        odd_imaginary += (x_real[2 * m + 1] * (-sin_value)) + (x_imaginary[2 * m + 1] * cos_value);
      }

      expo_real = cos((2 * M_PI * k) / n);
      expo_imaginary = -sin((2 * M_PI * k) / n);

      second_half_real[i] = (expo_real * odd_real) + (expo_imaginary * odd_imaginary * -1);
      second_half_imaginary[i] = (expo_real * odd_imaginary) + (expo_imaginary * odd_real);

      local_X_real[i] = even_real[i] + second_half_real[i];
      local_X_imaginary[i] = even_imaginary[i] + second_half_imaginary[i];
    }

    dest_process = my_rank + (comm_sz / 2);
    MPI_Send(even_real, local_k, MPI_DOUBLE, dest_process, 0, MPI_COMM_WORLD);
    MPI_Send(second_half_real, local_k, MPI_DOUBLE, dest_process, 0, MPI_COMM_WORLD);
    MPI_Send(even_imaginary, local_k, MPI_DOUBLE, dest_process, 0, MPI_COMM_WORLD);
    MPI_Send(second_half_imaginary, local_k, MPI_DOUBLE, dest_process, 0, MPI_COMM_WORLD);
  }
  else
  {
    source = my_rank - (comm_sz / 2);
    MPI_Recv(even_real, local_k, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(second_half_real, local_k, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(even_imaginary, local_k, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(second_half_imaginary, local_k, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (int i = 0; i < local_k; i++)
    {
      local_X_real[i] = even_real[i] - second_half_real[i];
      local_X_imaginary[i] = even_imaginary[i] - second_half_imaginary[i];
    }
  }

  MPI_Gather(local_X_real, local_k, MPI_DOUBLE, X_real, local_k, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Gather(local_X_imaginary, local_k, MPI_DOUBLE, X_imaginary, local_k, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (my_rank == 0)
  {
    for (int i = 0; i < n; i++)
    {
      printf("XR[%d] : %lf \t XI[%d] : %lf\n", i, X_real[i], i, X_imaginary[i]);
    }
  }

  MPI_Finalize();

  return 0;
}

void InitializeSamples(float x_real[], float x_imaginary[], int n)
{
  x_real[0] = 3.6;
  x_imaginary[0] = 2.6;

  x_real[1] = 2.9;
  x_imaginary[1] = 6.3;

  x_real[2] = 5.6;
  x_imaginary[2] = 4;

  x_real[3] = 4.8;
  x_imaginary[3] = 9.1;

  x_real[4] = 3.3;
  x_imaginary[4] = 0.4;

  x_real[5] = 5.9;
  x_imaginary[5] = 4.8;

  x_real[6] = 5;
  x_imaginary[6] = 2.6;

  x_real[7] = 4.3;
  x_imaginary[7] = 4.1;

  for (int i = 8; i < n; i++)
  {
    x_real[i] = 0;
    x_imaginary[i] = 0;
  }
}