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

		/**��ȡĿ��㵽Բ����������Ŀ�굽Բ���ߵļн�
		*point��Ŀ���
		*circleCenter��Բ���ĵ�
		*circleRadius��Բ�뾶
		*pSpatialReference������ϵͳ
		*tangentAngle������ֵ
		**/
		static void getCircleTangentAngle(const osg::Vec3d& point,const osg::Vec3d& circleCenter,
			const double& circleRadius,const osgEarth::SpatialReference* pSpatialReference,double& tangentAngle);

		/**��ȡĿ��㵽���������������Ŀ�굽��������ߵļн�
		*point��Ŀ���
		*geometry������α߽��
		*pSpatialReference������ϵͳ
		*tangentAngle������ֵ1
		*tangentAngle2������ֵ2
		**/
		static void getPolygonTangentAngle(const osg::Vec3d& point,const std::vector<osg::Vec3d>& geometry,
			const osgEarth::SpatialReference* pSpatialReference,double& tangentAngle,double& tangentAngle2);

		/**��ȡ���������
		*geometry������α߽��
		**/
		static osg::Vec3d getBarycenter(const std::vector<osg::Vec3d>& geometry);

		/**��ȡĿ��㵽���������������Ŀ�귽λ�Ƿ���ļн�
		*point��Ŀ���
		*pointAzimuth��Ŀ��ķ��еķ�λ��
		*center�����ĵ�
		*pSpatialReference������ϵͳ
		*tangentAngle������ֵ
		**/
		static void getAngleByAzimuth(const osg::Vec3d& point,const double& pointAzimuth,
			const osg::Vec3d& center,const osgEarth::SpatialReference* pSpatialReference,double& angle);

		/**����A��ľ��ȡ�γ�ȡ��߶ȣ�B�����A��ľ��룬��λ�����ǣ�����B��ľ��ȣ�γ�ȣ��߶�
		*point����֪��A
		*distance��δ֪��B���A��ľ���
		*pointAzimuth��δ֪��B���A��ķ�λ
		*pointPitching��δ֪��B���A�������
		*pSpatialReference������ϵͳ
		**/
		static osg::Vec3d computeAnotherPoint(const osg::Vec3d& point,
			const double& distance,const double& pointAzimuth,const double& pointPitching,
			const osgEarth::SpatialReference* pSpatialReference);

		/**����A��ľ��ȡ�γ�ȡ��߶�, B��ľ��ȡ�γ�ȡ��߶ȣ�����B�����A��ľ��룬��λ������
		*pointA����֪��A
		*poinB����֪��B
		*distance������ֵB���A��ľ���
		*pointAzimuth������ֵ��B���A��ķ�λ
		*pointPitching������ֵ��B���A�������
		*pSpatialReference������ϵͳ
		**/
		static void computeAnotherPointPosture(const osg::Vec3d& pointA,
			const osg::Vec3d& pointB,double& distance,double& pointAzimuth,double& pointPitching,
			const osgEarth::SpatialReference* pSpatialReference);
	};
}

#endif