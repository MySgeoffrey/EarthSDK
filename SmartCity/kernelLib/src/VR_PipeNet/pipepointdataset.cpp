#include "pipenet/pipepointdataset.h"
#include "pipenet/pipepoint.h"

namespace PipeNet
{
	CPipePointDataSet::CPipePointDataSet()
	{

	}

	CPipePointDataSet::~CPipePointDataSet()
	{

	}

	std::vector<CPipePoint*>& CPipePointDataSet::getPipePoints()
	{
		return this->mPipePoints;
	}

	CPipePoint* CPipePointDataSet::getPipePoint(const osg::Vec3d& in_geoPosition)
	{
		for (int i = 0; i < this->mPipePoints.size(); ++i)
		{
			CPipePoint* pPipePoint = this->mPipePoints.at(i);
			if (pPipePoint && (pPipePoint->getGeoPosition() - in_geoPosition).length() < 0.00000001)
			{
				return pPipePoint;
			}
		}
		return NULL;
	}

	CPipePoint* CPipePointDataSet::getPipePoint(const std::string& in_pipePointId)
	{
		for (int i = 0; i < this->mPipePoints.size(); ++i)
		{
			CPipePoint* pPipePoint = this->mPipePoints.at(i);
			if (pPipePoint && pPipePoint->getID() == in_pipePointId)
			{
				return pPipePoint;
			}
		}
		return NULL;
	}
}