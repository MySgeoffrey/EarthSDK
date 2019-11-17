#ifndef MYCHARTS57MAP_H
#define MYCHARTS57MAP_H

#include "mychartdrawlib/mychartdrawlib_global.h"
#include "mycharts57datalib/imys57mapdata.h"

namespace MyChart
{
	typedef std::vector<IMyLayer*> Layers;
	/// <summary>
	/// S57海图绘制类
	/// </summary>
	class MYCHARTDRAWLIB_EXPORT IMyS57Map
		: public IMyMap
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		IMyS57Map();

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">对象名称</param>
		/// <param name="pData">绘制数据对象</param>
		IMyS57Map(const QString& name,IMyS57MapData* pData);

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">对象名称</param>
		IMyS57Map(const QString& name);

		/// <summary>
		/// 图层初始化
		/// </summary>
		virtual void initial();

		/// <summary>
		/// 给海图设置绘制数据
		/// </summary>
		/// <param name="pData">数据对象（内部引用）</param>
		virtual void setData(IMyS57MapData* pData);

		/// <summary>
		/// 获取图层的绘制数据
		/// </summary>
		/// <returns>绘制数据</returns>
		virtual IMyS57MapData* getData();

		/// <summary>
		/// 重置海图的绘制符号
		/// </summary>
		virtual void resetSymbol();

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IMyS57Map();

		/// <summary>
		/// 获取海图的最新绘制的结果
		/// </summary>
		/// <returns>最新绘制的结果</returns>
		virtual QPixmap& getImage();

		/// <summary>
		/// 绘制图层接口
		/// </summary>
		/// <param name="pArgs">当前绘制参数</param>
		virtual bool draw(IMyDrawArgs* pArgs);

	protected:
		virtual bool drawGraphic(Layers& layers,IMyDrawArgs* pArgs);

		virtual bool drawText(Layers& layers,std::vector<QRect> currentTextRects,IMyDrawArgs* pArgs);

		virtual bool constructDrawLayer();

	protected:
		IMyS57MapData* mpS57Data;
		Layers mPolygonLayers;
		Layers mPolylineLayers;
		Layers mPointLayers;
		QPixmap mImage;
	};

}

#endif // MYCHARTS57MAP_H
