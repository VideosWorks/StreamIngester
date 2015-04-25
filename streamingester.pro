#-------------------------------------------------
#
# Project created by QtCreator 2015-04-25T19:32:32
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = streamingester
CONFIG   += console
CONFIG   += c++14

CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += src/main.cpp \
    src/rpc.cpp \
    src/ingest.cpp

HEADERS += \
    src/rpc.h \
    src/ingest.h
