#define RPNG_IMPLEMENTATION
#define RPNG_DEFLATE_IMPLEMENTATION
#include "fractal.h"
#include "rpng.h"
#include <math.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  int width = 600;
  int height = 600;
  int maxIter = 1000;
  if (argc >= 2) {
    width = atoi(argv[1]);
    height = atoi(argv[1]);
  }
  if (argc == 3) {
    maxIter = atoi(argv[2]);
  }

  long double xs[width];
  long double ys[height];
  long double step = 3.0 / width;
  long double p0 = -1.5;
  for (int i = 0; i < width; i += 1) {
    xs[i] = p0 + step * i;
    ys[i] = p0 + step * i;
  }
  double *out = mandelbrot(xs, ys, height, width, maxIter);

  char *data = RPNG_MALLOC(width * height * 3);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      double iterations = out[x * width + y];
      if ((int)iterations == maxIter) {
        data[(y * width + x) * 3] = 0;
        data[(y * width + x) * 3 + 1] = 0;
        data[(y * width + x) * 3 + 2] = 0;
      } else {
        data[(y * width + x) * 3] = (int)floor((float)iterations / (float)maxIter * 255);
        data[(y * width + x) * 3 + 1] = (int)floor((log(iterations) + 1.0 - (float)iterations / (float)maxIter) / 2.0 * 255);
        data[(y * width + x) * 3 + 2] = (int)floorl((log(iterations) + (1.0 - (float)iterations / (float)maxIter)) * 255);
      }
    }
  }

  rpng_save_image("mandelbrot.png", data, width, height, 3, 8);
  free(data);

  return 0;
}
