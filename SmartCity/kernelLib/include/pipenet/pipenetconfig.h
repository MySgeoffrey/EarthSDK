#ifndef VR_PIPENET_GLOBAL_H
#define VR_PIPENET_GLOBAL_H

#include <exception>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>

using namespace std;

#ifdef VR_PIPENET_EXPORTS
# define VR_PIPENET_EXPORT _declspec(dllexport)
#else
# define VR_PIPENET_EXPORT _declspec(dllimport)
#endif

#endif // VR_PIPENET_GLOBAL_H
