TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += App/main.cpp \
    Recognizers/Utils.cpp \
    Recognizers/NearestNeighbour.cpp \
    Recognizers/NearestCentroid.cpp \
    Recognizers/LinearPredictorFunction.cpp \
    App/App.cpp

HEADERS += \
    Recognizers/Recognizer.hpp \
    Recognizers/Utils.hpp \
    Recognizers/NearestNeighbour.hpp \
    Recognizers/NearestCentroid.hpp \
    Recognizers/LinearPredictorFunction.hpp \
    App/App.hpp
