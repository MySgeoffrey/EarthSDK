#ifndef MYCHARTARITHMETIC_H
#define MYCHARTARITHMETIC_H

#include "mychartcommonlib/mychartcommonlib_global.h"
#include "mychartcommonlib/mychartarithmetic.h"
#include "math.h"

#define PI 3.1415926

namespace MyChart
{
	struct Angle;
	/// <summary>
	/// 数学算法接口类
	/// </summary>
	class MYCHARTCOMMONLIB_EXPORT CMyChartArithmetic
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		CMyChartArithmetic();

		/// <summary>
		/// 析构函数
		/// </summary>
		~CMyChartArithmetic();

		/// <summary>
		/// 方位角计算接口
		/// </summary>
		/// <param name="latA">第一个点的纬度</param>
		/// <param name="lonA">第一个点的经度</param>
		/// <param name="latB">第二个点的纬度</param>
		/// <param name="lonB">第二个点的经度</param>
		/// <returns></returns>
		static Angle azimuth(Angle latA, Angle lonA, Angle latB, Angle lonB);

		/// <summary>
		/// 弧度转角度
		/// </summary>
		/// <param name="radians">弧度值</param>
		/// <returns>角度值</returns>
		static double radiansToDegrees(double radians)
		{
			return  radians * 180.0 / PI;
		}

		/// <summary>
		/// 角度转弧度
		/// </summary>
		/// <param name="degrees">角度值</param>
		/// <returns>弧度值</returns>
		static double degreesToRadians(double degrees)
		{
			return PI * degrees / 180.0f;
		}

	public:
		
	};

	/// <summary>
	/// 角度类
	/// </summary>
	struct Angle
	{
	public:
		/// <summary>
		/// 返回的弧度数
		/// </summary>
		double Radians;

		Angle()
			: Radians(0)
		{
		
		}

		Angle(double radians)
			: Radians(radians)
		{
		
		}

		/// <summary>
		/// 给定一个弧度创建一个新的Angle对象
		/// </summary>
        /// <example>
        /// <code>
        /// using S57Chart.Core; 
        /// 
        /// double angle=1.2;//弧度值
        /// Angle degree= Angle.FromRadians(angle);
        /// //degree的角度值是68.754935415698
        /// //degree的弧度值是1.2
        /// </code>
        /// </example>
		static Angle fromRadians(double radians)
		{
			Angle res;
			res.Radians = radians;	
			return res;
		}

		/// <summary>
		/// 给定一个角度创建一个新的Angle对象
		/// </summary>
        /// <example>
        /// <code>
        /// using S57Chart.Core; 
        /// 
        /// double angle=68;//角度
        /// Angle degree = Angle.FromDegrees(angle);
        /// //degree的弧度值是1.1868238913561442
        /// //degree的角度值为68.0
        /// </code>
        /// </example>
		static Angle fromDegrees(double degrees)
		{
			Angle res;
			res.Radians = PI * degrees / 180.0;
			return res;
		}

		double degrees()
		{
			return CMyChartArithmetic::radiansToDegrees(this->Radians);
		}
	};
}
#endif // MYCHARTARITHMETIC_H
