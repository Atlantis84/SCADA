#-------------------------------------------------
#
# Project created by QtCreator 2023-07-27T14:37:08
#
#-------------------------------------------------

QT += core network gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
CONFIG += plugin
QMAKE_CXXFLAGS_RELEASE = -Od -ZI -MD
QMAKE_LFLAGS_RELEASE = /DEBUG /INCREMENTAL:NO
TARGET = FinsUDPService
TEMPLATE = lib

DEFINES += FINSUDPSERVICE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        finsudpservice.cpp

HEADERS += \
        finsudpservice.h \
        finsudpservice_global.h 
DESTDIR = $$PWD\..\CentralControl\lib\x64
INCLUDEPATH += ..\QsLog
DEFINES += QSLOG_IS_SHARED_LIBRARY_IMPORT
DEFINES += QS_LOG_LINE_NUMBERS
LIBS += -L$$PWD\..\CentralControl\lib\x64 -lQsLog2

unix {
    target.path = /usr/lib
    INSTALLS += target
}
