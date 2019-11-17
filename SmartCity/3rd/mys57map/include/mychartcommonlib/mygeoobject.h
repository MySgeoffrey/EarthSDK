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
	/// 地图绘制参数类
	/// </summary>
	class MYCHARTCOMMONLIB_EXPORT IMyDrawArgs
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		IMyDrawArgs();

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IMyDrawArgs();

		/// <summary>
		/// 获取绘制范围
		/// </summary>
		/// <returns>地图绘制范围</returns>
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
	/// 地理对象基类
	/// </summary>
	class MYCHARTCOMMONLIB_EXPORT IMyGeoObject
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		IMyGeoObject();

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">对象名称</param>
		IMyGeoObject(const QString& name);

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IMyGeoObject();

		/// <summary>
		/// 获取地理对象名称
		/// </summary>
		/// <returns>地理对象名称</returns>
		QString getName();

		/// <summary>
		/// 获取地理对象最大纬度
		/// </summary>
		/// <returns>最大纬度</returns>
		double getNorth();

		/// <summary>
		/// 获取地理对象最小纬度
		/// </summary>
		/// <returns>最小纬度</returns>
		double getSouth();

		/// <summary>
		/// 获取地理对象最大经度
		/// </summary>
		/// <returns>最大经度</returns>
		double getEast();

		/// <summary>
		/// 获取地理对象最小经度
		/// </summary>
		/// <returns>最小经度</returns>
		double getWest();

		/// <summary>
		/// 获取地理对象外包围盒
		/// </summary>
		/// <returns>地理外包围盒</returns>
		OGREnvelope& getEnvelope();

		/// <summary>
		/// 执行点选 
		/// </summary>
		/// <param name="screenPosX">屏幕X坐标</param>
		/// <param name="screenPosY">屏幕Y坐标</param>
		/// <param name="pDrawArgs">绘图参数</param>
		/// <returns>执行点选结果</returns>
		virtual IMyQueryResult* excutePointSelection(
			const float& screenPosX,const float& screenPosY,
			IMyDrawArgs* pDrawArgs);

	private:
		OGREnvelope mEnvelope;
		QString mName;
	};


}

#endif // MYCHARTOBJECT_H
