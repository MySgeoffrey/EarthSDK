#include "addShpDlg.h"
#include <QMessageBox>
#include <QWidget>
#include <QFileDialog>
#include <qtextcodec.h>
#include "GeneratedFiles/ui_addShpDlg.h"
#include "string"
#include <QTimer>
#include <time.h>
#include <ctime>
#include <QString>  
#include <osgEarthSymbology/Style>
#include <osgEarthSymbology/LineSymbol>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthSymbology/Geometry>
#include <osgEarthFeatures/Feature>
#include <osgEarthAnnotation/FeatureNode>
#include "globemap/globemap.h"
#include "framework/common/globesetting.h"
//#include <..\\osgdb_osgearth_s57map\\osgdb_osgearth_s57map.h>

using namespace  std;
using namespace Globe;
using osgEarth::Symbology::Style;
using osgEarth::Symbology::LineSymbol;
using osgEarth::Symbology::Color;
using osgEarth::Drivers::OGRFeatureOptions;
using osgEarth::Drivers::FeatureGeomModelOptions;
using osgEarth::Symbology::StyleSheet;
using osgEarth::ModelLayerOptions;
using osgEarth::ModelLayer;

std::string getUtf8StringEx(std::string str)
{
	std::string utf8String;
	CGlobeSetting::instance()->gb2312ToUtf8(str, utf8String);
	return utf8String;
}

CAddShp::CAddShp(QWidget *parent, Qt::WindowFlags f)
	: QDialog(parent, f)
{
	/*
	this->ui = new Ui::enviromentSetFrm();
	ui->setupUi(this);
	connect(ui->isShowSunshine,SIGNAL(clicked(bool)),this,SLOT(slotIsShowSunshine()));*/

}

Globe::CAddShp::CAddShp( VRGlobeWidget* globeWidget )
{
	this->ui = new Ui::addShpFrm();
	ui->setupUi(this);
	this->mGlobeWidget = globeWidget;
	this->setFixedSize(this->width(),this->height());
	setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
	setAttribute(Qt::WA_DeleteOnClose);//自动释放
	connect(ui->selectDataPathBtn,SIGNAL(clicked()),this,SLOT(slotOpenFileDlg()));
	connect(ui->cancleBtn,SIGNAL(clicked()),this,SLOT(slotCancleBtn()));	
	connect(ui->OkBtn,SIGNAL(clicked()),this,SLOT(slotOkBtn()));
}

CAddShp::~CAddShp()
{

}

void Globe::CAddShp::slotOpenFileDlg()
{
	mFilepath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit(getUtf8StringEx("打开矢量文件").c_str()), " ",  QString::fromLocal8Bit(getUtf8StringEx("文件格式(*.shp *.000 *.my)").c_str())); 
	QFileInfo fi;
	fi = QFileInfo(mFilepath);
	mFilepath = mFilepath;
	mFileName = fi.fileName();
	ui->dataName->setText(mFileName);
	ui->dataPath->setText(mFilepath);

}

void Globe::CAddShp::slotOkBtn()
{
	if (mFilepath == NULL)
	{
		QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("请选择一个矢量数据文件"));
		return;
	}
#if 0
	Style style;
	LineSymbol* ls=style.getOrCreateSymbol<LineSymbol>();
	QColor qc=QColor::fromHsl(rand()%360,rand()%256,rand()%200);
	ls->stroke()->color() = Color::Color(osgEarth::Symbology::Color::Yellow, 1);
	ls->stroke()->width()=3.0f;

	style.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() =
		osgEarth::Symbology::Color(osgEarth::Symbology::Color::Green, 0.5);
	/*osgEarth::Symbology::ExtrusionSymbol* extrusion = style.getOrCreate<osgEarth::Symbology::ExtrusionSymbol>();
	extrusion->height() = 50.0;*/

	style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
	style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;

	OGRFeatureOptions featureOptions;
	featureOptions.url()= mFilepath.toStdString();
	featureOptions.buildSpatialIndex() = true;
	//featureOptions.geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;


	FeatureGeomModelOptions geomOptions;
	geomOptions.featureOptions()=featureOptions;
	geomOptions.styles()=new StyleSheet();
	geomOptions.styles()->addStyle(style);
	geomOptions.enableLighting()=false;

	ModelLayerOptions layerOptions(mFileName.toStdString(),geomOptions);
	//layerOptions.overlay()=false;
	ModelLayer* pVectorLayer = new ModelLayer(layerOptions);
	mGlobeWidget->getMapRef()->addModelLayer(pVectorLayer);
	emit createVectorLayer(pVectorLayer);
#else

	/*osgEarth::Drivers::MyS57MapOptions s57Option;
	s57Option.maxDataLevel() = 20;
	s57Option.key() = mFilepath.toStdString();
	osgEarth::ImageLayerOptions imageLayerOption(s57Option);
	osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer(mFileName.toStdString(), imageLayerOption);

	mGlobeWidget->getMapRef()->addImageLayer(pImageLayer);
	emit createVectorLayer(pImageLayer);*/
	osgEarth::ImageLayer* pImageLayer = Globe::CGlobeMap::getInstance()->addCFGShpModelLayer(mFilepath.toStdString(),mFileName.toStdString());
	emit createVectorLayer(pImageLayer);
#endif
	this->close();

}

void Globe::CAddShp::slotCancleBtn()
{
	this->close();
}




