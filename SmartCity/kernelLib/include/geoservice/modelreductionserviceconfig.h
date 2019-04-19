#ifndef _MODELREDUCTIONSERVICECONFIG_H
#define _MODELREDUCTIONSERVICECONFIG_H

#include <exception>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

#ifdef VR_GEOSERVICE_EXPORTS
# define VR_GEOSERVICE_EXPORT _declspec(dllexport)
#else
# define VR_GEOSERVICE_EXPORT _declspec(dllimport)
#endif

#endif