#ifndef _IMYTRAILLINE_H
#define _IMYTRAILLINE_H

#include "mymeaturelib/mychartmeaturelib_global.h"
#include "mychartmaplib/imyshaperecord.h"
#include "mymeaturelib/GPRMC.h"

namespace MyChart
{
	class MYCHARTMEATURELIB_EXPORT IMyTrailShapeRecord
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

		virtual void setEditing(const bool& value);

		virtual bool getEditing();

	private:
		std::list<GPS_DATA> mGpsDatas;

		bool mIsEditing;
	};

}

#endif //_IMYTRAILLINE_H