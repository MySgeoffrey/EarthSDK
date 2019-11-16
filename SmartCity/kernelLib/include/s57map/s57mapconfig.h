/**
 * Project ivbu
 * @author shengjialiang
 * @version v1.0
 */


#ifndef _S57MAPCONFIG_H
#define _S57MAPCONFIG_H

#ifdef VR_S57MAP_LIB
#define VR_S57MAP_EXPORT   __declspec(dllexport)
#else
#define VR_S57MAP_EXPORT   __declspec(dllimport)
#endif

#include <string>
#include <vector>
#include <map>
#include <queue>

#endif //_S57MAPCONFIG_H