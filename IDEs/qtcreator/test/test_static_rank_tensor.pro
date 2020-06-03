TEMPLATE = app
TARGET = test_tensor

CONFIG -= qt
CONFIG += depend_includepath debug
win*: CONFIG += console

QMAKE_CXXFLAGS += -fno-inline
QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -Wno-unknown-pragmas
#QMAKE_CXXFLAGS += --coverage

# If ublas tests are build with boost source code then,
# then boost headers and boost libraries should be used.
exists($${BOOST_ROOT}/boost-build.jam)
{
    INCLUDEPATH += $${BOOST_ROOT}
	LIBS += -L../../../../../../stage/lib
	QMAKE_RPATHDIR += ../../../../../../stage/lib
}

INCLUDEPATH += \
    ../../../include

LIBS +=-lboost_unit_test_framework
# -lgcov

HEADERS += \
	../../../test/tensor/utility.hpp

SOURCES += \
    ../../../test/tensor/test_tensor.cpp \
    ../../../test/tensor/test_extents.cpp \
    ../../../test/tensor/test_strides.cpp \
    ../../../test/tensor/test_expression.cpp \
    ../../../test/tensor/test_expression_evaluation.cpp \
    ../../../test/tensor/test_functions.cpp \
    ../../../test/tensor/test_operators_comparison.cpp \
    ../../../test/tensor/test_operators_arithmetic.cpp \
    ../../../test/tensor/test_tensor_matrix_vector.cpp \
    ../../../test/tensor/test_multiplication.cpp \
    ../../../test/tensor/test_algorithms.cpp \
    ../../../test/tensor/test_einstein_notation.cpp \
    ../../../test/tensor/test_multi_index.cpp \
    ../../../test/tensor/test_multi_index_utility.cpp



