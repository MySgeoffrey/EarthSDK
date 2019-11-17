#ifndef MYCHARTCOORSYSTEMLIB_H
#define MYCHARTCOORSYSTEMLIB_H

#include "mychartcommonlib/mychartcommonlib_global.h"

namespace MyChart
{
	/// <summary>
	/// ����ϵͳ����ö��
	/// </summary>
	enum EMyCoorsystemType
	{
		MCT_Rectangular,///<ֱ������ϵ

		MCT_Geo_WGS84,///<wgs84��������ϵ

		MCT_Prj_MECATOR,///<mecatorͶӰ����ϵ

		MCT_None ///<δ��������ϵ
	};
}
#endif // MYCHARTCOORSYSTEMLIB_H
