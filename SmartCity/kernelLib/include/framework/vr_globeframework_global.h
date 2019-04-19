#ifndef VR_GLOBEFRAMEWORK_GLOBAL_H
#define VR_GLOBEFRAMEWORK_GLOBAL_H

#include <exception>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

#ifdef VR_PIPEFRAMEWORK_EXPORTS
# define VR_PIPEFRAMEWORK_EXPORT _declspec(dllexport)
#else
# define VR_PIPEFRAMEWORK_EXPORT _declspec(dllimport)
#endif

#define INTERFACE_NOT_IMPLEMENTED throw std::exception("it's not be implemented !");

#endif // VR_GLOBEFRAMEWORK_GLOBAL_H
