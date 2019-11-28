#include "pipenet/pipelinedataset.h"
#include "pipenet/pipeline.h"
#include "pipenet/pipepoint.h"

namespace PipeNet
{
	CPipeLineDataSet::CPipeLineDataSet()
	{

	}

	CPipeLineDataSet::~CPipeLineDataSet()
	{

	}

	std::vector<CPipeLine*>& CPipeLineDataSet::getPipeLines()
	{
		return this->mPipeLines;
	}

	bool CPipeLineDataSet::getPipeLines(CPipePoint* in_pipePoint, std::vector<CPipeLine*>& inout_pipeLines)
	{
		bool r = false;
		
		for (int i = 0; i < this->mPipeLines.size(); ++i)
		{
			CPipeLine* pPipeLine = this->mPipeLines.at(i);
			if (pPipeLine)
			{
				if ((pPipeLine->getStartGeoPosition() - in_pipePoint->getGeoPosition()).length() < 0.00000001
					|| (pPipeLine->getEndGeoPosition() - in_pipePoint->getGeoPosition()).length() < 0.00000001)
				{
					inout_pipeLines.push_back(pPipeLine);
				}
			}
		}
		r = r = !inout_pipeLines.empty();
		return r;
	}

	bool CPipeLineDataSet::getPipeLines(const std::string& in_pipePointID,
		std::vector<CPipeLine*>& inout_pipeLines)
	{
		bool r = false;
		std::map<std::string,std::vector<CPipeLine*>>::iterator itr = 
			this->mPipeLinesEx.find(in_pipePointID);
		if (itr != this->mPipeLinesEx.end())
		{
			inout_pipeLines = itr->second;
			r = true;
			return r;
		}
		for (int i = 0; i < this->mPipeLines.size(); ++i)
		{
			CPipeLine* pPipeLine = this->mPipeLines.at(i);
			if (pPipeLine && (pPipeLine->getStartPointID() == in_pipePointID
				|| pPipeLine->getEndPointID() == in_pipePointID))
			{
				inout_pipeLines.push_back(pPipeLine);
			}
		}
		r = !inout_pipeLines.empty();
		return r;
	}
}