#include "fractal.h"
#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

// If we are at 0, 0 then we want to fit height x width points in the space of
// -1.5 to 1.5 that is a real width / height of 3 When we zoom the center
// changes but also the real width / height 1x -> 2x should cut the width in
// half centered about the new x, y
// at a center of given a height and width of 600x800 0,0 is at 600/2 800/2 =
// 300, 400
double *setupMandelbrot(int winHeight, int winWidth, int maxIter, float centerX,
                        float centerY, float frameWidth, float frameHeight) {
  float xs[winWidth];
  float ys[winHeight];
  float step_x = frameWidth / winWidth;
  float step_y = frameHeight / winHeight;
  float x0 = centerX - frameWidth / 2.0;
  float y0 = centerY - frameHeight / 2.0;
  for (int i = 0; i < winWidth; i += 1) {
    xs[i] = x0 + step_x * i;
  }
  for (int j = 0; j < winHeight; j++) {
    ys[j] = y0 + step_y * j;
  }
  double *out = mandelbrot(xs, ys, winHeight, winWidth, maxIter);

  return out;
}

void renderMandelbrot(double *fractals, int height, int width, int maxIter) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      double iterations = fractals[y * width + x];
      Color color;
      if ((int)iterations == maxIter) {
        color = BLACK;
      } else {
        int r = (int)floor((float)iterations / (float)maxIter * 255);
        int g = (int)floor(
            (log(iterations) + 1.0 - (float)iterations / (float)maxIter) / 2.0 *
            255);
        int b = (int)floorl(
            (log(iterations) + (1.0 - (float)iterations / (float)maxIter)) *
            255);
        color = (Color){r, g, b, 255};
      }
      DrawPixel(x, y, color);
    }
    DrawPixel(0, 0, RED);
  }
}

int main(void) {
  int height = 600;
  int width = 800;
  int maxIter = 1000;
  float x0 = 0.0;
  float y0 = 0.0;
  float cx = x0;
  float cy = y0;
  float fw0 = 3.0;
  float fh0 = 3.0;
  double *mandelbrotArr =
      setupMandelbrot(height, width, maxIter, x0, y0, fw0, fh0);

  InitWindow(width, height, "Mandelbrot");
  Camera2D camera = {0};
  float zoom = 1.0f;
  bool rerender = true;
  while (!WindowShouldClose()) {
    float wheel = GetMouseWheelMove();
    bool mouse = IsMouseButtonPressed(0);
    if ((wheel == 0 && mouse) || (wheel == 0 && GetCharPressed() == '+')) {
      wheel = 1;
    }
    if ((wheel == 0 && IsMouseButtonPressed(1)) ||
        (wheel == 0 && GetCharPressed() == '-')) {
      wheel = -1;
    }
    if (wheel != 0) {
      // Get the world point that is under the mouse
      Vector2 mouseWorldPos = GetMousePosition();

      // Set the offset to where the mouse is

      // Set the target to match, so that the camera maps the world space point
      // under the cursor to the screen space point under the cursor at any zoom
      // camera.target = mouseWorldPos;

      // Zoom increment
      const float zoomIncrement = 0.35f;

      zoom += (wheel * zoomIncrement);
      if (zoom < zoomIncrement) {
        zoom = zoomIncrement;
      }
      float fw = fw0 / zoom;
      float fh = fh0 / zoom;
      cx = (cx - fw / 2.0) + (fw / width) * mouseWorldPos.x;
      cy = (cy - fh / 2.0) + (fh / height) * mouseWorldPos.y;
      mandelbrotArr = setupMandelbrot(height, width, maxIter, cx, cy, fw, fh);
      rerender = true;
      /*printf("x %f ", mouseWorldPos.x);
      printf("y %f \n", mouseWorldPos.y);
      printf("cx %f ", cx);
      printf("cy %f\n", cy);
      printf("ox %f ", fw);
      printf("oy %f\n", fh);
      printf("zoom %f \n", zoom);*/
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // BeginMode2D(camera);
    if (rerender) {
      renderMandelbrot(mandelbrotArr, height, width, maxIter);
      //  rerender = false;
    }
    // EndMode2D();
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
