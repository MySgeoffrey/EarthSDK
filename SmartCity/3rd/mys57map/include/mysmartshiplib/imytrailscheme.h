#ifndef _IMYTRAILSCHEME_H
#define _IMYTRAILSCHEME_H

#include "mysmartshiplib/mysmartshiplib_global.h"
#include "mysmartshiplib/imytraillinelayer.h"
#include "mychartmaplib/imyimagelayer.h"

namespace MyChart
{
	enum DirectionType
	{
		DT_EAST,
		DT_WEST,
		DT_SOUTH,
		DT_NORTH,

		DT_EW,
		DT_SN
	};

	enum TrailShapeType
	{
		TST_JIN,//#×ÖÐÍ
		TST_Z//Z×ÖÐÍ
	};

	class MYSMARTSHIPLIB_EXPORT IMyTrailScheme
	{
	public:
		IMyTrailScheme(void);
		~IMyTrailScheme(void);

		void reset();

		void reset(IMyLayer* pLayer);

		static IMyTrailScheme* autoInstance();

		static IMyTrailScheme* customInstance();

	public:
		MyChart::IVector3 StartPosition;
		MyChart::IVector3 EndPosition;
		DirectionType TrailDirection;
		DirectionType StartDirection;
		IMyPolyLineLayer* RefOutLineLayer;
		IMyPolyLineLayer* RefIslandLayer;
		IMyPointLayer* RefStartPointLayer;
		IMyPointLayer* RefEndPointLayer;
		IMyPolyLineLayer* RefTrialLayer;
		IMyPolyLineLayer* RefTrialCheckLayer;
		double EW_IntervalDistance;
		double SN_IntervalDistance;
		double DistanceToOutLine;
		TrailShapeType ShapeType;
	};

	class MYSMARTSHIPLIB_EXPORT IMyTsmpMap
	{
	public:
		IMyTsmpMap(void);

		~IMyTsmpMap(void);

		static IMyTsmpMap* instance();

		virtual void reset();

		virtual void resetMeatureLayer();

		virtual bool toFile(const QString& filePath);

		virtual bool fromFile(const QString& filePath);

	public:
		MyChart::IMyImageLayer* BackImageLayer;
		IMyPolyLineLayer* TrackTrailLayer;
		IMyPointLayer* TrailPointLayer;
		IMyPointLayer* ImagePointLayer;

		QString PathOfTrackTrail;
		QString PathOfBackImage;
		
	private:
		static IMyTsmpMap* s_TsmpMap;
	};
}
#endif