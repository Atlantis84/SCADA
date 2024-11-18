QT += core gui
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = lib
DEFINES += DMDBACCESS_LIBRARY

CONFIG += c++11
QMAKE_CXXFLAGS_RELEASE = -Od -ZI -MD
QMAKE_LFLAGS_RELEASE = /DEBUG /INCREMENTAL:NO
CONFIG += plugin
DESTDIR = $$PWD\..\CentralControl\lib\x64

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    TitleBar.cpp \
    dbaccessimpl.cpp \
    dmdbaccessinterface.cpp \
    imessagebox.cpp \
    titlewidget.cpp

HEADERS += \
    DMDBAccess_global.h \
    TitleBar.h \
    dbaccessimpl.h \
    dmdbaccessinterface.h \
    imessagebox.h \
    titlewidget.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
