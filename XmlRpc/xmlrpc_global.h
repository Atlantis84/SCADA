#ifndef XMLRPC_GLOBAL_H
#define XMLRPC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(XMLRPC_LIBRARY)
#  define XMLRPCSHARED_EXPORT Q_DECL_EXPORT
#else
#  define XMLRPCSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XMLRPC_GLOBAL_H
