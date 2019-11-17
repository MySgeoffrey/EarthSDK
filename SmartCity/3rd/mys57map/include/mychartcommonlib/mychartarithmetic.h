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
	/// ��ѧ�㷨�ӿ���
	/// </summary>
	class MYCHARTCOMMONLIB_EXPORT CMyChartArithmetic
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		CMyChartArithmetic();

		/// <summary>
		/// ��������
		/// </summary>
		~CMyChartArithmetic();

		/// <summary>
		/// ��λ�Ǽ���ӿ�
		/// </summary>
		/// <param name="latA">��һ�����γ��</param>
		/// <param name="lonA">��һ����ľ���</param>
		/// <param name="latB">�ڶ������γ��</param>
		/// <param name="lonB">�ڶ�����ľ���</param>
		/// <returns></returns>
		static Angle azimuth(Angle latA, Angle lonA, Angle latB, Angle lonB);

		/// <summary>
		/// ����ת�Ƕ�
		/// </summary>
		/// <param name="radians">����ֵ</param>
		/// <returns>�Ƕ�ֵ</returns>
		static double radiansToDegrees(double radians)
		{
			return  radians * 180.0 / PI;
		}

		/// <summary>
		/// �Ƕ�ת����
		/// </summary>
		/// <param name="degrees">�Ƕ�ֵ</param>
		/// <returns>����ֵ</returns>
		static double degreesToRadians(double degrees)
		{
			return PI * degrees / 180.0f;
		}

	public:
		
	};

	/// <summary>
	/// �Ƕ���
	/// </summary>
	struct Angle
	{
	public:
		/// <summary>
		/// ���صĻ�����
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
		/// ����һ�����ȴ���һ���µ�Angle����
		/// </summary>
        /// <example>
        /// <code>
        /// using S57Chart.Core; 
        /// 
        /// double angle=1.2;//����ֵ
        /// Angle degree= Angle.FromRadians(angle);
        /// //degree�ĽǶ�ֵ��68.754935415698
        /// //degree�Ļ���ֵ��1.2
        /// </code>
        /// </example>
		static Angle fromRadians(double radians)
		{
			Angle res;
			res.Radians = radians;	
			return res;
		}

		/// <summary>
		/// ����һ���Ƕȴ���һ���µ�Angle����
		/// </summary>
        /// <example>
        /// <code>
        /// using S57Chart.Core; 
        /// 
        /// double angle=68;//�Ƕ�
        /// Angle degree = Angle.FromDegrees(angle);
        /// //degree�Ļ���ֵ��1.1868238913561442
        /// //degree�ĽǶ�ֵΪ68.0
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
