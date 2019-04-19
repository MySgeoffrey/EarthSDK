#include "pipenet/pipepointdataset.h"

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
}