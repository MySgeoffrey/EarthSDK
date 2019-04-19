#ifndef VR_MEASUREMENT_H
#define VR_MEASUREMENT_H

#include <exception>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

namespace SceneTool
{
	//测量模式
	enum MeasureType
	{
		//平面模式
		FLAT_MT,

		//球面模式
		SPHERE_MT
	};
}

#ifdef VR_SCENETOOL_EXPORTS
# define VR_SCENETOOL_EXPORT _declspec(dllexport)
#else
# define VR_SCENETOOL_EXPORT _declspec(dllimport)
#endif

#endif // VR_MEASUREMENT_H
