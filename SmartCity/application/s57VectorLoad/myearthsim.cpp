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
#include "mychartmapcontrol/mychartmapcontrol.h"
#include "mychartsymbollib/mychartsymbollib.h"
#include "mychartdatadriverlib/mychartsdatalib.h"
#include "mychartcommonlib/mychartcommonlib.h"
#include "mychartmaplib//imymapmanager.h"
#include "mychartdrawlib/imys57map.h"

using namespace osgEarth::Drivers;
using namespace osgEarth;

myEarthSim::myEarthSim(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	setlocale(LC_ALL, "chs");
	this->ui.gridLayout3D->setMargin(0);
	this->ui.gridLayout3D->setSpacing(0);
	this->ui.gridLayout3D->setContentsMargins(0, 0, 0, 0);

	initialFireEngineScene();
}

myEarthSim::~myEarthSim()
{

}

void myEarthSim::initialFireEngineScene()
{
	this->slotShowEarthControl();
	this->slotShowMapControl();
	this->slotActionAddTDTImage();
	
}

void myEarthSim::slotShowMapControl()
{
	MyChart::CS57Symbollib::instance()->initial();
	MyChart::CMyShpDataDriver::initial();
	MyChartMapControl* pMapControl = new MyChartMapControl(this);
	pMapControl->getGridLayer()->setVisible(true);
	this->mpMapWidget = pMapControl;
	this->ui.gridLayout2D->addWidget(pMapControl);

	pMapControl->setBackgroundColor(QColor(255,255,255,255));
	pMapControl->setCoorsystemType(MyChart::MCT_Prj_MECATOR/*MyChart::MCT_Geo_WGS84*/);
	//MyChart::CMyChartCommonlib::MapCoorsystemType = MyChart::MCT_Prj_MECATOR;//MCT_Prj_MECATOR
	pMapControl->fullExtend();
	pMapControl->setMapDrawMode(MDM_SYN);
	pMapControl->refresh();
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
	this->ui.gridLayout3D->addWidget(this->mpGlobeWidget);
	Globe::CGlobeMap::getInstance()->initialise(this->mpGlobeWidget->getMapRef());
#endif
}

void myEarthSim::slotActionAddTDTImage()
{
	Globe::CGlobeMap::getInstance()->addImageLayer("","GOOGLE",Globe::MapType::WEB_GOOGLE_MT);
}

void myEarthSim::slotLoadS57()
{
	//三维添加海图
	std::string runPath = osgDB::getCurrentWorkingDirectory();
	std::string shpPath = runPath + "/C1313100.000";

	ShpTileOptions featureOptions;
	featureOptions.key() = shpPath;
	osgEarth::ImageLayerOptions imageLayerOption(shpPath,featureOptions);
	imageLayerOption.minLevel() = 0;
	imageLayerOption.maxLevel() = 15;
	osgEarth::ImageLayer* pLayer = new ImageLayer(shpPath, imageLayerOption);
	this->mpGlobeWidget->getMapRef()->addImageLayer(pLayer);

	//二维添加海图
	QString filePath = QString::fromStdString(shpPath);
	if (filePath.contains(".000"))
	{
		MyChart::CMyChartS57DataDriver driver;
		MyChart::IMyVectorMapData* pS57Data = driver.readS57Map(filePath);
		if (pS57Data)
		{
			MyChart::IMyS57Map* pS57Map = new MyChart::IMyS57Map(pS57Data->getName(), pS57Data);
			pS57Map->setFilePath(pS57Data->getFilePath());
			MyChart::IMyMapManager::instance()->getMaps().push_back(pS57Map);
			this->mpMapWidget->fullExtend();
			this->mpMapWidget->refresh();
		}
	}
}
