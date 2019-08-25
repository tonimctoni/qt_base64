TEMPLATE = app
TARGET = qt_base64

QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp
SOURCES += window.cpp base64encoderdecoder.cpp
HEADERS += window.hpp base64encoderdecoder.hpp

