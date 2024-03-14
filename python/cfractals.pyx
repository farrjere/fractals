from cython.parallel import prange
import numpy as np
cimport numpy as np

def calcualte_z(double complex[:] cs, int max_iter, int power):
    # Cython version of calcualting the mandelbrot set
    cdef unsigned int i, length
    cdef double complex z, c
    cdef int[:] output = np.empty(len(cs), dtype=np.int32)
    
    length = len(cs)
    #using nogil as we are using prange from openmp and this area does not call into the python interpretter until we are done
    with nogil:
        #this uses openmp to paraellelize out across all of the complex numbers we have, schedule='guided' handles breaking the tasks up nicely
        for i in prange(length, schedule='guided'):
            z = 0
            output[i] = 0
            c = cs[i]
            while output[i] < max_iter and (z.real * z.real + z.imag * z.imag) < 4:
                z = z * z + c
                output[i] += 1
    return output
