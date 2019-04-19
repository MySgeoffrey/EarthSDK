#include "myearthsim.h"
#include <osgDB/FileUtils>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <QMessageBox>
#include <QComboBox>
#include <QStackedLayout>
#include <QCursor>
#include <QDateTime>
#include <osgEarthSymbology/AltitudeSymbol>
#include "globemap/globemap.h"
#include <osgEarthUtil/RTTPicker>
#include <osgEarth/Registry>
#include <osgEarthUtil/MouseCoordsTool>
#include <QFileDialog>
#include "tilesource/osgdb_osgearth_shp.h"

using namespace osgEarth::Drivers;
using namespace osgEarth;

myEarthSim::myEarthSim(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	setlocale(LC_ALL, "chs");
	this->ui.gridLayout->setMargin(0);
	this->ui.gridLayout->setSpacing(0);
	this->ui.gridLayout->setContentsMargins(0, 0, 0, 0);

	initialFireEngineScene();
}

myEarthSim::~myEarthSim()
{

}

void myEarthSim::initialFireEngineScene()
{
	this->slotShowEarthControl();
	this->slotActionAddTDTImage();
	
}

void myEarthSim::slotShowEarthControl()
{
#if 1 //显示三维场景控件 
	//创建场景控件对象（参数必须为NULL）
	this->mpGlobeWidget = new VRGlobeWidget();
	//获取工程的工作目录(VR-GlobeConfig3D.earth文件一般与exe在同一目录)
	std::string runPath = osgDB::getCurrentWorkingDirectory();
	std::string runPath_3d = runPath + "/VR-GlobeConfig3D.earth";
	//场景控件初始化，建立三维地图
	this->mpGlobeWidget->initial(true, runPath_3d.c_str());
	//将三维场景控件添加到qt界面
	this->ui.gridLayout->addWidget(this->mpGlobeWidget);
	Globe::CGlobeMap::getInstance()->initialise(this->mpGlobeWidget->getMapRef());
#endif
}

void myEarthSim::slotActionAddTDTImage()
{
	//Globe::CGlobeMap::getInstance()->addImageLayer("","d00fa",Globe::MapType::WEB_TDT_IMAGE_MT);
}

void myEarthSim::slotLoadS57()
{

	std::string runPath = osgDB::getCurrentWorkingDirectory();
	std::string shpPath = runPath + "/C1313100.000";

	ShpTileOptions featureOptions;
	featureOptions.key() = shpPath;
	osgEarth::ImageLayerOptions imageLayerOption(shpPath,featureOptions);
	imageLayerOption.minLevel() = 0;
	imageLayerOption.maxLevel() = 15;
	osgEarth::ImageLayer* pLayer = new ImageLayer(shpPath, imageLayerOption);
	this->mpGlobeWidget->getMapRef()->addImageLayer(pLayer);
}
