#ifndef VR_GEOOBJECT_GLOBAL_H
#define VR_GEOOBJECT_GLOBAL_H

#include <exception>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

#ifdef VR_GEOOBJECT_EXPORTS
# define VR_GEOOBJECT_EXPORT _declspec(dllexport)
#else
# define VR_GEOOBJECT_EXPORT _declspec(dllimport)
#endif

#endif // VR_GEOOBJECT_GLOBAL_H
