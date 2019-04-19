#ifndef VR_RASTERGEOMETRYCONFIG_H
#define VR_RASTERGEOMETRYCONFIG_H

#include <exception>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

#ifdef VR_RASTERGEOMETRY_EXPORTS
# define VR_RASTERGEOMETRY_EXPORT _declspec(dllexport)
#else
# define VR_RASTERGEOMETRY_EXPORT _declspec(dllimport)
#endif

#define INTERFACE_NOT_IMPLEMENTED throw std::exception("it's not be implemented !");

#endif // VR_RASTERGEOMETRYCONFIG_H
