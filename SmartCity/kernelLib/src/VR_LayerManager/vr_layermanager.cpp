#include "vr_layermanager.h"
#include <QTextCodec>
#include <..\\..\\osgdb_osgearth_googlemap\\osgdb_osgearth_googlemap.h>
#include <..\\..\\osgdb_osgearth_arcgisimage\\osgdb_osgearth_arcgisimage.h>
#include <..\\..\\osgdb_osgearth_gaodemap\\osgdb_osgearth_gaodemap.h>
#include <..\\..\\osgdb_osgearth_tianditumap\\osgdb_osgearth_tianditumap.h>
#include <..\\..\\osgdb_osgearth_tiandituimage\\osgdb_osgearth_tiandituimage.h>
#include <..\\..\\osgdb_osgearth_seamap\\osgdb_osgearth_seamap.h>
#include <..\\..\\osgdb_osgearth_s57map\\osgdb_osgearth_s57map.h>
#include <..\\..\\osgdb_osgearth_bing\\BingOptions>
#include "src/VR_GlobeFrm/addModelDlg.h"
#include "src/VR_GlobeFrm/addDemDlg.h"
#include "src/VR_GlobeFrm/addDomDlg.h"
#include "src/VR_GlobeFrm/addShpDlg.h"
#include "src/VR_GlobeFrm/globe_ui.h"
#include <osgEarthDrivers/tms/TMSOptions>
#include <map>
#include <Wininet.h>
#include <osgEarthDrivers/xyz/XYZOptions>
#include "geometryeditor/polygoneditor.h"
#include "geometryeditor/pointeditor.h"
#include "geometryeditor/polylineeditorex.h"
#include "geometryeditor/scutcheoneditor.h"
#include <QMessageBox>
#include "geoobject/geopolygon.h"
#include "geoobject/geoline.h"
#include "geoobject/geopoint.h"
#include "geoobject/geoscutcheon.h"

#pragma comment(lib,"Wininet.lib")
#pragma execution_character_set("utf-8")

std::map<std::string, osg::ref_ptr<TerrainLayer>> g_layers;

VR_LayerManager::VR_LayerManager(QWidget *parent)
	: QTreeWidget(parent),mpModelMenu(NULL)
{
	this->mpRefGlobeWidget = NULL;
	this->mpCurrentModelParentItem = NULL;
	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	this->mpGeometryEditor = NULL;
}

VR_LayerManager::~VR_LayerManager()
{

}

void VR_LayerManager::initial()
{
	this->createDefaultItems();
}

VR_GlobeWidgetEx* VR_LayerManager::getGlobeWidget()
{
	return this->mpRefGlobeWidget;
}

void VR_LayerManager::setGlobeWidget(VR_GlobeWidgetEx* pRefGlobeWidget)
{
	 this->mpRefGlobeWidget = pRefGlobeWidget;
}

void VR_LayerManager::createDefaultItems()
{
	this->setHeaderHidden(true);

	//创建场景根节点
	this->mpRootItem = new VRLayerItem();
	this->mpRootItem->setText(0,QString(tr("场景管理")).arg(1));
	std::string iconName = "systemImages\\layermangericon\\layer.png";
	this->mpRootItem->setIcon(0,QIcon(iconName.c_str()));
	this->addTopLevelItem(this->mpRootItem);
	this->intialWebMapItem();
	this->expandAll();

	//创建模型快捷菜单
	this->mpModelMenu = new QMenu(this);
	QAction * pAddModelAction = new QAction(tr("添加模型"),this);
	QAction * pRemoveModelAction = new QAction(tr("移除模型"),this);
	QAction * pModelPropertyAction = new QAction(tr("模型属性"),this);

	connect(pAddModelAction, SIGNAL(triggered()), this, SLOT(slotAddModel()));
	connect(pRemoveModelAction, SIGNAL(triggered()), this, SLOT(slotRemoveModel()));
	connect(pModelPropertyAction, SIGNAL(triggered()), this, SLOT(slotModelProperty()));

	this->mpModelMenu->addAction(pAddModelAction);
	/*this->mpModelMenu->addAction(pRemoveModelAction);
	this->mpModelMenu->addAction(pModelPropertyAction);*/

	//创建本地影像图层快捷菜单
	this->mpLocalImageGroupMenu = new QMenu(this);
	QAction * pAddImageAction = new QAction(tr("添加影像图层"), this);
	connect(pAddImageAction, SIGNAL(triggered()), this, SLOT(slotAddLocalImage()));
	this->mpLocalImageGroupMenu->addAction(pAddImageAction);

	//创建本地高程图层快捷菜单
	this->mpLocalDemGroupMenu = new QMenu(this);
	QAction * pAddDemAction = new QAction(tr("添加高程图层"), this);
	connect(pAddDemAction, SIGNAL(triggered()), this, SLOT(slotAddLocalDem()));
	this->mpLocalDemGroupMenu->addAction(pAddDemAction);

	//创建本地矢量图层快捷菜单
	this->mpLocalVectorGroupMenu = new QMenu(this);
	QAction * pAddShpAction = new QAction(tr("添加矢量数据"), this);
	connect(pAddShpAction, SIGNAL(triggered()), this, SLOT(slotAddLocalVector()));
	this->mpLocalVectorGroupMenu->addAction(pAddShpAction);

	this->mpLocalLayerMenu = new QMenu(this);
	QAction * pDeleteLayerAction = new QAction(tr("删除图层"), this);
	connect(pDeleteLayerAction, SIGNAL(triggered()), this, SLOT(slotRemoveLayer()));
	this->mpLocalLayerMenu->addAction(pDeleteLayerAction);
	
	this->mpImageGroupPropertyMenu = new QMenu(this);
	QAction * pLayerPropertyAction = new QAction(tr("图层属性"), this);
	connect(pLayerPropertyAction, SIGNAL(triggered()), this, SLOT(slotImageProperty()));
	this->mpImageGroupPropertyMenu->addAction(pLayerPropertyAction);

	this->mpGeoDrawMenu = new QMenu(this);
	QAction * pAddGeoIconAction = new QAction(tr("绘制地标"), this);
	//pAddGeoIconAction->setIcon(QIcon("systemImages/layermangericon/point.png"));
	connect(pAddGeoIconAction, SIGNAL(triggered()), this, SLOT(slotAddGeoIcon()));
	this->mpGeoDrawMenu->addAction(pAddGeoIconAction);

	QAction * pAddGeoPolylineAction = new QAction(tr("绘制折线"), this);
	//pAddGeoIconAction->setIcon(QIcon("systemImages/layermangericon/drawLine.png"));
	connect(pAddGeoPolylineAction, SIGNAL(triggered()), this, SLOT(slotAddPolyline()));
	this->mpGeoDrawMenu->addAction(pAddGeoPolylineAction);

	QAction * pAddGeoPolygonAction = new QAction(tr("绘制多边形"), this);
	//pAddGeoPolygonAction->setIcon(QIcon("systemImages/layermangericon/drawPolygon.png"));
	connect(pAddGeoPolygonAction, SIGNAL(triggered()), this, SLOT(slotAddPolygon()));
	this->mpGeoDrawMenu->addAction(pAddGeoPolygonAction);

	QAction * pAddScutcheonAction = new QAction(tr("绘制标牌"), this);
	connect(pAddScutcheonAction, SIGNAL(triggered()), this, SLOT(slotAddScutcheon()));
	this->mpGeoDrawMenu->addAction(pAddScutcheonAction);

	QAction * pAddGeoArrowAction = new QAction(tr("添加箭头"), this);
	connect(pAddGeoArrowAction, SIGNAL(triggered()), this, SLOT(slotAddArrow()));
	this->mpGeoDrawMenu->addAction(pAddGeoArrowAction);
}

void VR_LayerManager::slotAddLocalImage(osgEarth::ImageLayer* pImageLayer)
{
	if (this->mpCurrentModelParentItem != NULL && pImageLayer)
	{
		VRWebImageLayerItem* pModelItem = new VRWebImageLayerItem();
		pModelItem->setText(0, QString(tr(pImageLayer->getName().c_str())).arg(1));
		pModelItem->setIcon(0, QIcon("systemImages/layermangericon/dem.png"));
		pModelItem->WebImageLayer = pImageLayer;
		this->mpCurrentModelParentItem->addChild(pModelItem);
	}
}

void VR_LayerManager::slotAddLocalImage()
{
	osgEarth::ImageLayer* pLocalImageLayer = NULL;
	Globe::CAddDom* addModel = new Globe::CAddDom(this->mpRefGlobeWidget, pLocalImageLayer);
	addModel->setModal(false);
	int r = addModel->exec();
	if (r == 1)
	{
		pLocalImageLayer = Globe::CAddDom::getImageLayer();
		if (this->mpCurrentModelParentItem != NULL && pLocalImageLayer)
		{
			VRLocalImageLayerItem* pModelItem = new VRLocalImageLayerItem();
			pModelItem->setText(0, QString(tr(pLocalImageLayer->getName().c_str())).arg(1));
			pModelItem->setIcon(0, QIcon("systemImages/layermangericon/basicscene.png"));
			pModelItem->setCheckState(0, Qt::Checked);
			pModelItem->WebImageLayer = pLocalImageLayer;
			this->mpCurrentModelParentItem->addChild(pModelItem);
		}
		Globe::CAddDom::setNullImageLayer();
	}
}

void VR_LayerManager::slotImageProperty()
{
	VRWebImageLayerItem* pWebImageLayerItem = dynamic_cast<VRWebImageLayerItem*>(this->mpCurrentModelParentItem);
	if (pWebImageLayerItem && pWebImageLayerItem->WebImageLayer)
	{
		ImageTransparentDlg* pImageDlg = new ImageTransparentDlg(this);
		pImageDlg->setLayer(pWebImageLayerItem->WebImageLayer);
		pImageDlg->exec();
	}
	
}

void VR_LayerManager::slotRemoveLayer()
{
	if (mpCurrentModelParentItem)
	{
		VRLocalDemLayerItem* pLocalDemLayerItem = 
			dynamic_cast<VRLocalDemLayerItem*>(this->mpCurrentModelParentItem);
		if (pLocalDemLayerItem)
		{
			this->mpRefGlobeWidget->getMapRef()->removeElevationLayer(pLocalDemLayerItem->WebElevationLayer);
		}
		else
		{
			VRLocalImageLayerItem* pLocalImageLayerItem =
				dynamic_cast<VRLocalImageLayerItem*>(this->mpCurrentModelParentItem);
			if (pLocalImageLayerItem)
			{
				this->mpRefGlobeWidget->getMapRef()->removeImageLayer(pLocalImageLayerItem->WebImageLayer);
			}
		}
		mpCurrentModelParentItem->parent()->removeChild(mpCurrentModelParentItem);

		mpCurrentModelParentItem = NULL;
	}
}

void VR_LayerManager::slotAddLocalVector()
{
	Globe::CAddShp* addModel = new Globe::CAddShp(this->mpRefGlobeWidget);
	connect(addModel, SIGNAL(createVectorLayer(osgEarth::ImageLayer*)), this, SLOT(createVectorLayer(osgEarth::ImageLayer*)));
	int r = addModel->exec();
}

void VR_LayerManager::createVectorLayer(osgEarth::ImageLayer* pVectorLayer)
{
	if (this->mpCurrentModelParentItem != NULL && pVectorLayer)
	{
		VRLocalVectorLayerItem* pVectorItem = new VRLocalVectorLayerItem();
		pVectorItem->setText(0, QString(tr(pVectorLayer->getName().c_str())).arg(1));
		pVectorItem->setIcon(0, QIcon("systemImages/layermangericon/localVector.png"));
		pVectorItem->setCheckState(0, pVectorLayer->getVisible() ? Qt::Checked : Qt::Unchecked);
		pVectorItem->VectorLayer = pVectorLayer;
		this->mpCurrentModelParentItem->addChild(pVectorItem);
	}
}

void VR_LayerManager::slotAddLocalDem()
{
	osgEarth::ElevationLayer* pLocalDemLayer = NULL;
	Globe::CAddDem* addModel = new Globe::CAddDem(this->mpRefGlobeWidget, pLocalDemLayer);
	addModel->setModal(false);
	int r = addModel->exec();
	if (r == 1)
	{
		pLocalDemLayer = Globe::CAddDem::getDemLayer();
		if (this->mpCurrentModelParentItem != NULL && pLocalDemLayer)
		{
			VRLocalDemLayerItem* pModelItem = new VRLocalDemLayerItem();
			pModelItem->setText(0, QString(tr(pLocalDemLayer->getName().c_str())).arg(1));
			pModelItem->setIcon(0, QIcon("systemImages/layermangericon/localDem.png"));
			pModelItem->setCheckState(0, Qt::Checked);
			pModelItem->WebElevationLayer = pLocalDemLayer;
			this->mpCurrentModelParentItem->addChild(pModelItem);
		}
		Globe::CAddDem::setNullDemLayer();
	}
}

void VR_LayerManager::slotAddModel()
{
	osgEarth::Util::ObjectLocatorNode* pRefNodeLocator = NULL;
	Globe::CAddModel* addModel = new Globe::CAddModel(this->mpRefGlobeWidget,pRefNodeLocator);
	addModel->setModal(false);
	int r = addModel->exec();
	if (r == 1)
	{
		if (this->mpCurrentModelParentItem != NULL && pRefNodeLocator)
		{
			VRLocalModelItem* pModelItem = new VRLocalModelItem();
			pModelItem->setText(0,QString(tr(pRefNodeLocator->getName().c_str())).arg(1));
			pModelItem->setIcon(0,QIcon("systemImages/layermangericon/localModel.png"));
			pModelItem->setCheckState(0,Qt::CheckState::Checked);
			pModelItem->mpRefModelNode = pRefNodeLocator;
			this->mpCurrentModelParentItem->addChild(pModelItem);
		}
	}
}

void VR_LayerManager::slotRemoveModel()
{

}

void VR_LayerManager::slotModelProperty()
{

}

void VR_LayerManager::intialWebMapItem()
{
	VRLayerItem* pWebMapItem = new VRLayerItem();
	pWebMapItem->setText(0, QString(tr("网络瓦片地图")).arg(1));
	pWebMapItem->setIcon(0, QIcon("systemImages/layermangericon/webData.png"));
	this->mpRootItem->addChild(pWebMapItem);
	DWORD flag;
	BOOL con = InternetGetConnectedState(&flag, 0);
	{
		VRWebImageLayerItem* pGoogleMapItem = new VRWebImageLayerItem();
		pGoogleMapItem->setText(0, QString(tr("谷歌影像图")).arg(1));
		pGoogleMapItem->setCheckState(0, Qt::Checked);
		pGoogleMapItem->setIcon(0, QIcon("systemImages\\layermangericon\\googleMap.png"));

		//////谷歌影像
		osgEarth::Drivers::GooglemapOptions googleImageOption;
		googleImageOption.maxDataLevel() = 20;
		googleImageOption.key() = "s";
		osgEarth::ImageLayerOptions imageLayerOption(googleImageOption);

		osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer("GoogleImageLayer", imageLayerOption);
		pGoogleMapItem->WebImageLayer = pImageLayer;

		if (con)
		{
			this->mpRefGlobeWidget->getMapRef()->addImageLayer(pImageLayer);
		}
		pWebMapItem->addChild(pGoogleMapItem);
	}

	{
		VRWebImageLayerItem* pGoogleMapItem = new VRWebImageLayerItem();
		pGoogleMapItem->setText(0, QString(tr("谷歌地形图")).arg(1));
		pGoogleMapItem->setCheckState(0, Qt::Checked);
		pGoogleMapItem->setIcon(0, QIcon("systemImages\\layermangericon\\googleMap.png"));

		//////谷歌影像
		osgEarth::Drivers::GooglemapOptions googleImageOption;
		googleImageOption.maxDataLevel() = 20;
		googleImageOption.key() = "t";
		osgEarth::ImageLayerOptions imageLayerOption(googleImageOption);

		osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer("GoogleTerrainLayer", imageLayerOption);
		pGoogleMapItem->WebImageLayer = pImageLayer;
		pImageLayer->setVisible(false);
		pGoogleMapItem->setCheckState(0, pImageLayer->getVisible() ? Qt::Checked : Qt::Unchecked);

		/*if (con)
		{
		this->mpRefGlobeWidget->getMapRef()->addImageLayer(pImageLayer);
		}*/
		pWebMapItem->addChild(pGoogleMapItem);
	}

	{
		VRWebImageLayerItem* pGoogleMapItem = new VRWebImageLayerItem();
		pGoogleMapItem->setText(0, QString(tr("谷歌标签层")).arg(1));
		pGoogleMapItem->setIcon(0, QIcon("systemImages\\layermangericon\\googleMap.png"));

		//////谷歌影像
		osgEarth::Drivers::GooglemapOptions googleImageOption;
		googleImageOption.maxDataLevel() = 20;
		googleImageOption.key() = "h";
		osgEarth::ImageLayerOptions imageLayerOption(googleImageOption);

		osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer("GoogleLabelLayer", imageLayerOption);
		pImageLayer->setVisible(false);
		pGoogleMapItem->WebImageLayer = pImageLayer;
		pGoogleMapItem->setCheckState(0, pImageLayer->getVisible() ? Qt::Checked : Qt::Unchecked);
		/*if (con)
		{
			this->mpRefGlobeWidget->getMapRef()->addImageLayer(pImageLayer);
		}*/
		pWebMapItem->addChild(pGoogleMapItem);
	}

	{
		VRWebImageLayerItem* pBingMapItem = new VRWebImageLayerItem();
		pBingMapItem->setText(0, QString(tr("Bing地图")).arg(1));
		pBingMapItem->setCheckState(0, Qt::Checked);
		pBingMapItem->setIcon(0, QIcon("systemImages\\layermangericon\\bing.png"));

		//////Bing影像
		osgEarth::Drivers::BingOptions bingImageOption;
		bingImageOption.maxDataLevel() = 20;
		osgEarth::ImageLayerOptions imageLayerOption(bingImageOption);

		osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer("BingImageLayer", imageLayerOption);
		pBingMapItem->WebImageLayer = pImageLayer;
		pImageLayer->setVisible(false);
		pBingMapItem->setCheckState(0, pImageLayer->getVisible() ? Qt::Checked : Qt::Unchecked);
		/*if (con)
		{
		this->mpRefGlobeWidget->getMapRef()->addImageLayer(pImageLayer);
		}*/
		pWebMapItem->addChild(pBingMapItem);
	}

	{
		VRWebImageLayerItem* pGaoDeMapItem = new VRWebImageLayerItem();
		pGaoDeMapItem->setText(0, QString(tr("高德影像")).arg(1));
		pGaoDeMapItem->setIcon(0, QIcon("systemImages\\layermangericon\\gaodeMap.png"));

		//////gaoDe影像
		osgEarth::Drivers::GaoDemapOptions gaoDeImageOption;
		gaoDeImageOption.maxDataLevel() = 18;
		gaoDeImageOption.key() = "6";
		osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer("GaoDeImageLayer", gaoDeImageOption);
		pGaoDeMapItem->WebImageLayer = pImageLayer;
		pImageLayer->setVisible(false);
		//this->mpRefGlobeWidget->getMapRef()->addImageLayer(pImageLayer);
		pGaoDeMapItem->setCheckState(0, pImageLayer->getVisible() ? Qt::Checked : Qt::Unchecked);
		pWebMapItem->addChild(pGaoDeMapItem);

	}

	{
		VRWebImageLayerItem* pGaoDeMapItem = new VRWebImageLayerItem();
		pGaoDeMapItem->setText(0, QString(tr("高德地图")).arg(1));
		pGaoDeMapItem->setIcon(0, QIcon("systemImages\\layermangericon\\gaodeMap.png"));

		//////gaoDe影像
		osgEarth::Drivers::GaoDemapOptions gaoDeImageOption;
		gaoDeImageOption.maxDataLevel() = 18;
		gaoDeImageOption.key() = "7";
		osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer("GaoDeMapLayer", gaoDeImageOption);
		pGaoDeMapItem->WebImageLayer = pImageLayer;
		pImageLayer->setVisible(false);
		//this->mpRefGlobeWidget->getMapRef()->addImageLayer(pImageLayer);
		pGaoDeMapItem->setCheckState(0, pImageLayer->getVisible() ? Qt::Checked : Qt::Unchecked);
		pWebMapItem->addChild(pGaoDeMapItem);

	}

	{
		VRWebImageLayerItem* pGaoDeMapItem = new VRWebImageLayerItem();
		pGaoDeMapItem->setText(0, QString(tr("高德路网")).arg(1));
		pGaoDeMapItem->setIcon(0, QIcon("systemImages\\layermangericon\\gaodeMap.png"));

		//////gaoDe影像
		osgEarth::Drivers::GaoDemapOptions gaoDeImageOption;
		gaoDeImageOption.maxDataLevel() = 18;
		gaoDeImageOption.key() = "8";
		osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer("GaoDeRoadLayer", gaoDeImageOption);
		pGaoDeMapItem->WebImageLayer = pImageLayer;
		pImageLayer->setVisible(false);
		//this->mpRefGlobeWidget->getMapRef()->addImageLayer(pImageLayer);
		pGaoDeMapItem->setCheckState(0, pImageLayer->getVisible() ? Qt::Checked : Qt::Unchecked);
		pWebMapItem->addChild(pGaoDeMapItem);

	}

	{
		VRWebImageLayerItem* pTDTMapItem = new VRWebImageLayerItem();
		pTDTMapItem->setText(0, QString(tr("天地图矢量")).arg(1));
		pTDTMapItem->setCheckState(0, Qt::Checked);
		pTDTMapItem->setIcon(0, QIcon("systemImages\\layermangericon\\tdtVector.png"));

		//////tdt矢量
		osgEarth::Drivers::TianDiTumapOptions tdtMapOption;
		tdtMapOption.maxDataLevel() = 17;
		osgEarth::ImageLayer* pMapLayer = new osgEarth::ImageLayer("TDTMapLayer", tdtMapOption);
		pTDTMapItem->WebImageLayer = pMapLayer;
		pMapLayer->setVisible(false);
		pTDTMapItem->setCheckState(0, pMapLayer->getVisible() ? Qt::Checked : Qt::Unchecked);
		//this->mpRefGlobeWidget->getMapRef()->addImageLayer(pMapLayer);
		pWebMapItem->addChild(pTDTMapItem);
	}
	{
		VRWebImageLayerItem* pTDTImageItem = new VRWebImageLayerItem();
		pTDTImageItem->setText(0, QString(tr("天地图影像")).arg(1));
		//pTDTImageItem->setCheckState(0,Qt::Checked);
		pTDTImageItem->setIcon(0, QIcon("systemImages\\layermangericon\\tdtMap.png"));

		//////tdt影像
		osgEarth::Drivers::TianDiTuImageOptions tdtImageOption;
		tdtImageOption.maxDataLevel() = 17;
		osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer("TDTImageLayer", tdtImageOption);
		pImageLayer->setVisible(false);
		pTDTImageItem->WebImageLayer = pImageLayer;
		pTDTImageItem->setCheckState(0, pImageLayer->getVisible() ? Qt::Checked : Qt::Unchecked);
		//this->mpRefGlobeWidget->getMapRef()->addImageLayer(pImageLayer);
		pWebMapItem->addChild(pTDTImageItem);
	}

	{
		VRWebImageLayerItem* pSeaMapMapItem = new VRWebImageLayerItem();
		pSeaMapMapItem->setText(0, QString(tr("船讯海图")).arg(1));
		pSeaMapMapItem->setIcon(0, QIcon("systemImages\\layermangericon\\chuanXunMap.png"));

		//////Arcgis影像
		osgEarth::Drivers::SeaMapOptions seaMapOptions;
		seaMapOptions.maxDataLevel() = 20;
		osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer("SeaImageLayer", seaMapOptions);
		pImageLayer->setVisible(false);
		pSeaMapMapItem->WebImageLayer = pImageLayer;
		pSeaMapMapItem->setCheckState(0, pImageLayer->getVisible() ? Qt::Checked : Qt::Unchecked);
		pWebMapItem->addChild(pSeaMapMapItem);
	}

	{
		VRWebImageLayerItem* pArcGisMapItem = new VRWebImageLayerItem();
		pArcGisMapItem->setText(0, QString(tr("ArcGis影像")).arg(1));
		pArcGisMapItem->setIcon(0, QIcon("systemImages\\layermangericon\\arcgis.png"));

		//////Arcgis影像
		osgEarth::Drivers::ArcgisImageOptions arcgisImageOption;
		arcgisImageOption.maxDataLevel() = 15;
		osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer("ArcgisImageLayer", arcgisImageOption);
		pImageLayer->setVisible(false);
		pArcGisMapItem->WebImageLayer = pImageLayer;
		pArcGisMapItem->setCheckState(0, pImageLayer->getVisible() ? Qt::Checked : Qt::Unchecked);
		pWebMapItem->addChild(pArcGisMapItem);
	}

	//{
	//	VRWebImageLayerItem* pArcGisMapItem = new VRWebImageLayerItem();
	//	pArcGisMapItem->setText(0, QString(tr("OpenStreetMap")).arg(1));
	//	pArcGisMapItem->setIcon(0, QIcon("systemImages\\layermangericon\\arcgis.png"));

	//	//////Arcgis影像
	//	osgEarth::Drivers::XYZOptions xyzImageOption;
	//	xyzImageOption.maxDataLevel() = 15;
	//	xyzImageOption.url() = "http://[abc].tile.openstreetmap.org/{z}/{x}/{y}.png";
	//	osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer("OpenStreetMapLayer", xyzImageOption);
	//	pImageLayer->setVisible(false);
	//	pArcGisMapItem->WebImageLayer = pImageLayer;
	//	pArcGisMapItem->setCheckState(0, pImageLayer->getVisible() ? Qt::Checked : Qt::Unchecked);
	//	pWebMapItem->addChild(pArcGisMapItem);
	//}

	VRLayerItem* pWebDemItem = new VRLayerItem();
	pWebDemItem->setText(0, QString(tr("网络数字高程")).arg(1));
	pWebDemItem->setIcon(0, QIcon("systemImages\\layermangericon\\webData.png"));
	this->mpRootItem->addChild(pWebDemItem);
#if 0
	{
		VRWebDemLayerItem* pGoogleDemItem = new VRWebDemLayerItem();
		pGoogleDemItem->setText(0,QString(tr("谷歌高程")).arg(1));
		pGoogleDemItem->setCheckState(0,Qt::Checked);
		pGoogleDemItem->setIcon(0,QIcon("systemImages\\layermangericon\\wordSurface.png"));
		pWebDemItem->addChild(pGoogleDemItem);
	}
#endif

	{
		VRWebDemLayerItem* pTDTDemItem = new VRWebDemLayerItem();
		pTDTDemItem->setText(0, QString(tr("ReadyMap")).arg(1));
		pTDTDemItem->setCheckState(0, Qt::Unchecked);
		pTDTDemItem->setIcon(0, QIcon("systemImages\\layermangericon\\wordSurface.png"));

		////高程
		osgEarth::Drivers::TMSOptions elvOption;
		elvOption.url() = "http://readymap.org/readymap/tiles/1.0.0/9/";
		osgEarth::ElevationLayer* pDemLayer = new osgEarth::ElevationLayer("elevation", elvOption);
		pTDTDemItem->WebElevationLayer = pDemLayer;
		pTDTDemItem->WebElevationLayer->setVisible(false);
		/*if (con)
		{
		this->mpRefGlobeWidget->getMapRef()->addElevationLayer(pDemLayer);
		}*/
		pWebDemItem->addChild(pTDTDemItem);
	}

	VRLayerItem* pLocalDataItem = new VRLayerItem();
	pLocalDataItem->setText(0, QString(tr("本地数据源")).arg(1));
	pLocalDataItem->setIcon(0, QIcon("systemImages\\layermangericon\\local.png"));
	this->mpRootItem->addChild(pLocalDataItem);
	{
		VRLocalImageGroupItem* pLocalImageItem = new VRLocalImageGroupItem();
		pLocalImageItem->setText(0, QString(tr("本地影像图层")).arg(1));
		pLocalImageItem->setIcon(0, QIcon("systemImages\\layermangericon\\image.png"));
		pLocalDataItem->addChild(pLocalImageItem);

		VRLocalDemGroupItem* pLocalDemItem = new VRLocalDemGroupItem();
		pLocalDemItem->setText(0, QString(tr("本地数字高程")).arg(1));
		pLocalDemItem->setIcon(0, QIcon("systemImages\\layermangericon\\dem.png"));
		pLocalDataItem->addChild(pLocalDemItem);

		VRLocalVectorGroupItem* pLocalVectorItem = new VRLocalVectorGroupItem();
		pLocalVectorItem->setText(0, QString(tr("本地矢量数据")).arg(1));
		pLocalVectorItem->setIcon(0, QIcon("systemImages\\layermangericon\\vector.png"));
		pLocalDataItem->addChild(pLocalVectorItem);

		VRLocalModelLayerItem* pLocalModelItem = new VRLocalModelLayerItem();
		pLocalModelItem->setText(0, QString(tr("本地模型")).arg(1));
		pLocalModelItem->setIcon(0, QIcon("systemImages\\layermangericon\\surfacemodel.png"));
		pLocalDataItem->addChild(pLocalModelItem);
	}

	VRGeoDrawGroupItem* pGeoDrawDataItem = new VRGeoDrawGroupItem();
	pGeoDrawDataItem->setText(0, QString(tr("地理标绘")).arg(1));
	pGeoDrawDataItem->setIcon(0, QIcon("systemImages\\layermangericon\\draw.png"));
	this->mpRootItem->addChild(pGeoDrawDataItem);
}

void VR_LayerManager::contextMenuEvent(QContextMenuEvent * event )
{
	QPoint point = QCursor::pos();
	point = mapFromGlobal(point);
	QTreeWidgetItem *item = this->itemAt(point);
	if (item != NULL)
	{
		if (dynamic_cast<VRLocalModelLayerItem*>(item))
		{
			this->mpCurrentModelParentItem= item;
			this->mpModelMenu->exec(QCursor::pos());
			this->mpCurrentModelParentItem = NULL;
		}
		else if (dynamic_cast<VRLocalImageGroupItem*>(item))
		{
			this->mpCurrentModelParentItem = item;
			this->mpLocalImageGroupMenu->exec(QCursor::pos());
			this->mpCurrentModelParentItem = NULL;
		}
		else if (dynamic_cast<VRLocalDemGroupItem*>(item))
		{
			this->mpCurrentModelParentItem = item;
			this->mpLocalDemGroupMenu->exec(QCursor::pos());
			this->mpCurrentModelParentItem = NULL;
		}
		else if (dynamic_cast<VRLocalVectorGroupItem*>(item))
		{
			this->mpCurrentModelParentItem = item;
			this->mpLocalVectorGroupMenu->exec(QCursor::pos());
		}
		else if (dynamic_cast<VRLocalDemLayerItem*>(item))
		{
			this->mpCurrentModelParentItem = item;
			this->mpLocalLayerMenu->exec(QCursor::pos());
		}
		else if (dynamic_cast<VRLocalImageLayerItem*>(item))
		{
			this->mpCurrentModelParentItem = item;
			this->mpLocalLayerMenu->exec(QCursor::pos());
		}
		else if (dynamic_cast<VRWebImageLayerItem*>(item))
		{
			this->mpCurrentModelParentItem = item;
			this->mpImageGroupPropertyMenu->exec(QCursor::pos());
		}
		else if (dynamic_cast<VRGeoDrawGroupItem*>(item))
		{
			this->mpCurrentModelParentItem = item;
			this->mpGeoDrawMenu->exec(QCursor::pos());
		}
		event->accept();
	}
}

void VR_LayerManager::mousePressEvent(QMouseEvent *event)
{
	QTreeWidget::mousePressEvent(event);
}

void VR_LayerManager::mouseReleaseEvent(QMouseEvent *  event)
{
	QPoint point = QCursor::pos();
	point = mapFromGlobal(point);
	QTreeWidgetItem *item = this->itemAt(point);
	if (item)
	{
		Qt::CheckState oldCheckstatus = item->checkState(0);
		QTreeWidget::mouseReleaseEvent(event);
		Qt::CheckState curCheckstatus = item->checkState(0);
		bool changed = (oldCheckstatus != curCheckstatus);
		if (changed)
		{
			
			if (dynamic_cast<VRWebImageLayerItem*>(item))
			{
				VRWebImageLayerItem* pWebImageLayerItem = dynamic_cast<VRWebImageLayerItem*>(item);
				if (pWebImageLayerItem && pWebImageLayerItem->WebImageLayer)
				{
					bool visible = !pWebImageLayerItem->WebImageLayer->getVisible();
					if (visible && NULL == this->mpRefGlobeWidget->getMapRef()->getImageLayerByName(pWebImageLayerItem->WebImageLayer->getName()))
					{
						DWORD flag;
						BOOL con = InternetGetConnectedState(&flag, 0);
						if (con)
							this->mpRefGlobeWidget->getMapRef()->addImageLayer(pWebImageLayerItem->WebImageLayer);
					}
					else if (NULL != this->mpRefGlobeWidget->getMapRef()->getImageLayerByName(pWebImageLayerItem->WebImageLayer->getName()))
					{
						//pWebImageLayerItem->WebImageLayer->disable("");
						this->mpRefGlobeWidget->getMapRef()->removeImageLayer(pWebImageLayerItem->WebImageLayer);
					}
					pWebImageLayerItem->WebImageLayer->setVisible(visible);
					g_layers[pWebImageLayerItem->WebImageLayer->getName()] = pWebImageLayerItem->WebImageLayer;
				}
			}
			else if (dynamic_cast<VRWebDemLayerItem*>(item))
			{
				VRWebDemLayerItem* pWebDemItem = dynamic_cast<VRWebDemLayerItem*>(item);
				if ( pWebDemItem && pWebDemItem->WebElevationLayer != NULL)
				{
					bool visible = !pWebDemItem->WebElevationLayer->getVisible();
					if (visible)
					{
						this->mpRefGlobeWidget->getMapRef()->addElevationLayer(pWebDemItem->WebElevationLayer);
					}
					else
					{
						this->mpRefGlobeWidget->getMapRef()->removeElevationLayer(pWebDemItem->WebElevationLayer);
					}
					pWebDemItem->WebElevationLayer->setVisible(!pWebDemItem->WebElevationLayer->getVisible());
				}
			}
			else if (dynamic_cast<VRLocalVectorLayerItem*>(item))
			{
				VRLocalVectorLayerItem* pVectorItem = dynamic_cast<VRLocalVectorLayerItem*>(item);
				pVectorItem->VectorLayer->setVisible(!pVectorItem->VectorLayer->getVisible());
			}
			else if (dynamic_cast<VRLocalModelItem*>(item))
			{
				VRLocalModelItem* pModelItem = dynamic_cast<VRLocalModelItem*>(item);
				if (curCheckstatus == Qt::CheckState::Checked)
				{
					pModelItem->mpRefModelNode->setNodeMask(1);
				}
				else
				{
					pModelItem->mpRefModelNode->setNodeMask(0);
				}
			}
			else if (dynamic_cast<VRGeoObjectItem*>(item))
			{
				VRGeoObjectItem* pGeoItem = dynamic_cast<VRGeoObjectItem*>(item);
				pGeoItem->Object->setVisible(!pGeoItem->Object->getVisible());
			}
		}
		
	}
}

void VR_LayerManager::mouseDoubleClickEvent(QMouseEvent *event)
{
	QPoint point = QCursor::pos();
	point = mapFromGlobal(point);
	QTreeWidgetItem *item = this->itemAt(point);
	if (item)
	{
		if (dynamic_cast<VRLocalModelItem*>(item))
		{
			VRLocalModelItem* pLocalModelItem = dynamic_cast<VRLocalModelItem*>(item);
			if (pLocalModelItem && pLocalModelItem->mpRefModelNode)
			{
				pLocalModelItem->mpRefModelNode->getLocator()->getPosition();
				osgEarth::Viewpoint vp("",
					pLocalModelItem->mpRefModelNode->getLocator()->getPosition().x(),
					pLocalModelItem->mpRefModelNode->getLocator()->getPosition().y(),
					pLocalModelItem->mpRefModelNode->getLocator()->getPosition().z() + 10,
					0, -90, 500);
				this->mpRefGlobeWidget->flyTo(vp, 3);
			}
		}
		else if (dynamic_cast<VRGeoObjectItem*>(item))
		{
			VRGeoObjectItem* pObjectItem = dynamic_cast<VRGeoObjectItem*>(item);

			osgEarth::Viewpoint vp("",
				pObjectItem->Object->getGeoPosition().x(),
				pObjectItem->Object->getGeoPosition().y(),
				pObjectItem->Object->getGeoPosition().z() + 10,
				0, -90, 500);
			this->mpRefGlobeWidget->flyTo(vp, 3);
		}
	}
}

void VR_LayerManager::slotAddGeoIcon()
{
	if (NULL != this->mpGeometryEditor)
	{
		QMessageBox::information(this, "警告信息", "当前存在对象编辑！", QMessageBox::Ok);
		return;
	}
	GeometryEditor::IPointEditor* pEditor = new GeometryEditor::IPointEditor(this->mpRefGlobeWidget->getMapNodeRef());
	pEditor->beginEdit();
	pEditor->addToolListener(this);
	this->mpGeometryEditor = pEditor;
	this->mpRefGlobeWidget->addEventHandler(this->mpGeometryEditor);
}

void VR_LayerManager::slotAddPolyline()
{
	if (NULL != this->mpGeometryEditor)
	{
		QMessageBox::information(this, "警告信息", "当前存在对象编辑！",QMessageBox::Ok);
		return;
	}
	GeometryEditor::IPolylineEditor* pEditor = new GeometryEditor::IPolylineEditor(this->mpRefGlobeWidget->getMapNodeRef());
	pEditor->beginEdit();
	pEditor->addToolListener(this);
	this->mpGeometryEditor = pEditor;
	this->mpRefGlobeWidget->addEventHandler(this->mpGeometryEditor);
	
}

void VR_LayerManager::slotAddPolygon()
{
	if (NULL != this->mpGeometryEditor)
	{
		QMessageBox::information(this, "警告信息", "当前存在对象编辑！", QMessageBox::Ok);
		return;
	}
	GeometryEditor::IPolygonEditor* pEditor = new GeometryEditor::IPolygonEditor(this->mpRefGlobeWidget->getMapNodeRef());
	pEditor->beginEdit();
	pEditor->addToolListener(this);
	this->mpGeometryEditor = pEditor;
	this->mpRefGlobeWidget->addEventHandler(this->mpGeometryEditor);
}

void VR_LayerManager::slotAddScutcheon()
{
	if (NULL != this->mpGeometryEditor)
	{
		QMessageBox::information(this, "警告信息", "当前存在对象编辑！", QMessageBox::Ok);
		return;
	}
	GeometryEditor::IScutcheonEditor* pEditor = new GeometryEditor::IScutcheonEditor(this->mpRefGlobeWidget->getMapNodeRef());
	pEditor->beginEdit();
	pEditor->addToolListener(this);
	this->mpGeometryEditor = pEditor;
	this->mpRefGlobeWidget->addEventHandler(this->mpGeometryEditor);
}

void VR_LayerManager::slotAddArrow()
{
	if (NULL != this->mpGeometryEditor)
	{
		QMessageBox::information(this, "警告信息", "当前存在对象编辑！", QMessageBox::Ok);
		return;
	}
	GeometryEditor::IArrowEditor* pEditor = new GeometryEditor::IArrowEditor(this->mpRefGlobeWidget->getMapNodeRef());
	pEditor->beginEdit();
	pEditor->addToolListener(this);
	this->mpGeometryEditor = pEditor;
	this->mpRefGlobeWidget->addEventHandler(this->mpGeometryEditor);
}

QString getIcon(Common::ISceneObject* pSceneObject)
{
	if (dynamic_cast<Geo::CGeoPolygon*>(pSceneObject))
	{
		return "systemImages/layermangericon/drawPolygon.png";
	}
	else if (dynamic_cast<Geo::CGeoLine*>(pSceneObject))
	{
		return "systemImages/layermangericon/drawLine.png";
	}
	else if (dynamic_cast<Geo::CGeoPoint*>(pSceneObject))
	{
		return "systemImages/layermangericon/point.png";
	}
	else if (dynamic_cast<Geo::CGeoScutcheon*>(pSceneObject))
	{
		return "systemImages/layermangericon/scutcheon.png";
	}

	return "systemImages/layermangericon/drawPoint.png";
}

void VR_LayerManager::refreshObjectToTree(Common::ISceneObject* pSceneObject)
{
	if (this->mpCurrentModelParentItem && pSceneObject)
	{
		VRGeoDrawGroupItem* pGeodrawGroupItem =
			dynamic_cast<VRGeoDrawGroupItem*>(this->mpCurrentModelParentItem);
		if (pGeodrawGroupItem)
		{
			pSceneObject->setName("未命名");
			VRGeoObjectItem* pGeoObjectItem = new VRGeoObjectItem();
			pGeoObjectItem->Object = pSceneObject;
			pGeoObjectItem->setText(0, QString(tr(pGeoObjectItem->Object->getName().c_str())).arg(1));
			pGeoObjectItem->setIcon(0, QIcon(getIcon(pSceneObject)));
			pGeoObjectItem->setCheckState(0,Qt::CheckState::Checked);
			pGeodrawGroupItem->addChild(pGeoObjectItem);
		}
	}
}

void VR_LayerManager::onEvent(Common::IObject* pResult)
{
	//处理编辑对象双击事件
	if (this->mpGeometryEditor)
	{
		Common::ISceneObject* pRenderObject = this->mpGeometryEditor->generateSceneObject();
		if (pRenderObject)
		{
			pRenderObject->loadToScene();
			pRenderObject->renderToScene();
			this->refreshObjectToTree(pRenderObject);
		}
		this->mpGeometryEditor->endEdit(true);
		this->mpGeometryEditor->removeToolListener(this);
		this->mpRefGlobeWidget->removeEventHandler(this->mpGeometryEditor);
		this->mpGeometryEditor = NULL;
		
	}
}
