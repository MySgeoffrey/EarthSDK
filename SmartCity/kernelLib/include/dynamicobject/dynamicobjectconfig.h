#ifndef VR_DYNAMICOBJECT_H
#define VR_DYNAMICOBJECT_H

#include <exception>
#include <stdio.h>
#include <vector>
#include <string>

#ifdef VR_DYNAMICOBJECT_EXPORTS
# define VR_DYNAMICOBJECT_EXPORT _declspec(dllexport)
#else
# define VR_DYNAMICOBJECT_EXPORT _declspec(dllimport)
#endif

#endif // VR_DYNAMICOBJECT_H
