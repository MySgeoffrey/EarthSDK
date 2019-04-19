#ifndef _MODELREDUCTIONSERVICE_H
#define _MODELREDUCTIONSERVICE_H

#include "geoservice/modelreductionserviceconfig.h"

namespace Service
{
	class VR_GEOSERVICE_EXPORT CModelReductionService
	{
	public:
		CModelReductionService();

		virtual ~CModelReductionService();

		void run();
	};	
}


#endif