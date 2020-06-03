TEMPLATE = app
TARGET = test_tensor

CONFIG -= qt
CONFIG += depend_includepath debug
CONFIG += c++17

QMAKE_CXXFLAGS += -fno-inline
QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -Wno-unknown-pragmas
#QMAKE_CXXFLAGS += --coverage

# ublas include directory
BOOST_ROOT   = ../../../../..
UBLAS_ROOT   = $${BOOST_ROOT}/libs/numeric/ublas
TEST_ROOT    = $${UBLAS_ROOT}/test
INCLUDEPATH += $${UBLAS_ROOT}/include

# If ublas tests are build with boost source code then,
# then boost headers and boost libraries should be used.
#exists($${BOOST_ROOT}/boost-build.jam)
#{
#    INCLUDEPATH += $${BOOST_ROOT}
#	LIBS += -L../../../../../../stage/lib
#	QMAKE_RPATHDIR += ../../../../../../stage/lib
#}

#INCLUDEPATH += ../../../include

LIBS +=-lboost_unit_test_framework
# -lgcov

HEADERS +=  $${TEST_ROOT}/tensor/utility.hpp

SOURCES += \
    $${TEST_ROOT}/tensor/test_tensor_matrix_vector.cpp \
    $${TEST_ROOT}/tensor/test_tensor.cpp \
    $${TEST_ROOT}/tensor/test_strides.cpp \
    $${TEST_ROOT}/tensor/test_stride_iterator.cpp \
    $${TEST_ROOT}/tensor/test_operators_comparison.cpp \
    $${TEST_ROOT}/tensor/test_operators_arithmetic.cpp \
    $${TEST_ROOT}/tensor/test_multiplication.cpp \    
    $${TEST_ROOT}/tensor/test_multi_index.cpp \
    $${TEST_ROOT}/tensor/test_multi_index_utility.cpp \
    $${TEST_ROOT}/tensor/test_functions.cpp \
    $${TEST_ROOT}/tensor/test_expression.cpp \
    $${TEST_ROOT}/tensor/test_expression_evaluation.cpp \
    $${TEST_ROOT}/tensor/test_extents.cpp \
    $${TEST_ROOT}/tensor/test_einstein_notation.cpp \
    $${TEST_ROOT}/tensor/test_algorithms.cpp
