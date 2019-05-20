#ifndef _PIPEPOINTLAYER_H_
#define _PIPEPOINTLAYER_H_

#include "pipenet/pipenetconfig.h"

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

	protected:
		std::vector<CPipePoint*> mPipePoints;
	};
}
#endif