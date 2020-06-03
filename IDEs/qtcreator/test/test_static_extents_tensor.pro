TEMPLATE = app
TARGET = test_static_extents_tensor

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
    ../../../test/tensor/test_static_tensor_matrix_vector.cpp \
    ../../../test/tensor/test_static_tensor.cpp \
    ../../../test/tensor/test_static_strides.cpp \
    ../../../test/tensor/test_static_operators_comparison.cpp \
    ../../../test/tensor/test_static_operators_arithmetic.cpp \
    ../../../test/tensor/test_static_extents.cpp \
    ../../../test/tensor/test_static_expression_evaluation.cpp


