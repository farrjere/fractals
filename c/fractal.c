//
// Created by Jeremy Farrell on 3/3/24.
//
#include <stdio.h>
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define ARRAY_BYTES(arr) (sizeof((arr)[0]) * ARRAY_SIZE(arr))

#include <complex.h>
#include <omp.h>
#include <stdlib.h>

double *mandelbrot(float *xs, float *ys, int height, int width, int maxIter) {
  double *iterations = malloc(height * width * sizeof(double));
#pragma omp parallel shared(iterations) shared(xs) shared(ys) shared(height) shared(width) \
    shared(maxIter)
  {
#pragma omp for collapse(2)
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        double complex c = xs[x] + I * ys[y];
        int n = 0;
        double complex z = 0;
        while (cabs(z) < 2 && n < maxIter) {
          z = z * z + c;
          n += 1;
        }
        if (n == maxIter) {
          iterations[y * width + x] = (double)n;
        } else {
          iterations[y * width + x] = n + 1 - clog(clog(cabs(z)));
        }
      }
    }
  }
  // i 0 j 0 .. 5 iterations[5], i 1 j 0 iterations[8]
  // 0 5 -> i[5] 1 0 -> i[6] 1 5 -> i[11]
  // 0 7 -> i[7] 1 0 -> i[8] 5 0 -> i[40] 5 7 -> i[47]
  return iterations;
}
