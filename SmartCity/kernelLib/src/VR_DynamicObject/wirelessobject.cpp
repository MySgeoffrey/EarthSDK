#include "dynamicobject/wirelessobject.h"

using namespace DynamicObject;

DynamicObject::CWirelessObject::CWirelessObject( osgEarth::MapNode* mMapNode ) : CGeoSegment(mMapNode)
{
	mpWirelessObjectPos = osg::Vec3d(0,0,0);
	mPitchAngle = 0.0;
	mRadius = 0.0;
	mHasLoadResource = false;
	mIsRenderToScene = false;
	mCreateTime = 0.0;
	mDestroyTimeInterval = 1.0;
	mAssociatedFacilityID = "";
}

DynamicObject::CWirelessObject::~CWirelessObject()
{
	deRenderFromScene();
	unLoadFromScene();
}

void DynamicObject::CWirelessObject::setWirelessObjectPos( const osg::Vec3d& objctPos )
{
	mpWirelessObjectPos = objctPos;
}

osg::Vec3d DynamicObject::CWirelessObject::getWirelessObjectPos()
{
	return mpWirelessObjectPos;
}

void DynamicObject::CWirelessObject::setTargetInfo( const float& pitchAngle,const float& radius )
{
	mPitchAngle = pitchAngle;
	mRadius = radius;
}

void DynamicObject::CWirelessObject::getTargetInfo( float& pitchAngle,float& radius )
{
	pitchAngle = mPitchAngle;
	radius = mRadius;
}

bool DynamicObject::CWirelessObject::synUpdate()
{
	if(mIsRenderToScene)
		return true;
	this->renderToScene();
	this->getStartPoint() = mpWirelessObjectPos;

	osg::Vec3d startWorldPos;
	osgEarth::GeoPoint geoStartPoint(this->getMapNode()->getMapSRS(), mpWirelessObjectPos, osgEarth::ALTMODE_ABSOLUTE);
	geoStartPoint.toWorld(startWorldPos);

	osg::Vec3d zAxis(0, 0, 1);
	osg::Vec3d upDir = startWorldPos;
	upDir.normalize();
	osg::Vec3d leftDir = upDir ^ zAxis;
	leftDir.normalize();
	osg::Vec3d northDir = leftDir ^ upDir;
	northDir.normalize();

	//northDir以upDir为中心轴顺时针转mPitchAngle
	osg::Quat q;
	q.makeRotate(osg::inDegrees(mPitchAngle),upDir);
	osg::Matrix rotateMatrix = osg::Matrix::rotate(q);	
	northDir = rotateMatrix * northDir;
	northDir.normalize();

	osg::Vec3d endPoint = startWorldPos + northDir * mRadius;
	osgEarth::GeoPoint geoEndPoint;
	geoEndPoint.fromWorld(this->getMapNode()->getMapSRS(),endPoint);
	this->getEndPoint() = geoEndPoint.vec3d();
	this->updateSceneObject();
	mIsRenderToScene = true;
	return true;
}

bool DynamicObject::CWirelessObject::reveiveSignal( DynamicObject::CDataSingal* pRefSignal )
{
	osg::Vec3d geoPosition = osg::Vec3d(pRefSignal->longitude,pRefSignal->latitude,pRefSignal->altitude);
	double azimuth = pRefSignal->azimuth;
	double radius = pRefSignal->radius;
	if (mPathMutex.lock() == 0)
	{
		setWirelessObjectPos(geoPosition);
		setTargetInfo(azimuth,radius);
		mPathMutex.unlock();
	}
	return true;
}

bool DynamicObject::CWirelessObject::hasLoadResource()
{
	bool r = false;
	if (this->asynLock())
	{
		r = mHasLoadResource;
		this->asynUnlock();
	}
	return r;
}

bool DynamicObject::CWirelessObject::aSynLoad()
{
	if (this->asynLock())
	{
		this->getStartPoint() = mpWirelessObjectPos;
		this->getEndPoint() = mpWirelessObjectPos;
		this->loadToScene();
		mHasLoadResource = true;
		this->asynUnlock();
	}
	return true;
}

void DynamicObject::CWirelessObject::setCreateTime( const double& createTime )
{
	mCreateTime = createTime;
}

double DynamicObject::CWirelessObject::getCreateTime()
{
	return mCreateTime;
}

void DynamicObject::CWirelessObject::setDestroyTimeInterval( const double& timeInterval )
{
	mDestroyTimeInterval = timeInterval;
}

double DynamicObject::CWirelessObject::getDestroyTimeInterval()
{
	return mDestroyTimeInterval;
}

void DynamicObject::CWirelessObject::setAssociatedFacilityID( const std::string& facilityID )
{
	mAssociatedFacilityID = facilityID;
}

std::string DynamicObject::CWirelessObject::getAssociatedFacilityID()
{
	return mAssociatedFacilityID;
}
