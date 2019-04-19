#include "pipenet/pipeline.h"

namespace PipeNet
{
	CPipeLine::CPipeLine()
		: mRadius(0.5),mStartPointID(""),mEndPointID(""),mMaterial(""),
		mStartGeoPosition(osg::Vec3d(0,0,0)),mEndGeoPosition(osg::Vec3d(0,0,0))
	{

	}

	CPipeLine::~CPipeLine()
	{

	}

	double& CPipeLine::getRadius()
	{
		return this->mRadius;
	}

	std::string& CPipeLine::getStartPointID()
	{
		return this->mStartPointID;
	}

	std::string& CPipeLine::getEndPointID()
	{
		return this->mEndPointID;
	}

	std::string& CPipeLine::getMaterial()
	{
		return this->mMaterial;
	}

	osg::Vec3d& CPipeLine::getStartGeoPosition()
	{
		return this->mStartGeoPosition;
	}

	osg::Vec3d& CPipeLine::getEndGeoPosition()
	{
		return this->mEndGeoPosition;
	}
}