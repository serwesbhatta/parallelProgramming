#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void get_input(int my_rank, double *left_endpt, double *right_endpt, int *n_p);

double trap(double left_endpt, double right_endpt, int trap_count, double base_len);

double f(double x);

int main()
{
  int my_rank, comm_sz;
  int n, local_n;
  double a, b, local_a, local_b, h;
  double local_area = 0, total_area = 0;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  get_input(my_rank, &a, &b, &n);

  h = (b - a) / n;
  local_n = n / comm_sz;

  local_a = a + my_rank * local_n * h;
  local_b = local_a + local_n * h;
  local_area = trap(local_a, local_b, local_n, h);

  MPI_Reduce(&local_area, &total_area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (my_rank == 0)
  {
    printf("The total estimated area is %.15e\n", total_area);
  }

  MPI_Finalize();

  return 0;
}

void get_input(int my_rank, double *left_endpt, double *right_endpt, int *n_p)
{
  FILE *file;
  if (my_rank == 0)
  {
    file = fopen("input.txt", "r");
    if (file == NULL)
    {
      fprintf(stderr, "Error opening file\n");
      exit(1);
    }
    fscanf(file, "%lf %lf %d", left_endpt, right_endpt, n_p);
    fclose(file);
  }
  MPI_Bcast(left_endpt, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(right_endpt, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(n_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

double trap(double left_endpt, double right_endpt, int trap_count, double base_len)
{
  double approx, x;

  approx = (f(left_endpt) + f(right_endpt)) / 2.0;

  for (int i = 1; i <= trap_count - 1; i++)
  {
    x = left_endpt + i * base_len;
    approx += f(x);
  }
  approx *= base_len;

  return approx;
}

double f(double x)
{
  return x * x;
}