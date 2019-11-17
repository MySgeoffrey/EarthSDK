#ifndef MYCHARTOBJECT_H
#define MYCHARTOBJECT_H

#include "mychartcommonlib/mychartcommonlib_global.h"
#include "ogr_core.h"
#include <QtGui/QPainter>
#include <QtGui/qpaintdevice.h>
#include "mychartcommonlib/mychartcoorsystem.h"
#include <vector>
#include <qrect.h>

namespace MyChart
{
	class MYCHARTCOMMONLIB_EXPORT IMyQueryResult
	{
	public:
		IMyQueryResult()
		{}

		virtual ~IMyQueryResult()
		{}
	};

	class MYCHARTCOMMONLIB_EXPORT IMyQueryListener
	{
	public:
		IMyQueryListener()
		{}

		virtual ~IMyQueryListener()
		{}

		virtual void onQueryEvent(std::vector<IMyQueryResult*>& pResult)
		{
			
		}
	};

	/// <summary>
	/// ��ͼ���Ʋ�����
	/// </summary>
	class MYCHARTCOMMONLIB_EXPORT IMyDrawArgs
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		IMyDrawArgs();

		/// <summary>
		/// ��������
		/// </summary>
		virtual ~IMyDrawArgs();

		/// <summary>
		/// ��ȡ���Ʒ�Χ
		/// </summary>
		/// <returns>��ͼ���Ʒ�Χ</returns>
		virtual OGREnvelope& getExtend()
		{
			if (!this->Envelope.IsInit())
			{
				this->Envelope.MaxX = geoRightDownX;
				this->Envelope.MinX = geoLeftUpX;
				this->Envelope.MaxY = geoLeftUpY;
				this->Envelope.MinY = geoRightDownY;
			}
			return this->Envelope;
		}

	public:
		int Width;
		int Height;
		double Scale;
		double prjLeftUpX;
		double prjLeftUpY;
		double prjRightDownX;
		double prjRightDownY;
		double geoLeftUpX;
		double geoLeftUpY;
		double geoRightDownX;
		double geoRightDownY;
		MyChart::EMyCoorsystemType CoorsystemType;
		OGREnvelope Envelope;
		QPainter* Painter;
	};

	/// <summary>
	/// ����������
	/// </summary>
	class MYCHARTCOMMONLIB_EXPORT IMyGeoObject
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		IMyGeoObject();

		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="name">��������</param>
		IMyGeoObject(const QString& name);

		/// <summary>
		/// ��������
		/// </summary>
		virtual ~IMyGeoObject();

		/// <summary>
		/// ��ȡ�����������
		/// </summary>
		/// <returns>�����������</returns>
		QString getName();

		/// <summary>
		/// ��ȡ����������γ��
		/// </summary>
		/// <returns>���γ��</returns>
		double getNorth();

		/// <summary>
		/// ��ȡ���������Сγ��
		/// </summary>
		/// <returns>��Сγ��</returns>
		double getSouth();

		/// <summary>
		/// ��ȡ���������󾭶�
		/// </summary>
		/// <returns>��󾭶�</returns>
		double getEast();

		/// <summary>
		/// ��ȡ���������С����
		/// </summary>
		/// <returns>��С����</returns>
		double getWest();

		/// <summary>
		/// ��ȡ����������Χ��
		/// </summary>
		/// <returns>�������Χ��</returns>
		OGREnvelope& getEnvelope();

		/// <summary>
		/// ִ�е�ѡ 
		/// </summary>
		/// <param name="screenPosX">��ĻX����</param>
		/// <param name="screenPosY">��ĻY����</param>
		/// <param name="pDrawArgs">��ͼ����</param>
		/// <returns>ִ�е�ѡ���</returns>
		virtual IMyQueryResult* excutePointSelection(
			const float& screenPosX,const float& screenPosY,
			IMyDrawArgs* pDrawArgs);

	private:
		OGREnvelope mEnvelope;
		QString mName;
	};


}

#endif // MYCHARTOBJECT_H
