TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Utils.cpp \
    MasterImages.cpp \
    ClosestNeighbour.cpp \
    DecisionFunction.cpp \
    LabManager.cpp

HEADERS += \
    Recognizer.hpp \
    Utils.hpp \
    ClosestNeighbour.hpp \
    MasterImages.hpp \
    DecisionFunction.hpp \
    LabManager.hpp
