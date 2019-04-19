#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QtGui/QWidget>
#include <QTreeWidget>
#include "qmenu.h"
#include "qevent.h"
#include <osgEarth/ImageLayer>
#include <osgEarth/ElevationLayer>
#include <vr_globewidget.h>
#include <osgEarthUtil/ObjectLocator>
#include <osgEarth/ModelLayer>
#include "framework/tool/itoollistener.h"
#include "geometryeditor/geometryeditor.h"
#include "vr_layermanagerconfig.h"

#pragma execution_character_set("utf-8")
/// <summary>
/// 图层Item
/// </summary>
class VR_LAYERMANAGER_DLL VRLayerItem : public QTreeWidgetItem
{
public :
	VRLayerItem(){};
	~VRLayerItem(){};
};
/// <summary>
/// 本地影像图层组Item
/// </summary>
class VR_LAYERMANAGER_DLL VRLocalImageGroupItem : public QTreeWidgetItem
{
public:
	VRLocalImageGroupItem(){};
	~VRLocalImageGroupItem(){};
};
/// <summary>
/// 本地高程图层组Item
/// </summary>
class VR_LAYERMANAGER_DLL VRLocalDemGroupItem : public QTreeWidgetItem
{
public:
	VRLocalDemGroupItem(){};
	~VRLocalDemGroupItem(){};
};
/// <summary>
/// 本地矢量图层组Item
/// </summary>
class VR_LAYERMANAGER_DLL VRLocalVectorGroupItem : public QTreeWidgetItem
{
public:
	VRLocalVectorGroupItem(){};
	~VRLocalVectorGroupItem(){};
};
/// <summary>
/// 本地矢量图层Item
/// </summary>
class VR_LAYERMANAGER_DLL VRLocalVectorLayerItem : public QTreeWidgetItem
{
public:
	VRLocalVectorLayerItem(){};
	~VRLocalVectorLayerItem(){};
public:
	osgEarth::ImageLayer* VectorLayer;
};
/// <summary>
/// 网络影像图层Item
/// </summary>
class VR_LAYERMANAGER_DLL VRWebImageLayerItem : public QTreeWidgetItem
{
public :
	VRWebImageLayerItem()
	{
		WebImageLayer = NULL;
	};
	~VRWebImageLayerItem()
	{
	
	};

public:
	osgEarth::ImageLayer* WebImageLayer;
};
/// <summary>
/// 网络DEM图层Item
/// </summary>
class VR_LAYERMANAGER_DLL VRWebDemLayerItem : public QTreeWidgetItem
{
public :
	VRWebDemLayerItem(){ WebElevationLayer = NULL;};
	~VRWebDemLayerItem(){};

public:
	osgEarth::ElevationLayer* WebElevationLayer;
};

/// <summary>
/// 本地影像图层Item
/// </summary>
class VR_LAYERMANAGER_DLL VRLocalImageLayerItem : public VRWebImageLayerItem
{
public:
	VRLocalImageLayerItem()
	{
		WebImageLayer = NULL;
	};
	~VRLocalImageLayerItem()
	{

	};

};
/// <summary>
/// 本地dem图层Item
/// </summary>
class VR_LAYERMANAGER_DLL VRLocalDemLayerItem : public VRWebDemLayerItem
{
public:
	VRLocalDemLayerItem(){ WebElevationLayer = NULL; };
	~VRLocalDemLayerItem(){};

};
/// <summary>
/// 本地模型图层Item
/// </summary>
class VR_LAYERMANAGER_DLL VRLocalModelLayerItem : public QTreeWidgetItem
{
public :
	VRLocalModelLayerItem(){};
	~VRLocalModelLayerItem(){};
};
/// <summary>
/// 本地模型Item
/// </summary>
class VR_LAYERMANAGER_DLL VRLocalModelItem : public QTreeWidgetItem
{
public :
	VRLocalModelItem(){mpRefModelNode = NULL;};
	~VRLocalModelItem(){};

public:
	osgEarth::Util::ObjectLocatorNode* mpRefModelNode;
};

class VR_LAYERMANAGER_DLL VRGeoDrawGroupItem : public VRLayerItem
{
public:
	VRGeoDrawGroupItem(){};
	~VRGeoDrawGroupItem(){};

};

class VR_LAYERMANAGER_DLL VRGeoObjectItem : public QTreeWidgetItem
{
public:
	VRGeoObjectItem(){ Object = NULL; };
	~VRGeoObjectItem(){};

public:
	Common::ISceneObject* Object;
};

/// <summary>
/// 基础图层管理类
/// </summary>
class VR_LAYERMANAGER_DLL VRLayerManager 
	: public QTreeWidget, public Tool::IToolListener
{
	Q_OBJECT

public:
	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="parent">父控件</param>
	VRLayerManager(QWidget *parent = 0);


	/// <summary>
	/// 构造函数
	/// </summary>
	~VRLayerManager();

	/// <summary>
	/// 获取场景控件
	/// </summary>
	VRGlobeWidget* getGlobeWidget();

	/// <summary>
	/// 关联场景控件
	/// </summary>
	/// <param name="pRefGlobeWidget">场景控件</param>
	void setGlobeWidget(VRGlobeWidget* pRefGlobeWidget);

	/// <summary>
	/// 初始化图层管理树
	/// </summary>
	void initial();

private:
	virtual void createDefaultItems();

	void intialWebMapItem();



protected:
	void contextMenuEvent(QContextMenuEvent * event ); 
	void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *  event);
	void mouseDoubleClickEvent(QMouseEvent *event);

	private slots:

	void slotAddLocalImage(osgEarth::ImageLayer* pImageLayer);

	void slotAddLocalImage();

	void slotAddLocalDem();

	void slotAddLocalVector();

	void slotRemoveLayer();

	void slotAddModel();

	void slotRemoveModel();

	void slotModelProperty();

	void slotImageProperty();

	void createVectorLayer(osgEarth::ImageLayer* pVectorLayer);

	void slotAddGeoIcon();

	void slotAddPolyline();

	void slotAddPolygon();

	void slotAddScutcheon();

	void slotAddArrow();

	void onEvent(Common::IObject* pResult);

	void refreshObjectToTree(Common::ISceneObject* pSceneObject);
private:
	VRLayerItem* mpRootItem;
	QMenu* mpModelMenu;
	QMenu* mpLocalImageGroupMenu;
	QMenu* mpLocalDemGroupMenu;
	QMenu* mpLocalLayerMenu;
	QMenu* mpLocalVectorGroupMenu;
	QMenu* mpImageGroupPropertyMenu;
	QMenu* mpGeoDrawMenu;
	VRGlobeWidget* mpRefGlobeWidget;
	QTreeWidgetItem * mpCurrentModelParentItem;

	GeometryEditor::IGeometryEditor* mpGeometryEditor;
};

class VR_LAYERMANAGER_DLL VRServiceItem : public QTreeWidgetItem
{
public :
	VRServiceItem()
	{
	
		ServiceType = 0;
	};
	~VRServiceItem(){};

public:
	int ServiceType;
};
class VR_LAYERMANAGER_DLL VRServiceManager 
	: public QTreeWidget
{
	Q_OBJECT

public:
	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="parent">父控件</param>
	VRServiceManager(QWidget *parent = 0);


	/// <summary>
	/// 构造函数
	/// </summary>
	~VRServiceManager();

	/// <summary>
	/// 获取场景控件
	/// </summary>
	VRGlobeWidget* getGlobeWidget();

	/// <summary>
	/// 关联场景控件
	/// </summary>
	/// <param name="pRefGlobeWidget">场景控件</param>
	void setGlobeWidget(VRGlobeWidget* pRefGlobeWidget);

	/// <summary>
	/// 初始化图层管理树
	/// </summary>
	void initial();

private:
	virtual void createDefaultItems();

protected:
	void contextMenuEvent(QContextMenuEvent * event ); 
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *  event);
	void mouseDoubleClickEvent(QMouseEvent *event);

	private slots:

	void slotModelTransform();

private:
	VRServiceItem* mpRootItem;
	VRGlobeWidget* mpRefGlobeWidget;
};
#endif // LAYERMANAGER_H
