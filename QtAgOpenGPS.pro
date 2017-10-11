#-------------------------------------------------
#
# Project created by QtCreator 2017-09-29T22:07:25
#
#-------------------------------------------------

QT       += core gui opengl quick quickwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtAgOpenGPS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#DEFINES += DEBUG_VEC
#DEFINES += TESTING
#DEFINES += TEST_NMEA

INCLUDEPATH += $$PWD/classes

SOURCES += main.cpp\
        formgps.cpp \
    classes/cabline.cpp \
    classes/vec2.cpp \
    classes/vec3.cpp \
    classes/vec4.cpp \
    classes/cvec.cpp \
    classes/cflag.cpp \
    classes/cperimeter.cpp \
    classes/ccamera.cpp \
    classes/cworldgrid.cpp \
    classes/cmodulecomm.cpp \
    classes/csection.cpp \
    testlists.cpp \
    classes/cnmea.cpp \
    classes/cvehicle.cpp \
    testnmea.cpp \
    classes/ccontour.cpp \
    glwidget.cpp \
    formgps_opengl.cpp \
    classes/cboundary.cpp \
    mainoverlay.cpp \
    openglcontrolitem.cpp \
    openglcontrol.cpp \
    formgps_ui.cpp \
    aogsettings.cpp \
    gltest.cpp \
    formgps_udpcomm.cpp \
    formgps_position.cpp \
    formgps_serial.cpp

HEADERS  += formgps.h \
    classes/cabline.h \
    classes/vec2.h \
    classes/vec3.h \
    classes/vec4.h \
    classes/cvec.h \
    classes/cflag.h \
    classes/cperimeter.h \
    classes/ccamera.h \
    classes/glm.h \
    classes/cworldgrid.h \
    classes/cmodulecomm.h \
    classes/csection.h \
    classes/cnmea.h \
    classes/cvehicle.h \
    classes/ccontour.h \
    glwidget.h \
    classes/cboundary.h \
    btnenum.h \
    mainoverlay.h \
    openglcontrolitem.h \
    openglcontrol.h \
    qmlutil.h \
    aogsettings.h \
    gltest.h

FORMS    += formgps.ui \
    mainoverlay.ui

RESOURCES += \
    agopengps.qrc

DISTFILES += \
    TODO.txt
