#ifndef _PIPELINELAYER_H_
#define _PIPELINELAYER_H_

#include "pipenet/pipenetconfig.h"

namespace PipeNet
{
	class CPipeLine;
	class VR_PIPENET_EXPORT CPipeLineDataSet
	{
	public:
		CPipeLineDataSet();

		virtual ~CPipeLineDataSet();

		std::vector<CPipeLine*>& getPipeLines();

		bool getPipeLines(const std::string& in_pipePointID,std::vector<CPipeLine*>& inout_pipeLines);
	protected:
		std::vector<CPipeLine*> mPipeLines;
		std::map<std::string,std::vector<CPipeLine*>> mPipeLinesEx;
	};
}

#endif