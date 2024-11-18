#ifndef TCPCOMMONUSE_GLOBAL_H
#define TCPCOMMONUSE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TCPCOMMONUSE_LIBRARY)
#  define TCPCOMMONUSESHARED_EXPORT Q_DECL_EXPORT
#else
#  define TCPCOMMONUSESHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QTcpSocket>
#include <QTcpServer>

#endif // TCPCOMMONUSE_GLOBAL_H
