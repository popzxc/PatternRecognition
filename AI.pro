TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += App/main.cpp \
    App/App.cpp \
    Recognizers/Utils.cpp \
    Recognizers/NearestNeighbour.cpp \
    Recognizers/NearestCentroid.cpp \
    Recognizers/LinearPredictorFunction.cpp \
    Recognizers/KMeans.cpp \
    Recognizers/ThresholdBasedRecognizer.cpp \
    Reductors/PCA.cpp \
    Reductors/FakeReductor.cpp \
    Reductors/ICA.cpp

HEADERS += \
    App/App.hpp \
    Recognizers/Recognizer.hpp \
    Recognizers/Utils.hpp \
    Recognizers/NearestNeighbour.hpp \
    Recognizers/NearestCentroid.hpp \
    Recognizers/LinearPredictorFunction.hpp \
    Recognizers/Utils.hpp \
    Recognizers/KMeans.hpp \
    Recognizers/ThresholdBasedRecognizer.hpp \
    Reductors/Reductor.hpp \
    Reductors/PCA.hpp \
    Reductors/FakeReductor.hpp \
    Reductors/ICA.hpp
