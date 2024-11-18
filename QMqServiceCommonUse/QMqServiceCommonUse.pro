#-------------------------------------------------
#
# Project created by QtCreator 2024-06-12T09:54:30
#
#-------------------------------------------------

QT       += gui
QT += widgets network serialport core sql multimedia

TARGET = QMqServiceCommonUse
TEMPLATE = lib

QMAKE_CXXFLAGS_RELEASE = -Od -ZI -MD
QMAKE_LFLAGS_RELEASE = /DEBUG /INCREMENTAL:NO

DEFINES += QMQSERVICECOMMONUSE_LIBRARY

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
        iniconfigabstract.cpp \
        mqserviceabstract.cpp \
        qmqservicecommonuse.cpp \
        threeinoneiniconfig.cpp \
        threeinonemqservice.cpp

HEADERS += \
        iniconfigabstract.h \
        mqserviceabstract.h \
        qmqservicecommonuse.h \
        qmqservicecommonuse_global.h  \
        threeinoneiniconfig.h \
        threeinonemqservice.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR = $$PWD\..\CentralControl\lib\x64
INCLUDEPATH += ../QMqServiceSrc/rabbitmq-c/include
LIBS += -L$$PWD/../QMqServiceSrc/rabbitmq-c/lib/ -lrabbitmq.4
LIBS += -L$$PWD/../QMqServiceSrc/rabbitmq-c/lib/ -llibrabbitmq.4

DESTDIR = $$PWD\..\CentralControl\lib\x64
INCLUDEPATH += ..\QsLog
DEFINES += QSLOG_IS_SHARED_LIBRARY_IMPORT
DEFINES += QS_LOG_LINE_NUMBERS
LIBS += -L$$PWD\..\CentralControl\lib\x64 -lQsLog2
