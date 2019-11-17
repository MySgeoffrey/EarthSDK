#ifndef MYCHARTS52SYMBOLLIB_GLOBAL_H
#define MYCHARTS52SYMBOLLIB_GLOBAL_H

#include <QtCore/qglobal.h>
#include <qstring.h>
#include <vector>
#include <map>
#include <QtGui/QPen>
#include <QPoint>

#ifdef MYCHARTS52SYMBOLLIB_LIB
# define MYCHARTS52SYMBOLLIB_EXPORT Q_DECL_EXPORT
#else
# define MYCHARTS52SYMBOLLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // MYCHARTS52SYMBOLLIB_GLOBAL_H
