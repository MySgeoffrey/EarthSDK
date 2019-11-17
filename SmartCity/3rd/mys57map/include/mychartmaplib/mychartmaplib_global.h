#ifndef MYCHARTMAPLIB_GLOBAL_H
#define MYCHARTMAPLIB_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QString>
#include <QStringList>
#include <vector>
#include <QtGui/QPen>

#ifdef MYCHARTMAPLIB_LIB
# define MYCHARTMAPLIB_EXPORT Q_DECL_EXPORT
#else
# define MYCHARTMAPLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // MYCHARTMAPLIB_GLOBAL_H
