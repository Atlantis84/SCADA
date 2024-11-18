#-------------------------------------------------
#
# Project created by QtCreator 2023-12-22T17:54:31
#
#-------------------------------------------------

QT       -= gui

TARGET = ModbusClientDll
TEMPLATE = lib

DEFINES += MODBUSCLIENTDLL_LIBRARY

QMAKE_CXXFLAGS_RELEASE = -Od -ZI -MD
QMAKE_LFLAGS_RELEASE = /DEBUG /INCREMENTAL:NO

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
        lib/modbus-data.c \
        lib/modbus-rtu.c \
        lib/modbus-tcp.c \
        lib/modbus.c \
        modbusclientdll.cpp

HEADERS += \
        lib/config.h \
        lib/modbus-private.h \
        lib/modbus-rtu-private.h \
        lib/modbus-rtu.h \
        lib/modbus-tcp-private.h \
        lib/modbus-tcp.h \
        lib/modbus-version.h \
        lib/modbus.h \
        modbusclientdll.h \
        modbusclientdll_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR = $$PWD\..\CentralControl\lib\x64
INCLUDEPATH += ..\QsLog
DEFINES += QSLOG_IS_SHARED_LIBRARY_IMPORT
DEFINES += QS_LOG_LINE_NUMBERS
LIBS += -L$$PWD\..\CentralControl\lib\x64 -lQsLog2

INCLUDEPATH += $$PWD/lib
LIBS += -Ldll -lws2_32
