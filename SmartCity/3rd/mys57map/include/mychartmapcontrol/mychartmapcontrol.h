#ifndef MYCHARTMAPCONTROL_H
#define MYCHARTMAPCONTROL_H

#include <QtWidgets/QWidget>
#include <QTime>
#include "mychartmapcontrol/mychartmapcontrol_global.h"
#include "mychartcommonlib/mychartcoorsystem.h"
#include "ogr_core.h"
#include "mychartmaplib/imymap.h"
#include "mychartmaplib/imygridlayer.h"

/// <summary>
/// 地图操作类型枚举
/// </summary>
enum EMapOperateType
{
	MOT_PAN,///<漫游
	MOT_ZOOMIN,///<放大
	MOT_ZOOMOUT,///<缩小
	MOT_ZOOMMAPIMAGE,///<图片缩放
	MOT_MAPEDIT,///<地图编辑
	MOT_POINTSELECT,///<点选 
	MOT_POLYGONSELECT,///<多边形选
	MOT_NONE///<无定义
};

/// <summary>
/// 地图控件
/// </summary>
class MYCHARTMAPCONTROL_EXPORT MyChartMapControl : public QWidget
{
	Q_OBJECT

public:
	/// <summary>
	/// 地图控件构造函数
	/// </summary>
	MyChartMapControl(QWidget *parent = 0);

	/// <summary>
	/// 地图控件析构函数
	/// </summary>
	~MyChartMapControl();

	/// <summary>
	/// 设置当前地图绘制的坐标系统类型
	/// </summary>
	/// <param name="coorsystemType">坐标系统类型</param>
	void setCoorsystemType(const MyChart::EMyCoorsystemType& coorsystemType);

	/// <summary>
	/// 获取当前地图绘制的坐标系统类型
	/// </summary>
	/// <returns>坐标系统类型</returns>
	MyChart::EMyCoorsystemType getCoorsystemType();

	/// <summary>
	/// 获取当前地图绘制的范围
	/// </summary>
	/// <returns>当前地图绘制的范围</returns>
	virtual OGREnvelope& getExtend();

	/// <summary>
	/// 获取当前地图绘制的比例尺
	/// </summary>
	/// <returns>当前地图绘制的比例尺</returns>
	virtual double getCurScale();

	/// <summary>
	/// 获取当前地图的距离网格图层
	/// </summary>
	/// <returns>距离网格图层</returns>
	virtual MyChart::IMyGridLayer* getGridLayer();

	/// <summary>
	/// 设置当前地图绘制的操作类型
	/// </summary>
	/// <param name="type">操作类型</param>
	void setOperateType(const EMapOperateType& type);

	/// <summary>
	/// 获取当前地图绘制的操作类型
	/// </summary>
	/// <returns>操作类型</returns>
	EMapOperateType getOperateType();

	/// <summary>
	/// 保存屏幕至本地
	/// </summary>
	/// <param name="path">图片路径</param>
	bool saveImage(const QString& path);

	/// <summary>
	/// 添加查询监听
	/// </summary>
	/// <param name="pRefListener">查询监听</param>
	void addQueryListener(MyChart::IMyQueryListener* pRefListener);

	/// <summary>
	/// 移除查询监听
	/// </summary>
	/// <param name="pRefListener">查询监听</param>
	void removeQueryListener(MyChart::IMyQueryListener* pRefListener);

	/// <summary>
	/// 移除所有查询监听
	/// </summary>
	void removeAllQueryListeners();

	/// <summary>
	/// 通知查询监听，且返回当前的查询结果
	/// </summary>
	void notifyQueryListeners(std::vector<MyChart::IMyQueryResult*>& result);

	/// <summary>
	/// 设置当前地图绘制的背景颜色
	/// </summary>
	/// <param name="color">背景颜色</param>
	void setBackgroundColor(const QColor& color);

	/// <summary>
	/// 获取当前地图绘制的背景颜色
	/// </summary>
	/// <returns>背景颜色</returns>
	QColor getBackgroundColor();
	
	/// <summary>
	/// 添加地图
	/// </summary>
	/// <param name="pRefMap">地图对象（内部引用）</param>
	bool addMap(MyChart::IMyMap* pRefMap);

	/// <summary>
	/// 根据地图名称获取地图对象
	/// </summary>
	/// <param name="name">地图名称</param>
	/// <returns>地图对象</returns>
	MyChart::IMyMap* getMap(const QString& name);

	/// <summary>
	/// 设置地图声明当前操作工具
	/// </summary>
	/// <param name="pActiveTool">当前操作工具</param>
	void setActiveTool(MyChart::IMyTool* pActiveTool);

	/// <summary>
	/// 获取地图声明当前操作工具
	/// </summary>
	/// <returns>当前操作工具</returns>
	MyChart::IMyTool* getActiveTool();

	/// <summary>
	/// 通知地图控件当前工具操作发生变化
	/// </summary>
	void notifyActiveToolChanged();

	/// <summary>
	/// 通知地图控件重绘活动地图
	/// </summary>
	void notifyRedrawActiveLayer();

	/// <summary>
	/// 当前是否执行拖动操作
	/// </summary>
	/// <returns>是否执行拖动操作</returns>
	bool isMouseDragging();

	/// <summary>
	/// 更新当前地图集的包围盒
	/// </summary>
	void updateBounds();

	/// <summary>
	/// 全图显示
	/// </summary>
	void fullExtend();

	/// <summary>
	/// 刷新地图
	/// </summary>
	void refresh();

	/// <summary>
	/// 获取地图初始状态下的包围盒
	/// </summary>
	/// <returns>初始状态下的包围盒</returns>
	virtual OGREnvelope& getInitialExtend();

	/// <summary>
	/// 设置地图初始状态下的包围盒
	/// </summary>
	/// <param name="extend">初始状态下的包围盒</param>
	virtual void setInitialExtend(const OGREnvelope& extend);

	/// <summary>
	/// 计算屏幕坐标转成当前投影坐标系下的坐标
	/// </summary>
	/// <param name="screenCoor">屏幕坐标</param>
	/// <param name="prjX">X坐标</param>
	/// <param name="prjY">Y坐标</param>
	void screenCoor2PrjCoor(const QPointF& screenCoor,
		double& prjX,double& prjY);

	/// <summary>
	/// 控件重绘事件
	/// </summary>
	/// <param name="evt">绘制参数</param>
	virtual void paintEvent(QPaintEvent * evt);

	/// <summary>
	/// 鼠标按下事件
	/// </summary>
	/// <param name="evt">参数</param>
	virtual void mousePressEvent(QMouseEvent * evt);

	/// <summary>
	/// 鼠标双击事件
	/// </summary>
	/// <param name="evt">参数</param>
	virtual void mouseDoubleClickEvent(QMouseEvent* evt);
	
	/// <summary>
	/// 鼠标释放事件
	/// </summary>
	/// <param name="evt">参数</param>
	virtual void mouseReleaseEvent(QMouseEvent * evt);

	/// <summary>
	/// 鼠标移动事件
	/// </summary>
	/// <param name="evt">参数</param>
	virtual void mouseMoveEvent(QMouseEvent * evt);

	/// <summary>
	/// 鼠标滚轮事件
	/// </summary>
	/// <param name="evt">参数</param>
	virtual void wheelEvent(QWheelEvent * evt);

	/// <summary>
	/// 控件重置事件
	/// </summary>
	/// <param name="evt">参数</param>
	virtual void resizeEvent(QResizeEvent * evt);

	/// <summary>
	/// 按键按下事件
	/// </summary>
	/// <param name="evt">参数</param>
	virtual void keyPressEvent(QKeyEvent * event);

	/// <summary>
	/// 按键释放事件
	/// </summary>
	/// <param name="evt">参数</param>
	virtual void keyReleaseEvent(QKeyEvent * event);

	/// <summary>
	/// 控件时钟事件
	/// </summary>
	/// <param name="evt">参数</param>
	virtual void timerEvent(QTimerEvent* event);

	/// <summary>
	/// 绘制比例尺
	/// </summary>
	/// <param name="evt">参数</param>
	virtual void drawScale(QPainter& painter);

	/// <summary>
	/// 设置比例尺的显隐
	/// </summary>
	/// <param name="visible">显隐</param>
	virtual void setScaleVisible(const bool& visible);

	/// <summary>
	/// 获取比例尺的显隐
	/// </summary>
	virtual bool getScaleVisible();
private:
	void panMap();
	void zoomInMap();
	void zoomOutMap();
	void computeDrawExtend();
	void reComputeDrawExtend(const QPointF& lastPostion,
		const QPointF& curPostion);
	void scaleMapAtPoint(const QPointF& curMousePos,const double& scale);
	void computeCurPrjPosition(const QPointF& pos);
	void excutePointSelection(const QPointF& curMousePos);

	bool drawMap(const int& width,const int& height);
	void drawMap(QPainter& painter);
	void drawZoomShape(QPainter& painter);
	void reDrawActiveMap(QPainter& painter);
	void notifyUpdate(QPainter& painter);
	void drawMapScale(QPainter& painter);
	void drawPanMap(QPainter& painter);
	void drawZoomImage(QPainter& painter);
	void drawEditMap(QPainter& painter);
protected:
	OGREnvelope mInitialExtend;
	OGREnvelope mGeoExtend;
	OGREnvelope mPrjExtend;
	QColor mBackgroundColor;
	QPixmap mBackRenderImage;
	QPixmap mEditRenderImage;
	QPixmap mTopRenderImage;
	QPixmap mRenderBufferImage;
	bool mIsMousePress;
	bool mIsMouseDragging;
	bool mNeedRedrawMap;
	QPointF mMouseLastPos;
	QPointF mMoveDirection;
	EMapOperateType mCurMapOperateType;
	EMapOperateType mLastMapOperateType;
	QPointF mLeftTopPrjPos;
	QPointF mRightBottomPrjPos;
	QPointF mCurMousePrjPos;
	double mCurScale;
	double mCurScaleFactor;
	QTime mScaleTimer; 
	QPointF mLastLeftTopPrjPos;
	QDateTime mLastWheelTime;
	bool mScaleVisible;
	MyChart::EMyCoorsystemType mCoorsystemType;
	int mTimerID;
	MyChart::IMyTool* mpRefTool;
	bool mActiveToolChanged;
	bool mRedrawActiveLayer;
	MyChart::IMyGridLayer* mpGridLayer;
	MyChart::IMyDrawArgs mLastDrawArgs;
	std::vector<MyChart::IMyQueryListener*> mQueryListeners;
};

#endif // MYCHARTMAPCONTROL_H
