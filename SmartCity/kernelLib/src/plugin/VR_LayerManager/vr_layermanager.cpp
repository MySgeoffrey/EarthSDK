#include "vr_layermanager.h"
#include <QTextCodec>
#include "..\\osgdb_osgearth_googlemap\\osgdb_osgearth_googlemap.h"
#include "..\\osgdb_osgearth_tdt\\osgdb_osgearth_tdt.h"
#include "..\\osgdb_osgearth_gaodemap\\osgdb_osgearth_gaodemap.h"
//#include <..\\osgdb_osgearth_tianditumap\\osgdb_osgearth_tianditumap.h>
//#include <..\\osgdb_osgearth_tiandituimage\\osgdb_osgearth_tiandituimage.h>
//#include <..\\osgdb_osgearth_seamap\\osgdb_osgearth_seamap.h>
//#include <..\\osgdb_osgearth_s57map\\osgdb_osgearth_s57map.h>
#include "..\\osgdb_osgearth_bing\\BingOptions"
#include "../../../src/VR_GlobeFrm/addModelDlg.h"
#include "../../../src/VR_GlobeFrm/addDemDlg.h"
#include "../../../src/VR_GlobeFrm/addDomDlg.h"
#include "../../../src/VR_GlobeFrm/addShpDlg.h"
#include "../../../src/VR_GlobeFrm/globe_ui.h"
#include <osgEarthDrivers/tms/TMSOptions>
#include <map>
#include <Wininet.h>
#include <osgEarthDrivers/xyz/XYZOptions>
#include "geometryeditor/polygoneditor.h"
#include "geometryeditor/pointeditor.h"
#include "geometryeditor/polylineeditorex.h"
#include "geometryeditor/scutcheoneditor.h"
#include "framework/common/globesetting.h"
#include <QMessageBox>
#include <qheaderview.h>

#pragma comment(lib,"Wininet.lib")

//#pragma execution_character_set("utf-8")
std::map<std::string, osg::ref_ptr<TerrainLayer>> g_layers;

std::string getUtf8String(std::string str)
{
	std::string utf8String;
	CGlobeSetting::instance()->gb2312ToUtf8(str, utf8String);
	return utf8String;
}
VRLayerManager::VRLayerManager(QWidget *parent)
	: QTreeWidget(parent),mpModelMenu(NULL)
{
	this->mpRefGlobeWidget = NULL;
	this->mpCurrentModelParentItem = NULL;
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	this->mpGeometryEditor = NULL;
}

VRLayerManager::~VRLayerManager()
{

}

void VRLayerManager::initial()
{
	this->createDefaultItems();
}

VRGlobeWidget* VRLayerManager::getGlobeWidget()
{
	return this->mpRefGlobeWidget;
}

void VRLayerManager::setGlobeWidget(VRGlobeWidget* pRefGlobeWidget)
{
	 this->mpRefGlobeWidget = pRefGlobeWidget;
}

void VRLayerManager::createDefaultItems()
{
	this->setHeaderHidden(true);

	//创建场景根节点
	this->mpRootItem = new VRLayerItem();
	this->mpRootItem->setText(0,QString(getUtf8String("地理图层组").c_str()).arg(1));
	std::string iconName = "systemImages\\layermangericon\\layer.png";
	this->mpRootItem->setIcon(0,QIcon(iconName.c_str()));
	this->addTopLevelItem(this->mpRootItem);
	this->intialWebMapItem();
	this->expandToDepth(0);

	//创建模型快捷菜单
	this->mpModelMenu = new QMenu(this);
	QAction * pAddModelAction = new QAction(getUtf8String("添加模型").c_str(),this);
	QAction * pRemoveModelAction = new QAction(getUtf8String("移除模型").c_str(),this);
	QAction * pModelPropertyAction = new QAction(getUtf8String("模型属性").c_str(),this);

	connect(pAddModelAction, SIGNAL(triggered()), this, SLOT(slotAddModel()));
	connect(pRemoveModelAction, SIGNAL(triggered()), this, SLOT(slotRemoveModel()));
	connect(pModelPropertyAction, SIGNAL(triggered()), this, SLOT(slotModelProperty()));

	this->mpModelMenu->addAction(pAddModelAction);
	/*this->mpModelMenu->addAction(pRemoveModelAction);
	this->mpModelMenu->addAction(pModelPropertyAction);*/

	//创建本地影像图层快捷菜单
	this->mpLocalImageGroupMenu = new QMenu(this);
	QAction * pAddImageAction = new QAction(getUtf8String("添加影像").c_str(), this);
	connect(pAddImageAction, SIGNAL(triggered()), this, SLOT(slotAddLocalImage()));
	this->mpLocalImageGroupMenu->addAction(pAddImageAction);

	//创建本地高程图层快捷菜单
	this->mpLocalDemGroupMenu = new QMenu(this);
	QAction * pAddDemAction = new QAction(getUtf8String("添加高程").c_str(), this);
	connect(pAddDemAction, SIGNAL(triggered()), this, SLOT(slotAddLocalDem()));
	this->mpLocalDemGroupMenu->addAction(pAddDemAction);

	//创建本地矢量图层快捷菜单
	this->mpLocalVectorGroupMenu = new QMenu(this);
	QAction * pAddShpAction = new QAction(getUtf8String("添加矢量").c_str(), this);
	connect(pAddShpAction, SIGNAL(triggered()), this, SLOT(slotAddLocalVector()));
	this->mpLocalVectorGroupMenu->addAction(pAddShpAction);

	this->mpLocalLayerMenu = new QMenu(this);
	QAction * pDeleteLayerAction = new QAction(getUtf8String("删除图层").c_str(), this);
	connect(pDeleteLayerAction, SIGNAL(triggered()), this, SLOT(slotRemoveLayer()));
	this->mpLocalLayerMenu->addAction(pDeleteLayerAction);
	
	this->mpImageGroupPropertyMenu = new QMenu(this);
	QAction * pLayerPropertyAction = new QAction(getUtf8String("图层属性").c_str(), this);
	connect(pLayerPropertyAction, SIGNAL(triggered()), this, SLOT(slotImageProperty()));
	this->mpImageGroupPropertyMenu->addAction(pLayerPropertyAction);

	this->mpGeoDrawMenu = new QMenu(this);
	QAction * pAddGeoIconAction = new QAction(getUtf8String("绘制地标").c_str(), this);
	//pAddGeoIconAction->setIcon(QIcon("systemImages/layermangericon/point.png"));
	connect(pAddGeoIconAction, SIGNAL(triggered()), this, SLOT(slotAddGeoIcon()));
	this->mpGeoDrawMenu->addAction(pAddGeoIconAction);

	QAction * pAddGeoPolylineAction = new QAction(getUtf8String("绘制折线").c_str(), this);
	//pAddGeoIconAction->setIcon(QIcon("systemImages/layermangericon/drawLine.png"));
	connect(pAddGeoPolylineAction, SIGNAL(triggered()), this, SLOT(slotAddPolyline()));
	this->mpGeoDrawMenu->addAction(pAddGeoPolylineAction);

	QAction * pAddGeoPolygonAction = new QAction(getUtf8String("绘制多边形").c_str(), this);
	//pAddGeoPolygonAction->setIcon(QIcon("systemImages/layermangericon/drawPolygon.png"));
	connect(pAddGeoPolygonAction, SIGNAL(triggered()), this, SLOT(slotAddPolygon()));
	this->mpGeoDrawMenu->addAction(pAddGeoPolygonAction);

	QAction * pAddScutcheonAction = new QAction(getUtf8String("绘制标牌").c_str(), this);
	connect(pAddScutcheonAction, SIGNAL(triggered()), this, SLOT(slotAddScutcheon()));
	this->mpGeoDrawMenu->addAction(pAddScutcheonAction);

	QAction * pAddGeoArrowAction = new QAction(getUtf8String("添加箭头").c_str(), this);
	connect(pAddGeoArrowAction, SIGNAL(triggered()), this, SLOT(slotAddArrow()));
	this->mpGeoDrawMenu->addAction(pAddGeoArrowAction);
}

void VRLayerManager::slotAddLocalImage(osgEarth::ImageLayer* pImageLayer)
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

void VRLayerManager::slotAddLocalImage()
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

void VRLayerManager::slotImageProperty()
{
	VRWebImageLayerItem* pWebImageLayerItem = dynamic_cast<VRWebImageLayerItem*>(this->mpCurrentModelParentItem);
	if (pWebImageLayerItem && pWebImageLayerItem->WebImageLayer)
	{
		ImageTransparentDlg* pImageDlg = new ImageTransparentDlg(this);
		pImageDlg->setLayer(pWebImageLayerItem->WebImageLayer);
		pImageDlg->exec();
	}
	
}

void VRLayerManager::slotRemoveLayer()
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

void VRLayerManager::slotAddLocalVector()
{
	Globe::CAddShp* addModel = new Globe::CAddShp(this->mpRefGlobeWidget);
	connect(addModel, SIGNAL(createVectorLayer(osgEarth::ImageLayer*)), this, SLOT(createVectorLayer(osgEarth::ImageLayer*)));
	int r = addModel->exec();
}

void VRLayerManager::createVectorLayer(osgEarth::ImageLayer* pVectorLayer)
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

void VRLayerManager::slotAddLocalDem()
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

void VRLayerManager::slotAddModel()
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

void VRLayerManager::slotRemoveModel()
{

}

void VRLayerManager::slotModelProperty()
{

}

void VRLayerManager::intialWebMapItem()
{
	VRLayerItem* pWebMapItem = new VRLayerItem();
	pWebMapItem->setText(0, QString(getUtf8String("在线地图").c_str()).arg(1));
	pWebMapItem->setIcon(0, QIcon("systemImages/layermangericon/webData.png"));
	this->mpRootItem->addChild(pWebMapItem);
	DWORD flag;
	BOOL con = InternetGetConnectedState(&flag, 0);
	{
		VRWebImageLayerItem* pGoogleMapItem = new VRWebImageLayerItem();
		pGoogleMapItem->setText(0, QString(getUtf8String("谷歌影像图").c_str()).arg(1));
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
		pGoogleMapItem->setText(0, QString(getUtf8String("谷歌地形图").c_str()).arg(1));
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
		pGoogleMapItem->setText(0, QString(getUtf8String("谷歌标签层").c_str()).arg(1));
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
		pBingMapItem->setText(0, QString(getUtf8String("Bing地图").c_str()).arg(1));
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
		pGaoDeMapItem->setText(0, QString(getUtf8String("高德影像").c_str()).arg(1));
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
		pGaoDeMapItem->setText(0, QString(getUtf8String("高德地图").c_str()).arg(1));
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
		pGaoDeMapItem->setText(0, QString(getUtf8String("高德路网").c_str()).arg(1));
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
		pTDTMapItem->setText(0, QString(getUtf8String("天地图矢量").c_str()).arg(1));
		pTDTMapItem->setCheckState(0, Qt::Checked);
		pTDTMapItem->setIcon(0, QIcon("systemImages\\layermangericon\\tdtVector.png"));

		//////tdt矢量
		osgEarth::Drivers::TDTOptions tdtMapOption;
		tdtMapOption.maxDataLevel() = 17;
		tdtMapOption.key() = "vec_c";
		osgEarth::ImageLayer* pMapLayer = new osgEarth::ImageLayer("TDTMapLayer", tdtMapOption);
		pTDTMapItem->WebImageLayer = pMapLayer;
		pMapLayer->setVisible(false);
		pTDTMapItem->setCheckState(0, pMapLayer->getVisible() ? Qt::Checked : Qt::Unchecked);
		//this->mpRefGlobeWidget->getMapRef()->addImageLayer(pMapLayer);
		pWebMapItem->addChild(pTDTMapItem);
	}
	{
		VRWebImageLayerItem* pTDTImageItem = new VRWebImageLayerItem();
		pTDTImageItem->setText(0, QString(getUtf8String("天地图影像").c_str()).arg(1));
		pTDTImageItem->setCheckState(0,Qt::Checked);
		pTDTImageItem->setIcon(0, QIcon("systemImages\\layermangericon\\tdtMap.png"));

		//////tdt影像
		osgEarth::Drivers::TDTOptions tdtImageOption;
		tdtImageOption.key() = "img_c";
		tdtImageOption.maxDataLevel() = 17;
		osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer("TDTImageLayer", tdtImageOption);
		pImageLayer->setVisible(false);
		pTDTImageItem->WebImageLayer = pImageLayer;
		pTDTImageItem->setCheckState(0, pImageLayer->getVisible() ? Qt::Checked : Qt::Unchecked);
		//this->mpRefGlobeWidget->getMapRef()->addImageLayer(pImageLayer);
		pWebMapItem->addChild(pTDTImageItem);
	}

	{
		//VRWebImageLayerItem* pSeaMapMapItem = new VRWebImageLayerItem();
		//pSeaMapMapItem->setText(0, QString(tr("船讯海图")).arg(1));
		//pSeaMapMapItem->setIcon(0, QIcon("systemImages\\layermangericon\\chuanXunMap.png"));

		////////Arcgis影像
		//osgEarth::Drivers::SeaMapOptions seaMapOptions;
		//seaMapOptions.maxDataLevel() = 20;
		//osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer("SeaImageLayer", seaMapOptions);
		//pImageLayer->setVisible(false);
		//pSeaMapMapItem->WebImageLayer = pImageLayer;
		//pSeaMapMapItem->setCheckState(0, pImageLayer->getVisible() ? Qt::Checked : Qt::Unchecked);
		//pWebMapItem->addChild(pSeaMapMapItem);
	}

	{
		//VRWebImageLayerItem* pArcGisMapItem = new VRWebImageLayerItem();
		//pArcGisMapItem->setText(0, QString(tr("ArcGis影像")).arg(1));
		//pArcGisMapItem->setIcon(0, QIcon("systemImages\\layermangericon\\arcgis.png"));

		////////Arcgis影像
		//osgEarth::Drivers::ArcgisImageOptions arcgisImageOption;
		//arcgisImageOption.maxDataLevel() = 15;
		//osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer("ArcgisImageLayer", arcgisImageOption);
		//pImageLayer->setVisible(false);
		//pArcGisMapItem->WebImageLayer = pImageLayer;
		//pArcGisMapItem->setCheckState(0, pImageLayer->getVisible() ? Qt::Checked : Qt::Unchecked);
		//pWebMapItem->addChild(pArcGisMapItem);
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
	pWebDemItem->setText(0, QString(getUtf8String("在线高程").c_str()).arg(1));
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
		pTDTDemItem->setText(0, QString(getUtf8String("ReadyMap").c_str()).arg(1));
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
	pLocalDataItem->setText(0, QString(getUtf8String("本地图层").c_str()).arg(1));
	pLocalDataItem->setIcon(0, QIcon("systemImages\\layermangericon\\local.png"));
	this->mpRootItem->addChild(pLocalDataItem);
	{
		VRLocalImageGroupItem* pLocalImageItem = new VRLocalImageGroupItem();
		pLocalImageItem->setText(0, QString(getUtf8String("影像数据").c_str()).arg(1));
		pLocalImageItem->setIcon(0, QIcon("systemImages\\layermangericon\\image.png"));
		pLocalDataItem->addChild(pLocalImageItem);

		VRLocalDemGroupItem* pLocalDemItem = new VRLocalDemGroupItem();
		pLocalDemItem->setText(0, QString(getUtf8String("高程数据").c_str()).arg(1));
		pLocalDemItem->setIcon(0, QIcon("systemImages\\layermangericon\\dem.png"));
		pLocalDataItem->addChild(pLocalDemItem);

		VRLocalVectorGroupItem* pLocalVectorItem = new VRLocalVectorGroupItem();
		pLocalVectorItem->setText(0, QString(getUtf8String("矢量数据").c_str()).arg(1));
		pLocalVectorItem->setIcon(0, QIcon("systemImages\\layermangericon\\vector.png"));
		pLocalDataItem->addChild(pLocalVectorItem);

		VRLocalVectorGroupItem* pLocalQinXieItem = new VRLocalVectorGroupItem();
		pLocalQinXieItem->setText(0, QString(getUtf8String("倾斜数据").c_str()).arg(1));
		pLocalQinXieItem->setIcon(0, QIcon("systemImages\\layermangericon\\vector.png"));
		pLocalDataItem->addChild(pLocalQinXieItem);

		VRLocalModelLayerItem* pLocalModelItem = new VRLocalModelLayerItem();
		pLocalModelItem->setText(0, QString(getUtf8String("模型数据").c_str()).arg(1));
		pLocalModelItem->setIcon(0, QIcon("systemImages\\layermangericon\\surfacemodel.png"));
		pLocalDataItem->addChild(pLocalModelItem);
	}

	VRGeoDrawGroupItem* pGeoDrawDataItem = new VRGeoDrawGroupItem();
	pGeoDrawDataItem->setText(0, QString(getUtf8String("地理标注").c_str()).arg(1));
	pGeoDrawDataItem->setIcon(0, QIcon("systemImages\\layermangericon\\draw.png"));
	this->mpRootItem->addChild(pGeoDrawDataItem);
}

void VRLayerManager::contextMenuEvent(QContextMenuEvent * event )
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

void VRLayerManager::mousePressEvent(QMouseEvent *event)
{
	QTreeWidget::mousePressEvent(event);
}

void VRLayerManager::mouseReleaseEvent(QMouseEvent *  event)
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

void VRLayerManager::mouseDoubleClickEvent(QMouseEvent *event)
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

void VRLayerManager::slotAddGeoIcon()
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

void VRLayerManager::slotAddPolyline()
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

void VRLayerManager::slotAddPolygon()
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

void VRLayerManager::slotAddScutcheon()
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

void VRLayerManager::slotAddArrow()
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
	/*if (dynamic_cast<Geo::CGeoPolygon*>(pSceneObject))
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
	}*/

	return "systemImages/layermangericon/drawPoint.png";
}

void VRLayerManager::refreshObjectToTree(Common::ISceneObject* pSceneObject)
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

void VRLayerManager::onEvent(Common::IObject* pResult)
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

VRServiceManager::VRServiceManager(QWidget *parent )
	: QTreeWidget(parent)
{
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

VRServiceManager::~VRServiceManager()
{

}

VRGlobeWidget* VRServiceManager::getGlobeWidget()
{
	return this->mpRefGlobeWidget;
}

void VRServiceManager::setGlobeWidget(VRGlobeWidget* pRefGlobeWidget)
{
	this->mpRefGlobeWidget = pRefGlobeWidget;
}

void VRServiceManager::initial()
{
	this->createDefaultItems();
}

void VRServiceManager::createDefaultItems()
{
	this->setHeaderHidden(true);

	//创建场景根节点
	this->mpRootItem = new VRServiceItem();
	this->mpRootItem->setText(0,QString(getUtf8String("服务列表").c_str()).arg(1));
	std::string iconName = "systemImages\\layermangericon\\layer.png";
	this->mpRootItem->setIcon(0,QIcon(iconName.c_str()));
	this->addTopLevelItem(this->mpRootItem);
	//模型轻量化服务
	{
		VRServiceItem* pModelHandlerItem = new VRServiceItem();
		pModelHandlerItem->ServiceType = 1;
		pModelHandlerItem->setText(0, QString(getUtf8String("模型轻量化服务").c_str()).arg(1));
		pModelHandlerItem->setIcon(0, QIcon("systemImages/layermangericon/webData.png"));
		this->mpRootItem->addChild(pModelHandlerItem);
	}
	//模型格式转换服务
	{
		VRServiceItem* pModelTransformItem = new VRServiceItem();
		pModelTransformItem->ServiceType = 2;
		pModelTransformItem->setText(0, QString(getUtf8String("模型格式转换服务").c_str()).arg(1));
		pModelTransformItem->setIcon(0, QIcon("systemImages/layermangericon/webData.png"));
		this->mpRootItem->addChild(pModelTransformItem);
	}
	//管网三维建模服务
	{
		VRServiceItem* pPipeModelerItem = new VRServiceItem();
		pPipeModelerItem->ServiceType = 3;
		pPipeModelerItem->setText(0, QString(getUtf8String("管网三维建模服务").c_str()).arg(1));
		pPipeModelerItem->setIcon(0, QIcon("systemImages/layermangericon/webData.png"));
		this->mpRootItem->addChild(pPipeModelerItem);
	}
	//矢量海图切片服务
	/*{
		VRServiceItem* pTileCreateorItem = new VRServiceItem();
		pTileCreateorItem->ServiceType = 4;
		pTileCreateorItem->setText(0, QString(getUtf8String("矢量海图切片服务").c_str()).arg(1));
		pTileCreateorItem->setIcon(0, QIcon("systemImages/layermangericon/webData.png"));
		this->mpRootItem->addChild(pTileCreateorItem);
	}*/
	this->expandToDepth(0);
}

void VRServiceManager::contextMenuEvent(QContextMenuEvent * event )
{

}

void VRServiceManager::mousePressEvent(QMouseEvent *event)
{

}

void VRServiceManager::mouseReleaseEvent(QMouseEvent *  event)
{

}

void VRServiceManager::mouseDoubleClickEvent(QMouseEvent *event)
{
	QPoint point = QCursor::pos();
	point = mapFromGlobal(point);
	QTreeWidgetItem *pItem = this->itemAt(point);
	if (pItem)
	{
		VRServiceItem* pServiceItem = dynamic_cast<VRServiceItem*>(pItem);
		if (pServiceItem)
		{
			if (pServiceItem->ServiceType == 1)
			{
				ModelTransformDlg* modelTransformDlg = new ModelTransformDlg(this);
				modelTransformDlg->exec();
			}		
		}
	}
	
}

void VRServiceManager::slotModelTransform()
{

}
