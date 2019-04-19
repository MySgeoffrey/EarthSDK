#ifndef VR_MEASUREMENT_H
#define VR_MEASUREMENT_H

#include <exception>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

namespace SceneTool
{
	//����ģʽ
	enum MeasureType
	{
		//ƽ��ģʽ
		FLAT_MT,

		//����ģʽ
		SPHERE_MT
	};
}

#ifdef VR_SCENETOOL_EXPORTS
# define VR_SCENETOOL_EXPORT _declspec(dllexport)
#else
# define VR_SCENETOOL_EXPORT _declspec(dllimport)
#endif

#endif // VR_MEASUREMENT_H
