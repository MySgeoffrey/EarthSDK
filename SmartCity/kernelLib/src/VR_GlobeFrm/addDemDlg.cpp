#include "addDemDlg.h"
#include <QMessageBox>
#include <QWidget>
#include <QFileDialog>
#include "GeneratedFiles/ui_addDemDlg.h"
#include "string"
#include <QTimer>
#include <time.h>
#include <ctime>
#include <osgEarthDrivers\gdal\GDALOptions>
#include <osgEarthDrivers/tms/TMSOptions>
using namespace  std;
using namespace Globe;

osgEarth::ElevationLayer * CAddDem::s_pDemLayer = NULL;

CAddDem::CAddDem(QWidget *parent /*= 0*/, Qt::WindowFlags f /*= 0*/)
	: QDialog(parent, f)
{
	/*
	this->ui = new Ui::enviromentSetFrm();
	ui->setupUi(this);
	connect(ui->isShowSunshine,SIGNAL(clicked(bool)),this,SLOT(slotIsShowSunshine()));*/

}

Globe::CAddDem::CAddDem(VRGlobeWidget* globeWidget, osgEarth::ElevationLayer*& pRefDemLayer)
{
	this->ui = new Ui::addDemFrm();
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

CAddDem::~CAddDem()
{

}

osgEarth::ElevationLayer* CAddDem::getDemLayer()
{
	return CAddDem::s_pDemLayer;
}

void CAddDem::setNullDemLayer()
{
	CAddDem::s_pDemLayer = NULL;
}
	

void Globe::CAddDem::slotOpenFileGdalDlg()
{
	mFilePathGdal = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("tif(*.tif)")); 
	QFileInfo fi;
	fi = QFileInfo(mFilePathGdal);
	mFileNameGdal = fi.fileName();
	ui->demNameGdalBtn->setText(mFileNameGdal);
	ui->demPathGdalBtn->setText(mFilePathGdal);
}

void Globe::CAddDem::slotOpenFileTmsDlg()
{
	mFilePathTms = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("xml(*.xml)")); 
	QFileInfo fi;
	fi = QFileInfo(mFilePathTms);
	mFileNameTms = fi.fileName();
	ui->demNameTmsBtn->setText(mFileNameTms);
	ui->demPathTmsBtn->setText(mFilePathTms);
}

void Globe::CAddDem::slotOkBtn()
{
	if (mFilePathGdal == NULL && mFilePathTms == NULL)
	{
		QMessageBox::information(this,"error","please select a dem data .");
		return;
	}
	else if (mFilePathGdal !=NULL)
	{
		osgEarth::Drivers::GDALOptions dem_gdal;
		dem_gdal.url() = mFilePathGdal.toStdString();
		osgEarth::ElevationLayer* pDemLayer = new osgEarth::ElevationLayer(mFileNameGdal.toStdString(), dem_gdal );
		mGlobeWidget->getMapRef()->addElevationLayer(pDemLayer);
		CAddDem::s_pDemLayer = pDemLayer;
	}
	else if (mFilePathTms !=NULL)
	{
		osgEarth::Drivers::TMSOptions elvOption;
		elvOption.url() = mFilePathTms.toStdString();
		osgEarth::ElevationLayer* pDemLayer = new osgEarth::ElevationLayer(mFileNameTms.toStdString(), elvOption);
		mGlobeWidget->getMapRef()->addElevationLayer(pDemLayer);
		CAddDem::s_pDemLayer = pDemLayer;
	}
	this->accept();
	this->close();
}

void Globe::CAddDem::slotCancleBtn()
{
	this->close();
}






