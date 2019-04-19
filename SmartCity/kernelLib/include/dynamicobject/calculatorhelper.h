#ifndef _CALCULATORHELPER_H
#define _CALCULATORHELPER_H

#include "dynamicobject/dynamicobjectconfig.h"
#include <osg/Vec3d>
#include <osgEarth/SpatialReference>

namespace DynamicObject
{
	class VR_DYNAMICOBJECT_EXPORT CCalculatorHelper
	{
	public:
		static double computeAzimuth(const osg::Vec3d& beginPoint,const osg::Vec3d& endPoint);

		static bool isPointInCircle(const osg::Vec3d& point,const osg::Vec3d& circleCenter,
			const double& circleRadius,const osgEarth::SpatialReference* pSpatialReference);

		static bool isPointInGeometry(const osg::Vec3d& point,
			const std::vector<osg::Vec3d>& geometry,const osgEarth::SpatialReference* pSpatialReference );

		/**获取目标点到圆中心连线与目标到圆切线的夹角
		*point：目标点
		*circleCenter：圆中心点
		*circleRadius：圆半径
		*pSpatialReference：坐标系统
		*tangentAngle：返回值
		**/
		static void getCircleTangentAngle(const osg::Vec3d& point,const osg::Vec3d& circleCenter,
			const double& circleRadius,const osgEarth::SpatialReference* pSpatialReference,double& tangentAngle);

		/**获取目标点到多边形质心连线与目标到多边形切线的夹角
		*point：目标点
		*geometry：多边形边界点
		*pSpatialReference：坐标系统
		*tangentAngle：返回值1
		*tangentAngle2：返回值2
		**/
		static void getPolygonTangentAngle(const osg::Vec3d& point,const std::vector<osg::Vec3d>& geometry,
			const osgEarth::SpatialReference* pSpatialReference,double& tangentAngle,double& tangentAngle2);

		/**获取多边形质心
		*geometry：多边形边界点
		**/
		static osg::Vec3d getBarycenter(const std::vector<osg::Vec3d>& geometry);

		/**获取目标点到多边形质心连线与目标方位角方向的夹角
		*point：目标点
		*pointAzimuth：目标的飞行的方位角
		*center：中心点
		*pSpatialReference：坐标系统
		*tangentAngle：返回值
		**/
		static void getAngleByAzimuth(const osg::Vec3d& point,const double& pointAzimuth,
			const osg::Vec3d& center,const osgEarth::SpatialReference* pSpatialReference,double& angle);

		/**根据A点的经度、纬度、高度，B点相对A点的距离，方位，仰角，计算B点的经度，纬度，高度
		*point：已知点A
		*distance：未知点B相对A点的距离
		*pointAzimuth：未知点B相对A点的方位
		*pointPitching：未知点B相对A点的仰角
		*pSpatialReference：坐标系统
		**/
		static osg::Vec3d computeAnotherPoint(const osg::Vec3d& point,
			const double& distance,const double& pointAzimuth,const double& pointPitching,
			const osgEarth::SpatialReference* pSpatialReference);

		/**根据A点的经度、纬度、高度, B点的经度、纬度、高度，计算B点相对A点的距离，方位，仰角
		*pointA：已知点A
		*poinB：已知点B
		*distance：返回值B相对A点的距离
		*pointAzimuth：返回值点B相对A点的方位
		*pointPitching：返回值点B相对A点的仰角
		*pSpatialReference：坐标系统
		**/
		static void computeAnotherPointPosture(const osg::Vec3d& pointA,
			const osg::Vec3d& pointB,double& distance,double& pointAzimuth,double& pointPitching,
			const osgEarth::SpatialReference* pSpatialReference);
	};
}

#endif