#include "addDomDlg.h"
#include <QMessageBox>
#include <QWidget>
#include <QFileDialog>
#include "GeneratedFiles/ui_addDomDlg.h"
#include "string"
#include <QTimer>
#include <time.h>
#include <ctime>
#include <QString>  
#include <osgEarthSymbology/Style>
#include <osgEarthSymbology/LineSymbol>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers\gdal\GDALOptions>
#include <osgEarthDrivers/tms/TMSOptions>


using namespace  std;
using namespace Globe;

osgEarth::ImageLayer* CAddDom::s_pImageLayer = NULL;

CAddDom::CAddDom(QWidget *parent /*= 0*/, Qt::WindowFlags f /*= 0*/)
	: QDialog(parent, f)
{
	/*
	this->ui = new Ui::enviromentSetFrm();
	ui->setupUi(this);
	connect(ui->isShowSunshine,SIGNAL(clicked(bool)),this,SLOT(slotIsShowSunshine()));*/

}

Globe::CAddDom::CAddDom(VRGlobeWidget* globeWidget, osgEarth::ImageLayer*& pRefImageLayer)
{
	this->ui = new Ui::addDomFrm();
	ui->setupUi(this);
	this->mGlobeWidget = globeWidget;
	this->setFixedSize(this->width(),this->height());
	setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
	setAttribute(Qt::WA_DeleteOnClose);//×Ô¶¯ÊÍ·Å
	connect(ui->selectPathGdalBtn,SIGNAL(clicked()),this,SLOT(slotOpenFileGdalDlg()));
	connect(ui->selectPathTmsBtn,SIGNAL(clicked()),this,SLOT(slotOpenFileTmsDlg()));
	connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(slotOkBtn()));
	connect(ui->cancleBtn,SIGNAL(clicked()),this,SLOT(slotCancleBtn()));
}


CAddDom::~CAddDom()
{

}

osgEarth::ImageLayer* CAddDom::getImageLayer()
{
	return s_pImageLayer;
}

void CAddDom::setNullImageLayer()
{
	s_pImageLayer = NULL;
}

void Globe::CAddDom::slotOpenFileGdalDlg()
{
	mFilePathGdal = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("tif(*.tif)")); 
	QFileInfo fi;
	fi = QFileInfo(mFilePathGdal);
	mFileNameGdal = fi.fileName();
	ui->domNameGdalBtn->setText(mFileNameGdal);
	ui->domPathGdalBtn->setText(mFilePathGdal);
}

void Globe::CAddDom::slotOpenFileTmsDlg()
{

	mFilePathTms = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("xml(*.xml)")); 
	QFileInfo fi;
	fi = QFileInfo(mFilePathTms);
	mFileNameTms = fi.fileName();
	ui->domNameTmsBtn->setText(mFileNameTms);
	ui->domPathTmsBtn->setText(mFilePathTms);
}

void Globe::CAddDom::slotOkBtn()
{
	if (mFilePathGdal == NULL && mFilePathTms == NULL)
	{
		QMessageBox::information(this,"error","please select a dom data .");
		return;
	}
	else if (mFilePathGdal !=NULL)
	{
		osgEarth::Drivers::GDALOptions image_gdal;
		image_gdal.url() = mFilePathGdal.toStdString();
		osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer(mFileNameGdal.toStdString(), image_gdal );
		mGlobeWidget->getMapRef()->addImageLayer(pImageLayer);
		CAddDom::s_pImageLayer = pImageLayer;
	}
	else if (mFilePathTms !=NULL)
	{
		osgEarth::Drivers::TMSOptions imgOption;
		imgOption.url() = mFilePathTms.toStdString();
		osgEarth::ImageLayer* pImageLayer = new osgEarth::ImageLayer( mFileNameTms.toStdString(), imgOption ) ;
		mGlobeWidget->getMapRef()->addImageLayer(pImageLayer);
		CAddDom::s_pImageLayer = pImageLayer;
	}
	this->accept();
	this->close();
}

void Globe::CAddDom::slotCancleBtn()
{
	this->close();
}





