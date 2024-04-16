import argparse
import array
from PIL import Image
from complex import (
    mandelbrot_naive,
    mandlebrot,
    julia,
    mandelbrot_numpy,
    mandelbrot_numba,
    mandelbrot_np_numba,
    mandelbrot_cython,
)
import numpy as np

complex_funcs = {
    "naieve": mandelbrot_naive,
    "mandelbrot": mandlebrot,
    "julia": julia,
    "mnp": mandelbrot_numpy,
    "numba": mandelbrot_numba,
    "mnpn": mandelbrot_np_numba,
    "cython": mandelbrot_cython,
}


def plot_fractal(
    fractal_type, iterations, width, height, output_path, cx, cy, power, loop
):
    func = complex_funcs[fractal_type]
    c = complex(cx, cy)
    input_x = [0 for i in range(width)]
    input_y = [0 for i in range(height)]
    x0, y0, x1, y1 = -1.9, -1.9, 1.9, 1.9
    x_step = (x1 - x0) / width
    y_step = (y1 - y0) / height
    for x in range(width):
        input_x[x] = x0 + x * x_step
    for y in range(height):
        input_y[y] = y0 + y * y_step
    for i in range(1, loop + 1):
        output = func(input_x, input_y, c, i * iterations, power)
        max_value = float(max(output))
        hues = np.array(
            [int(float(o) / max_value * 255) for o in output], dtype=np.uint8
        ).reshape((width, height))
        saturations = np.array(
            [255 for _ in range(len(output))], dtype=np.uint8
        ).reshape((width, height))
        values = np.array(
            [255 if o < i * iterations else 0 for o in output], dtype=np.uint8
        ).reshape((width, height))
        output_arr = np.dstack((hues, saturations, values))
        im = Image.fromarray(output_arr, mode="HSV")
        im = im.convert("RGB")
        im.save(f"{output_path}_{i}.png")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Fractal plotter")
    parser.add_argument(
        "-t", "--type", type=str, default="mnp", help="Type of fractal to plot"
    )
    parser.add_argument(
        "-i",
        "--iterations",
        type=int,
        default=1000,
        help="Max number of iterations for escape",
    )
    parser.add_argument(
        "-w", "--width", type=int, default=1200, help="Width of the image"
    )
    parser.add_argument(
        "-he", "--height", type=int, default=1200, help="Height of the image"
    )
    parser.add_argument(
        "-o",
        "--output",
        type=str,
        default="output",
        help="Output file name, without extension",
    )
    parser.add_argument(
        "-cx",
        type=float,
        default=0.274,
        help="real part of the complex number c, determining the julia set",
    )
    parser.add_argument(
        "-cy",
        type=float,
        default=0.008,
        help="imaginary part of the complex number c, determining the julia set",
    )
    parser.add_argument(
        "-p", "--power", type=int, default=2, help="Power to raise z to"
    )
    parser.add_argument(
        "-l", "--loop", type=int, default=1, help="Number of times to loop the fractal"
    )
    args = parser.parse_args()
    plot_fractal(
        args.type,
        args.iterations,
        args.width,
        args.height,
        args.output,
        args.cx,
        args.cy,
        args.power,
        args.loop,
    )
