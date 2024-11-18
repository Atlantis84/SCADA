TARGET = QsLog
VERSION = "2.0.0"
QT -= gui
CONFIG -= console
CONFIG -= app_bundle
CONFIG += shared
TEMPLATE = lib

DESTDIR = $$PWD\..\CentralControl\lib\x64
#OBJECTS_DIR = $$DESTDIR/obj
#MOC_DIR = $$DESTDIR/moc

win32 {
    DEFINES += QSLOG_IS_SHARED_LIBRARY
}


INCLUDEPATH += $$PWD
#DEFINES += QS_LOG_LINE_NUMBERS    # automatically writes the file and line for each log message
#DEFINES += QS_LOG_DISABLE         # logging code is replaced with a no-op
#DEFINES += QS_LOG_SEPARATE_THREAD # messages are queued and written from a separate thread
SOURCES += $$PWD/QsLogDest.cpp \
    $$PWD/QsLog.cpp \
    $$PWD/QsLogDestConsole.cpp \
    $$PWD/QsLogDestFile.cpp \
    $$PWD/QsLogDestFunctor.cpp

HEADERS += $$PWD/QsLogDest.h \
    $$PWD/QsLog.h \
    $$PWD/QsLogDestConsole.h \
    $$PWD/QsLogLevel.h \
    $$PWD/QsLogDestFile.h \
    $$PWD/QsLogDisableForThisFile.h \
    $$PWD/QsLogDestFunctor.h

OTHER_FILES += \
    $$PWD/QsLogChanges.txt \
    $$PWD/QsLogReadme.txt \
    $$PWD/LICENSE.txt
