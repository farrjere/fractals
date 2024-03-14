//
// Created by Jeremy Farrell on 3/3/24.
//
#define ARRAY_SIZE(arr)     (sizeof(arr) / sizeof((arr)[0]))
#define ARRAY_BYTES(arr)        (sizeof((arr)[0]) * ARRAY_SIZE(arr))

#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double * mandelbrot(float *xs, float *ys, int elements, int maxIter) {
    double* iterations = malloc(elements * elements * sizeof(double));
    #pragma omp parallel shared(iterations) shared(xs) shared(ys) shared(elements) shared(maxIter)
    {
        #pragma omp for collapse(2)
        for(int i = 0; i < elements; i++) {
            for(int j = 0; j < elements; j++) {
                double complex c = xs[i] + I * ys[j];
                int n = 0;
                double complex z = 0;
                while(cabs(z) < 2 && n < maxIter) {
                    z = z * z + c;
                    n += 1;
                }
                if(n == maxIter) {
                    iterations[i * elements + j] = (double)n;
                }else {
                    iterations[i * elements + j] = n + 1 - clog(clog(cabs(z)));
                }

            }
        }
    }

    return iterations;
}
