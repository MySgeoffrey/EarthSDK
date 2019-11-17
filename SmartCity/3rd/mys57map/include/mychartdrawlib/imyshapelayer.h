#ifndef MYSHAPELAYER_H
#define MYSHAPELAYER_H

#include "mychartdrawlib/mychartdrawlib_global.h"
#include "mycharts57datalib/imys57layerdata.h"
#include "mychartmaplib/imylayer.h"
#include "mychartdrawlib/imys57map.h"
#include "mychartdrawlib/imys57layer.h"

namespace MyChart
{
	class MYCHARTDRAWLIB_EXPORT IMyShapeMap
		: public IMyS57Map
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		IMyShapeMap();

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">对象名称</param>
		/// <param name="pData">绘制数据对象</param>
		IMyShapeMap(const QString& name, IMyS57MapData* pData);

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">对象名称</param>
		IMyShapeMap(const QString& name);

	protected:
		virtual bool constructDrawLayer();
	};

	class MYCHARTDRAWLIB_EXPORT IMyShapeLayer
		: public MyChart::IMyS57Layer
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		IMyShapeLayer();

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">对象名称</param>
		IMyShapeLayer(const QString& name);

		/// <summary>
		/// 绘制图层接口
		/// </summary>
		/// <param name="pArgs">当前绘制参数</param>
		virtual bool draw(IMyDrawArgs* pArgs);

		virtual void initial();

		virtual IMySymbol* getSymbol();

	protected:

		virtual bool drawPointRecord(IMyShapeRecord* pRecord, IMyS52Symbol* pSymbol, IMyDrawArgs* pArgs);

		virtual bool drawPolylineRecord(IMyShapeRecord* pRecord, IMyS52Symbol* pSymbol, IMyDrawArgs* pArgs);

		virtual bool drawPolygonRecord(IMyShapeRecord* pRecord, IMyS52Symbol* pSymbol, IMyDrawArgs* pArgs);

	protected:
		//IMySymbol* mpSymbol;
	};
}
#endif // MYCHARTS57LAYER_H
