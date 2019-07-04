#-------------------------------------------------
#
# Project created by QtCreator 2014-09-05T14:43:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoDefectos
TEMPLATE = app

SOURCES += main.cpp \
    Video/video.cpp \
    Video/widget.cpp \
    DefectDetect/foodfill.cpp \
    DefectDetect/glcm.cpp \
    DefectDetect/grafo.cpp \
    DefectDetect/lbp.cpp \
    classifier/fuzzyartmap.cpp \
    classifier/hiperboxfuzzy.cpp \
    featuresExtractor/featuresextractor.cpp \
    featuresExtractor/shape.cpp \
    DefectClassifier/image.cpp \
    DefectClassifier/imagehelper.cpp \
    DefectClassifier/imageprocessor.cpp \
    DefectClassifier/managerpatterns.cpp \
    DefectClassifier/systemcontroller.cpp \
    DefectClassifier/systemcore.cpp \
    model.cpp

HEADERS  += \
    Video/video.h \
    Video/widget.h \
    DefectDetect/foodfill.h \
    DefectDetect/glcm.h \
    DefectDetect/grafo.h \
    DefectDetect/lbp.h \
    classifier/fuzzyartmap.h \
    classifier/helperNetwork.h \
    classifier/hiperboxfuzzy.h \
    featuresExtractor/featuresextractor.h \
    featuresExtractor/shape.h \
    DefectClassifier/image.h \
    DefectClassifier/imagehelper.h \
    DefectClassifier/imageprocessor.h \
    DefectClassifier/managerpatterns.h \
    DefectClassifier/systemcontroller.h \
    DefectClassifier/systemcore.h \
    DefectClassifier/helpers.h \
    model.h

FORMS    += widget.ui

PKGCONFIG += opencv
CONFIG += link_pkgconfig
