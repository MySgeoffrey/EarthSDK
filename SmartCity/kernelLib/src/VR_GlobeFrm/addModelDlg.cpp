#include "addModelDlg.h"
#include <QMessageBox>
#include <QWidget>
#include <QFileDialog>
//#include <Qt\qtextcodec.h>
#include "GeneratedFiles/ui_addModelDlg.h"
#include "string"
#include <QTimer>
#include <time.h>
#include <ctime>
#include <osgDB/readfile>  
#include <osgDB/writefile>  
#include <osg/Math>
#include "framework/common/iobject.h"


using namespace  std;
using namespace Globe;


CAddModel::CAddModel(QWidget *parent /*= 0*/, Qt::WindowFlags f /*= 0*/)
	: QDialog(parent, f)
{
	/*
	this->ui = new Ui::enviromentSetFrm();
	ui->setupUi(this);
	connect(ui->isShowSunshine,SIGNAL(clicked(bool)),this,SLOT(slotIsShowSunshine()));*/

}

Globe::CAddModel::CAddModel( VRGlobeWidget* globeWidget ,osgEarth::Util::ObjectLocatorNode*& pRefNodeLocator)
{
	this->ui = new Ui::addModelFrm();
	ui->setupUi(this);
	this->mGlobeWidget = globeWidget;
	this->setFixedSize(this->width(),this->height());
	setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
	setAttribute(Qt::WA_DeleteOnClose);//×Ô¶¯ÊÍ·Å
	connect(ui->cancleBtn,SIGNAL(clicked()),this,SLOT(slotCancleBtn()));
	connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(slotOkBtn()));
	connect(ui->selectModelPathBtn,SIGNAL(clicked()),this,SLOT(slotOpenFile()));
	this->mpRefNodeLocator = new osgEarth::Util::ObjectLocatorNode( mGlobeWidget->getMapNodeRef()->getMap() );  
	pRefNodeLocator = this->mpRefNodeLocator;
}

void Globe::CAddModel::slotOpenFile()
{
	mModelPath = QFileDialog::getOpenFileName(this, tr("Open Model File"), " ",
		tr("Model files(*.osg *.osgb *.flt *.ive *.stl *.3ds *.dae *.dxf *.x *.obj)")); 
	QFileInfo fi;
	fi = QFileInfo(mModelPath);
	mModelName = fi.fileName();
	ui->modelPath->setText(mModelPath);
	ui->modelName->setText(mModelName);
}

void Globe::CAddModel::slotOkBtn()
{
	if (mModelPath == NULL )
	{
		QMessageBox::information(this,"error","please select a modelFile .");
		return;
	}
	
	mModelScaleX = ui->setModelScaleX->text().toFloat();
	mModelScaleY = ui->setModelScaleY->text().toFloat();
	mModelScaleZ = ui->setModelScaleZ->text().toFloat();
	mModelLong = ui->setModelLong->text().toFloat();
	mModelLati = ui->setModelLati->text().toFloat();
	mModelAlti = ui->setModelAlti->text().toFloat();
	mModelRotateX = ui->setModelRouteX->text().toFloat();
	mModelRotateY = ui->setModelRouteY->text().toFloat();
	mModelRotateZ = ui->setModelRouteZ->text().toFloat();

	osg::Node* modelNode = osgDB::readNodeFile(QString::fromLocal8Bit(mModelPath.toLocal8Bit()).toStdString());

	osg::ref_ptr<Common::IObject> userData = new Common::IObject();
	userData->setName(mModelName.toStdString());
	modelNode->setUserData(userData);

	osg::MatrixTransform* mt = new osg::MatrixTransform();

	/*float iRadians = osg::DegreesToRadians(-45.0);
	osg::Quat quat(iRadians, osg::Vec3(0,0,1));*/
	mt->setMatrix(osg::Matrixd::scale(mModelScaleX,mModelScaleY,mModelScaleZ)*osg::Matrixd::rotate(osg::DegreesToRadians(mModelRotateX),osg::Vec3(1,0,0),osg::DegreesToRadians(mModelRotateY),osg::Vec3(0,1,0),osg::DegreesToRadians(mModelRotateZ),osg::Vec3(0,0,1)));

	//mt->setMatrix(osg::Matrixd::rotate(quat));
	//mt->setMatrix(osg::Matrixd::rotate(osg::PI_2,osg::Vec3(1,0,0),osg::PI_2,osg::Vec3(0,1,0),osg::PI_2,osg::Vec3(0,0,1)));
	//mt->setMatrix(osg::Matrixd::rotate(osg::PI_2,osg::Vec3(1,0,0)));

	mt->addChild( modelNode );

	
	this->mpRefNodeLocator->getLocator()->setPosition(osg::Vec3d(mModelLong,  mModelLati, mModelAlti ) );      
	this->mpRefNodeLocator->addChild( mt );
	this->mpRefNodeLocator->setName(mModelName.toStdString());
	osg::ref_ptr<osg::LOD> pLod = new osg::LOD();
	pLod->addChild( this->mpRefNodeLocator,0,10000);
	mGlobeWidget->getRootRef()->addChild(pLod);
	//osgEarth::Viewpoint vp( "", mModelLong, mModelLati, 1.0, -128.5, -19, 1620 );
	//this->mGlobeWidget->flyTo(vp,4);
	this->accept();
}

void Globe::CAddModel::slotCancleBtn()
{
	this->reject();
}

CAddModel::~CAddModel()
{

}




