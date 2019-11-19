#include "frmmain.h"
#include "ui_frmmain.h"
#include "iconhelper.h"
#include "myhelper.h"
#include <QtWidgets/QMessageBox>
#include <osgDB/FileUtils>
#include "globemap/globemap.h"
#include <osgEarth/HTTPClient>
#include <osgEarth/Notify>
#include <osgEarth/Config>
#include "framework/common/globesetting.h"
#include <QTextCodec>
#include "citybuilder/instancemodellayer.h"
#include "vr_layermanager.h"
#include "scenetool/distancemeasure.h"
#include "scenetool/areameasure.h"
#include <osgEarth/XmlUtils>
#include "GeneratedFiles/ui_splashScreen.h"
#include "pipenet/pipeline.h"
#include "pipenet/pipepoint.h"
#include "TestCase.h"
#include "framework/common/languageconfig.h"
//#include "pipenet/pipearithmetic.h"

#define _use_amap_api_ 1
#pragma execution_character_set("utf-8")

QStandardItemModel * DataModel = NULL;

CSplashScreenDlg::CSplashScreenDlg(QWidget *parent, Qt::WindowFlags f)
:QDialog(parent, f)
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

std::string getUtf8String(std::string str)
{
	std::string utf8String;
	CGlobeSetting::instance()->gb2312ToUtf8(str, utf8String);
	return utf8String;
}

void CSplashScreenDlg::waitMinites()
{
	this->msleep(1000);
}

frmMain::frmMain(QWidget *parent) :
QDialog(parent),
	ui(new Ui::frmMain)
{
	CSplashScreenDlg* pSplashScreenDlg = new CSplashScreenDlg();
	pSplashScreenDlg->show();
	
	//std::string utf8String;
	//std::string str("中国人");
	//CGlobeSetting::instance()->gb2312ToUtf8(str, utf8String);
	//pSplashScreenDlg->updateState(QString::fromLocal8Bit(utf8String.c_str()));//正在初始化应用程序
	qApp->processEvents();
	pSplashScreenDlg->waitMinites();

	ui->setupUi(this);
	myHelper::FormInCenter(this);
	//pSplashScreenDlg->updateState(QString::fromLocal8Bit("中"));//正在初始化界面风格
	qApp->processEvents();
	pSplashScreenDlg->waitMinites();
	this->InitStyle();
	this->setMouseTracking(true);

	this->mpGeometryEditor = NULL;

	//pSplashScreenDlg->updateState(QString::fromLocal8Bit("sdfas"));//正在初始化地球控件
	qApp->processEvents();
	pSplashScreenDlg->waitMinites();
#if 1 //添加地球控件
	CLanguageConfig::setValue(CLanguageConfig::StateTextType_Longitude,"Longitude:");
	CLanguageConfig::setValue(CLanguageConfig::StateTextType_Lantitude, "Lantitude:");
	CLanguageConfig::setValue(CLanguageConfig::StateTextType_TerrainAltitude, "TerrainAltitude:");
	CLanguageConfig::setValue(CLanguageConfig::StateTextType_ViewAltitude, "ViewAltitude:");

	this->mpGlobeWidget = new VRGlobeWidget();
	//获取工程的工作目录(VR-GlobeConfig3D.earth文件一般与exe在同一目录)
	std::string runPath = osgDB::getCurrentWorkingDirectory();
	std::string runPath_3d = runPath + "/VR-GlobeConfig3D.earth";
	//场景控件初始化，建立三维地图
	this->mpGlobeWidget->initial(true, runPath_3d.c_str());
	this->ui->globeLayerOut->setMargin(0);
	this->ui->globeLayerOut->setSpacing(0);
	this->ui->globeLayerOut->setContentsMargins(0, 0, 0, 0);
	this->ui->globeLayerOut->addWidget(this->mpGlobeWidget);

	
#endif

#if 1 //添加图层管理控件
	//pSplashScreenDlg->updateState(QString::fromLocal8Bit("加载完成"));//正在初始化图层面板
	qApp->processEvents();
	pSplashScreenDlg->waitMinites();
	VRLayerManager* pLayerManager = new VRLayerManager();
	pLayerManager->setGlobeWidget(this->mpGlobeWidget);
	pLayerManager->initial();
	this->ui->layerManagerLayout->setMargin(0);
	this->ui->layerManagerLayout->setSpacing(0);
	this->ui->layerManagerLayout->setContentsMargins(0, 0, 0, 0);
	this->ui->layerManagerLayout->addWidget(pLayerManager);
#endif

#if 1 //添加服务管理控件
	//pSplashScreenDlg->updateState(QString::fromLocal8Bit("sdfas"));//正在初始化图层面板
	qApp->processEvents();
	//pSplashScreenDlg->waitMinites();
	VRServiceManager* pServiceManager = new VRServiceManager();
	pServiceManager->setGlobeWidget(this->mpGlobeWidget);
	pServiceManager->initial();
	this->ui->serviceGridLayout->setMargin(0);
	this->ui->serviceGridLayout->setSpacing(0);
	this->ui->serviceGridLayout->setContentsMargins(0, 0, 0, 0);
	this->ui->serviceGridLayout->addWidget(pServiceManager);
#endif

	int x,y;
	this->mpGlobeWidget->getGlobeScalePosition(x,y);
	this->mpGlobeWidget->setGlobeScalePosition(x,y + 10);

	Globe::CGlobeMap::getInstance()->initialise(this->mpGlobeWidget->getMapRef());
#if 0
	Globe::CGlobeMap::getInstance()->addImageLayer("","tdtImage",Globe::MapType::WEB_TDT_MAP_MT);
#else
	//
	//std::string tilePath = runPath + "/earthData/tileData/";
	//Globe::CGlobeMap::getInstance()->addImageLayer(
	//	tilePath,"tileImage",Globe::GOOGLE_LOCALMECATOR,0,5);

	/*std::string cfgPath = runPath + "/earthData/vector/roadNet.my";
	osgEarth::ImageLayer* pBackImageLayer = Globe::CGlobeMap::getInstance()->addCFGShpModelLayer(cfgPath,"backImage");
	pBackImageLayer->setOpacity(1);*/
#endif
	this->processPipe();
	
	QStandardItemModel * pModel = new QStandardItemModel(); 
	pModel->setColumnCount(4); 
	pModel->setHeaderData(0, Qt::Horizontal, QString(tr("NO")));
	pModel->setHeaderData(1, Qt::Horizontal, QString(tr("Name")));
	pModel->setHeaderData(2, Qt::Horizontal, QString(tr("Address")));
	pModel->setHeaderData(3, Qt::Horizontal, QString(tr("Position")));
	//this->ui->placeSearchWidget->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed); //ResizeToContents
	//this->ui->placeSearchWidget->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed); 
	//this->ui->placeSearchWidget->horizontalHeader()->setResizeMode(2,QHeaderView::Fixed); 
	this->ui->placeSearchWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft); 
	this->ui->placeSearchWidget->setModel(pModel);
	this->ui->placeSearchWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->ui->placeSearchWidget->verticalHeader()->hide();
	DataModel = pModel;
	QObject::connect(this->ui->placeSearchWidget, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(placeSearchClicked(const QModelIndex &)));
	//this->on_btnMenu_Max_clicked();
	//pSplashScreenDlg->updateState(QString::fromLocal8Bit("safdsa"));//正在初始化应用程序
	qApp->processEvents();
	//pSplashScreenDlg->waitMinites();
	delete pSplashScreenDlg;
	pSplashScreenDlg = NULL;

#if 0
	osgEarth::GeoPoint p(this->mpGlobeWidget->getMapNodeRef()->getMapSRS(),
		osg::Vec3d(116.000, 39.002, 15));
	osg::ref_ptr<osg::Node> pModelNode = MeshGenerator::TestCase::exec(p);
	this->mpGlobeWidget->getMapNodeRef()->addChild(pModelNode);

	this->mpGlobeWidget->flyTo(116.000, 39.002, 15);
#endif
}

frmMain::~frmMain()
{
	delete ui;
}

void frmMain::processPipe()
{
	//PipeNet::CPipeLineDataSet* pPipeLineDataSet = NULL;
	//PipeNet::CPipePointDataSet* pPipePointDataSet = NULL;
	//QString dirPath = "D:/dataService/testPipeData";
	//QDir dir(dirPath);
	//if (dir.exists())
	//{
	//	QStringList namefilters;
	//	namefilters << "*.shp";
	//	QStringList files = dir.entryList(namefilters,QDir::Files | QDir::Readable,QDir::Name);
	//	if (!files.empty())
	//	{
	//		for (int i = 0 ; i < files.size(); ++i)
	//		{
	//			QString lPath = files.at(i);
	//			QString pipePath = dirPath + "/" + files.at(i);
	//			if (lPath.contains("YSLine"))
	//			{
	//				CityBuilder::CPipeLayerDriver::load(pipePath.toLocal8Bit().constData(),pPipeLineDataSet);
	//				CityBuilder::CInstanceModelLayer* pPipeLayer_GDLine = new CityBuilder::CInstanceModelLayer(this->mpGlobeWidget->getMapNodeRef());
	//				//pPipeLayer_GDLine->loadDataFromFile(pipePath.toLocal8Bit().constData());
	//				pPipeLayer_GDLine->loadToScene(pPipeLineDataSet);
	//				pPipeLayer_GDLine->renderToScene();
	//			}
	//			else if (lPath.contains("YSPoint"))
	//			{
	//				CityBuilder::CPipeLayerDriver::load(pipePath.toLocal8Bit().constData(),pPipePointDataSet);
	//			}
	//		}			
	//	}
	//}
	//if (pPipeLineDataSet && pPipePointDataSet)
	//{
	//	for (int i = 0; i < pPipePointDataSet->getPipePoints().size(); ++i)
	//	{
	//		PipeNet::CPipePoint* pPipePoint = pPipePointDataSet->getPipePoints().at(i);
	//		std::vector<PipeNet::CPipeLine*> inout_pipeLines;
	//		if (pPipeLineDataSet->getPipeLines(pPipePoint->getID(),inout_pipeLines))
	//		{
	//			pPipePoint->setAdjcentLines(inout_pipeLines);
	//		}
	//	}
	//}

	//for (int i = 0; i < pPipePointDataSet->getPipePoints().size(); ++i)
	//{
	//	PipeNet::CPipePoint* pPipePoint = pPipePointDataSet->getPipePoints().at(i);
	//	if (pPipePoint->getAdjcentLines().size() >= 2)
	//	{
	//		double radius = -0.5;
	//		//弯头建模参数
	//		CPipeLinkerParameter pParameter;
	//		pParameter.LinkerBackDistance = radius * 4;
	//		//处理中心点坐标
	//		osgEarth::GeoPoint centerGeoPos = osgEarth::GeoPoint(
	//			this->mpGlobeWidget->getMapNodeRef()->getMapSRS(),
	//			pPipePoint->getGeoPosition(),
	//			osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
	//		centerGeoPos.toWorld(pParameter.LinkerWorldPos);
	//		std::vector<PipeNet::CPipeLine*>& adjcentPipeLines = pPipePoint->getAdjcentLines();
	//		for (int j = 0; j < adjcentPipeLines.size(); ++j)
	//		{
	//			PipeNet::CPipeLine* pPipeLine = adjcentPipeLines.at(j);
	//			PipeNet::CPipePoint* pAdjencentPoint = NULL;
	//			if (pPipeLine->getStartPointID() == pPipePoint->getID())
	//			{
	//				std::string adjencentID = pPipeLine->getEndPointID();
	//				pAdjencentPoint = dynamic_cast<PipeNet::CPipePoint*>(pPipePointDataSet->getPipePoint(adjencentID));
	//			}
	//			else
	//			{
	//				std::string adjencentID = pPipeLine->getStartPointID();
	//				pAdjencentPoint = dynamic_cast<PipeNet::CPipePoint*>(pPipePointDataSet->getPipePoint(adjencentID));
	//			}
	//			if (pAdjencentPoint != NULL)
	//			{
	//				SubLinkerParameter* pSublinker = new SubLinkerParameter();
	//				pSublinker->Radius = pPipeLine->getRadius();
	//				pSublinker->NeedRingflange = true;
	//				pSublinker->RingflangeThick = pSublinker->Radius / 2;
	//				pSublinker->RingflangeRadius = pSublinker->Radius * 1.5;
	//				osgEarth::GeoPoint otherGeoPos = osgEarth::GeoPoint(
	//					this->mpGlobeWidget->getMapNodeRef()->getMapSRS(),
	//					pAdjencentPoint->getGeoPosition(),
	//					osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
	//				otherGeoPos.toWorld(pSublinker->WorldPos);

	//				pParameter.SubLinkerParameters.push_back(pSublinker);

	//				pParameter.LinkerBackDistance = pPipeLine->getRadius() * 1.8;
	//			}
	//		}
	//		//建模，且生成场景节点
	//		CPipeRenderData* pData = CPipeModelArithmetic::createModel(&pParameter);
	//		if (pData)
	//		{
	//			osg::Node* pNode = CPipeModelArithmetic::createNodeByData(pData);
	//			osg::LOD* pLod = new osg::LOD();
	//			pLod->addChild(pNode,0,1000);
	//			this->mpGlobeWidget->getRootRef()->addChild(pLod);
	//		}
	//	}
	//}
	//this->mpGlobeWidget->flyTo(118.3717, 35.1306,3000,0);//
	//osgEarth::Viewpoint vp("", 118.3717, 35.1306, 3000, 0, -90, 0.0);
	//this->mpGlobeWidget->getEarthManipulator()->setHomeViewpoint(vp);
}

void frmMain::InitStyle()
{
	//设置窗体标题栏隐藏
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
	//this->setWindowState(Qt::WindowMaximized);
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
	//CIconObjectGenerator::instance()->load(
	//	this->_vR_GlobeWidget->getRootRef(),
	//	this->_vR_GlobeWidget->getMapNodeRef());
	/*osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(this->_vR_GlobeWidget->getViewer());
	CObjectManager::instance()->initial(
	this->_vR_GlobeWidget->getRootRef(),
	this->_vR_GlobeWidget->getMapNodeRef(),
	pViewer);

	QString url = QString::fromStdString(osgDB::getCurrentWorkingDirectory() + "/aisData/VDR_Data.txt");
	CDynamicObjetReceiver::instance()->setDataURL(url);
	CDynamicObjetReceiver::instance()->start();*/
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
		ui->btnMenu_Max->setToolTip("Maximize");
	} else {
		location = this->geometry();
		this->setGeometry(qApp->desktop()->availableGeometry());
		IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
		ui->btnMenu_Max->setToolTip("Resume");
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
	int curRow = this->ui->placeSearchWidget->currentIndex().row();
	if (curRow >= 0)
	{
		QModelIndex latlonIndex = DataModel->index(curRow,3);
		QStringList lonlat = DataModel->data(latlonIndex).toString().split(',');
		if (lonlat.size() == 2)
		{
			double lon = lonlat.at(0).toDouble();
			double lat = lonlat.at(1).toDouble();
			this->mpGlobeWidget->flyTo(lon,lat,2000,2);
		}
	}
}

void frmMain::replyFinished(QNetworkReply* reply)
{
	QByteArray data = reply->readAll();
	QString content = data.data();
	if (content != "")
	{
#if _use_amap_api_ == 1
		osgEarth::Config config;
		bool r = config.fromJSON(content.toStdString());

		if (r)
		{
			osgEarth::Config poisConfig = config.child("pois");
			if (poisConfig.children().size() == 0)
			{
				QMessageBox::information(this, "Info", "result is nothing!", QMessageBox::Ok);
				return;
			}
			osgEarth::ConfigSet::iterator poiItr = poisConfig.children().begin();
			osgEarth::ConfigSet::iterator poiEitr = poisConfig.children().end();
			int rowIndex = 0;
			while (poiItr != poiEitr)
			{
				std::string name = poiItr->child("name").value();//->getIfSet<std::string>(name,)
				std::string location = poiItr->child("location").value();
				std::string address = poiItr->child("address").value();
				DataModel->setItem(rowIndex,0,new QStandardItem(QString::number(rowIndex+1)));
				DataModel->setItem(rowIndex,1,new QStandardItem(QString::fromStdString(name))); 
				DataModel->setItem(rowIndex,2,new QStandardItem(QString::fromStdString(address))); 
				DataModel->setItem(rowIndex,3,new QStandardItem(QString::fromStdString(location))); 
				this->ui->placeSearchWidget->resizeColumnToContents(0);
				this->ui->placeSearchWidget->resizeColumnToContents(1);
				this->ui->placeSearchWidget->resizeColumnToContents(2);
				this->ui->placeSearchWidget->resizeColumnToContents(3);
				++rowIndex;
				++poiItr;
			}

		}
		else 
		{
			QMessageBox::information(this,"Info", "result is nothing!", QMessageBox::Ok);
			return;
		}
#else
		//{"location":{"lon":"116.31588","level":"生活服务","lat":"39.894728"},"status":"0","msg":"ok","searchVersion":"4.8.0"}
		content = content.split("\"location\":").at(1);
		if (content.size() < 2)
		{
			return ;
		}
		{
			QStringList datas = content.split("},");
			content = datas.at(0);//{"lon":116.401003,"level":"行政区划","lat":39.903117
			content.replace("{","");
			QStringList placeInfos = content.split(",");
			std::map<QString,QString> infos;
			if (!placeInfos.empty() && placeInfos.size() >= 3)
			{
				for (int n = 0; n < placeInfos.size(); ++n)
				{
					QString info = placeInfos.at(n);
					QStringList fields = info.split(":");
					QString key,value;
					if (fields.size() > 0)
					{
						key = fields.at(0);
					}
					if (fields.size() > 1)
					{
						value = fields.at(1);
					}
					value.replace("\"","");
					infos[key] = value;
				}
				QString lon = infos["\"lon\""];
				QString lat = infos["\"lat\""];
				QString level = infos["\"level\""];
				DataModel->setItem(0,0,new QStandardItem(lon)); 
				DataModel->setItem(0,1,new QStandardItem(lat)); 
				DataModel->setItem(0,2,new QStandardItem(level)); 
				this->ui->placeSearchWidget->resizeColumnToContents(0);
				this->ui->placeSearchWidget->resizeColumnToContents(1);
				this->ui->placeSearchWidget->resizeColumnToContents(2);
			}
		}
#endif
	}
	else 
	{
		QMessageBox::information(this, "提示信息", "没有查询到相关信息!", QMessageBox::Ok);
		return;
	}
}

void frmMain::slotSearchPlaceName()
{
#if 1
	std::string key = "\"" +this->ui->keyPlaceName->text().trimmed().toStdString() +  "\"";
	//QString url = QString::fromStdString("http://api.tianditu.gov.cn/search?postStr={\"keyWord\":"+ key + ",\"level\":\"15\",\"mapBound\":\"73,0,15,54\",\"queryType\":\"7\",\"count\":\"20\",\"start\":\"0\"}&type=query&tk=77101ef255a0a04a7c892b8c3f8822d0");
#if _use_amap_api_ == 1
	QString url = QString::fromStdString("https://restapi.amap.com/v3/place/text?keywords=" + key + "&output=json&offset=5&page=1&key=f3ed2064cff467cc1f114d7f0c4f6ec6&extensions=all");
#else
	QString url = QString::fromStdString("http://api.tianditu.gov.cn/geocoder?ds={\"keyWord\":"+ key + "}&tk=77101ef255a0a04a7c892b8c3f8822d0");
#endif
	//创建一个请求
	QNetworkRequest request;
	request.setUrl(QUrl(url));
	//创建一个管理器
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	//发送GET请求
	QNetworkReply *reply = manager->get(request);
	//连接请求结束信号
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	if (DataModel && DataModel->rowCount() > 0)
		DataModel->removeRows(0,DataModel->rowCount());
#endif
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
	if (NULL != this->mpGeometryEditor)
	{
		QMessageBox::information(this, "警告信息", "当前存在对象编辑!", QMessageBox::Ok);
		return;
	}

	SceneTool::CDistanceMeasureTool* pDistanceMeasure = new SceneTool::CDistanceMeasureTool(
		this->mpGlobeWidget->getMapNodeRef(),SceneTool::FLAT_MT);
	pDistanceMeasure->setShowAzimuth(true);
	pDistanceMeasure->beginEdit("",NULL);
	this->mpGlobeWidget->addEventHandler(pDistanceMeasure);
	mpGeometryEditor = pDistanceMeasure;
}

void frmMain::slotAreaMeature()
{
	if (NULL != this->mpGeometryEditor)
	{
		QMessageBox::information(this, "警告信息", "当前存在对象编辑!", QMessageBox::Ok);
		return;
	}

	SceneTool::CAreaMeasureTool* pAreaMeasure = new SceneTool::CAreaMeasureTool(
		this->mpGlobeWidget->getMapNodeRef(),SceneTool::FLAT_MT);
	pAreaMeasure->beginEdit("",NULL);
	this->mpGlobeWidget->addEventHandler(pAreaMeasure);
	mpGeometryEditor = pAreaMeasure;

}

void frmMain::slotAzimuthMeature()
{

}

void frmMain::slotClearMeatureResult()
{
	if (NULL != this->mpGeometryEditor)
	{
		this->mpGeometryEditor->endEdit(true);
		this->mpGlobeWidget->removeEventHandler(this->mpGeometryEditor);
		this->mpGeometryEditor = NULL;
	}
	
}
