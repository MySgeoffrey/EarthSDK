#ifndef _IMYTRIALARITHMETIC_H
#define _IMYTRIALARITHMETIC_H

#include "mysmartshiplib/mysmartshiplib_global.h"
#include "mysmartshiplib/imytrailscheme.h"

namespace MyChart
{
	struct trailData
	{
		trailData()
		{
			direction = DirectionType::DT_NORTH;
			line = NULL;
		}

		DirectionType direction;
		IMyPolyline* line;
	};

	class MYSMARTSHIPLIB_EXPORT IMyTrialArithmetic
	{
	public:
		IMyTrialArithmetic(void);

		~IMyTrialArithmetic(void);

		static IMyPolyLineLayer* checkTrail(IMyTrailScheme* RefTrailScheme);

		static IMyPolyLineLayer* checkTrail(
			IMyTrailShapeRecord* pOutlineRecord,
			IMyTrailShapeRecord* pIslandRecord,
			IMyTrailShapeRecord* pTrailRecord,
			IMyTrailScheme* RefTrailScheme);

		static IMyPolyline* computeTrail(IMyTrailScheme* RefTrailScheme);

		static IMyPolyline* computeTrail(
			IMyTrailShapeRecord* pOutlineRecord,
			IMyTrailShapeRecord* pIslandRecord,
			IMyTrailScheme* RefTrailScheme);

		static bool getPointByDirection(
			IMyPolyline& line,
			IVector3& startPoint,
			IVector3& endPoint,
			bool& faceIsland,
			DirectionType& dirType);

		static IVector3 getPointByDirection(
			IVector3& referencePoint,
			IVector3& startPoint,
			IVector3& endPoint,
			DirectionType& dirType);

		static bool canIntersectWidth(IMyPolyline* line,
			IVector3& startPoint,
			IVector3& endPoint);
	};
}

#endif