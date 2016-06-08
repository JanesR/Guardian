#-------------------------------------------------
#
# Project created by QtCreator 2016-06-04T13:48:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = guardian_tcc
TEMPLATE = app

INCLUDEPATH += -I/usr/local/include
LIBS +=  -L"/usr/local/lib/*.a" "/usr/local/lib/*.so" \


SOURCES += main.cpp\
        mainwindow.cpp \
    controleBD.cpp \
    GPIOClass.cpp \
    recoFace.cpp \
    serialCommunication.cpp

HEADERS  += mainwindow.h \
    bdGuardian.h \
    GPIOClass.h \
    recoFace.h \
    serialCommunication.h

FORMS    += mainwindow.ui \
    igniTela.ui


OTHER_FILES += \
    controlaBD
