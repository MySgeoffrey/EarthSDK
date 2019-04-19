#include "pipenet/pipepoint.h"

namespace PipeNet
{
	CPipePoint::CPipePoint()
		: mGeoPosition(osg::Vec3d(0,0,0)),mDepth(1),mAttachmentType("")
	{

	}

	CPipePoint::~CPipePoint()
	{

	}

	osg::Vec3d& CPipePoint::getGeoPosition()
	{
		return this->mGeoPosition;
	}

	double& CPipePoint::getDepth()
	{
		return this->mDepth;
	}

	std::string& CPipePoint::getAttachmentType()
	{
		return this->mAttachmentType;
	}

	std::vector<CPipeLine*>& CPipePoint::getAdjcentLines()
	{
		return this->mAdjcentLines;
	}

	void CPipePoint::setAdjcentLines(const std::vector<CPipeLine*>& in_adjcentLines)
	{
		this->mAdjcentLines = in_adjcentLines;
	}
}