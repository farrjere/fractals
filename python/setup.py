from setuptools import setup
from setuptools import Extension
import numpy as np
ext_modules = [Extension("cfractals", ["cfractals.pyx"], extra_compile_args=['-fopenmp', '-I/opt/homebrew/Cellar/libomp/18.1.0/include/omp.h', '-I/opt/homebrew/Cellar/libomp/18.1.0/lib'], extra_link_args=['-fopenmp'])]
from Cython.Build import cythonize
setup(ext_modules=cythonize(ext_modules, compiler_directives={"language_level": 3}), include_dirs=np.get_include())