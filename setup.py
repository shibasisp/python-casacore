#!/usr/bin/env python
"""
Setup script for the CASACORE python wrapper.
"""
import os
import sys
import setuptools
from setuptools.command.build_ext import build_ext
from setuptools import setup, Extension, find_packages
from distutils.sysconfig import get_config_vars
from distutils import ccompiler
from distutils.version import LooseVersion
import argparse
from ctypes.util import find_library
import ctypes

from casacore import __version__, __mincasacoreversion__


def get_pybind_include(user=False):
    """Helper class to determine the pybind11 include path
    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked. """

    import pybind11
    return pybind11.get_include(user)


def find_library_file(libname):
    ''' Try to get the directory of the specified library.
    It adds to the search path the library paths given to distutil's build_ext.
    '''
    # Use a dummy argument parser to get user specified library dirs
    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument("--library-dirs", "-L", default='')
    args, unknown = parser.parse_known_args()
    user_libdirs = args.library_dirs.split(':')
    # Append default search path (not a complete list)
    libdirs = user_libdirs+[os.path.join(sys.prefix, 'lib'), '/usr/local/lib', '/usr/lib', '/usr/lib/x86_64-linux-gnu']
    compiler = ccompiler.new_compiler()
    return compiler.find_library_file(libdirs, libname)


# remove the strict-prototypes warning during compilation
(opt,) = get_config_vars('OPT')
os.environ['OPT'] = " ".join(
    flag for flag in opt.split() if flag != '-Wstrict-prototypes'
)

def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()

if sys.version_info[0] == 2:
    casa_python = 'casa_python'
else:
    casa_python = 'casa_python3'


def has_flag(compiler, flagname):
    """Return a boolean indicating whether a flag name is supported on
    the specified compiler.
    """
    import tempfile
    with tempfile.NamedTemporaryFile('w', suffix='.cpp') as f:
        f.write('int main (int argc, char **argv) { return 0; }')
        try:
            compiler.compile([f.name], extra_postargs=[flagname])
        except setuptools.distutils.errors.CompileError:
            return False
    return True


def cpp_flag(compiler):
    """Return the -std=c++[11/14] compiler flag.
    The c++14 is prefered over c++11 (when it is available).
    """
    if has_flag(compiler, '-std=c++14'):
        return '-std=c++14'
    elif has_flag(compiler, '-std=c++11'):
        return '-std=c++11'
    else:
        raise RuntimeError('Unsupported compiler -- at least C++11 support '
                           'is needed!')


extension_metas = (
    # name, sources, depends, libraries, include_dirs
    (
        "casacore.fitting._fitting",
        ["src/fit.cc", "src/fitting.cc"],
        ["src/fitting.h"],
        ['casa_scimath', 'casa_scimath_f', casa_python],
        [get_pybind_include(), get_pybind_include(user=True)],

    ),
    (
        "casacore.functionals._functionals",
        ["src/functional.cc", "src/functionals.cc"],
        ["src/functionals.h"],
        ['casa_scimath', 'casa_scimath_f', casa_python],
        [get_pybind_include(), get_pybind_include(user=True)],
    ),
    (
        "casacore.images._images",
        ["src/images.cc", "src/pyimages.cc"],
        ["src/pyimages.h"],
        ['casa_images', 'casa_coordinates',
         'casa_fits', 'casa_lattices', 'casa_measures',
         'casa_scimath', 'casa_scimath_f', 'casa_tables', 'casa_mirlib',
         casa_python],
        [get_pybind_include(), get_pybind_include(user=True)],
    ),
    (
        "casacore.measures._measures",
        ["src/pymeas.cc", "src/pymeasures.cc"],
        ["src/pymeasures.h"],
        ['casa_measures', 'casa_scimath', 'casa_scimath_f', 'casa_tables',
         casa_python],
        [get_pybind_include(), get_pybind_include(user=True)],
    ),
    (
        "casacore.quanta._quanta",
        ["src/quanta.cc", "src/quantamath.cc", "src/quantity.cc",
            "src/quantvec.cc"],
        ["src/quanta.h"],
        ["casa_casa", casa_python],
        [get_pybind_include(), get_pybind_include(user=True)],
    ),
    (
        "casacore.tables._tables",
        ["src/pytable.cc", "src/pytableindex.cc", "src/pytableiter.cc",
         "src/pytablerow.cc", "src/tables.cc", "src/pyms.cc"],
        ["src/tables.h"],
        ['casa_tables', 'casa_ms', casa_python],
        [get_pybind_include(), get_pybind_include(user=True)],
    )
)

# Find casacore libpath
libcasacasa = find_library_file('casa_casa')
if libcasacasa is None:
    raise Exception("Could not find libcasa_casa.so")

# Get version number from casacore
try:
    libcasa = ctypes.cdll.LoadLibrary(libcasacasa)
    getCasacoreVersion = libcasa.getVersion
    getCasacoreVersion.restype = ctypes.c_char_p
    casacoreversion = getCasacoreVersion()
except:
    # getVersion was fixed in casacore 2.3.0
    raise Exception("Your casacore version is older than 2.3.0 and " +
                    "incompatible with this version of python-casacore")

if LooseVersion(casacoreversion.decode()) < LooseVersion(__mincasacoreversion__):
    raise Exception("Your casacore version is too old. Minimum is " +
                    __mincasacoreversion__)

extensions = []
for meta in extension_metas:
    name, sources, depends, libraries, include_dirs = meta

    for library in libraries:
        if 'casa' in library:
            found_lib = find_library_file(library)
            if found_lib:
                depends = depends+[found_lib]

    extensions.append(Extension(name=name, sources=sources, depends=depends,
                                libraries=libraries, include_dirs=include_dirs,
                                language='c++'))


class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""

    c_opts = {
        'msvc': ['/EHsc'],
        'unix': [],
    }

    if sys.platform == 'darwin':
        c_opts['unix'] += ['-stdlib=libc++', '-mmacosx-version-min=10.7']

    def build_extensions(self):
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        if ct == 'unix':
            opts.append('-DVERSION_INFO="%s"' % self.distribution.get_version())
            opts.append(cpp_flag(self.compiler))
            if has_flag(self.compiler, '-fvisibility=hidden'):
                opts.append('-fvisibility=hidden')
        elif ct == 'msvc':
            opts.append('/DVERSION_INFO=\\"%s\\"' % self.distribution.get_version())
        for ext in self.extensions:
            ext.extra_compile_args = opts
        build_ext.build_extensions(self)


setup(name='python-casacore',
      version=__version__,
      description='A wrapper around CASACORE, the radio astronomy library',
      install_requires=['numpy', 'argparse', 'pybind11>=1.7'],
      author='Gijs Molenaar',
      author_email='gijs@pythonic.nl',
      url='https://github.com/casacore/python-casacore',
      keywords=['pyrap', 'casacore', 'utilities', 'astronomy'],
      long_description=read('README.rst'),
      packages=find_packages(),
      ext_modules=extensions,
      cmdclass={'build_ext': BuildExt},
      license='GPL')
