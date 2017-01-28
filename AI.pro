TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Utils.cpp \
    DecisionFunction.cpp \
    LabManager.cpp \
    NearestNeighbour.cpp \
    NearestCentroid.cpp

HEADERS += \
    Recognizer.hpp \
    Utils.hpp \
    DecisionFunction.hpp \
    LabManager.hpp \
    NearestNeighbour.hpp \
    NearestCentroid.hpp
