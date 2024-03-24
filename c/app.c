#include "fractal.h"
#include "raylib.h"
#include <math.h>
#include <stdbool.h>

const int MAXITER = 1000;
const float ZOOM_INC = 0.97f;
struct ZoomSeed {
  int x;
  int y;
  double iterations;
};

// If we are at 0, 0 then we want to fit height x width points in the space of
// -1.5 to 1.5 that is a real width / height of 3 When we zoom the center
// changes but also the real width / height 1x -> 2x should cut the width in
// half centered about the new x, y
// at a center of given a height and width of 600x800 0,0 is at 600/2 800/2 =
// 300, 400
double *setupMandelbrot(int winHeight, int winWidth, float centerX, float centerY, float frameWidth, float frameHeight) {
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
  double *out = mandelbrot(xs, ys, winHeight, winWidth, MAXITER);

  return out;
}

void renderMandelbrot(double *fractals, int height, int width, struct ZoomSeed *zs) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      double iterations = fractals[y * width + x];
      Color color;
      if ((int)iterations == MAXITER) {
        color = BLACK;
      } else {
        if (zs->iterations < iterations) {
          zs->iterations = iterations;
          zs->x = x;
          zs->y = y;
        }
        int r = (int)floor((float)iterations / (float)MAXITER * 255);
        int g = (int)floor((log(iterations) + 1.0 - (float)iterations / (float)MAXITER) / 2.0 * 255);
        int b = (int)floorl((log(iterations) + (1.0 - (float)iterations / (float)MAXITER)) * 255);
        color = (Color){r, g, b, 255};
      }
      DrawPixel(x, y, color);
    }
    DrawPixel(0, 0, RED);
  }
}

struct MandelbrotWindow {
  float cx;
  float cy;
  float fw0;
  float fh0;
  int height;
  int width;
  float zoom;
};

int isManualZoom() {
  float wheel = GetMouseWheelMove();
  bool mouse = IsMouseButtonPressed(0);
  if ((wheel == 0 && mouse) || (wheel == 0 && GetCharPressed() == '+')) {
    wheel = 1;
  }
  if ((wheel == 0 && IsMouseButtonPressed(1)) || (wheel == 0 && GetCharPressed() == '-')) {
    wheel = -1;
  }
  return wheel;
}

double *handleManualZoom(struct MandelbrotWindow *mw, int wheel) {
  // Get the world point that is under the mouse
  Vector2 mouseWorldPos = GetMousePosition();
  // Set the offset to where the mouse is
  // Set the target to match, so that the camera maps the world space point
  // under the cursor to the screen space point under the cursor at any zoom
  // camera.target = mouseWorldPos;
  mw->zoom += (wheel * ZOOM_INC);
  if (mw->zoom < ZOOM_INC) {
    mw->zoom = ZOOM_INC;
  }
  float fw = mw->fw0 / mw->zoom;
  float fh = mw->fh0 / mw->zoom;
  mw->cx = (mw->cx - fw / 2.0) + (fw / mw->width) * mouseWorldPos.x;
  mw->cy = (mw->cy - fh / 2.0) + (fh / mw->height) * mouseWorldPos.y;
  return setupMandelbrot(mw->height, mw->width, mw->cx, mw->cy, fw, fh);
}

int main(void) {
  struct MandelbrotWindow mw;
  mw.height = 600;
  mw.width = 800;
  mw.cx = 0.0;
  mw.cy = 0.0;
  mw.fw0 = 3.0;
  mw.fh0 = 3.0;
  mw.zoom = 1.0;
  double *mandelbrotArr = setupMandelbrot(mw.height, mw.width, mw.cx, mw.cy, mw.fw0, mw.fh0);
  struct ZoomSeed zs;
  zs.x = 0;
  zs.y = 0;
  InitWindow(mw.width, mw.height, "Mandelbrot");
  while (!WindowShouldClose()) {
    int wheel = isManualZoom();
    if (wheel != 0) {
      mandelbrotArr = handleManualZoom(&mw, wheel);
    } else {

      mw.zoom *= ZOOM_INC;
      float fw = mw.fw0 * mw.zoom;
      float fh = mw.fh0 * mw.zoom;
      mw.cx = (mw.cx - fw / 2.0) + (fw / mw.width) * zs.x;
      mw.cy = (mw.cy - fh / 2.0) + (fh / mw.height) * zs.y;
      zs.x = mw.width / 2;
      zs.y = mw.height / 2;
      mandelbrotArr = setupMandelbrot(mw.height, mw.width, mw.cx, mw.cy, fw, fh);
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    renderMandelbrot(mandelbrotArr, mw.height, mw.width, &zs);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
