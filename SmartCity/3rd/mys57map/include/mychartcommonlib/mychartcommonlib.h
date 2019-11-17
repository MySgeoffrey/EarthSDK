#ifndef MYCHARTCOMMONLIB_H
#define MYCHARTCOMMONLIB_H

#include "mychartcommonlib/mychartcommonlib_global.h"

namespace MyChart
{
	/// <summary>
	/// 公共参数接口类
	/// </summary>
	class MYCHARTCOMMONLIB_EXPORT CMyChartCommonlib
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		CMyChartCommonlib();


		/// <summary>
		/// 析构函数
		/// </summary>
		~CMyChartCommonlib();

	public:
		static QString ColorMode;///<颜色模式

		static int SafeContour;///<安全等深线

		static bool IsShowDepthPoint;///<是否显示等深点

		static bool IsEnglishTextShow;///<是否显示英文标注

		static bool IsShowMetaData;///<是否显示元物标

		static QString ModeOfShowLayer;///<图层显示模式

		static QString AreaModeOfSymbol;///<面状符号风格模式

		static QString PointModeOfSymbol;///<点符号风格模式
	};
}
#endif // MYCHARTCOMMONLIB_H
