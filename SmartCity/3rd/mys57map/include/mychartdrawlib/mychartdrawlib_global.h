#ifndef MYSMARTSHIPLIB_GLOBAL_H
#define MYSMARTSHIPLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef MYCHARTDRAWLIB_LIB
# define MYCHARTDRAWLIB_EXPORT Q_DECL_EXPORT
#else
# define MYCHARTDRAWLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // MYSMARTSHIPLIB_GLOBAL_H