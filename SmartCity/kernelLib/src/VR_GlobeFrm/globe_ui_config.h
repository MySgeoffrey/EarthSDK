#ifndef GLOBE_UI_GLOBAL_H
#define GLOBE_UI_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef VR_GLOBEFRM_LIB
# define GLOBE_UI_EXPORT Q_DECL_EXPORT
#else
# define GLOBE_UI_EXPORT Q_DECL_IMPORT
#endif

#endif // GLOBE_UI_GLOBAL_H
