#ifndef MYCHARTCOMMONLIB_H
#define MYCHARTCOMMONLIB_H

#include "mychartcommonlib/mychartcommonlib_global.h"

namespace MyChart
{
	/// <summary>
	/// ���������ӿ���
	/// </summary>
	class MYCHARTCOMMONLIB_EXPORT CMyChartCommonlib
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		CMyChartCommonlib();


		/// <summary>
		/// ��������
		/// </summary>
		~CMyChartCommonlib();

	public:
		static QString ColorMode;///<��ɫģʽ

		static int SafeContour;///<��ȫ������

		static bool IsShowDepthPoint;///<�Ƿ���ʾ�����

		static bool IsEnglishTextShow;///<�Ƿ���ʾӢ�ı�ע

		static bool IsShowMetaData;///<�Ƿ���ʾԪ���

		static QString ModeOfShowLayer;///<ͼ����ʾģʽ

		static QString AreaModeOfSymbol;///<��״���ŷ��ģʽ

		static QString PointModeOfSymbol;///<����ŷ��ģʽ
	};
}
#endif // MYCHARTCOMMONLIB_H
