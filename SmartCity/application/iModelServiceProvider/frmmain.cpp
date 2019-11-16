#include "frmmain.h"
#include "ui_frmmain.h"
#include "iconhelper.h"
#include "myhelper.h"
#include <osgDB/FileUtils>
#include <QTextCodec>
#include "GeneratedFiles/ui_splashScreen.h"
#include "ixservice_ifc/ifcparser.h"

QStandardItemModel * DataModel = NULL;

CSplashScreenDlg::CSplashScreenDlg(QWidget *parent, Qt::WFlags flags)
	:QDialog(parent,flags)
{
	this->ui = new Ui::splashScreen();
	this->ui->setupUi(this);

	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
}

CSplashScreenDlg::~CSplashScreenDlg()
{

}

void CSplashScreenDlg::updateState(QString info)
{
	this->ui->systemLoadInfo->setText(info);
}

void CSplashScreenDlg::waitMinites()
{
	this->msleep(1000);
}

frmMain::frmMain(QWidget *parent) :
QDialog(parent),
	ui(new Ui::frmMain)
{
	ui->setupUi(this);
	myHelper::FormInCenter(this);
	this->InitStyle();
	this->setMouseTracking(true);

	mpScenViewer = new Engine::CSceneViewer(this);
	mpScenViewer->initial();
	this->ui->globeLayerOut->addWidget(mpScenViewer);

	this->loadAis();
}

frmMain::~frmMain()
{
	delete ui;
}

void frmMain::processPipe()
{

}

void frmMain::InitStyle()
{
	//设置窗体标题栏隐藏
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
	this->setWindowState(Qt::WindowMaximized);
	location = this->geometry();
	max = false;
	mousePressed = false;

	//安装事件监听器,让标题栏识别鼠标双击
	ui->lab_Title->installEventFilter(this);

	IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
	IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
	IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
	IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
	IconHelper::Instance()->SetIcon(ui->btnMenu_About, QChar(0xf015), 10);
	//IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
}

//bool frmMain::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
//{
//	if (ea.getHandled())
//	{
//		return false;
//	}
//
//
//	osgViewer::View* view = static_cast<osgViewer::View*>(aa.asView());
//
//	if (ea.getEventType() == osgGA::GUIEventAdapter::KEYUP)
//	{
//		switch (ea.getKey())
//		{
//		case osgGA::GUIEventAdapter::KEY_Q:
//		{
//			/*CInstanceObjectManager* pObjectManger = new CInstanceObjectManager();
//			pObjectManger->create(this->_vR_GlobeWidget->getRootRef(), this->_vR_GlobeWidget->getMapNodeRef());
//			this->_vR_GlobeWidget->flyTo(osgEarth::Viewpoint("", 116,
//				39,
//				10, 0, -90, 10));*/
//			break;
//		}
//		case osgGA::GUIEventAdapter::KEY_B:
//		{
//			loadAis();
//			break;
//		}
//		case osgGA::GUIEventAdapter::KEY_P:
//		{
//
//			break;
//		}
//		case osgGA::GUIEventAdapter::KEY_D:
//		{
//			break;
//		}
//		case osgGA::GUIEventAdapter::KEY_A:
//		{
//
//		}
//		default:
//			break;
//		}
//
//	}
//
//	return false;
//}

void frmMain::loadAis()
{
#if 0
	osg::ref_ptr<osg::Node> pModelNode = osgDB::readNodeFile("E:/dataService/bim/school-arc/school-arc.osgb");
	//osg::ref_ptr<osg::Node> pModelNode = osgDB::readNodeFile("E:/dataService/bim/bridge/bridge.osgb");
	if (pModelNode)
	{
		this->mpScenViewer->getRoot()->addChild(pModelNode);
	}
#else
	CIfcParser* parser = new CIfcParser();
	osg::Group* pModelNode = parser->parse("E:/dataService/bim/test.ifc");
	if (pModelNode)
	{
		this->mpScenViewer->getRoot()->addChild(pModelNode);
	}
#endif
}

bool frmMain::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonDblClick) {
		this->on_btnMenu_Max_clicked();
		return true;
	}
	return QObject::eventFilter(obj, event);
}

void frmMain::mouseMoveEvent(QMouseEvent *e)
{
	if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
		this->move(e->globalPos() - mousePoint);
		e->accept();
	}
}

void frmMain::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		mousePressed = true;
		mousePoint = e->globalPos() - this->pos();
		e->accept();
	}
}

void frmMain::keyPressEvent(QKeyEvent * evt)
{
	if (evt->key() == Qt::Key_Alt){


	}
}

void frmMain::mouseReleaseEvent(QMouseEvent *)
{
	mousePressed = false;
}

void frmMain::on_btnMenu_Close_clicked()
{
	qApp->exit();
}

void frmMain::on_btnMenu_Max_clicked()
{
	if (max) {
		this->setGeometry(location);
		IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
		ui->btnMenu_Max->setToolTip("最大化");
	} else {
		location = this->geometry();
		this->setGeometry(qApp->desktop()->availableGeometry());
		IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
		ui->btnMenu_Max->setToolTip("还原");
	}
	max = !max;
	/*this->_vR_GlobeWidget->getViewer()->frame();*/
}

void frmMain::on_btnMenu_Min_clicked()
{
	this->showMinimized();
	//this->_vR_GlobeWidget->getViewer()->frame();
}

void frmMain::on_pushButton_clicked()
{
	//myHelper::ShowMessageBoxInfo("欢迎使用海洋大数据展示平台!");
}

void frmMain::on_pushButton_2_clicked()
{
	/*VR_ToolUI* pToolUI = new VR_ToolUI(this);
	pToolUI->exec();*/
}

void frmMain::placeSearchClicked(const QModelIndex & index)
{
	
}

void frmMain::replyFinished(QNetworkReply* reply)
{
	
}

void frmMain::slotSearchPlaceName()
{
}

void frmMain::slotShowToolControl()
{
	bool visible = this->ui->tabWidget->isVisible();
	this->ui->tabWidget->setVisible(!visible);
}

void frmMain::on_pushButton_3_clicked()
{

}

void frmMain::slotDisMeature()
{
	
}

void frmMain::slotAreaMeature()
{
	

}

void frmMain::slotAzimuthMeature()
{

}

void frmMain::slotClearMeatureResult()
{
	
}
