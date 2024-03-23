#include <stdio.h>
#include <math.h>

void InitializeSamples(float x_real[], float x_imaginary[], int n);

int main()
{
  int n = 1024, k, index;
  float x_real[n], x_imaginary[n];
  double even_real[n / 2], even_imaginary[n / 2], odd_real, odd_imaginary, expo_real, expo_imaginary, second_half_real[n / 2], second_half_imaginary[n / 2];
  double angle, sin_value, cos_value;
  double X_real[n], X_imaginary[n];

  InitializeSamples(x_real, x_imaginary, n);

  for (k = 0; k < n / 2; k++)
  {
    X_real[k] = 0;
    X_imaginary[k] = 0;

    even_real[k] = 0;
    odd_real = 0;
    even_imaginary[k] = 0;
    odd_imaginary = 0;

    for (int m = 0; m < (n / 2); m++)
    {
      angle = (4 * M_PI * m * k) / n;

      sin_value = sin(angle);
      cos_value = cos(angle);

      even_real[k] += (x_real[2 * m] * cos_value) + (x_imaginary[2 * m] * (-sin_value) * -1);
      even_imaginary[k] += (x_real[2 * m] * (-sin_value)) + (x_imaginary[2 * m] * cos_value);

      odd_real += (x_real[2 * m + 1] * cos_value) + (x_imaginary[2 * m + 1] * (-sin_value) * -1);
      odd_imaginary += (x_real[2 * m + 1] * (-sin_value)) + (x_imaginary[2 * m + 1] * cos_value);
    }

    expo_real = cos((2 * M_PI * k) / n);
    expo_imaginary = -sin((2 * M_PI * k) / n);

    second_half_real[k] = (expo_real * odd_real) + (expo_imaginary * odd_imaginary * -1);
    second_half_imaginary[k] = (expo_real * odd_imaginary) + (expo_imaginary * odd_real);

    X_real[k] = even_real[k] + second_half_real[k];
    X_imaginary[k] = even_imaginary[k] + second_half_imaginary[k];
  }

  for (k = n / 2; k < n; k++)
  {
    index = k % (n / 2);
    X_real[k] = even_real[index] - second_half_real[index];
    X_imaginary[k] = even_imaginary[index] - second_half_imaginary[index];
  }

  for (int i = 0; i < n; i++)
  {
    printf("XR[%d] : %lf \t XI[%d] : %lf\n", i, X_real[i], i, X_imaginary[i]);
  }

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