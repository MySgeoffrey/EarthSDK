#ifndef MYCHARTTILETOOL_GLOBAL_H
#define MYCHARTTILETOOL_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef MYCHARTTILETOOL_LIB
# define MYCHARTTILETOOL_EXPORT Q_DECL_EXPORT
#else
# define MYCHARTTILETOOL_EXPORT Q_DECL_IMPORT
#endif

#endif // MYCHARTTILETOOL_GLOBAL_H
