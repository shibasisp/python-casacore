# Don't use this cmake file for installation.
# We use for development (for now)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FIND_PACKAGE(Boost COMPONENTS python)

SET(Python_ADDITIONAL_VERSIONS 2.7 3.4 3.5)
find_package(PythonLibs)

PYTHON_ADD_MODULE(_fitting SHARED src/fit.cc src/fitting.cc)
target_include_directories(_fitting PUBLIC ${PYTHON_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(_fitting ${Boost_LIBRARIES} casa_scimath casa_scimath_f)

PYTHON_ADD_MODULE(_images SHARED src/images.cc src/pyimages.cc src/fitsconverter.cpp)
target_include_directories(_images PUBLIC ${PYTHON_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(_images ${Boost_LIBRARIES} casa_images casa_coordinates casa_fits casa_lattices
                      casa_measures casa_scimath casa_scimath_f casa_tables casa_mirlib casa_python)

PYTHON_ADD_MODULE(_functionals SHARED  src/functional.cc src/functionals.cc)
target_include_directories(_functionals PUBLIC ${PYTHON_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(_functionals ${Boost_LIBRARIES} casa_scimath casa_scimath_f boost_python casa_python)

PYTHON_ADD_MODULE(_measures SHARED src/pymeas.cc src/pymeasures.cc)
target_include_directories(_measures PUBLIC ${PYTHON_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(_measures ${Boost_LIBRARIES} casa_measures casa_scimath casa_scimath_f casa_tables casa_python)

PYTHON_ADD_MODULE(_quanta SHARED src/quanta.cc src/quantamath.cc src/quantity.cc src/quantvec.cc)
target_include_directories(_quanta PUBLIC ${PYTHON_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(_quanta ${Boost_LIBRARIES} casa_casa boost_python casa_python)

PYTHON_ADD_MODULE(_tables SHARED src/pytable.cc src/pytableindex.cc src/pytableiter.cc src/pytablerow.cc src/tables.cc)
target_include_directories(_tables PUBLIC ${PYTHON_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(_tables ${Boost_LIBRARIES} casa_tables boost_python casa_python)
