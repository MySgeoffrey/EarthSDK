#ifndef _PIPEPOINT_H_
#define _PIPEPOINT_H_

#include "pipenet/pipenetconfig.h"
#include "osg/Array"
#include "framework/common/iobject.h"

namespace PipeNet
{
	class CPipeLine;
	class VR_PIPENET_EXPORT CPipePoint
		: public Common::IObject
	{
	public:
		CPipePoint();

		virtual ~CPipePoint();

		osg::Vec3d& getGeoPosition();

		double& getDepth();

		std::string& getAttachmentType();

		std::vector<CPipeLine*>& getAdjcentLines();

		void setAdjcentLines(const std::vector<CPipeLine*>& in_adjcentLines);

	protected:
		std::vector<CPipeLine*> mAdjcentLines;
		osg::Vec3d mGeoPosition;
		double mDepth;
		std::string mAttachmentType;
	};
}
#endif