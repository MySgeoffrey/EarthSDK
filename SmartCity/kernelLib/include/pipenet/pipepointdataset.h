#ifndef _PIPEPOINTLAYER_H_
#define _PIPEPOINTLAYER_H_

#include "pipenet/pipenetconfig.h"
#include "osg/Array"

namespace PipeNet
{
	class CPipePoint;
	class VR_PIPENET_EXPORT CPipePointDataSet
	{
	public:
		CPipePointDataSet();

		virtual ~CPipePointDataSet();

		std::vector<CPipePoint*>& getPipePoints();

		CPipePoint* getPipePoint(const std::string& in_pipePointId);

		CPipePoint* getPipePoint(const osg::Vec3d& in_geoPosition);

	protected:
		std::vector<CPipePoint*> mPipePoints;
	};
}
#endif