#include <stdio.h>
#include <math.h>

void InitializeSamples(float x_real[], float x_imaginary[], int n);

int main()
{
  int n = 1024;
  float x_real[n], x_imaginary[n];

  InitializeSamples(x_real, x_imaginary, n);
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