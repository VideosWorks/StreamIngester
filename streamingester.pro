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


SOURCES += main.cpp \
    rpc.cpp \
    ingest.cpp

HEADERS += \
    rpc.h \
    ingest.h
