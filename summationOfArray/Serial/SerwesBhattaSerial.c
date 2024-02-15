/*
  Serwes Bhatta
  To compile: icc SerwesBhattaSerial.c -o mycode.exe
  To submit to queue: sbatch fronteraSerialSumScript
*/

#include <stdio.h>
#include <time.h>

#define ARRAY_SIZE 640000

int main()
{
  int Location[ARRAY_SIZE];
  long long sum = 0;
  struct timespec start, end;
  double elapsed_time;

  // Fill the array with numbers
  for (int i = 0; i < ARRAY_SIZE; i++)
  {
    Location[i] = i + 1;
  }

  // Start measuring time
  clock_gettime(CLOCK_MONOTONIC, &start);

  // Calculate the sum of all numbers in the array
  for (int i = 0; i < ARRAY_SIZE; i++)
  {
    sum += Location[i];
  }

  // Stop measuring time
  clock_gettime(CLOCK_MONOTONIC, &end);

  // Calculate elapsed time
  elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

  // Print the sum and elapsed time
  printf("The summation of all numbers is: %lld\n", sum);
  printf("Elapsed time: %f seconds\n", elapsed_time);

  return 0;
}
