#ifndef VR_GLOBEMAP_H
#define VR_GLOBEMAP_H

#include <exception>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

#ifdef VR_GLOBEMAP_EXPORTS
# define VR_GLOBEMAP_EXPORT _declspec(dllexport)
#else
# define VR_GLOBEMAP_EXPORT _declspec(dllimport)
#endif

#endif // VR_GLOBEMAP_H
