TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Utils.cpp \
    MasterImages.cpp \
    DecisionFunction.cpp \
    LabManager.cpp \
    NearestNeighbour.cpp

HEADERS += \
    Recognizer.hpp \
    Utils.hpp \
    MasterImages.hpp \
    DecisionFunction.hpp \
    LabManager.hpp \
    NearestNeighbour.hpp
