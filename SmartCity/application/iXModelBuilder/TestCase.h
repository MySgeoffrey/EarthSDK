#ifndef TestCase_h__
#define TestCase_h__

#include "modelbuilder/MeshUtil.h"
#include "osgEarth/GeoData"

namespace MeshGenerator
{
	class TestCase
	{
	public:
		TestCase() = delete;
		~TestCase() = delete;

	public:
		//static osg::Node* execLocal(const osgEarth::GeoPoint& geoPoint);
		static osg::Node* exec(const osgEarth::GeoPoint& geoPoint);
	};
}
#endif // TestCase_h__
