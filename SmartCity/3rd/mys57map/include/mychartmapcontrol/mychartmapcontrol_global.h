#ifndef MYCHARTMAPCONTROL_GLOBAL_H
#define MYCHARTMAPCONTROL_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QString>

#ifdef MYCHARTMAPCONTROL_LIB
# define MYCHARTMAPCONTROL_EXPORT Q_DECL_EXPORT
#else
# define MYCHARTMAPCONTROL_EXPORT Q_DECL_IMPORT
#endif

#endif // MYCHARTCOMMONLIB_GLOBAL_H
