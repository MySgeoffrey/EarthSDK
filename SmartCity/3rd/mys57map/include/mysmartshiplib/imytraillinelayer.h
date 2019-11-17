#ifndef _IMYTRAILLINELAYER_H
#define _IMYTRAILLINELAYER_H

#include "mysmartshiplib/mysmartshiplib_global.h"
#include "mychartmaplib/imylayer.h"
#include "mychartmapcontrol/mychartmapcontrol.h"
#include "mysmartshiplib/imytrailline.h"
#include "mysmartshiplib/GPRMC.h"
#include <map>

namespace MyChart
{
	class MYSMARTSHIPLIB_EXPORT IMyTrailLineSymbol 
		: public IMyLineSymbol
	{
	public:
		IMyTrailLineSymbol();

		virtual ~IMyTrailLineSymbol();

		virtual void setNeedDrawTrailPoint(const bool& needDrawTrailPoint);

		virtual bool getNeedDrawTrailPoint();

		virtual IMyPointSymbol* getTrailPointSymbol();

	private:
		IMyPointSymbol* mpTrailPointSymbol;
		bool mNeedDrawTrailPoint;
	};

	class MYSMARTSHIPLIB_EXPORT IMyTrailQueryResult
		: public IMyQueryResult
	{
	public:
		IMyTrailQueryResult()
		{}

		virtual ~IMyTrailQueryResult()
		{
		
		}

	public:
		GPS_DATA Data;
	};

	class MYSMARTSHIPLIB_EXPORT IMyPolyLineLayer
		: public IMyLayer
	{
	public:
		IMyPolyLineLayer();

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">对象名称</param>
		IMyPolyLineLayer(const QString& name);

		virtual ~IMyPolyLineLayer();

		virtual QPixmap& getImage() ;

		virtual IMySymbol* getSymbol();

		virtual IMyTrailShapeRecord* createRecord();

		/// <summary>
		/// 绘制图层接口
		/// </summary>
		/// <param name="pArgs">当前绘制参数</param>
		virtual bool draw(IMyDrawArgs* pArgs);

		virtual void update();

		/// <summary>
		/// 执行点选 
		/// </summary>
		/// <param name="screenPosX">屏幕X坐标</param>
		/// <param name="screenPosY">屏幕Y坐标</param>
		/// <param name="pDrawArgs">绘图参数</param>
		/// <returns>执行点选结果</returns>
		IMyQueryResult* excutePointSelection(
			const float& screenPosX,const float& screenPosY,
			IMyDrawArgs* pDrawArgs);

		int getPointIndex(const float& screenPosX,const float& screenPosY);

		void updatePointByIndex(const int& pointIndex,MyChart::IVector3& position);

		/// <summary>
		/// 将图层数据保存至本地文件 
		/// </summary>
		/// <param name="filePath">文件路径</param>
		/// <returns>是否保存成功</returns>
		bool toFile(const QString& filePath);

		bool toTxtFile(const QString& filePath);

		bool fromTxtFile(const QString& filePath);

		bool toDxfFile(const QString& filePath);

		bool fromDxfFile(const QString& filePath);

		/// <summary>
		/// 将图层数据保存至本地文件 
		/// </summary>
		/// <param name="filePath">文件路径</param>
		/// <returns>是否保存成功</returns>
		bool fromFile(const QString& filePath);

	protected:
		IMyTrailLineSymbol* mpSymbol;
		std::map<int,QPointF> mLastRenderPoints;
		int mCurrentRecordIndex; 
	};

	class MYSMARTSHIPLIB_EXPORT IMyPointLayer
		: public IMyPolyLineLayer
	{
	public:
		IMyPointLayer();

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">对象名称</param>
		IMyPointLayer(const QString& name);

		virtual ~IMyPointLayer();

		/// <summary>
		/// 绘制图层接口
		/// </summary>
		/// <param name="pArgs">当前绘制参数</param>
		virtual bool draw(IMyDrawArgs* pArgs);

		virtual IMyTrailShapeRecord* createRecord();
	};

	class MYSMARTSHIPLIB_EXPORT IMyPolyLineLayerEditor
		: public MyChart::IMyTool
	{
	public:
		IMyPolyLineLayerEditor();

		virtual ~IMyPolyLineLayerEditor();

		void setMapContorl(MyChartMapControl* pRefControl);

		void setEditingLayer(IMyPolyLineLayer* pRefLayer);

		virtual IMyTrailShapeRecord* createTrailShapeRecord();

		void setEditRecord(IMyTrailShapeRecord* pRefRecord);

		IMyTrailShapeRecord* getEditRecord();

		/// <summary>
		/// 事件处理之前的回调接口（子类重写）
		/// </summary>
		/// <param name="evt">正在发生的事件</param>
		virtual bool onBeforeEvent(QEvent* evt);

		/// <summary>
		/// 事件处理之后的回调接口（子类重写）
		/// </summary>
		/// <param name="evt">正在发生的事件</param>
		virtual bool onAfterEvent(QEvent* evt);

		/// <summary>
		/// 绘制事件处理之前的回调接口（子类重写）
		/// </summary>
		/// <param name="evt">正在发生的事件</param>
		virtual bool onBeforePaintEvent(IMyDrawArgs* pArgs);

		/// <summary>
		/// 绘制事件处理之后的回调接口（子类重写）
		/// </summary>
		/// <param name="evt">正在发生的事件</param>
		virtual bool onAfterPaintEvent(IMyDrawArgs* pArgs);

		virtual void beginEdit();

		virtual void endEdit();

		virtual bool isNeedMovePoint();

		virtual void setNeedMovePoint(const bool& value);

	protected:
		bool mIsOver;
		bool mIsMovePoint;
		bool mNeedMovePoint;
		int mLastHoverPointIndex;
		bool mNeedReStart;
		IMyPolyLineLayer* mpRefLayer;
		MyChartMapControl* mpRefControl;
		IMyTrailShapeRecord* mpRefShapeRecord;
	};

	class MYSMARTSHIPLIB_EXPORT IMyDistanceMeatureTool
		: public IMyPolyLineLayerEditor,public IMyPolyLineLayer
	{
	public:
		IMyDistanceMeatureTool();

		IMyDistanceMeatureTool(const QString& name);

		virtual ~IMyDistanceMeatureTool();

		/// <summary>
		/// 绘制图层接口
		/// </summary>
		/// <param name="pArgs">当前绘制参数</param>
		virtual bool draw(IMyDrawArgs* pArgs);

		virtual void beginEdit();

		virtual void endEdit();

	};

	class MYSMARTSHIPLIB_EXPORT IMyPolygonLayer
		: public IMyPolyLineLayer
	{
	public:
		IMyPolygonLayer();

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="name">对象名称</param>
		IMyPolygonLayer(const QString& name);

		virtual ~IMyPolygonLayer();

		/// <summary>
		/// 绘制图层接口
		/// </summary>
		/// <param name="pArgs">当前绘制参数</param>
		virtual bool draw(IMyDrawArgs* pArgs);
	};
	

	class MYSMARTSHIPLIB_EXPORT IMyPointLayerEditor
		: public IMyPolyLineLayerEditor
	{
	public:
		IMyPointLayerEditor();

		virtual ~IMyPointLayerEditor();


		/// <summary>
		/// 事件处理之前的回调接口（子类重写）
		/// </summary>
		/// <param name="evt">正在发生的事件</param>
		virtual bool onBeforeEvent(QEvent* evt);
	};

}

#endif //_IMYTRAILLINELAYER_H