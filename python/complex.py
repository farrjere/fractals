import numpy as np
from numba import jit, int32, float32, complex64
import cfractals


def mandelbrot_cython(xs, ys, _, max_iter, power=2):
    cs = np.array([complex(x, y) for x in xs for y in ys])
    output = cfractals.calcualte_z(cs, max_iter, power)
    return output


def mandelbrot_naive(xs, ys, _, max_iter, power=2):
    output = []

    for i in range(len(xs)):
        for j in range(len(ys)):
            c = complex(xs[i], ys[j])
            n = 0
            z = 0
            while abs(z) < 2 and n < max_iter:
                z = z**power + c
                n += 1
            output.append(n)
    return output


def mandlebrot(xs, ys, _, max_iter, power=2):
    # with mandlebrot z starts at 0 and c is constructed from the x and y coordinates
    # there is one Mandlebrot set

    cs = [complex(x, y) for x in xs for y in ys]
    output = [0 for _ in range(len(cs))]

    for i, c in enumerate(cs):
        n = 0
        z = 0
        while abs(z) < 4 and n < max_iter:
            z = z**power + c
            n += 1
        output[i] = n
    return output


@jit(nopython=True, cache=True, nogil=True, parallel=True)
def mandelbrot_numba(xs, ys, _, max_iter, power=2):
    # with mandlebrot z starts at 0 and c is constructed from the x and y coordinates
    # there is one Mandlebrot set

    cs = [complex(x, y) for x in xs for y in ys]
    output = [0 for _ in range(len(cs))]

    for i, c in enumerate(cs):
        n = 0
        z = 0
        while abs(z) < 2 and n < max_iter:
            z = z**power + c
            n += 1
        output[i] = n
    return output


@jit(nopython=True, parallel=True, cache=True)
def mandelbrot_np_numba(xs, ys, _, max_iter, power=2):
    # this implementation is the same as the numpy one, just with numba added
    # considering all we added is a decorator this is crazy fast
    x, y = np.array(xs), np.array(ys)
    c = np.ravel(x + y[:, None] * 1j)
    output = np.resize(
        np.array(
            0,
        ),
        c.shape,
    )
    z = np.zeros(c.shape, np.complex64)
    for it in range(max_iter):
        z = np.power(z, power) + c
        done = np.greater((z.real * z.real) + (z.imag * z.imag), 4.0)
        c = np.where(done, 0 + 0j, c)
        z = np.where(done, 0 + 0j, z)
        output = np.where(done, it, output)
    return output


def mandelbrot_numpy(xs, ys, c, max_iter, power=2):
    # calculate z using numpy, this is the original
    # routine from vegaseat's URL
    x, y = np.array(xs), np.array(ys)
    c = np.ravel(x + y[:, None] * 1j)
    output = np.resize(
        np.array(
            0,
        ),
        c.shape,
    )
    z = np.zeros(c.shape, np.complex64)
    for it in range(max_iter):
        z = np.power(z, power) + c
        done = np.greater((z.real * z.real) + (z.imag * z.imag), 4.0)
        c = np.where(done, 0 + 0j, c)
        z = np.where(done, 0 + 0j, z)
        output = np.where(done, it, output)
    return output


def julia(xs, ys, c, max_iter, power=2):
    # with julia sets c is a constant, which determines the Julia set
    # instead of using x and y to construct c we use x and y to construct z
    # there are many Julia sets, one for each c
    zs = [complex(x, y) for x in xs for y in ys]
    output = [0 for _ in range(len(zs))]
    for i, z in enumerate(zs):
        n = 0
        while abs(z) <= 2 and n < max_iter:
            z = z**power + c
            n += 1
        output[i] = n
    return output
