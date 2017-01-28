TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Utils.cpp \
    LabManager.cpp \
    NearestNeighbour.cpp \
    NearestCentroid.cpp \
    LinearPredictorFunction.cpp

HEADERS += \
    Recognizer.hpp \
    Utils.hpp \
    LabManager.hpp \
    NearestNeighbour.hpp \
    NearestCentroid.hpp \
    LinearPredictorFunction.hpp
