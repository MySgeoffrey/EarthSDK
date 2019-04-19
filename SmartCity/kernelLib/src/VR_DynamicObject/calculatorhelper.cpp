#include "dynamicobject/calculatorhelper.h"
#include <osgEarth/GeoMath>
#include <osgEarth/GeoData>
#include <osgEarthSymbology/Geometry>
#include <osg/Math>

using namespace DynamicObject;

bool CCalculatorHelper::isPointInCircle( const osg::Vec3d& point,const osg::Vec3d& circleCenter,
	const double& circleRadius,const osgEarth::SpatialReference* pSpatialReference )
{
	if(NULL == pSpatialReference)
		return false;

	double distance = osgEarth::GeoMath::distance(circleCenter, point, pSpatialReference);
	if(distance < circleRadius)
		return true;

	return false;
}

bool DynamicObject::CCalculatorHelper::isPointInGeometry( const osg::Vec3d& point,
	const std::vector<osg::Vec3d>& geometry,const osgEarth::SpatialReference* pSpatialReference )
{
	if(NULL == pSpatialReference)
		return false;

	osgEarth::Symbology::Ring geometryRing;
	for(int  index = 0; index < geometry.size(); ++index)
	{
		geometryRing.push_back(geometry.at(index));
	}
	geometryRing.close();
	if(geometryRing.contains2D(point.x(),point.y()))
		return true;

	return false;
}

void DynamicObject::CCalculatorHelper::getCircleTangentAngle( 
	const osg::Vec3d& point,const osg::Vec3d& circleCenter, const double& circleRadius,
	const osgEarth::SpatialReference* pSpatialReference,double& tangentAngle )
{
	if(NULL == pSpatialReference)
		return;

	osg::Vec3d objectWpoint(0,0,0);
	osgEarth::GeoPoint geoObjectPoint(pSpatialReference, point, osgEarth::ALTMODE_ABSOLUTE);
	geoObjectPoint.toWorld(objectWpoint);

	osg::Vec3d circleCenterWpoint(0,0,0);
	osgEarth::GeoPoint circleCenterPoint(pSpatialReference,circleCenter,osgEarth::ALTMODE_ABSOLUTE);
	circleCenterPoint.toWorld(circleCenterWpoint);

	osg::Vec3d pointCenterDir = circleCenterWpoint - objectWpoint;
	pointCenterDir.normalize();

	osg::Vec3d centerDir = circleCenterWpoint;
	centerDir.normalize();

	osg::Vec3d extentDir = centerDir ^ pointCenterDir;
	extentDir.normalize();

	osg::Vec3d extentWPoint = circleCenterWpoint + extentDir * circleRadius;
	osg::Vec3d pointExtentDir = extentWPoint - objectWpoint;
	pointExtentDir.normalize();

	double angle = acos((pointCenterDir * pointExtentDir) / (pointCenterDir.normalize() * pointExtentDir.normalize()));
	tangentAngle = osg::RadiansToDegrees(angle);
}

void DynamicObject::CCalculatorHelper::getPolygonTangentAngle( const osg::Vec3d& point,
	const std::vector<osg::Vec3d>& geometry, const osgEarth::SpatialReference* pSpatialReference, 
	double& tangentAngle,double& tangentAngle2 )
{
	if(3 > geometry.size())
		return;
	std::map<double,osg::Vec3d> anglePoinMap;
	for (int index = 0; index < geometry.size(); ++index)
		anglePoinMap.insert(std::make_pair(computeAzimuth(point,geometry.at(index)),point));
	osg::Vec3d minPoint = anglePoinMap.begin()->second;
	osg::Vec3d maxPoint = anglePoinMap.rbegin()->second;
	osg::Vec3d center = getBarycenter(geometry);

	osg::Vec3d centerWpoint(0,0,0);
	osgEarth::GeoPoint centerPoint(pSpatialReference,center,osgEarth::ALTMODE_ABSOLUTE);
	centerPoint.toWorld(centerWpoint);

	osg::Vec3d objectWpoint(0,0,0);
	osgEarth::GeoPoint objectPoint(pSpatialReference,point,osgEarth::ALTMODE_ABSOLUTE);
	objectPoint.toWorld(objectWpoint);

	osg::Vec3d minWpoint(0,0,0);
	osgEarth::GeoPoint minPos(pSpatialReference,minPoint,osgEarth::ALTMODE_ABSOLUTE);
	minPos.toWorld(minWpoint);

	osg::Vec3d maxWpoint(0,0,0);
	osgEarth::GeoPoint maxPos(pSpatialReference,maxPoint,osgEarth::ALTMODE_ABSOLUTE);
	maxPos.toWorld(maxWpoint);

	osg::Vec3d pointTocenter = centerWpoint - objectWpoint;
	pointTocenter.normalize();

	osg::Vec3d minDir = minWpoint - objectWpoint;
	minDir.normalize();

	osg::Vec3d maxDir = maxWpoint - objectWpoint;
	maxDir.normalize();

	double minAngle = acos((pointTocenter * minDir) / (pointTocenter.normalize() * minDir.normalize()));
	tangentAngle = osg::RadiansToDegrees(minAngle);

	double maxAngle = acos((pointTocenter * maxDir) / (pointTocenter.normalize() * maxDir.normalize()));
	tangentAngle2 = osg::RadiansToDegrees(maxAngle);
}

osg::Vec3d DynamicObject::CCalculatorHelper::getBarycenter(const std::vector<osg::Vec3d>& geometry)
{
	osg::Vec3d center(0,0,0);
	for (int i = 0; i < geometry.size(); ++i)
	{
		center += geometry.at(i);
	}
	center /= geometry.size();
	return center;
}

void DynamicObject::CCalculatorHelper::getAngleByAzimuth(const osg::Vec3d& point,const double& pointAzimuth,
			const osg::Vec3d& center,const osgEarth::SpatialReference* pSpatialReference,double& angle)
{
	if (NULL == pSpatialReference)
		return;

	osg::Vec3d objectWpoint(0,0,0);
	osgEarth::GeoPoint objectPoint(pSpatialReference,point,osgEarth::ALTMODE_ABSOLUTE);
	objectPoint.toWorld(objectWpoint);

	osg::Vec3d zAxis(0, 0, 1);
	osg::Vec3d upDir = objectWpoint;
	upDir.normalize();
	osg::Vec3d leftDir = upDir ^ zAxis;
	leftDir.normalize();
	osg::Vec3d northDir = leftDir ^ upDir;
	northDir.normalize();

	osg::Quat q;
	q.makeRotate(osg::inDegrees(pointAzimuth),upDir);
	osg::Matrix rotateMatrix = osg::Matrix::rotate(q);	
	osg::Vec3d objectMoveDir = rotateMatrix * northDir;
	objectMoveDir.normalize();

	osg::Vec3d centerWpoint(0,0,0);
	osgEarth::GeoPoint centerPoint(pSpatialReference,center,osgEarth::ALTMODE_ABSOLUTE);
	centerPoint.toWorld(centerWpoint);

	osg::Vec3d pointTocenter = centerWpoint - objectWpoint;
	pointTocenter.normalize();

	angle = acos((pointTocenter * objectMoveDir) / (pointTocenter.normalize() * objectMoveDir.normalize()));
	angle = osg::RadiansToDegrees(angle);
}

double DynamicObject::CCalculatorHelper::computeAzimuth( const osg::Vec3d& beginPoint,const osg::Vec3d& endPoint )
{
	double azimuth = osgEarth::GeoMath::bearing(
		beginPoint.y() * 3.1415926/180,
		beginPoint.x() * 3.1415926/180,
		endPoint.y() * 3.1415926/180,
		endPoint.x() * 3.1415926/180);
	azimuth = azimuth * 180 / 3.1415926;//将方位角从弧度转换成角度
	if (azimuth < 0)
	{
		azimuth += 360;
	}
	return azimuth;
}

osg::Vec3d DynamicObject::CCalculatorHelper::computeAnotherPoint( 
	const osg::Vec3d& point, const double& distance,const double& pointAzimuth,
	const double& pointPitching,const osgEarth::SpatialReference* pSpatialReference )
{
	if(NULL == pSpatialReference)
		return osg::Vec3d(0,0,0);

	osg::Vec3d objectWpoint(0,0,0);
	osgEarth::GeoPoint objectPoint(pSpatialReference,point,osgEarth::ALTMODE_ABSOLUTE);
	objectPoint.toWorld(objectWpoint);

	osg::Vec3d zAxis(0, 0, 1);
	osg::Vec3d upDir = objectWpoint;
	upDir.normalize();
	osg::Vec3d leftDir = upDir ^ zAxis;
	leftDir.normalize();
	osg::Vec3d northDir = leftDir ^ upDir;
	northDir.normalize();

	osg::Quat q;
	q.makeRotate(osg::inDegrees(pointAzimuth),upDir);
	osg::Matrix rotateMatrix = osg::Matrix::rotate(q);	
	osg::Vec3d objectMoveDir = rotateMatrix * northDir;
	objectMoveDir.normalize();

	osg::Vec3d anotherWpoint = objectWpoint + objectMoveDir * distance;
	osgEarth::GeoPoint anotherPoint;
	anotherPoint.fromWorld(pSpatialReference,anotherWpoint);

	double pointPitchingRadians = osg::DegreesToRadians(pointPitching);
	double altitudeDifference = tan(pointPitchingRadians) * distance;
	anotherPoint.z() = point.z() + altitudeDifference; 

	return anotherPoint.vec3d();
}

void DynamicObject::CCalculatorHelper::computeAnotherPointPosture( 
	const osg::Vec3d& pointA, const osg::Vec3d& pointB,double& distance,double& pointAzimuth,
	double& pointPitching,const osgEarth::SpatialReference* pSpatialReference )
{
	if(NULL == pSpatialReference)
		return;
	pointAzimuth = computeAzimuth(pointA,pointB);
	distance = osgEarth::GeoMath::distance(pointA, pointB, pSpatialReference);
	pointPitching = atan(abs(pointA.z() - pointB.z()) / distance);
	pointPitching = osg::RadiansToDegrees(pointPitching);
}
