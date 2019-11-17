#ifndef _IMYTRAILLINE_H
#define _IMYTRAILLINE_H

#include "mysmartshiplib/mysmartshiplib_global.h"
#include "mychartmaplib/imyshaperecord.h"
#include "mysmartshiplib/GPRMC.h"

namespace MyChart
{
	class MYSMARTSHIPLIB_EXPORT IMyTrailShapeRecord
		: public IMyShapeRecord
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="geometryType">要素的几何类型</param>
		IMyTrailShapeRecord(const GeometryType& geometryType);

		virtual ~IMyTrailShapeRecord();

		virtual std::list<GPS_DATA>& getGpsDatas();

		virtual GPS_DATA& getGpsData(int& index);

		virtual void update();

	private:
		std::list<GPS_DATA> mGpsDatas;

	};

}

#endif //_IMYTRAILLINE_H