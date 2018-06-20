from distutils.core import setup
from Cython.Build import cythonize
from distutils.extension import Extension
import numpy as np

ext = Extension("rays", ["rays.pyx"], include_dirs=[np.get_include()])

setup(
    name = "Rays",
    ext_modules = cythonize(ext)
)
