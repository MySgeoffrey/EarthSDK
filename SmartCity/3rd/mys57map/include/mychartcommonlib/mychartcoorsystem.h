#ifndef MYCHARTCOORSYSTEMLIB_H
#define MYCHARTCOORSYSTEMLIB_H

#include "mychartcommonlib/mychartcommonlib_global.h"

namespace MyChart
{
	/// <summary>
	/// 坐标系统类型枚举
	/// </summary>
	enum EMyCoorsystemType
	{
		MCT_Rectangular,///<直角坐标系

		MCT_Geo_WGS84,///<wgs84地理坐标系

		MCT_Prj_MECATOR,///<mecator投影坐标系

		MCT_None ///<未定义坐标系
	};
}
#endif // MYCHARTCOORSYSTEMLIB_H
