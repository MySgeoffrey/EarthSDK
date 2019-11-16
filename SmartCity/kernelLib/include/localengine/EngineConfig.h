/**
 * Project ivbu
 * @author shengjialiang
 * @version v1.0
 */


#ifndef _ENGINECONFIG_H
#define _ENGINECONFIG_H

#ifdef VR_LOCALENGINE_LIB
#define VR_LOCAL_ENGINE_EXPORT   __declspec(dllexport)
#else
#define VR_LOCAL_ENGINE_EXPORT   __declspec(dllimport)
#endif

#include <string>
#include <vector>
#include <map>
#include <queue>

#endif //_ENGINECONFIG_H