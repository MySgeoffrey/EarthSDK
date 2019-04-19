#ifndef MYEARTHSIM_H
#define MYEARTHSIM_H

#include <QtGui/QMainWindow>
#include <QMenu>
#include "QtGui/qdialog.h"
#include "GeneratedFiles/ui_myearthsim.h"
#include "GeneratedFiles/ui_graphictypeui.h"
#include "GeneratedFiles/ui_titleFrm.h"
#include "vr_globewidget.h"
#include "geometryeditor/polygoneditor.h"
#include "geoobject/geocircle.h"
#include "geoobject/geogrid.h"
#include "geoobject/geosegment.h"
#include "geoobject/geoscutcheon.h"
#include "scenetool/scenepicker.h"
#include "geographic/geopoint.h"
#include "geographic/geoline.h"
#include "geographic/geopolygon.h"
#include "geographic/geocirlce.h"
#include "scenetool/gridguieventhandle.h"

class IAreaToolListener;
class CMouseCoordsCallback;
class CCustomGUIEventHandler;
class TitleDlg;

class GraphicTypeDlg : public QDialog
{
public:
	GraphicTypeDlg(QWidget *parent = 0, Qt::WFlags flags = 0);

	~GraphicTypeDlg();

	QString getGraphicType();
private:
	Ui::GraphicTypeUI ui;
};

class myEarthSim : public QMainWindow
{
	Q_OBJECT

public:
	myEarthSim(QWidget *parent = 0, Qt::WFlags flags = 0);

	~myEarthSim();

	virtual void handlePickerEvent(Common::IObject* pResult);
	public slots:

		void initialFireEngineScene();

#if 1 //场景控件模块
		//创建并显示地球控件
		void slotShowEarthControl();
		//设置导航键位置
		void slotSetNavigationPosition();
		//飞行定位
		void slotFlyToTarget();
		//开启
		void slotActionStartGetMourseGeoPosition();
		//关闭
		void slotActionEndGetMourseGeoPosition();
		//GUI事件
		void slotActionGUIEventHandler();
		//状态栏的字体大小
		void slotActionStateTextSize();
		//获取相机的位置
		void slotActionGetCameraPosition();
		//根据两点计算方位角
		void slotActionComputeAzimuth();
		//设置相机的高度范围
		void slotActionSetCameraHeight();
		//设置相机的操作模式
		void slotActionSetCameraMode();
		//设置比例尺的位置
		void slotActionSetGlobeScalePosition();
		void initialMeatureStyle();

#endif
		
#if 1 //信息查询
		void slotStartPointQuery();
		void slotEndPointQuery();
#endif

#if 1 //图层管理模块
		void slotAddTMSImage();
		void slotAddTMSElevation();
		void slotAddGoogleImage();
		void slotUnVisibleTMSImage();
		void slotVisibleTMSImage();
		void slotUnVisibleGoogleImage();
		void slotVisibleGoogleImage();
		void slotUnVisibleTMSElevation();
		void slotVisibleTMSElevation();
		void slotAddCountryLineShp();
		void slotUnVisibleLineShp();
		void slotVisibleLineShp();
		void slotAddPolygonShp();
		void slotUnVisiblePolygonShp();
		void slotVisiblePolygonShp();
		void slotAddPointShp();
		void slotUnVisiblePointShp();
		void slotVisiblePointShp();
		//自定义瓦片
		void slotLoadCustomTile();
		void slotShowCustomTile();
		void slotHideCustomTile();

		//添加天地图影像
		void slotActionAddTDTImage();
		//添加本地dem
		void slotActionAddTifDem();

		//三维场景对象图层管理
		void slotActionCreateJFQLayer();
		void slotActionVisibleJFQLayer();
		void slotActionDeleteJFQLayer();
#endif

#if 1 //空间量算
		void slotBeginFlatDistance();
		void slotEndFlatDistance();
		void slotBeginSphereDistance();
		void slotEndSphereDistance();
		void slotBeginFlatArea();
		void slotEndFlatArea();
		void slotBeginSphereArea();
		void slotEndSphereArea();
		void slotBeginAzimuth();
		void slotEndAzimuth();
#endif

#if 1 //地理标绘模块
		void slotDrawCircle();
		//创建圆
		void slotActionCreateCircle();
		//销毁圆
		void slotActionDeleteCircle();
		//创建方形格子
		void slotActionCreateRectGrid();
		//销毁方形格子
		void slotActionDeleteRectGrid();
		//创建圆形格子
		void slotActionDeleteRingGrid();
		//销毁圆形格子
		void slotActionCreateRingGrid();
		//创建点对象
		void slotActionCreatePoint();
		//创建线对象
		void slotActionCreateLine();
		//创建面对象
		void slotActionCreatePolygon();
		//销毁点对象
		void slotActionDeletePoint();
		//销毁线对象
		void slotActionDeleteLine();
		//销毁面对象
		void slotActionDeletePolygon();
		//创建标牌
		void slotActionCreateScutcheon();
		//销毁标牌
		void slotActionDeleteScutcheon();
		//创建线段
		void slotActionCreateSegment();
		//销毁线段
		void slotActionDeleteSegment();
		//雷达扫描线
		void slotActionCreateRadarLine();
		void slotActionDeleteRadarLine();
		void slotActionModifyRadarColor();

#endif

#if 1 //编辑标绘
		void slotBeginEditorPoint();
		void slotEndEditorPoint();
		void slotBeginEditorLine();
		void slotEndEditorLine();
		void slotBeginEditorPolygon();
		void slotEndEditorPolygon();

		//开启地理图形标绘
		void slotActionBeginGraphicEdit();
		void slotActionBeginSelectedGraphicEdit();
		//关闭地理图形标绘
		void slotActionEndGraphicEdit();
#endif

#if 1 //态势显示
		void slotActionSetScutcheonFields();
		void slotAddDynamicObjects();
		void slotActionControlIconVisible();
		void slotActionControlScutVisible();
		void slotActionControlTrackLineVisible();
		void slotActionChangeTrackLineColor();
		void slotActionChangeTrackLineType();
		void slotActionChangeTrackLineWidth();
		void slotActionChangeTrackLineLength();
		void slotActionDirLineVisible();
		void slotActionChangeDirLineColor();
		void slotActionChangeDirLineType();
		void slotActionChangeDirLineWidth();
		void slotActionCircleTangentAngle();
		void slotActionPolygonTangentAngle();
		void slotActionComputeAngle();
		void slotActionComputeBanyCenter();
		void slotActionDynamicObjectVisible();
		void slotActionObjectVisibleByFacility();
		void slotActionChangeIconStyle();
		void slotActionChangeObjectIconStyle();
		void slotActionChangeScuStyle();
		void slotActionChangeObjecctScuStyle();
		void slotActionChangeTrackPoints();
		void slotActionChangeObjectTrackPoint();
		void slotActionChangeScuOffset();
		void slotActionChangeObjectScuShadow();
		void slotActionChangeScuShadow();
		void slotActionJudgePointInCircle();
		void slotActionJudgePointInGeometry();
		void slotActionEnableDragScu();
		void slotActionVisibleIconText();
		void slotActionVisibleObjectIconText();
		void slotActionVisibleScuLine();
		void slotActionChangeScuLineColor();
		void slotActionChangeScuLineType();
		void slotActionChangeScuLineWidth();
		//添加告警区域
		void slotAddWarnningRegion();
		void slotAddScanningRegion();
		void slotActionAddObjectListener();
		void slotActionDynamicObjectBlink();
#endif

		void getObjects();

private:
	Ui::myEarthSimClass ui;
	VRGlobeWidget* mpGlobeWidget;

	//界面

	TitleDlg* mpTitleDlg;


	//测量对象
	GeometryEditor::IGeometryEditor* mpGeometryEditor;

	//编辑对象
	GeometryEditor::IGeometryEditor* mpEditorObject;

	//地理对象绘制
	Geo::CGeoCircleEx* mpGeoRadarLine;
	Geo::CGeoRingGrid* mpGeoRingGrid;
	SceneTool::CGridMeasure* mpGeoRingGridMeature;
	Geo::CGeoRectGrid* mpGeoRectGrid;
	SceneTool::CGridMeasure* mpGeoRectGridMeature;
	Geo::CGeoScutcheon* mpGeoScutcheon;
	Geo::CGeoSegment* mpGeoSegment;

	//图形对象
	Graphic::CGeoCircle* mpGeoCircle;
	Graphic::CGeoPoint* mpGeoPoint;
	Graphic::CGeoLine* mpGeoLine;
	Graphic::CGeoPolygon* mpGeoPolygon;
	
	//信息查询
	SceneTool::CModelPickerTool* mpModelPicker;
	//鼠标地理位置监听
	CMouseCoordsCallback* mpMouseCoordsCallback;
	//界面GUI事件监听
	CCustomGUIEventHandler* mpCustomGUIEventHandler;
	//三类菜单
	QMenu* mpMenuMap;
	QMenu* mpMenuTarget;
	QMenu* mpMenuDevice;
};

class TitleDlg : public QDialog
{
	Q_OBJECT
public:
	TitleDlg(QWidget *parent = 0, Qt::WFlags flags = 0);

	~TitleDlg();

	void timerEvent ( QTimerEvent * event ); 

private slots:
	void slotExit();
	void slotFireSim();
	void slotDataSim();
	void slotAnalyse();
	void slotSetting();
public:
	myEarthSim* Owner;
private:
	Ui::TitleForm ui;
};

#endif // MYEARTHSIM_H
