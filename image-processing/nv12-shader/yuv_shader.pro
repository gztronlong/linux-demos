#-------------------------------------------------
#
# Project created by QtCreator 2017-09-25T08:56:47
#
#-------------------------------------------------

TEMPLATE = app
TARGET = yuv_shader
DESTDIR = bin

QT += opengl


HEADERS = gl_widget.h \
          yuv_window.h

SOURCES = gl_widget.cpp \
          yuv_window.cpp \
          main.cpp

CONFIG += qt debug
