#ifndef _PIPELINE_H_
#define _PIPELINE_H_

#include "pipenet/pipenetconfig.h"
#include "osg/Array"
#include "framework/common/iobject.h"

namespace PipeNet
{
	class VR_PIPENET_EXPORT CPipeLine : public Common::IObject
	{
	public:
		CPipeLine();

		virtual ~CPipeLine();

		std::string& getStartPointID();

		std::string& getEndPointID();

		std::string& getMaterial();

		double& getRadius();

		osg::Vec3d& getStartGeoPosition();

		osg::Vec3d& getEndGeoPosition();

	protected:
		double mRadius;
		std::string mStartPointID;
		std::string mEndPointID;
		std::string mMaterial;
		osg::Vec3d mStartGeoPosition;
		osg::Vec3d mEndGeoPosition;
	};
}
#endif