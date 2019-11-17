#ifndef MYCHARTS57LAYER_H
#define MYCHARTS57LAYER_H

#include "mychartdrawlib/mychartdrawlib_global.h"
#include "mycharts57datalib/imys57layerdata.h"
#include "mychartmaplib/imylayer.h"

namespace MyChart
{
	class IMyS52Symbol;
	class IMyS52LayerSymbol;
	/// <summary>
	/// S57图层绘制类
	/// </summary>
	class MYCHARTDRAWLIB_EXPORT IMyS57Layer
		: public MyChart::IMyLayer
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		IMyS57Layer();

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">对象名称</param>
		IMyS57Layer(const QString& name);

		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~IMyS57Layer();

		/// <summary>
		/// 图层初始化
		/// </summary>
		virtual void initial();

		/// <summary>
		/// 获取图层的渲染优先级
		/// </summary>
		/// <returns>渲染优先级</returns>
		virtual int getRenerPriority();

		/// <summary>
		/// 给图层设置绘制数据
		/// </summary>
		/// <param name="pData">数据对象（内部引用）</param>
		virtual void setData(IMyS57LayerData* pData);

		/// <summary>
		/// 获取图层的绘制数据
		/// </summary>
		/// <returns>绘制数据</returns>
		virtual IMyS57LayerData* getData();

		/// <summary>
		/// 重置图层的绘制符号
		/// </summary>
		virtual void resetSymbol();

		/// <summary>
		/// 获取图层的要素集合
		/// </summary>
		/// <returns>要素集合</returns>
		virtual std::vector<IMyShapeRecord*>& getRecords();

		/// <summary>
		/// 获取图层的属性集合
		/// </summary>
		/// <returns>属性集合</returns>
		virtual IMyProperty* getProperty();

		
		/// <summary>
		/// 获取图层的几何类型
		/// </summary>
		/// <returns>几何类型</returns>
		virtual GeometryType getType();

		/// <summary>
		/// 获取图层的类型
		/// </summary>
		/// <returns>类型</returns>
		virtual QString getLayerType();

		/// <summary>
		/// 获取图层的最新绘制的结果
		/// </summary>
		/// <returns>最新绘制的结果</returns>
		virtual QPixmap& getImage();

		/// <summary>
		/// 绘制图层接口
		/// </summary>
		/// <param name="pArgs">当前绘制参数</param>
		virtual bool draw(IMyDrawArgs* pArgs);

		/// <summary>
		/// 绘制图层文本接口
		/// </summary>
		/// <param name="currentTextRects">当前文本绘制矩形集合</param>
		/// <param name="pArgs">当前绘制参数</param>
		virtual bool drawText(std::vector<QRect>& currentTextRects,IMyDrawArgs* pArgs);

	protected:

		virtual bool drawRecord(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,IMyDrawArgs* pArgs);
		virtual bool drawPointRecord(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,IMyDrawArgs* pArgs);
		virtual bool drawMultiPointRecord(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,IMyDrawArgs* pArgs);
		virtual bool drawPolylineRecord(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,IMyDrawArgs* pArgs);
		virtual bool drawPolygonRecord(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,IMyDrawArgs* pArgs);
		
		virtual bool drawRecordText(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,std::vector<QRect>& currentTextRects,IMyDrawArgs* pArgs);
		virtual bool drawPointRecordText(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,std::vector<QRect>& currentTextRects,IMyDrawArgs* pArgs);
		virtual bool drawMultiPointRecordText(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,std::vector<QRect>& currentTextRects,IMyDrawArgs* pArgs);
		virtual bool drawPolylineRecordText(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,std::vector<QRect>& currentTextRects,IMyDrawArgs* pArgs);
		virtual bool drawPolygonRecordText(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,std::vector<QRect>& currentTextRects,IMyDrawArgs* pArgs);
		virtual bool drawText(QString& text,QRect& rect,QFont& font,std::vector<QRect>& currentTextRects,IMyDrawArgs* pArgs);

		virtual QImage symbolShow(QString codeOfTargetClass);
		virtual QPoint getDirectionBetweenCenterAndPivot(QString codeOfTargetClass);
	protected:
		IMyS57LayerData* mpData;
		IMySymbol* mpSymbol;
	};
}
#endif // MYCHARTS57LAYER_H
