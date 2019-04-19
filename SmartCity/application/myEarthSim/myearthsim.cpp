#include "myearthsim.h"
#include <osgDB/FileUtils>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>
#include <QMessageBox>
#include <QComboBox>
#include <QStackedLayout>
#include <QCursor>
#include <QDateTime>
#include "geometryeditor/pointeditor.h"
#include <osgEarthSymbology/AltitudeSymbol>
#include "globemap/globemap.h"
#include "scenetool/distancemeasure.h"
#include "scenetool/areameasure.h"
#include "scenetool/azimuthmeasure.h"
#include "scenetool/scenepicker.h"
#include <osgEarthUtil/RTTPicker>
#include <osgEarth/Registry>
#include <osgEarthUtil/MouseCoordsTool>
#include "dynamicobject/dynamicobjectlayer.h"
#include "QThread"
#include "datagenerater/vr_datagenerater.h"
#include "dynamicobject/objectmanager.h"
#include "dynamicobject/sceneobjectlayer.h"
#include "geometryeditor/geographiceditor.h"
#include "geographic/geographictype.h"
#include "geographic/geocirlce.h"
#include "geographic/geopolygon.h"
#include "geographic/geoline.h"
#include "dynamicobject/calculatorhelper.h"
#include "globenavigation.h"
#include <QFileDialog>

using namespace osgEarth::Drivers;
using namespace osgEarth;

//#pragma execution_character_set("utf-8")
Common::ISceneObject* g_pGraphicObject = NULL;
class CModelToolListener : public Tool::IToolListener
{
public:
	CModelToolListener()
	{
		Owner = NULL;
	}

	~CModelToolListener()
	{}

	 virtual void onEvent(Common::IObject* pResult)
	 {
		 if (NULL != this->Owner)
		 {
			 this->Owner->handlePickerEvent(pResult);
		 }
	 }
public:
	 myEarthSim* Owner;
};

class CMouseCoordsCallback 
	: public osgEarth::Util::MouseCoordsTool::Callback
{
public:
	CMouseCoordsCallback()
	{

	}

	virtual ~CMouseCoordsCallback() 
	{
	
	}

	virtual void set(const GeoPoint& coords, osg::View* view, MapNode* mapNode)
	{
		double lon = coords.x();
		double lat = coords.y();
	}
	virtual void reset(osg::View* view, MapNode* mapNode)
	{

	}
};

class CCustomDynamicObjectListener : public CDynamicObjectListener
{
public:
	CCustomDynamicObjectListener()
	{
	}

	~CCustomDynamicObjectListener()
	{}

	virtual void objectInRegin(const std::string& objectName,const std::string& objectID,
		const std::string& layerType,const std::string& layerName,const std::string& layerObjectName)
	{
		//�����������֮���޸ı�ͷ�Լ��켣�ߵ���ɫ
		Common::IObject* pObject = CObjectManager::instance()->getDetectionTarget(objectID);
		DynamicObject::CDynamicObject* pDynamicObject = dynamic_cast<DynamicObject::CDynamicObject*>(pObject);
		if(NULL == pDynamicObject)
			return;
		pDynamicObject->changeScutcheonHeaderColor(osgEarth::Symbology::Color::Blue);
		pDynamicObject->changeTrackLineColor(osg::Vec4(0,0,1,1));
		CObjectManager::instance()->changeIconImage(objectID,"uav_nofly.png");
		std::string imageName = CObjectManager::instance()->getIconImage(objectID);
	}

	virtual void objectOutRegin(const std::string& objectName,const std::string& objectID,
		const std::string& layerType,const std::string& layerName,const std::string& layerObjectName)
	{
		//����ɳ������֮�󣬻ָ���ͷ�Լ��켣�ߵ���ɫ
		Common::IObject* pObject = CObjectManager::instance()->getDetectionTarget(objectID);
		DynamicObject::CDynamicObject* pDynamicObject = dynamic_cast<DynamicObject::CDynamicObject*>(pObject);
		if(NULL == pDynamicObject)
			return;
		pDynamicObject->recoverScutcheonHeaderColor();
		pDynamicObject->recoverTrackLineColor();
		CObjectManager::instance()->changeIconImage(objectID,"uav_normal.png");
		return;
	}

	virtual void objectDestroy(const std::string& objectName,const std::string& objectID,const bool& isInRegin,
		const std::string& layerType,const std::string& layerName,const std::string& layerObjectName)
	{
		int a = 1;
		a++;
	}

	/**���󴴽�ʱ�ص�(�˻ص��ӿ��������ݽ����̵߳���)
	*pNewDynamicObject���¶���ָ��
	**/
	virtual void onCreateDynamicObject(Common::IObject* pNewDynamicObject)
	{
	
	}

	/**������źŵĻص�(�˻ص��ӿ��������ݽ����̵߳���)
	*pCurDynamicObject����ǰ����ָ��
	**/
	virtual void onDynamicObjectReceiveSignal(Common::IObject* pCurDynamicObject)
	{
	
	}
};

class CCustomGUIEventHandler : public osgGA::GUIEventHandler
{
public:
	CCustomGUIEventHandler()
	{
		GlobeWidget = NULL;
	}

	virtual ~CCustomGUIEventHandler()
	{
	
	}

	bool handle(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& aa)
	{
		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::PUSH://��갴��
		{
			
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				//������
				if (GlobeWidget != NULL)
				{
					osg::Vec3d geoPosition = GlobeWidget->getGeoPosition(
						aa.asView(),ea.getX(),ea.getY());
					if (geoPosition != osg::Vec3d(-1,-1,0))
					{
						
					}
				}
			}
			else if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{
				//����Ҽ�
			}
			else if (ea.getButton() == osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON)
			{
				//����м�
			}
			break;
		}
		case osgGA::GUIEventAdapter::KEYDOWN://���̰���
		{
			break;
		}
		case osgGA::GUIEventAdapter::MOVE://����ƶ�
		{

			break;
		}
		case osgGA::GUIEventAdapter::DRAG://����϶�
		{

			break;
		}
		case osgGA::GUIEventAdapter::RELEASE://���̧��
		{

			break;
		}
		case osgGA::GUIEventAdapter::DOUBLECLICK://���˫��
		{
			
			break;
		}
		}
		return false;
	}

public:
	VRGlobeWidget* GlobeWidget;
};

myEarthSim::myEarthSim(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	setlocale(LC_ALL, "chs");
	this->ui.gridLayout->setMargin(0);
	this->ui.gridLayout->setSpacing(0);
	this->ui.gridLayout->setContentsMargins(0, 0, 0, 0);

	this->mpModelPicker = NULL;
	this->mpGeometryEditor = NULL;
	this->mpEditorObject = NULL;
	this->mpGeoCircle = NULL;
	this->mpGeoRectGrid = NULL;
	this->mpGeoRingGrid = NULL;
	this->mpGeoPoint = NULL;
	this->mpGeoLine = NULL;
	this->mpGeoPolygon = NULL;
	this->mpGeoScutcheon = NULL;
	this->mpGeoSegment = NULL;
	this->mpGeoRadarLine = NULL;
	this->mpMouseCoordsCallback = NULL;
	this->mpCustomGUIEventHandler = NULL;
	this->mpGeoRingGridMeature = NULL;
	this->mpGeoRectGridMeature = NULL;
#if 1
	this->mpMenuMap = new QMenu(this);
	this->mpMenuTarget = new QMenu(this);
	this->mpMenuDevice = new QMenu(this);

	QAction *distanceMeasureAction = new QAction(tr("&distance"), this);
    QAction *areaMeasureAction = new QAction(tr("&area"), this);
    QAction *returnAction = new QAction(tr("&reset"), this);
    QAction *plotAction = new QAction(tr("&draw"), this);

	this->mpMenuMap->addAction(distanceMeasureAction);
	this->mpMenuMap->addAction(areaMeasureAction);
	this->mpMenuMap->addAction(returnAction);
	this->mpMenuMap->addAction(plotAction);

	QAction *powerZoneAction = new QAction(tr("&circel show/hide"), this);
	this->mpMenuDevice->addAction(powerZoneAction);

	QAction *handleAction = new QAction(tr("&handle"), this);
	QAction *cancelWarnAction = new QAction(tr("&cancelWarning"), this);
	QAction *signTargetAction = new QAction(tr("&markObject"), this);
	this->mpMenuTarget->addAction(handleAction);
	this->mpMenuTarget->addAction(cancelWarnAction);
	this->mpMenuTarget->addAction(signTargetAction);

#endif

	initialFireEngineScene();
}

myEarthSim::~myEarthSim()
{
	//�ر����ݽ����߳�
	CDataGenerater::instance()->stopSendData();

	//�رպ�̨���ݼ����߳�
	CObjectManager::instance()->closeDataProcessThread();
}

void myEarthSim::initialFireEngineScene()
{
	this->slotShowEarthControl();
	this->slotActionAddTDTImage();
	this->slotFlyToTarget();

	this->mpTitleDlg = new TitleDlg(this);
	mpTitleDlg->setGeometry(0,0,1500,mpTitleDlg->height());
	mpTitleDlg->Owner = this;
	mpTitleDlg->show();

	this->ui.menuBar->setVisible(false);
	this->showFullScreen();
	this->slotSetNavigationPosition();

	//Globe::CGlobeMap::getInstance()->addImageLayer("","gaodeImage",Globe::MapType::WEB_TDT_MAP_MT);
}

void myEarthSim::initialMeatureStyle()
{
	//��λ�����ַ��
	osgEarth::Symbology::Style& azimuthStyle = SceneTool::CAzimuthMeasure::getTextStyle();
	azimuthStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()= "fonts/SIMLI.TTF";
	azimuthStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = osgEarth::Symbology::Color::Yellow;
	azimuthStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 30;
	azimuthStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloBackdropType() = osgText::Text::BackdropType::OUTLINE;
	azimuthStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloImplementation() =  osgText::Text::BackdropImplementation::STENCIL_BUFFER;
	azimuthStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->halo()->color() =osgEarth::Symbology::Color(0,0,0,1);
	azimuthStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->pixelOffset() = osg::Vec2s(10,10);


	//������ַ��
	osgEarth::Symbology::Style& areaStyle = SceneTool::CAreaMeasure::getTextStyle();
	areaStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()= "fonts/SIMLI.TTF";
	areaStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = osgEarth::Symbology::Color::Yellow;
	areaStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 30;
	areaStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloBackdropType() = osgText::Text::BackdropType::OUTLINE;
	areaStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloImplementation() =  osgText::Text::BackdropImplementation::STENCIL_BUFFER;
	areaStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->halo()->color() =osgEarth::Symbology::Color(0,0,0,1);
	areaStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->pixelOffset() = osg::Vec2s(10,10);

	//�������ַ��
	osgEarth::Symbology::Style& distanceStyle = SceneTool::CDistanceMeasure::getTextStyle();
	distanceStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = osgEarth::Symbology::Color::Yellow;
	distanceStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 20;
	distanceStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloBackdropType() = osgText::Text::BackdropType::OUTLINE;
	distanceStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloImplementation() =  osgText::Text::BackdropImplementation::STENCIL_BUFFER;
	distanceStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->halo()->color() =osgEarth::Symbology::Color(0,0,0,1);
	distanceStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->pixelOffset() = osg::Vec2s(10,10);
}

void myEarthSim::handlePickerEvent(Common::IObject* pResult)
{
	g_pGraphicObject = NULL;
	SceneTool::CPickerResult* pPickerResult = dynamic_cast<SceneTool::CPickerResult*>(pResult);
	if (pPickerResult)
	{
		//����ʵ��λ��
		QPoint pos(pPickerResult->getScreenPosition().x() + this->geometry().x(),
				this->height() - pPickerResult->getScreenPosition().y() + this->geometry().y());
		//��������(0����1���У�2����)
		int mouseButton = pPickerResult->getMouseButton();
		Common::IObject* pCurObject = pPickerResult->getPickerObject();
		if (pCurObject)
		{
			if (Graphic::CGeoPoint* pGeoPoint = dynamic_cast<Graphic::CGeoPoint*>(pCurObject))
			{
				//ѡ�е��Ƕ���Ϊ��
				int x = 0;
			}
			else if (Graphic::CGeoCircle* pGeoCircle = dynamic_cast<Graphic::CGeoCircle*>(pCurObject))
			{
				//ѡ�е��Ƕ���ΪԲ
				//��ǵ�ǰѡ�еĶ���
				g_pGraphicObject = pGeoCircle;
			}
			else if (Graphic::CGeoPolygon* pGeoPolygon = dynamic_cast<Graphic::CGeoPolygon*>(pCurObject))
			{
				//ѡ�е��Ƕ���Ϊ�����
				//��ǵ�ǰѡ�еĶ���
				g_pGraphicObject = pGeoPolygon;
			}
			std::string content = "";
			content += "name:" + pCurObject->getName() + "\n";
			content += "id:" + pCurObject->getID() + "\n";
			content += "type:" + pCurObject->getType() + "\n";
			QString qContent = QString::fromLocal8Bit(content.c_str());
			
			if (pCurObject->getType() == "Target")
			{
				//����Ŀ��˵�
				//this->mpMenuTarget->exec(pos);
				DynamicObject::CDynamicObject* pTargetObject = 
					dynamic_cast<DynamicObject::CDynamicObject*>(CObjectManager::instance()->getObject(pCurObject->getID()));
				if (pTargetObject)
				{
					pTargetObject->setScutcheonVisible(!pTargetObject->getScutcheonVisible());
				}
			}
			else if (pCurObject->getType() == "Device")
			{
				//�����豸�˵�
				//this->mpMenuDevice->exec(pos);
			}
		}
		else
		{
			//this->mpMenuMap->exec(pos);
		}

	}
}

void myEarthSim::getObjects()
{
	Core::IObjectContainer container;
	std::vector<std::string> keys;
	container.getKeys(keys);
	if (!keys.empty())
	{
		for (int i = 0; i < keys.size(); ++i)
		{
			std::string id = keys.at(i);
			Common::IObject* pObject = container.getObject(id);
			if (pObject)
			{
				//pObject���û�Ҫ�õ��Ķ���
			}
		}
	}
}

void myEarthSim::slotShowEarthControl()
{
#if 1 //��ʾ��ά�����ؼ� 
	//���������ؼ����󣨲�������ΪNULL��
	this->mpGlobeWidget = new VRGlobeWidget();
	//��ȡ���̵Ĺ���Ŀ¼(VR-GlobeConfig3D.earth�ļ�һ����exe��ͬһĿ¼)
	std::string runPath = osgDB::getCurrentWorkingDirectory();
	std::string runPath_3d = runPath + "/VR-GlobeConfig3D.earth";
	//�����ؼ���ʼ����������ά��ͼ
	this->mpGlobeWidget->initial(true, runPath_3d.c_str());
	//����ά�����ؼ���ӵ�qt����
	this->ui.gridLayout->addWidget(this->mpGlobeWidget);

#endif

	//QCursor *myCursor=new QCursor(QPixmap("G:/workfiles/myEarth/MyEarth2010/trunk/application/myEarthSim/Resources/pan.png"),-1,-1);    //-1,-1��ʾ�ȵ�λ��ͼƬ����
	//this->mpGlobeWidget->setCursor(*myCursor); 
	Globe::CGlobeMap::getInstance()->initialise(this->mpGlobeWidget->getMapRef());
	//��ʼ���������ַ��
	initialMeatureStyle();
	slotStartPointQuery();
	slotActionChangeTrackLineType();
	//slotActionControlScutVisible();
	slotActionSetScutcheonFields();
	/*this->showFullScreen();
	this->ui.menuBar->setVisible(false);
	this->slotBeginSphereDistance();*/
}

void myEarthSim::slotActionSetGlobeScalePosition()
{
	int x, y;
	if (this->mpGlobeWidget)
	{
		//���ñ����ߵ�λ������
		this->mpGlobeWidget->getGlobeNavigation()->getGlobeScalePosition(x, y);
		x += 50;
		if (x > 1200)
			x = 50;
		this->mpGlobeWidget->getGlobeNavigation()->setGlobeScalePosition(x, y);
		int size = 20;

		//���ñ��������ַ��
		osgText::Text* pGlobeScaleText = this->mpGlobeWidget->getGlobeNavigation()->getGlobeScaleText();
		osgText::Font* font = osgText::readFontFile("fonts/SIMLI.TTF");
		pGlobeScaleText->setFont(font);
		pGlobeScaleText->setCharacterSize(size);
		pGlobeScaleText->setFontResolution(size, size);
		pGlobeScaleText->setColor(osg::Vec4(1.0f, 0.0f, 1.0f, 1.0f));
		pGlobeScaleText->setBackdropImplementation(osgText::Text::BackdropImplementation::STENCIL_BUFFER);
		pGlobeScaleText->setBackdropColor(osg::Vec4(0,1,0,1));
		pGlobeScaleText->setBackdropType(osgText::Text::BackdropType::OUTLINE);
		//���ñ����߾������ַ��
		osgText::Text* pScaleDisText = this->mpGlobeWidget->getGlobeNavigation()->getScaleDisText();
		pScaleDisText->setFont(font);
		pScaleDisText->setCharacterSize(size);
		pScaleDisText->setFontResolution(size, size);
		pScaleDisText->setColor(osg::Vec4(1.0f, 0.0f, 1.0f, 1.0f));
		pScaleDisText->setBackdropImplementation(osgText::Text::BackdropImplementation::STENCIL_BUFFER);
		pScaleDisText->setBackdropColor(osg::Vec4(0,1,0,1));
		pScaleDisText->setBackdropType(osgText::Text::BackdropType::OUTLINE);
	}
}

void myEarthSim::slotSetNavigationPosition()
{
	int x, y;
	if (this->mpGlobeWidget)
	{
		//����ָ����λ��
		this->mpGlobeWidget->getGlobeNavigation()->getNavigationPosition(x, y);
		y += 80;
		if (y > 1200)
			y = 50;
		this->mpGlobeWidget->getGlobeNavigation()->setNavigationPosition(x, y);
	}
}

void myEarthSim::slotFlyToTarget()
{
#if 0
	//������λ�ã��߶ȵ���Ϣ���з���
	this->mpGlobeWidget->flyTo(osgEarth::Viewpoint("", 118.824292, 32.048700, 3000, 0, -90, 0.0), 2);
#else
	//������λ�ã�Ŀ������뾶����Ϣ���з���
	//this->mpGlobeWidget->flyTo(116.325612, 39.906085,3000,0);//
	//osgEarth::Viewpoint vp("", 116.325612, 39.906085, 3000, 0, -90, 0.0);
	this->mpGlobeWidget->flyTo(116.329612, 39.906085,3000,0);//
	osgEarth::Viewpoint vp("", 116.329612, 39.906085, 3000, 0, -90, 0.0);
	this->mpGlobeWidget->getEarthManipulator()->setHomeViewpoint(vp);
#endif
}

void myEarthSim::slotActionStartGetMourseGeoPosition()
{
	if (NULL == this->mpMouseCoordsCallback)
	{
		this->mpMouseCoordsCallback = new CMouseCoordsCallback();
		this->mpGlobeWidget->getMouseCoordsTool()->addCallback(this->mpMouseCoordsCallback);
	}
}

void myEarthSim::slotActionEndGetMourseGeoPosition()
{
	if (NULL != this->mpMouseCoordsCallback)
	{
		this->mpGlobeWidget->getMouseCoordsTool()->removedCallback(this->mpMouseCoordsCallback);
		this->mpMouseCoordsCallback = NULL;
	}
}

void myEarthSim::slotActionGUIEventHandler()
{
	if (NULL == this->mpCustomGUIEventHandler)
	{
		//�û���CCustomGUIEventHandler���У����ո���GUI�¼�
		this->mpCustomGUIEventHandler = new CCustomGUIEventHandler();
		this->mpCustomGUIEventHandler->GlobeWidget = this->mpGlobeWidget;
		this->mpGlobeWidget->addEventHandler(this->mpCustomGUIEventHandler);
	}
}

void myEarthSim::slotActionStateTextSize()
{
	if (this->mpGlobeWidget)
	{
		int size = this->mpGlobeWidget->getStateTextSize();
		this->mpGlobeWidget->setStateTextSize(size + 5);

		osgText::Text* pStateText = this->mpGlobeWidget->getGlobeNavigation()->getStateLabelText();
		osgText::Font* font = osgText::readFontFile("fonts/SIMLI.TTF");
		pStateText->setFont(font);
		pStateText->setColor(osg::Vec4(1.0f, 0.0f, 1.0f, 1.0f));
		pStateText->setBackdropImplementation(osgText::Text::BackdropImplementation::STENCIL_BUFFER);
		pStateText->setBackdropColor(osg::Vec4(0,1,0,1));
		pStateText->setBackdropType(osgText::Text::BackdropType::OUTLINE);
	}
}

void myEarthSim::slotActionGetCameraPosition()
{
	osg::Vec3d cameraPosition = this->mpGlobeWidget->getCameraPosition();
	double lon = cameraPosition.x();
	double lat = cameraPosition.y();
	double alt = cameraPosition.z();//����ĸ߶�
}

void myEarthSim::slotActionComputeAzimuth()
{
	osg::Vec3d beginPoint(116,39,0);
	osg::Vec3d endPoint(116.1,39.1,0);
	double azimuth = SceneTool::CAzimuthMeasure::computeAzimuth(beginPoint,endPoint);
}

void myEarthSim::slotActionSetCameraMode()
{
	if (this->mpGlobeWidget)
	{
		//�����Ƿ�Ϊ3D����ģʽ
		bool use3DCameraMode = this->mpGlobeWidget->getUse3DCameraMode();
		this->mpGlobeWidget->setUse3DCameraMode(!use3DCameraMode);

		//�����Ƿ������
		bool canPanMap = this->mpGlobeWidget->getCanPanMap();
		this->mpGlobeWidget->setCanPanMap(!canPanMap);
	}
}

void myEarthSim::slotActionSetCameraHeight()
{
#if 0
	double minCameraHeight = 10000;
	double maxCameraHeight = 1000000;
	if (this->mpGlobeWidget)
	{
		this->mpGlobeWidget->setCameraHeight(minCameraHeight,maxCameraHeight);
	}
#else
	double minCameraLevel = 2;
	double maxCameraLevel = 12;
	if (this->mpGlobeWidget)
	{
		this->mpGlobeWidget->setCameraLevel(minCameraLevel,maxCameraLevel);
	}
#endif
}

void myEarthSim::slotStartPointQuery()
{
	if (NULL == this->mpModelPicker)
	{
		//����ģ�Ͳ�ѯ����
		SceneTool::CModelPickerTool* pPicker = new SceneTool::CModelPickerTool(this->mpGlobeWidget->getMapNodeRef());
		//������ѯ��������������ȡ����ѡ��ѯ��������Ϣ����Ҫ����demo�Լ����壩
		CModelToolListener* pModelToolListener = new CModelToolListener();
		pModelToolListener->Owner = this;
		pPicker->addToolListener(pModelToolListener);
		this->mpGlobeWidget->addEventHandler(pPicker);
	}
}

void myEarthSim::slotEndPointQuery()
{
	if (NULL != this->mpModelPicker)
	{
		this->mpGlobeWidget->removeEventHandler(this->mpModelPicker);
		this->mpModelPicker = NULL;
	}
}

void myEarthSim::slotDrawCircle()
{
	
}

void myEarthSim::slotActionCreateCircle()
{
	if (NULL == this->mpGeoCircle)
	{

		Graphic::CGeoCircle* pCircle = new Graphic::CGeoCircle(this->mpGlobeWidget->getMapNodeRef());
		//���ö���ĵ�������
		pCircle->setGeoPosition(osg::Vec3d(116, 39, 10));
		//���������ɫ
		pCircle->getStyle().getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Symbology::Color(1,0,0,0.5);
		pCircle->getStyle().getOrCreate<LineSymbol>()->stroke()->color() = Color::Red;
		pCircle->getStyle().getOrCreate<LineSymbol>()->stroke()->color().a() = 0.5;
		pCircle->getStyle().getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		pCircle->getStyle().getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_GPU;			
		//���ö���İ뾶
		pCircle->setRadius(100000);
		pCircle->loadToScene();
		pCircle->renderToScene();
		this->mpGeoCircle = pCircle;
		
	}
	
}

void myEarthSim::slotActionDeleteCircle()
{
	if (this->mpGeoCircle)
	{
		this->mpGeoCircle->deRenderFromScene();
		this->mpGeoCircle->unLoadFromScene();
		delete this->mpGeoCircle;
		this->mpGeoCircle = NULL;
		
	}
}

void myEarthSim::slotActionCreateRectGrid()
{
	if (NULL == this->mpGeoRectGrid)
	{
		//���������������
		Geo::CGeoRectGrid* pGeoRectGrid = new Geo::CGeoRectGrid(this->mpGlobeWidget->getMapNodeRef());
		//���ö���ĵ�������
		pGeoRectGrid->setGeoPosition(osg::Vec3d(116, 39, 10));
		//���ö���ĸ߶�
		pGeoRectGrid->setHeight(100000);
		//���ö���Ŀ��
		pGeoRectGrid->setWidth(100000);
		//���ö��������
		pGeoRectGrid->setRowCount(6);
		//���ö��������
		pGeoRectGrid->setColCount(6);
		//����
		pGeoRectGrid->loadToScene();
		//��Ⱦ
		pGeoRectGrid->renderToScene();
		this->mpGeoRectGrid = pGeoRectGrid;
		
		this->mpGeoRectGridMeature = new SceneTool::CGridMeasure(this->mpGlobeWidget->getMapNodeRef(),this->mpGeoRectGrid);
		this->mpGeoRectGridMeature->getStyle().getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 15.0;//���������С
		this->mpGeoRectGridMeature->getStyle().getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = osgEarth::Symbology::Color::Yellow;//����������ɫ
		this->mpGlobeWidget->addEventHandler(this->mpGeoRectGridMeature);
	}
}

void myEarthSim::slotActionDeleteRectGrid()
{
	if (this->mpGeoRectGrid)
	{
		this->mpGeoRectGrid->deRenderFromScene();
		this->mpGeoRectGrid->unLoadFromScene();
		delete this->mpGeoRectGrid;
		this->mpGeoRectGrid = NULL;
		
		if (this->mpGeoRectGridMeature)
		{
			this->mpGlobeWidget->removeEventHandler(this->mpGeoRectGridMeature);
			this->mpGeoRectGridMeature = NULL;
		}
	}
}

void myEarthSim::slotActionDeleteRingGrid()
{
	if (this->mpGeoRingGrid)
	{
		this->mpGeoRingGrid->deRenderFromScene();
		this->mpGeoRingGrid->unLoadFromScene();
		delete this->mpGeoRingGrid;
		this->mpGeoRingGrid = NULL;
		
		if (this->mpGeoRingGridMeature)
		{
			this->mpGlobeWidget->removeEventHandler(this->mpGeoRingGridMeature);
			this->mpGeoRingGridMeature = NULL;
		}
	}
}

void myEarthSim::slotActionCreateRingGrid()
{
	if (NULL == this->mpGeoRingGrid)
	{
		//����Բ������
		Geo::CGeoRingGrid* pGeoRingGrid = new Geo::CGeoRingGrid(this->mpGlobeWidget->getMapNodeRef());
		//���ö���ĵ�������
		pGeoRingGrid->setGeoPosition(osg::Vec3d(116, 39, 10));
		//���ö���İ뾶
		pGeoRingGrid->setRadius(100000);
		pGeoRingGrid->setSubRingCount(3);
		pGeoRingGrid->setSectorCount(8);
		pGeoRingGrid->loadToScene();
		pGeoRingGrid->renderToScene();
		this->mpGeoRingGrid = pGeoRingGrid;

		this->mpGeoRingGridMeature = new SceneTool::CGridMeasure(this->mpGlobeWidget->getMapNodeRef(),this->mpGeoRingGrid);
		this->mpGeoRingGridMeature->getDirectionType() = 1;////0����λ�ǣ�1����ʱ�ӷ���
		this->mpGeoRingGridMeature->getStyle().getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 15.0;//���������С
		this->mpGeoRingGridMeature->getStyle().getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = osgEarth::Symbology::Color::Yellow;//����������ɫ
		this->mpGlobeWidget->addEventHandler(this->mpGeoRingGridMeature);
	}
}

void myEarthSim::slotAddTMSImage()
{
	//���ر�׼TMS��ʽӰ��
	Globe::CGlobeMap::getInstance()->addImageLayer(
		"http://readymap.org/readymap/tiles/1.0.0/7/","TMSImage",Globe::TMS_MT);
}

void myEarthSim::slotAddTMSElevation()
{
	//���ر�׼TMS��ʽ�߳�
	Globe::CGlobeMap::getInstance()->addElevationLayer(
		"http://readymap.org/readymap/tiles/1.0.0/9/","TMSElevation");
}

void myEarthSim::slotAddGoogleImage()
{
	//���ر�׼Google��ʽӰ��
	Globe::CGlobeMap::getInstance()->addImageLayer(
		"","GoogleImage",Globe::WEB_GOOGLE_MT);
}

void myEarthSim::slotActionAddTDTImage()
{
	//���ر�׼tdt��ʽӰ��
	Globe::CGlobeMap::getInstance()->addImageLayer(
		"","gaoImage",Globe::WEB_GAODE_MAP_MT);
}

void myEarthSim::slotActionAddTifDem()
{

}

void myEarthSim::slotUnVisibleTMSImage()
{
	osgEarth::ImageLayer* pImagetlayer  = Globe::CGlobeMap::getInstance()->getImageLayerByName("TMSImage");
	if(pImagetlayer)
		pImagetlayer->setVisible(false);
}

void myEarthSim::slotUnVisibleGoogleImage()
{
	osgEarth::ImageLayer* pImagetlayer  = Globe::CGlobeMap::getInstance()->getImageLayerByName("GoogleImage");
	if(pImagetlayer)
		pImagetlayer->setVisible(false);
}

void myEarthSim::slotUnVisibleTMSElevation()
{
	osgEarth::ElevationLayer* pElevationLayer  = Globe::CGlobeMap::getInstance()->getElevationLayerByName("TMSElevation");
	if(pElevationLayer)
		pElevationLayer->setVisible(false);
}

void myEarthSim::slotVisibleTMSImage()
{
	osgEarth::ImageLayer* pImagetlayer  = Globe::CGlobeMap::getInstance()->getImageLayerByName("TMSImage");
	if(pImagetlayer)
		pImagetlayer->setVisible(true);
}

void myEarthSim::slotVisibleGoogleImage()
{
	osgEarth::ImageLayer* pImagetlayer  = Globe::CGlobeMap::getInstance()->getImageLayerByName("GoogleImage");
	if(pImagetlayer)
		pImagetlayer->setVisible(true);
}

void myEarthSim::slotVisibleTMSElevation()
{
	osgEarth::ElevationLayer* pElevationLayer  = Globe::CGlobeMap::getInstance()->getElevationLayerByName("TMSElevation");
	if(pElevationLayer)
		pElevationLayer->setVisible(true);
}

void myEarthSim::slotAddCountryLineShp()
{
	Globe::CGlobeMap::getInstance()->addLineShpModelLayer(
		"earthData/vector/world.shp","CountryLine",Color::Red,20.0);
}

void myEarthSim::slotUnVisibleLineShp()
{
	osgEarth::ModelLayer* pModelLayer  = Globe::CGlobeMap::getInstance()->getModelLayerByName("CountryLine");
	if(pModelLayer)
		pModelLayer->setVisible(false);
}

void myEarthSim::slotVisibleLineShp()
{
	osgEarth::ModelLayer* pModelLayer  = Globe::CGlobeMap::getInstance()->getModelLayerByName("CountryLine");
	if(pModelLayer)
		pModelLayer->setVisible(true);
}

void myEarthSim::slotAddPolygonShp()
{
	Globe::CGlobeMap::getInstance()->addPolygonShpModelLayer(
		"earthData/vector/world.shp","CountryPolygon",Color::Blue,Color::Yellow,5.0);
}

void myEarthSim::slotUnVisiblePolygonShp()
{
	osgEarth::ModelLayer* pModelLayer  = Globe::CGlobeMap::getInstance()->getModelLayerByName("CountryPolygon");
	if(pModelLayer)
		pModelLayer->setVisible(false);
}

void myEarthSim::slotVisiblePolygonShp()
{
	osgEarth::ModelLayer* pModelLayer  = Globe::CGlobeMap::getInstance()->getModelLayerByName("CountryPolygon");
	if(pModelLayer)
		pModelLayer->setVisible(true);
}

void myEarthSim::slotAddPointShp()
{
	Globe::CGlobeMap::getInstance()->addPointShpModelLayer(
		"earthData/vector/province.shp","ProvincePoint",Color::White,30.0,"","pinyin");
}

void myEarthSim::slotUnVisiblePointShp()
{
	osgEarth::ModelLayer* pModelLayer  = Globe::CGlobeMap::getInstance()->getModelLayerByName("ProvincePoint");
	if(pModelLayer)
		pModelLayer->setVisible(false);
}

void myEarthSim::slotVisiblePointShp()
{
	osgEarth::ModelLayer* pModelLayer  = Globe::CGlobeMap::getInstance()->getModelLayerByName("ProvincePoint");
	if(pModelLayer)
		pModelLayer->setVisible(true);
}

void myEarthSim::slotLoadCustomTile()
{
	//����̫�ָ�ʽӰ��

#if 1 //���عȸ�ī������Ƭ����
	Globe::CGlobeMap::getInstance()->addImageLayer(
		"F:/dataService/image_tile/nj_tiles/tiles/","CustomImage",Globe::GOOGLE_LOCALMECATOR,0,16);
#endif

#if 0 //���عȸ���ƫ84��Ƭ����
	Globe::CGlobeMap::getInstance()->addImageLayer(
		"F:/dataService/image_tile/nj_tiles/tiles/","CustomImage",Globe::GOOGLE_LOCAL84,0,16);
#endif

#if 0 //�������ͼ��Ƭ����
	Globe::CGlobeMap::getInstance()->addImageLayer(
		"F:/dataService/image_tile/nj_tiles/tiles/","CustomImage",Globe::CUSTOM_MT,0,16);
#endif

}

void myEarthSim::slotShowCustomTile()
{
	osgEarth::ImageLayer* pImagetlayer  = Globe::CGlobeMap::getInstance()->getImageLayerByName("CustomImage");
	if(pImagetlayer)
		pImagetlayer->setVisible(true);
}

void myEarthSim::slotHideCustomTile()
{
	osgEarth::ImageLayer* pImagetlayer  = Globe::CGlobeMap::getInstance()->getImageLayerByName("CustomImage");
	if(pImagetlayer)
		pImagetlayer->setVisible(false);
}

void myEarthSim::slotActionCreateJFQLayer()
{
	std::string layerID = "JFQLayerID";//���ο����û��Լ�����ͼ���id�������guid����Ψһ
	std::string layerName = "JFQLayerName";//���ο����û��Լ�����ͼ������ƣ�����Ψһ
	std::string layerType = "JFQLayerType";//���ο����û��Լ�����ͼ���Type������������������Type��ͬ
	DynamicObject::CSceneObjectLayer* pWarnningObjectLayer  =
		Globe::CGlobeMap::getInstance()->getSceneObjectLayerByName(layerName);
	if (pWarnningObjectLayer != NULL)
		return ;

	pWarnningObjectLayer =	Globe::CGlobeMap::getInstance()->addSceneObjectLayer(
		layerID,layerName,layerType);
	for (int index = 0; index < 10; ++index)
	{
		std::string id = QString::number(index).toStdString();
		std::string name = "obejct" + id;
		Graphic::CGeoCircle* pWarnningObject = new Graphic::CGeoCircle(this->mpGlobeWidget->getMapNodeRef());
		pWarnningObject->setID(id);
		pWarnningObject->setName(name);
		//���ö���ĵ�������
		pWarnningObject->setGeoPosition(osg::Vec3d(117, 39 - index * 2, 10));
		//���������ɫ
		pWarnningObject->getStyle().getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Symbology::Color(1,0,0,0.8);
		//���ö���İ뾶
		pWarnningObject->setRadius(100000);
		//����Բ������ʼ��λ�Ƕ�
		pWarnningObject->setBeginAngle(0);
		//����Բ������ֹ��λ�Ƕ�
		pWarnningObject->setEndAngle(360);
		pWarnningObjectLayer->addObject(pWarnningObject);
	}
	pWarnningObjectLayer->renderToScene();
}

void myEarthSim::slotActionVisibleJFQLayer()
{
	std::string layerID = "JFQLayerID";
	std::string layerName = "JFQLayerName";
	std::string layerType = "JFQLayerType";
	DynamicObject::CSceneObjectLayer* pWarnningObjectLayer  =
		Globe::CGlobeMap::getInstance()->getSceneObjectLayerByName(layerName);
	if (pWarnningObjectLayer != NULL)
	{
		bool visible = pWarnningObjectLayer->getVisible();
		pWarnningObjectLayer->setVisible(!visible);
	}
}

void myEarthSim::slotActionDeleteJFQLayer()
{
	std::string layerID = "JFQLayerID";
	std::string layerName = "JFQLayerName";
	std::string layerType = "JFQLayerType";
	DynamicObject::CSceneObjectLayer* pWarnningObjectLayer  =
		Globe::CGlobeMap::getInstance()->getSceneObjectLayerByName(layerName);
	if (pWarnningObjectLayer != NULL)
	{
		pWarnningObjectLayer->deRenderFromScene();
		Globe::CGlobeMap::getInstance()->removeSceneObjectLayerByName(layerName);
	}
}

void myEarthSim::slotBeginFlatDistance()
{
	if (NULL != this->mpGeometryEditor)
	{
		QMessageBox::information(this, "������Ϣ", "��ǰ���ڶ���༭��", QMessageBox::Ok);
		return;
	}
#if 0
	//�����������⹤��,����MeasureType measureType��ʾ���ⷽʽ�������������ƽ������
	SceneTool::CDistanceMeasure* pDistanceMeasure = new SceneTool::CDistanceMeasure(
		this->mpGlobeWidget->getMapNodeRef(),SceneTool::FLAT_MT);
	
	//��ʼ��������
	pDistanceMeasure->beginEdit();
	//����Լ���
	pDistanceMeasure->addToolListener(pDistanceMeasure);
	//�������ؼ������¼�����
	this->mpGlobeWidget->addEventHandler(pDistanceMeasure);
	mpGeometryEditor = pDistanceMeasure;
#else
	SceneTool::CDistanceMeasureTool* pDistanceMeasure = new SceneTool::CDistanceMeasureTool(
		this->mpGlobeWidget->getMapNodeRef(),SceneTool::FLAT_MT);
	pDistanceMeasure->beginEdit("",NULL);
	this->mpGlobeWidget->addEventHandler(pDistanceMeasure);
	mpGeometryEditor = pDistanceMeasure;
#endif
}

void myEarthSim::slotEndFlatDistance()
{
	//������������
	this->mpGeometryEditor->endEdit(true);
	//�ӳ����ؼ�ɾ����Ӧ���¼�����
	this->mpGlobeWidget->removeEventHandler(this->mpGeometryEditor);
	this->mpGeometryEditor = NULL;
}

void myEarthSim::slotBeginSphereDistance()
{
	if (NULL != this->mpGeometryEditor)
	{
		QMessageBox::information(this, "������Ϣ", "��ǰ���ڶ���༭��", QMessageBox::Ok);
		return;
	}
#if 0
	SceneTool::CDistanceMeasure* pDistanceMeasure = new SceneTool::CDistanceMeasure(
		this->mpGlobeWidget->getMapNodeRef(),SceneTool::SPHERE_MT);
	pDistanceMeasure->beginEdit();
	pDistanceMeasure->addToolListener(pDistanceMeasure);
	this->mpGlobeWidget->addEventHandler(pDistanceMeasure);
	mpGeometryEditor = pDistanceMeasure;
#else
	SceneTool::CDistanceMeasureTool* pDistanceMeasure = new SceneTool::CDistanceMeasureTool(
		this->mpGlobeWidget->getMapNodeRef(),SceneTool::SPHERE_MT);
	pDistanceMeasure->beginEdit("",NULL);
	this->mpGlobeWidget->addEventHandler(pDistanceMeasure);
	mpGeometryEditor = pDistanceMeasure;
#endif
}

void myEarthSim::slotEndSphereDistance()
{
	this->mpGeometryEditor->endEdit(true);
	this->mpGlobeWidget->removeEventHandler(this->mpGeometryEditor);
	this->mpGeometryEditor = NULL;
}

void myEarthSim::slotBeginFlatArea()
{
	if (NULL != this->mpGeometryEditor)
	{
		QMessageBox::information(this, "������Ϣ", "��ǰ���ڶ���༭��", QMessageBox::Ok);
		return;
	}
#if 0
	SceneTool::CAreaMeasure* pAreaMeasure = new SceneTool::CAreaMeasure(
		this->mpGlobeWidget->getMapNodeRef(),SceneTool::FLAT_MT);
	osgEarth::Symbology::Style& style = pAreaMeasure->getTextStyle();
	style.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = osgEarth::Symbology::Color::Yellow;
	style.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 30;
	style.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloBackdropType() = osgText::Text::BackdropType::OUTLINE;
	style.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloImplementation() =  osgText::Text::BackdropImplementation::STENCIL_BUFFER;
	style.getOrCreate<osgEarth::Symbology::TextSymbol>()->halo()->color() =osgEarth::Symbology::Color(0,0,0,1);
	style.getOrCreate<osgEarth::Symbology::TextSymbol>()->pixelOffset() = osg::Vec2s(10,10);
	pAreaMeasure->beginEdit();
	pAreaMeasure->addToolListener(pAreaMeasure);
	this->mpGlobeWidget->addEventHandler(pAreaMeasure);
	mpGeometryEditor = pAreaMeasure;
#else
	SceneTool::CAreaMeasureTool* pAreaMeasure = new SceneTool::CAreaMeasureTool(
		this->mpGlobeWidget->getMapNodeRef(),SceneTool::FLAT_MT);
	pAreaMeasure->beginEdit("",NULL);
	this->mpGlobeWidget->addEventHandler(pAreaMeasure);
	mpGeometryEditor = pAreaMeasure;
#endif
}

void myEarthSim::slotEndFlatArea()
{
	this->mpGeometryEditor->endEdit(true);
	this->mpGlobeWidget->removeEventHandler(this->mpGeometryEditor);
	this->mpGeometryEditor = NULL;
}

void myEarthSim::slotBeginSphereArea()
{
	if (NULL != this->mpGeometryEditor)
	{
		QMessageBox::information(this, "������Ϣ", "��ǰ���ڶ���༭��", QMessageBox::Ok);
		return;
	}
#if 0
	SceneTool::CAreaMeasure* pAreaMeasure = new SceneTool::CAreaMeasure(
		this->mpGlobeWidget->getMapNodeRef(),SceneTool::SPHERE_MT);
	pAreaMeasure->beginEdit();
	pAreaMeasure->addToolListener(pAreaMeasure);
	this->mpGlobeWidget->addEventHandler(pAreaMeasure);
	mpGeometryEditor = pAreaMeasure;
#else
	SceneTool::CAreaMeasureTool* pAreaMeasure = new SceneTool::CAreaMeasureTool(
		this->mpGlobeWidget->getMapNodeRef(),SceneTool::SPHERE_MT);
	pAreaMeasure->beginEdit("",NULL);
	this->mpGlobeWidget->addEventHandler(pAreaMeasure);
	mpGeometryEditor = pAreaMeasure;
#endif
}

void myEarthSim::slotEndSphereArea()
{
	this->mpGeometryEditor->endEdit(true);
	this->mpGlobeWidget->removeEventHandler(this->mpGeometryEditor);
	this->mpGeometryEditor = NULL;
}

void myEarthSim::slotBeginAzimuth()
{
	if (NULL != this->mpGeometryEditor)
	{
		QMessageBox::information(this, "������Ϣ", "��ǰ���ڶ���༭��", QMessageBox::Ok);
		return;
	}
	SceneTool::CAzimuthMeasure* pAzimuthMeasure = new SceneTool::CAzimuthMeasure(
		this->mpGlobeWidget->getMapNodeRef());
	pAzimuthMeasure->beginEdit();
	pAzimuthMeasure->addToolListener(pAzimuthMeasure);
	this->mpGlobeWidget->addEventHandler(pAzimuthMeasure);
	mpGeometryEditor = pAzimuthMeasure;
}

void myEarthSim::slotEndAzimuth()
{
	this->mpGeometryEditor->endEdit(true);
	this->mpGlobeWidget->removeEventHandler(this->mpGeometryEditor);
	this->mpGeometryEditor = NULL;
}

void myEarthSim::slotActionCreatePoint()
{
	if (NULL == this->mpGeoPoint)
	{
		//���������
		this->mpGeoPoint = new Graphic::CGeoPoint(this->mpGlobeWidget->getMapNodeRef());
		//���ö���ĵ�������
		mpGeoPoint->setGeoPosition(osg::Vec3d(116.329612, 39.906085, 10));
		osgEarth::Symbology::Style style = mpGeoPoint->getStyle();
		mpGeoPoint->setName("���ֵص�");
		mpGeoPoint->setType("Target");//����������type���ַ�������
		Style& pointStyle = this->mpGeoPoint->getStyle();
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()= "fonts/simhei.ttf";//ָ����������·��
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 20.0;//���������С
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = osgEarth::Symbology::Color::Red;//����������ɫ
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_BOTTOM;
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->declutter() = false;
		pointStyle.getOrCreate<osgEarth::Symbology::IconSymbol>()->declutter() = false;
		pointStyle.getOrCreate<osgEarth::Symbology::IconSymbol>()->alignment() = osgEarth::Symbology::IconSymbol::ALIGN_CENTER_BOTTOM;
		pointStyle.getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral(
			"systemImages/resources/image/fire.png");//���õ�ͼ����ʾ��ͼƬ
		pointStyle.getOrCreate<osgEarth::Annotation::IconSymbol>()->scale() = 0.3;
		mpGeoPoint->loadToScene();
		mpGeoPoint->renderToScene();
		
	}
}

void myEarthSim::slotActionCreateLine()
{
	if (NULL == this->mpGeoLine)
	{
		//�����߶���
		this->mpGeoLine = new Graphic::CGeoLine(this->mpGlobeWidget->getMapNodeRef());
		std::vector<osg::Vec3d>& geometry = this->mpGeoLine->getGeometry();
		geometry.push_back(osg::Vec3d(116.16, 39.66, 10));
		geometry.push_back(osg::Vec3d(116.66, 39.66, 10));
		geometry.push_back(osg::Vec3d(116.66, 40.16, 10));
		geometry.push_back(osg::Vec3d(116.16, 40.16, 10));
		osgEarth::Symbology::Style& pathStyle = this->mpGeoLine->getStyle();
		pathStyle.getOrCreate<LineSymbol>()->stroke()->color() = osgEarth::Symbology::Color(osgEarth::Symbology::Color::Yellow, 1);
		pathStyle.getOrCreate<LineSymbol>()->stroke()->width() = 5.0f;
		//����򿪻�������
		//pathStyle.getOrCreate<LineSymbol>()->stroke()->stippleFactor() = 3;
		//pathStyle.getOrCreate<LineSymbol>()->stroke()->stipplePattern() = 0x1C47;
		pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;
		pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;
		pathStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->tessellation() = 20;
		pathStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->verticalOffset() = 0.1;
		mpGeoLine->loadToScene();
		mpGeoLine->renderToScene();
		
	}
}

void myEarthSim::slotActionCreatePolygon()
{
	if (NULL == this->mpGeoPolygon)
	{
		//���������
		this->mpGeoPolygon = new Graphic::CGeoPolygon(this->mpGlobeWidget->getMapNodeRef());
		std::vector<osg::Vec3d>& geometry = mpGeoPolygon->getGeometry();
		geometry.push_back(osg::Vec3d(116.16, 39.66, 10));
		geometry.push_back(osg::Vec3d(116.66, 39.66, 10));
		geometry.push_back(osg::Vec3d(116.66, 40.16, 10));
		geometry.push_back(osg::Vec3d(116.16, 40.16, 10));
		osgEarth::Symbology::Style& polygonStyle = this->mpGeoPolygon->getStyle();
		polygonStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
		polygonStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
		polygonStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->width() = 5.0;
		polygonStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() =
			osgEarth::Symbology::Color(osgEarth::Symbology::Color::Red, 0.6);
		polygonStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->color() = osgEarth::Symbology::Color(osgEarth::Symbology::Color::Yellow, 1);
		polygonStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->tessellation() = 20;
		polygonStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
			->verticalOffset() = 0.1;
		mpGeoPolygon->loadToScene();
		mpGeoPolygon->renderToScene();
		
	}
}

void myEarthSim::slotActionDeletePoint()
{
	if (this->mpGeoPoint)
	{
		this->mpGeoPoint->deRenderFromScene();
		this->mpGeoPoint->unLoadFromScene();
		delete this->mpGeoPoint;
		this->mpGeoPoint = NULL;
		
	}
}

void myEarthSim::slotActionDeleteLine()
{
	if (this->mpGeoLine)
	{
		this->mpGeoLine->deRenderFromScene();
		this->mpGeoLine->unLoadFromScene();
		delete this->mpGeoLine;
		this->mpGeoLine = NULL;
		
	}
}

void myEarthSim::slotActionDeletePolygon()
{
	if (this->mpGeoPolygon)
	{
		this->mpGeoPolygon->deRenderFromScene();
		this->mpGeoPolygon->unLoadFromScene();
		delete this->mpGeoPolygon;
		this->mpGeoPolygon = NULL;
		
	}
}

void myEarthSim::slotActionCreateScutcheon()
{
	if (NULL == this->mpGeoScutcheon)
	{
		Geo::CGeoScutcheon* pGeoScutcheon = new Geo::CGeoScutcheon(this->mpGlobeWidget->getMapNodeRef());
		//�����ֶ���Ϊ���Ʊ�ͷ����ʾ
		pGeoScutcheon->setName("101�����˻�");
		pGeoScutcheon->setGeoPosition(osg::Vec3d(116,39,10));

		pGeoScutcheon->setIntervalBetweenTitleAndContent(70);
		//���ñ�ͷ��������
		pGeoScutcheon->getTitleSymbol()->alignment() = TextSymbol::ALIGN_CENTER_TOP;
		pGeoScutcheon->getTitleSymbol()->halo()->color() = Color::Black;
		pGeoScutcheon->getTitleSymbol()->fill()->color() = Color::Red;
		pGeoScutcheon->getTitleSymbol()->size() = 16;
		//�������ݵ�������
		pGeoScutcheon->getContentSymbol()->alignment() = TextSymbol::ALIGN_LEFT_BASE_LINE;
		pGeoScutcheon->getContentSymbol()->halo()->color() = Color::Black;
		pGeoScutcheon->getContentSymbol()->fill()->color() = Color::White;
		pGeoScutcheon->getContentSymbol()->size() = 16;

		//�����ֶ���ֵ�ķָ���
		pGeoScutcheon->setSeparativeSign(" # ");
		//�����м��
		pGeoScutcheon->setVerticalSpacing(20);

		//���Ʊ���ͼƬ�����ݱ��������Զ��仯��
		pGeoScutcheon->getStyle().getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral(
			"systemImages/resources/image/pannelbackground.png");
		//��ͷ����ͼƬ
		pGeoScutcheon->getTitleImageStyle().getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral(
			"systemImages/resources/image/pannelbackground_title.png");
		pGeoScutcheon->getFields().clear();
		pGeoScutcheon->getFields().push_back(Geo::Field("���","20180804"));
		pGeoScutcheon->getFields().push_back(Geo::Field("����","���˻�D"));
		pGeoScutcheon->getFields().push_back(Geo::Field("����","���̽��"));
		Geo::Field field = Geo::Field("����","�й�����");
		field.valid = false;//validΪfalseʱ������ʾ���ֶ�
		pGeoScutcheon->getFields().push_back(field);
		field = Geo::Field("����","���ƹ���");
		field.valid = false;//validΪfalseʱ������ʾ���ֶ�
		pGeoScutcheon->getFields().push_back(field);
		pGeoScutcheon->getFields().push_back(Geo::Field("����","20130704"));
		pGeoScutcheon->getFields().push_back(Geo::Field("�û�","�й�����"));
		pGeoScutcheon->getFields().push_back(Geo::Field("��ַ","�й�����"));
		pGeoScutcheon->getFields().push_back(Geo::Field("���","���պ���"));
		pGeoScutcheon->getFields().push_back(Geo::Field("����","30��"));
		pGeoScutcheon->getFields().push_back(Geo::Field("����","̫��"));
		pGeoScutcheon->getFields().push_back(Geo::Field("��Ȩ","20��"));
		pGeoScutcheon->loadToScene();
		pGeoScutcheon->renderToScene();
		pGeoScutcheon->setScreenCoordinatesOffset(osg::Vec2s(100,-100));
		this->mpGeoScutcheon = pGeoScutcheon;
		
	}
}

void myEarthSim::slotActionDeleteScutcheon()
{
	if (this->mpGeoScutcheon)
	{
		this->mpGeoScutcheon->deRenderFromScene();
		this->mpGeoScutcheon->unLoadFromScene();
		delete this->mpGeoScutcheon;
		this->mpGeoScutcheon = NULL;
		
	}
}

void myEarthSim::slotActionCreateSegment()
{
	if (this->mpGeoSegment == NULL)
	{
		Geo::CGeoSegment* pGeoSegment = new Geo::CGeoSegment(this->mpGlobeWidget->getMapNodeRef());
		pGeoSegment->getStartPoint() = osg::Vec3d(116,39,10);
		pGeoSegment->getEndPoint() = osg::Vec3d(116,39.1,10);
		pGeoSegment->getStyle().getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->color() = osgEarth::Symbology::Color(osgEarth::Symbology::Color::Yellow, 1);
		pGeoSegment->getStyle().getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->width() = 4;
		pGeoSegment->loadToScene();
		pGeoSegment->renderToScene();
		this->mpGeoSegment = pGeoSegment;
		
	}
}

void myEarthSim::slotActionDeleteSegment()
{
	if (this->mpGeoSegment != NULL)
	{
		this->mpGeoSegment->deRenderFromScene();
		this->mpGeoSegment->unLoadFromScene();
		delete this->mpGeoSegment;
		this->mpGeoSegment = NULL;
		
		
	}
}

void myEarthSim::slotActionCreateRadarLine()
{
	if (NULL == this->mpGeoRadarLine)
	{
		//�״�ɨ���ߣ������ð�͸������Ч��
		Geo::CGeoCircleEx* pCircle = new Geo::CGeoCircleEx(this->mpGlobeWidget->getMapNodeRef());
		//���ö���ĵ�������
		pCircle->setGeoPosition(osg::Vec3d(116, 39, 1));
		//���������ɫ
		pCircle->getStyle().getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Symbology::Color(1,1,0,0.5);
		//���ö���İ뾶
		pCircle->setRadius(100000);
		//����Բ������ʼ��λ�Ƕ�
		pCircle->setBeginAngle(30);
		//����Բ������ֹ��λ�Ƕ�
		pCircle->setEndAngle(300);
		//�Ƿ�������ת
		pCircle->setRotateEnable(true);
		//Ĭ��ת��ϵ����ÿһ֡��ת�仯
		pCircle->setRotateFactor(2);
		//����Բ������ɫ�Ƿ�ʹ�ý���͸����
		pCircle->useGradualTransparent() = true;
		pCircle->loadToScene();
		pCircle->renderToScene();
		this->mpGeoRadarLine = pCircle;
		
	}
}

void myEarthSim::slotActionModifyRadarColor()
{
	if (this->mpGeoRadarLine != NULL)
	{
		this->mpGeoRadarLine->deRenderFromScene();
		this->mpGeoRadarLine->unLoadFromScene();
		//���������ɫ
		this->mpGeoRadarLine->getStyle().getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Symbology::Color(1,0,0,0.5);
		//���ö���ĵ�������
		this->mpGeoRadarLine->setGeoPosition(osg::Vec3d(117, 40, 1));
		this->mpGeoRadarLine->loadToScene();
		this->mpGeoRadarLine->renderToScene();
	}
}

void myEarthSim::slotActionDeleteRadarLine()
{
	if (this->mpGeoRadarLine != NULL)
	{
		this->mpGeoRadarLine->deRenderFromScene();
		this->mpGeoRadarLine->unLoadFromScene();
		delete this->mpGeoRadarLine;
		this->mpGeoRadarLine = NULL;
	}
}

void myEarthSim::slotBeginEditorPoint()
{
	if (NULL != this->mpEditorObject)
	{
		QMessageBox::information(this, "������Ϣ", "��ǰ���ڶ���༭��", QMessageBox::Ok);
		return;
	}
	GeometryEditor::IPointEditor* pEditor = new GeometryEditor::IPointEditor(this->mpGlobeWidget->getMapNodeRef());
	pEditor->beginEdit();
	this->mpGlobeWidget->addEventHandler(pEditor);
	mpEditorObject = pEditor;
}

void myEarthSim::slotEndEditorPoint()
{
	if(this->mpEditorObject)
	{
		this->mpEditorObject->endEdit(true);
		this->mpGlobeWidget->removeEventHandler(this->mpEditorObject);
		this->mpEditorObject = NULL;
	}
}

void myEarthSim::slotBeginEditorLine()
{
	if (NULL != this->mpEditorObject)
	{
		QMessageBox::information(this, "������Ϣ", "��ǰ���ڶ���༭��", QMessageBox::Ok);
		return;
	}
	GeometryEditor::IPolylineEditor* pEditor = new GeometryEditor::IPolylineEditor(this->mpGlobeWidget->getMapNodeRef());
	pEditor->beginEdit();
	this->mpGlobeWidget->addEventHandler(pEditor);
	mpEditorObject = pEditor;
}

void myEarthSim::slotEndEditorLine()
{
	if(this->mpEditorObject)
	{
		this->mpEditorObject->endEdit(true);
		this->mpGlobeWidget->removeEventHandler(this->mpEditorObject);
		this->mpEditorObject = NULL;
	}
}

void myEarthSim::slotBeginEditorPolygon()
{
	if (NULL != this->mpEditorObject)
	{
		QMessageBox::information(this, "������Ϣ", "��ǰ���ڶ���༭��", QMessageBox::Ok);
		return;
	}
	GeometryEditor::IPolygonEditor* pEditor = new GeometryEditor::IPolygonEditor(this->mpGlobeWidget->getMapNodeRef());
	pEditor->beginEdit();
	this->mpGlobeWidget->addEventHandler(pEditor);
	mpEditorObject = pEditor;
}

void myEarthSim::slotEndEditorPolygon()
{
	if(this->mpEditorObject)
	{
		this->mpEditorObject->endEdit(true);
		this->mpGlobeWidget->removeEventHandler(this->mpEditorObject);
		this->mpEditorObject = NULL;
	}
}


void myEarthSim::slotActionBeginGraphicEdit()
{
	if (NULL != this->mpEditorObject)
	{
		QMessageBox::information(this, "������Ϣ", "��ǰ���ڶ���༭��", QMessageBox::Ok);
		return;
	}

	GraphicTypeDlg graphicTypeDlg(this);
	if (graphicTypeDlg.exec() != 1)
		return;
	std::string type = graphicTypeDlg.getGraphicType().toStdString();
	if (type != "")
	{
		GeometryEditor::IGeoGraphicEditor * pEditor = new GeometryEditor::IGeoGraphicEditor(this->mpGlobeWidget->getMapNodeRef());
		//if (g_pGraphicObject)
		//{
		//	g_pGraphicObject->getStyle().getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() =
		//		osgEarth::Symbology::Color(osgEarth::Symbology::Color::Yellow, 0.6);
		//	g_pGraphicObject->getStyle().getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
		//	g_pGraphicObject->getStyle().getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
		//	g_pGraphicObject->getStyle().getOrCreate<osgEarth::Symbology::LineSymbol>()
		//		->stroke()->width() = 5.0;
		//	g_pGraphicObject->getStyle().getOrCreate<osgEarth::Symbology::LineSymbol>()
		//		->stroke()->color() = osgEarth::Symbology::Color(osgEarth::Symbology::Color::Blue, 1);
		//	//��һ������Ϊ�༭ͼ�ε�����
		//	//�ڶ���Ϊ��ǰҪ�༭ͼ�ζ���Ϊ��ʱ����༭�µ�ͼ�Σ���Ϊ�����������е�ͼ�ν��ж��α༭��
		//	pEditor->beginEdit(
		//		g_pGraphicObject->getType(),
		//		g_pGraphicObject);
		//}
		//else
		{
			pEditor->getGeometryStyle().getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() =
				osgEarth::Symbology::Color(osgEarth::Symbology::Color::Yellow, 0.6);
			pEditor->getGeometryStyle().getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
			pEditor->getGeometryStyle().getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
			pEditor->getGeometryStyle().getOrCreate<osgEarth::Symbology::LineSymbol>()
				->stroke()->width() = 5.0;
			pEditor->getGeometryStyle().getOrCreate<osgEarth::Symbology::LineSymbol>()
				->stroke()->color() = osgEarth::Symbology::Color(osgEarth::Symbology::Color::Blue, 1);
			//��һ������Ϊ�༭ͼ�ε�����
			//�ڶ���Ϊ��ǰҪ�༭ͼ�ζ���Ϊ��ʱ����༭�µ�ͼ�Σ���Ϊ�����������е�ͼ�ν��ж��α༭��
			//���������Ҫ����һ��,���ڶ���������Ϊ���ǣ�����ʹ�õڶ���������getType()��Ϊtypeֵ
			pEditor->beginEdit(type,NULL);
		}
		this->mpGlobeWidget->addEventHandler(pEditor);
		mpEditorObject = pEditor;
	}
}

void myEarthSim::slotActionBeginSelectedGraphicEdit()
{
	if (NULL != this->mpEditorObject)
	{
		QMessageBox::information(this, "������Ϣ", "��ǰ���ڶ���༭��", QMessageBox::Ok);
		return;
	}

	GeometryEditor::IGeoGraphicEditor * pEditor = new GeometryEditor::IGeoGraphicEditor(this->mpGlobeWidget->getMapNodeRef());
	if (g_pGraphicObject)
	{
		pEditor->getGeometryStyle().getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() =
			osgEarth::Symbology::Color(osgEarth::Symbology::Color::Red, 0.6);
		pEditor->getGeometryStyle().getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
		pEditor->getGeometryStyle().getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
		pEditor->getGeometryStyle().getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->width() = 5.0;
		pEditor->getGeometryStyle().getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->color() = osgEarth::Symbology::Color(osgEarth::Symbology::Color::Blue, 1);
		//��һ������Ϊ�༭ͼ�ε�����
		//�ڶ���Ϊ��ǰҪ�༭ͼ�ζ���Ϊ��ʱ����༭�µ�ͼ�Σ���Ϊ�����������е�ͼ�ν��ж��α༭��
		pEditor->beginEdit(
			g_pGraphicObject->getType(),
			g_pGraphicObject);

		//��ҵ���������ö���״̬�Ľӿ�
		g_pGraphicObject->setState(Common::ISceneObject::SOS_EDITING);
	}
	this->mpGlobeWidget->addEventHandler(pEditor);
	mpEditorObject = pEditor;
}

void myEarthSim::slotActionEndGraphicEdit()
{
	if(this->mpEditorObject)
	{
		Common::ISceneObject* pGraphicObject = this->mpEditorObject->generateSceneObject();
		if (pGraphicObject)
		{
			if (pGraphicObject->getType() == Graphic::GT_POLYLINE)
			{
				Graphic::CGeoLine * pPolyline = dynamic_cast<Graphic::CGeoLine*>(pGraphicObject);
				if (pPolyline)
				{
					std::vector<osg::Vec3d>& geometry = pPolyline->getGeometry();
					QString fileName;
					fileName = QFileDialog::getSaveFileName(this,
						tr("Save Data"), "", tr("Data Files (*.ini)"));

					if (!fileName.isNull())
					{
						CDataCreator::tofile(fileName,geometry);
					}

					
					pPolyline->deRenderFromScene();
					pPolyline->unLoadFromScene();
				}
			}
			else if (pGraphicObject->getType() == Graphic::GT_CIRCLE)
			{
				Graphic::CGeoCircle * pCircle = dynamic_cast<Graphic::CGeoCircle*>(pGraphicObject);
				if (pCircle)
				{
					if (pCircle->getGeometry().size() >= 2)
					{
						osg::Vec3d geoPosition = pCircle->getGeoPosition();
						double radius = pCircle->getRadius();
					}
				}
			}
		}
		this->mpEditorObject->endEdit(true);
		this->mpGlobeWidget->removeEventHandler(this->mpEditorObject);
		this->mpEditorObject = NULL;
	}
}

std::string intToString(int data)
{
	char str[200];
	sprintf(str, "%d", data);
	return str;
}

void myEarthSim::slotAddDynamicObjects()
{
#if 0
	DynamicObject::CDynamicObjectLayer* pDynamicObjectLayer  = 
		Globe::CGlobeMap::getInstance()->addDynamicObjectLayer(
		"0","DynamicLayer",mpGlobeWidget->getMapNodeRef());
	for (int index = 0; index < 200; ++index)
	{
		std::string id = intToString(index);
		std::string name = "obejct" + id;
		DynamicObject::CDynamicObject* pDynamicObject = pDynamicObjectLayer->addDynamicObject(id,name);
		pDynamicObject->setScutcheonContent("obejct1");
		pDynamicObject->setScutcheonVisible(true);
		/*pDynamicObject->setScutcheonVisible(false);
		pDynamicObject->setTrackLineVisible(true);*/
		pDynamicObject->setModelPath("earthData/model/fly.ive");
		pDynamicObject->setPosition(osg::Vec3d(118.824292 + index * 0.1,32.048700 + index * 0.1,100));
	}
#else
	//̬�ƶ����ʼ��
	CObjectManager::instance()->initial(
		this->mpGlobeWidget->getRootRef(),
		this->mpGlobeWidget->getMapNodeRef(),
		(osgViewer::Viewer*)this->mpGlobeWidget->getViewer(),
		this->mpGlobeWidget->getScreenCamera());

	//�������ݽ����߳�
	CDataGenerater::instance()->start();
#endif
}

void myEarthSim::slotAddWarnningRegion()
{
	DynamicObject::CSceneObjectLayer* pWarnningObjectLayer  = 
		Globe::CGlobeMap::getInstance()->addSceneObjectLayer(
		"0","WarnningObjectLayer",Globe::CSceneObjectLayerType::WARNREGION);

	for (int index = 0; index < 4; ++index)
	{
		std::string id = intToString(index);
		std::string name = "obejct" + id;
		//���������
		Graphic::CGeoPolygon* pWarnningObject = new Graphic::CGeoPolygon(this->mpGlobeWidget->getMapNodeRef());
		pWarnningObject->setID(id);
		pWarnningObject->setName(name);
		std::vector<osg::Vec3d>& geometry = pWarnningObject->getGeometry();
		osg::Vec3d conterPos = osg::Vec3d(117, 39 - index * 2, 10);
		geometry.push_back(osg::Vec3d(conterPos.x() - 1, conterPos.y() + 0.9, 10));
		geometry.push_back(osg::Vec3d(conterPos.x() + 1, conterPos.y() + 0.9, 10));
		geometry.push_back(osg::Vec3d(conterPos.x() + 1, conterPos.y() - 0.9, 10));
		geometry.push_back(osg::Vec3d(conterPos.x() - 1.5, conterPos.y() - 0.9, 10));
		osgEarth::Symbology::Style& polygonStyle = pWarnningObject->getStyle();
		polygonStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
		polygonStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
		polygonStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->width() = 5.0;
		polygonStyle.getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() =
			osgEarth::Symbology::Color(osgEarth::Symbology::Color::Yellow, 0.6);
		polygonStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->color() = osgEarth::Symbology::Color(osgEarth::Symbology::Color::Yellow, 0.0);
		polygonStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->tessellation() = 20;
		polygonStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
			->verticalOffset() = 0.1;
		pWarnningObjectLayer->addObject(pWarnningObject);
	}

	for (int index = 4; index < 10; ++index)
	{
		std::string id = intToString(index);
		std::string name = "obejct" + id;
		Graphic::CGeoCircle* pWarnningObject = new Graphic::CGeoCircle(this->mpGlobeWidget->getMapNodeRef());
		pWarnningObject->setID(id);
		pWarnningObject->setName(name);
		//���ö���ĵ�������
		pWarnningObject->setGeoPosition(osg::Vec3d(117, 39 - index * 2, 10));
		//���������ɫ
		pWarnningObject->getStyle().getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Symbology::Color(1,1,0,0.5);
		//���ö���İ뾶
		pWarnningObject->setRadius(100000);
		//����Բ������ʼ��λ�Ƕ�
		pWarnningObject->setBeginAngle(0);
		//����Բ������ֹ��λ�Ƕ�
		pWarnningObject->setEndAngle(360);
		pWarnningObjectLayer->addObject(pWarnningObject);
	}
	pWarnningObjectLayer->renderToScene();
	//���澯��������̬��Ŀ�������󣬱��ڼ��Ŀ���Ƿ����澯��
	CObjectManager::instance()->setWarnningObjectLayer(pWarnningObjectLayer);
}

void myEarthSim::slotActionControlIconVisible()
{
	bool isVisible = CObjectManager::instance()->getIconVisible();
	CObjectManager::instance()->setIconVisible(!isVisible);
}

void myEarthSim::slotActionControlScutVisible()
{
	bool isVisible = CObjectManager::instance()->getScutcheonVisible();
	CObjectManager::instance()->setScutcheonVisible(!isVisible);
}

void myEarthSim::slotActionControlTrackLineVisible()
{
	bool isVisible = CObjectManager::instance()->getTrackLineVisible();
	CObjectManager::instance()->setTrackLineVisible(!isVisible);
}

void myEarthSim::slotActionChangeTrackLineColor()
{
	static bool isChange = true;
	if(isChange)
		CObjectManager::instance()->setTrackLineColor(osg::Vec4f(0,0,1,1));
	else
		CObjectManager::instance()->setTrackLineColor(osg::Vec4f(1,0,0,1));
	isChange = !isChange;
}

void myEarthSim::slotActionChangeTrackLineType()
{
	DynamicObject::LineType lineType = CObjectManager::instance()->getTrackLineType();
	if(lineType == DynamicObject::DOTTED_LT)
		CObjectManager::instance()->setTrackLineType(DynamicObject::SOLID_LT);
	else
		CObjectManager::instance()->setTrackLineType(DynamicObject::DOTTED_LT);
}

void myEarthSim::slotActionChangeTrackLineLength()
{
	static bool isChange = true;
	if(isChange)
		CObjectManager::instance()->setTrackLinePointSize(1000000);
	else
		CObjectManager::instance()->setTrackLinePointSize(50);
	isChange = !isChange;
}

void myEarthSim::slotActionDirLineVisible()
{
	bool isVisible = CObjectManager::instance()->getDirectionallineVisible();
	CObjectManager::instance()->setDirectionallineVisible(!isVisible);
}

void myEarthSim::slotActionChangeDirLineColor()
{
	static bool isChange = true;
	if(isChange)
		CObjectManager::instance()->setDirectionallineColor(osg::Vec4f(1,0,0,1));
	else
		CObjectManager::instance()->setDirectionallineColor(osg::Vec4f(1,1,0,1));
	isChange = !isChange;
}

void myEarthSim::slotActionChangeDirLineType()
{
	DynamicObject::LineType lineType = CObjectManager::instance()->getDirectionallineType();
	if(lineType == DynamicObject::DOTTED_LT)
		CObjectManager::instance()->setDirectionallineType(DynamicObject::SOLID_LT);
	else
		CObjectManager::instance()->setDirectionallineType(DynamicObject::DOTTED_LT);
}

void myEarthSim::slotActionChangeTrackLineWidth()
{
	static bool isChange = true;
	if(isChange)
		CObjectManager::instance()->setTrackLineWidth(2.0);
	else
		CObjectManager::instance()->setTrackLineWidth(5.0);
	isChange = !isChange;
}

void myEarthSim::slotActionChangeDirLineWidth()
{
	static bool isChange = true;
	if(isChange)
		CObjectManager::instance()->setDirectionallineWidth(2.0);
	else
		CObjectManager::instance()->setDirectionallineWidth(5.0);
	isChange = !isChange;
}

void myEarthSim::slotActionAddObjectListener()
{
	CCustomDynamicObjectListener* pCustomDynamicObjectListener = new CCustomDynamicObjectListener;
	CObjectManager::instance()->addDynamicObjectListener(pCustomDynamicObjectListener);
}

void myEarthSim::slotActionDynamicObjectBlink()
{
	Core::IObjectContainer objectContainer = CObjectManager::instance()->
		getDetectionTargetsByGroup(DynamicObject::CDynamicObjectGroupType::RADARTARGET);
	std::vector<std::string> keys;
	objectContainer.getKeys(keys);
	if(0 == keys.size())
		return;
	DynamicObject::CDynamicObject* pDynamicObject = 
		dynamic_cast<DynamicObject::CDynamicObject*>(objectContainer.getObject(keys.at(0)));
	if(NULL == pDynamicObject)
		return;
	std::string objectType = pDynamicObject->getType();
	osg::Vec3d pos = pDynamicObject->getModelCurrentPoint();
	this->mpGlobeWidget->flyTo(pos.x(), pos.y(),10000,2);
	pDynamicObject->setObejctBlink(5.0);
	pDynamicObject->setObejctBlinkInterval(0.1);
}

void myEarthSim::slotActionSetScutcheonFields()
{
	static bool isChange = true;
	if(isChange)
	{
		std::vector<std::string> fields;
		fields.push_back("���");
		fields.push_back("����");
		fields.push_back("��λ");
		fields.push_back("����");
		fields.push_back("����");
		fields.push_back("γ��");
		CObjectManager::instance()->setScutcheonVisibleFields(fields);
	}
	/*else
	{
		std::vector<std::string> fields;
		fields.push_back("���");
		fields.push_back("����");
		fields.push_back("����");
		fields.push_back("����");
		fields.push_back("����");
		fields.push_back("����");
		fields.push_back("�û�");
		fields.push_back("��ַ");
		fields.push_back("���");
		fields.push_back("����");
		CObjectManager::instance()->setScutcheonVisibleFields(fields);
	}*/
	isChange = !isChange;
}

void myEarthSim::slotActionCircleTangentAngle()
{
	std::vector<DynamicObject::CSceneObjectLayer*> layers = CObjectManager::instance()->getSceneObjectLayers();
	if(layers.size() == 0)
		return;
	DynamicObject::CSceneObjectLayer* pLayer = layers.at(0);
	if(NULL == pLayer)
		return;
	std::vector<std::string> keys;
	pLayer->getKeys(keys);
	if (keys.empty())
		return;
	std::string id = keys.at(keys.size() - 5);
	Graphic::CGeoCircle* pGeoCircle =dynamic_cast<Graphic::CGeoCircle*>(pLayer->getObject(id));
	if(NULL == pGeoCircle)
		return;

	Core::IObjectContainer objectContainer = CObjectManager::instance()->
		getDetectionTargetsByGroup(DynamicObject::CDynamicObjectGroupType::RADARTARGET);
	keys.clear();
	objectContainer.getKeys(keys);
	if(0 == keys.size())
		return;
	DynamicObject::CDynamicObject* pDynamicObject = 
		dynamic_cast<DynamicObject::CDynamicObject*>(objectContainer.getObject(keys.at(0)));
	if(NULL == pDynamicObject)
		return;
	osg::Vec3d pos = pDynamicObject->getModelCurrentPoint();
	double angle = 0.0;
	DynamicObject::CCalculatorHelper::getCircleTangentAngle(
		pos,pGeoCircle->getGeoPosition(),pGeoCircle->getRadius(),this->mpGlobeWidget->getMapNodeRef()->getMap()->getSRS(),angle);
}

void myEarthSim::slotActionPolygonTangentAngle()
{
	std::vector<DynamicObject::CSceneObjectLayer*> layers = CObjectManager::instance()->getSceneObjectLayers();
	if(layers.size() == 0)
		return;
	DynamicObject::CSceneObjectLayer* pLayer = layers.at(0);
	if(NULL == pLayer)
		return;
	std::vector<std::string> keys;
	pLayer->getKeys(keys);
	if (keys.empty())
		return;
	std::string id = keys.at(2);
	Graphic::CGeoPolygon* pGeoPolygon =dynamic_cast<Graphic::CGeoPolygon*>(pLayer->getObject(id));
	if(NULL == pGeoPolygon)
		return;

	Core::IObjectContainer objectContainer = CObjectManager::instance()->
		getDetectionTargetsByGroup(DynamicObject::CDynamicObjectGroupType::RADARTARGET);
	keys.clear();
	objectContainer.getKeys(keys);
	if(0 == keys.size())
		return;
	DynamicObject::CDynamicObject* pDynamicObject = 
		dynamic_cast<DynamicObject::CDynamicObject*>(objectContainer.getObject(keys.at(keys.size() - 1)));
	if(NULL == pDynamicObject)
		return;
	osg::Vec3d pos = pDynamicObject->getModelCurrentPoint();
	double angle = 0.0,angle2 = 0.0;
	DynamicObject::CCalculatorHelper::getPolygonTangentAngle(pos,pGeoPolygon->getGeometry(),
		this->mpGlobeWidget->getMapNodeRef()->getMap()->getSRS(),angle,angle2);
}

void myEarthSim::slotActionComputeAngle()
{
	osg::Vec3d targetPoint;
	double targetAzimuth;
	osg::Vec3d centerPoint;
	double resultAngle;
	DynamicObject::CCalculatorHelper::getAngleByAzimuth(
		targetPoint,targetAzimuth,centerPoint,this->mpGlobeWidget->getMapNodeRef()->getMapSRS(),
		resultAngle);
}

void myEarthSim::slotActionComputeBanyCenter()
{
	Graphic::CGeoPolygon* pGeoPolygon = new Graphic::CGeoPolygon();//(�û��ṩ�Ķ���ζ���)
	osg::Vec3d baryCenter = DynamicObject::CCalculatorHelper::getBarycenter(pGeoPolygon->getGeometry());
}

void myEarthSim::slotActionDynamicObjectVisible()
{
	Core::IObjectContainer objectContainer = CObjectManager::instance()->
		getDetectionTargetsByGroup(DynamicObject::CDynamicObjectGroupType::RADARTARGET);
	std::vector<std::string> keys;
	objectContainer.getKeys(keys);
	if(0 == keys.size())
		return;
	DynamicObject::CDynamicObject* pDynamicObject = 
		dynamic_cast<DynamicObject::CDynamicObject*>(objectContainer.getObject(keys.at(0)));
	if(NULL == pDynamicObject)
		return;

	static bool isChange = true;
	if(isChange)
	{
		pDynamicObject->setScutcheonVisible(false);
		pDynamicObject->setTrackLineVisible(false);
	}
	else
	{
		pDynamicObject->setScutcheonVisible(true);
		pDynamicObject->setTrackLineVisible(true);
	}

	isChange = !isChange;
}

void myEarthSim::slotAddScanningRegion()
{
	static bool isChange = true;
	if(isChange)
	{
		DynamicObject::CSceneObjectLayer* pScanningObjectLayer  = 
			Globe::CGlobeMap::getInstance()->addSceneObjectLayer(
			"10","ScanningObjectLayer",Globe::CSceneObjectLayerType::SCANREGION);

		for (int index = 1; index < 10; ++index)
		{
			std::string id = intToString(index);
			std::string name = "obejct" + id;
			Geo::CGeoCircleEx* pScannningObject = new Geo::CGeoCircleEx(this->mpGlobeWidget->getMapNodeRef());
			pScannningObject->setID(id);
			pScannningObject->setName(name);
			//���ö���ĵ�������
			pScannningObject->setGeoPosition(osg::Vec3d(115, 39 - index * 2, 10));
			//���������ɫ
			pScannningObject->getStyle().getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Symbology::Color(1,1,0,0.5);
			//���ö���İ뾶
			pScannningObject->setRadius(100000);
			//����Բ������ʼ��λ�Ƕ�
			pScannningObject->setBeginAngle(30);
			//����Բ������ֹ��λ�Ƕ�
			pScannningObject->setEndAngle(300);
			//�Ƿ�������ת
			pScannningObject->setRotateEnable(true);
			//Ĭ��ת��ϵ����ÿһ֡��ת�仯
			pScannningObject->setRotateFactor(2);
			//����Բ������ɫ�Ƿ�ʹ�ý���͸����
			pScannningObject->useGradualTransparent() = true;
			pScanningObjectLayer->addObject(pScannningObject);
		}
		pScanningObjectLayer->renderToScene();
		//���澯��������̬��Ŀ�������󣬱��ڼ��Ŀ���Ƿ����澯��
		CObjectManager::instance()->setScanningObjectLayer(pScanningObjectLayer);
	}
	else
	{
		DynamicObject::CSceneObjectLayer* pScanningObjectLayer  = 
			Globe::CGlobeMap::getInstance()->getSceneObjectLayerByID("10");

		std::vector<std::string> keys;
		pScanningObjectLayer->getKeys(keys);
		for (int index = 0; index < keys.size(); ++index)
		{
			std::string id = keys.at(index);
			Geo::CGeoCircleEx* pScannningObject = dynamic_cast<Geo::CGeoCircleEx*>(pScanningObjectLayer->getObject(id));
			//���������ɫ
			pScannningObject->getStyle().getOrCreate<
				osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Symbology::Color(1,0,0,0.5);
		}

		Geo::CGeoCircleEx* pScannningObject = new Geo::CGeoCircleEx(this->mpGlobeWidget->getMapNodeRef());
		pScannningObject->setID("1000");
		pScannningObject->setName("obejct1000");
		//���ö���ĵ�������
		pScannningObject->setGeoPosition(osg::Vec3d(115, 39 - 10 * 2, 10));
		//���������ɫ
		pScannningObject->getStyle().getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color() = osgEarth::Symbology::Color(1,1,0,0.5);
		//���ö���İ뾶
		pScannningObject->setRadius(100000);
		//����Բ������ʼ��λ�Ƕ�
		pScannningObject->setBeginAngle(30);
		//����Բ������ֹ��λ�Ƕ�
		pScannningObject->setEndAngle(300);
		//�Ƿ�������ת
		pScannningObject->setRotateEnable(true);
		//Ĭ��ת��ϵ����ÿһ֡��ת�仯
		pScannningObject->setRotateFactor(2);
		//����Բ������ɫ�Ƿ�ʹ�ý���͸����
		pScannningObject->useGradualTransparent() = true;
		pScanningObjectLayer->addObject(pScannningObject);

		pScanningObjectLayer->deRenderFromScene();
		pScanningObjectLayer->renderToScene();
	}

	isChange = !isChange;
}

void myEarthSim::slotActionObjectVisibleByFacility()
{
	static bool isChange = true;
	if(isChange)
	{
		CObjectManager::instance()->setFacilityDetectionTargets("00000",true,true,true,true);
		CObjectManager::instance()->setFacilityDetectionTargets("11111",true,true,true,true);
	}
	else
	{
		CObjectManager::instance()->setFacilityDetectionTargets("00000",false,false,false,false);
		CObjectManager::instance()->setFacilityDetectionTargets("11111",false,false,false,false);
	}
	isChange = !isChange;
}

void myEarthSim::slotActionChangeIconStyle()
{
	CObjectManager::instance()->setIconFont("fonts/SIMLI.TTF");
	CObjectManager::instance()->setIconTextSize(30.0);
	CObjectManager::instance()->setIconTextColor(osgEarth::Symbology::Color::Red);
}

void myEarthSim::slotActionChangeObjectIconStyle()
{
	Core::IObjectContainer objectContainer = CObjectManager::instance()->
		getDetectionTargetsByGroup(DynamicObject::CDynamicObjectGroupType::RADARTARGET);
	std::vector<std::string> keys;
	objectContainer.getKeys(keys);
	if(0 == keys.size())
		return;
	DynamicObject::CDynamicObject* pDynamicObject = 
		dynamic_cast<DynamicObject::CDynamicObject*>(objectContainer.getObject(keys.at(0)));
	if(NULL == pDynamicObject)
		return;
	osgEarth::Symbology::Style& style = pDynamicObject->getIconStyle();
	style.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()= "fonts/SIMLI.TTF";
	style.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 30.0;
	style.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = osgEarth::Symbology::Color::Red;
	pDynamicObject->updateIconStyle();
}

void myEarthSim::slotActionChangeScuStyle()
{
	CObjectManager::instance()->setScutcheonTitleFont("fonts/SIMLI.TTF");
	CObjectManager::instance()->setScutcheonTitleTextSize(30.0);
	CObjectManager::instance()->setScutcheonTitleTextColor(osgEarth::Symbology::Color::Yellow);
	CObjectManager::instance()->setScutcheonTitleBackgroundImage("systemImages/resources/image/pannelbackground_title.png");
	CObjectManager::instance()->setScutcheonContentFont("fonts/SIMLI.TTF");
	CObjectManager::instance()->setScutcheonContentSize(30.0);
	CObjectManager::instance()->setScutcheonContentColor(osgEarth::Symbology::Color::Blue);
	CObjectManager::instance()->setScutcheonContentBackgroundImage("systemImages/resources/image/pannelImage1.png");
	//CObjectManager::instance()->setScutcheonScreenCoordinatesOffset(osg::Vec2s(30,30));
	CObjectManager::instance()->setScutcheonSeparativeSign(" # ");
	CObjectManager::instance()->setScutcheonVerticalSpacing(20);
}

void myEarthSim::slotActionChangeObjecctScuStyle()
{
	Core::IObjectContainer objectContainer = CObjectManager::instance()->
		getDetectionTargetsByGroup(DynamicObject::CDynamicObjectGroupType::RADARTARGET);
	std::vector<std::string> keys;
	objectContainer.getKeys(keys);
	if(0 == keys.size())
		return;
	DynamicObject::CDynamicObject* pDynamicObject = 
		dynamic_cast<DynamicObject::CDynamicObject*>(objectContainer.getObject(keys.at(0)));
	if(NULL == pDynamicObject)
		return;

	osgEarth::Symbology::TextSymbol* titleSymbol = pDynamicObject->getScutcheonTitleSymbol();
	titleSymbol->fill()->color() = osgEarth::Symbology::Color::Yellow;
	titleSymbol->size() = 30.0;
	titleSymbol->font() = "fonts/SIMLI.TTF";

	osgEarth::Symbology::TextSymbol* contentSymbol = pDynamicObject->getScutcheonContentSymbol();
	contentSymbol->font() = "fonts/SIMLI.TTF";
	contentSymbol->fill()->color() = osgEarth::Symbology::Color::Blue;
	contentSymbol->size() = 30.0;

	pDynamicObject->getScutcheonTitleImageStyle().getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral(
		"systemImages/resources/image/pannelbackground_title.png");

	pDynamicObject->getScutcheonStyle().getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral(
		"systemImages/resources/image/pannelImage1.png");

	pDynamicObject->setScutcheonSeparativeSign(" # ");
	pDynamicObject->setScutcheonVerticalSpacing(20);
	pDynamicObject->updateScutcheonStyle();
}

void myEarthSim::slotActionChangeTrackPoints()
{
	static bool isChange = true;
	if(isChange)
		CObjectManager::instance()->setTrackLinePointPattern(true);
	else
		CObjectManager::instance()->setTrackLinePointPattern(false);
	isChange = !isChange;
}

void myEarthSim::slotActionChangeObjectTrackPoint()
{
	Core::IObjectContainer objectContainer = CObjectManager::instance()->
		getDetectionTargetsByGroup(DynamicObject::CDynamicObjectGroupType::RADARTARGET);
	std::vector<std::string> keys;
	objectContainer.getKeys(keys);
	if(0 == keys.size())
		return;
	DynamicObject::CDynamicObject* pDynamicObject = 
		dynamic_cast<DynamicObject::CDynamicObject*>(objectContainer.getObject(keys.at(0)));
	if(NULL == pDynamicObject)
		return;
	pDynamicObject->setTrackLinePointPattern(true);
}

void myEarthSim::slotActionChangeScuOffset()
{
	Core::IObjectContainer objectContainer = CObjectManager::instance()->
		getDetectionTargetsByGroup(DynamicObject::CDynamicObjectGroupType::RADARTARGET);
	std::vector<std::string> keys;
	objectContainer.getKeys(keys);
	if(0 == keys.size())
		return;
	DynamicObject::CDynamicObject* pDynamicObject = 
		dynamic_cast<DynamicObject::CDynamicObject*>(objectContainer.getObject(keys.at(0)));
	if(NULL == pDynamicObject)
		return;
	std::string objectType = pDynamicObject->getType();
	osg::Vec3d pos = pDynamicObject->getModelCurrentPoint();
	this->mpGlobeWidget->flyTo(pos.x(), pos.y(),10000,2);
	osg::Vec2s offset = pDynamicObject->getScutcheonScreenCoordinatesOffset();
	offset += osg::Vec2s(100.0,100.0);
	pDynamicObject->setScutcheonScreenCoordinatesOffset(offset);
}

void myEarthSim::slotActionChangeObjectScuShadow()
{
	Core::IObjectContainer objectContainer = CObjectManager::instance()->
		getDetectionTargetsByGroup(DynamicObject::CDynamicObjectGroupType::RADARTARGET);
	std::vector<std::string> keys;
	objectContainer.getKeys(keys);
	if(0 == keys.size())
		return;
	DynamicObject::CDynamicObject* pDynamicObject = 
		dynamic_cast<DynamicObject::CDynamicObject*>(objectContainer.getObject(keys.at(0)));
	if(NULL == pDynamicObject)
		return;

	osgEarth::Symbology::TextSymbol* titleSymbol = pDynamicObject->getScutcheonTitleSymbol();
	osgEarth::Symbology::TextSymbol* contentSymbol = pDynamicObject->getScutcheonContentSymbol();

	static bool isChange = true;
	if(isChange)
	{
		titleSymbol->haloBackdropType() = osgText::Text::BackdropType::NONE;
		contentSymbol->haloBackdropType() = osgText::Text::BackdropType::NONE;
	}
	else
	{
		titleSymbol->haloBackdropType() = osgText::Text::BackdropType::OUTLINE;
		contentSymbol->haloBackdropType() = osgText::Text::BackdropType::OUTLINE;
	}
	isChange = !isChange;

	pDynamicObject->updateScutcheonStyle();
}

void myEarthSim::slotActionChangeScuShadow()
{
	static bool isChange = true;
	if(isChange)
	{
		CObjectManager::instance()->setScutcheonContentUseShadow(false);
		CObjectManager::instance()->setScutcheonTitleUseShadow(false);
	}
	else
	{
		CObjectManager::instance()->setScutcheonContentUseShadow(true);
		CObjectManager::instance()->setScutcheonTitleUseShadow(true);
	}
	isChange = !isChange;
}

void myEarthSim::slotActionJudgePointInCircle()
{
	osg::Vec3d currentPos(100,30,0.0);
	osg::Vec3d centerPos(99,29,0.0);
	bool result = DynamicObject::CCalculatorHelper::isPointInCircle(currentPos,centerPos,1000000,mpGlobeWidget->getMapRef()->getSRS());
}

void myEarthSim::slotActionJudgePointInGeometry()
{
	osg::Vec3d currentPos(100,30,0.0);
	std::vector<osg::Vec3d> geometry;
	geometry.push_back(osg::Vec3d(90,20,0.0));
	geometry.push_back(osg::Vec3d(90,40,0.0));
	geometry.push_back(osg::Vec3d(110,40,0.0));
	geometry.push_back(osg::Vec3d(110,20,0.0));
	bool result = DynamicObject::CCalculatorHelper::isPointInGeometry(currentPos,geometry,mpGlobeWidget->getMapRef()->getSRS());
}

void myEarthSim::slotActionEnableDragScu()
{
	static bool isChange = true;
	if(isChange)
		CObjectManager::instance()->setEnableDragScutcheon(true);
	else
		CObjectManager::instance()->setEnableDragScutcheon(false);
	isChange = !isChange;
}

void myEarthSim::slotActionVisibleIconText()
{
	static bool isChange = true;
	if(isChange)
		CObjectManager::instance()->setIconTextVisible(true);
	else
		CObjectManager::instance()->setIconTextVisible(false);
	isChange = !isChange;
}

void myEarthSim::slotActionVisibleObjectIconText()
{
	Core::IObjectContainer objectContainer = CObjectManager::instance()->
		getDetectionTargetsByGroup(DynamicObject::CDynamicObjectGroupType::RADARTARGET);
	std::vector<std::string> keys;
	objectContainer.getKeys(keys);
	if(0 == keys.size())
		return;
	DynamicObject::CDynamicObject* pDynamicObject = 
		dynamic_cast<DynamicObject::CDynamicObject*>(objectContainer.getObject(keys.at(0)));
	if(NULL == pDynamicObject)
		return;
	std::string objectType = pDynamicObject->getType();
	osg::Vec3d pos = pDynamicObject->getModelCurrentPoint();
	this->mpGlobeWidget->flyTo(pos.x(), pos.y(),10000,2);
	static bool isChange = true;
	if(isChange)
		pDynamicObject->setIconTextVisible(true);
	else
		pDynamicObject->setIconTextVisible(false);
	pDynamicObject->updateIconStyle();
	isChange = !isChange;
}

void myEarthSim::slotActionVisibleScuLine()
{
	static bool isChange = true;
	if(isChange)
		CObjectManager::instance()->setScuLineVisible(false);
	else
		CObjectManager::instance()->setScuLineVisible(true);
	isChange = !isChange;
}

void myEarthSim::slotActionChangeScuLineColor()
{
	static bool isChange = true;
	if(isChange)
		CObjectManager::instance()->setScuLineColor(osg::Vec4f(1,1,0,1));
	else
		CObjectManager::instance()->setScuLineColor(osg::Vec4f(1,0,0,1));
	isChange = !isChange;
}

void myEarthSim::slotActionChangeScuLineType()
{
	DynamicObject::LineType lineType = CObjectManager::instance()->getScuLineType();
	if(lineType == DynamicObject::DOTTED_LT)
		CObjectManager::instance()->setScuLineType(DynamicObject::SOLID_LT);
	else
		CObjectManager::instance()->setScuLineType(DynamicObject::DOTTED_LT);
}

void myEarthSim::slotActionChangeScuLineWidth()
{
	static bool isChange = true;
	if(isChange)
		CObjectManager::instance()->setScuLineWidth(2.0);
	else
		CObjectManager::instance()->setScuLineWidth(5.0);
	isChange = !isChange;
}

GraphicTypeDlg::GraphicTypeDlg(QWidget *parent , Qt::WFlags flags )
	: QDialog(parent,flags)
{
	this->ui.setupUi(this);

	QStringList graphicItems;
	/*graphicItems.push_back(Graphic::GT_ARC.c_str());
	graphicItems.push_back(Graphic::GT_DOUBLEARROW.c_str());*/
	/*graphicItems.push_back(Graphic::GT_MULTIPOINT.c_str());*/
	graphicItems.push_back(Graphic::GT_POINT.c_str());
	graphicItems.push_back(Graphic::GT_POLYLINE.c_str());
	graphicItems.push_back(Graphic::GT_POLYGON.c_str());
	graphicItems.push_back(Graphic::GT_CIRCLE.c_str());
	graphicItems.push_back(Graphic::GT_TRIANGLE.c_str());
	graphicItems.push_back(Graphic::GT_SECTOR.c_str());
	graphicItems.push_back(Graphic::GT_BOW.c_str());
	graphicItems.push_back(Graphic::GT_FREELINE.c_str());
	graphicItems.push_back(Graphic::GT_ELLIPSE.c_str());
	graphicItems.push_back(Graphic::GT_RECTANGLE.c_str());
	graphicItems.push_back(Graphic::GT_BEZIERARROW.c_str());
	graphicItems.push_back(Graphic::GT_DIRECTARROW.c_str());
	this->ui.comboBox->addItems(graphicItems);
}

GraphicTypeDlg::~GraphicTypeDlg()
{

}

QString GraphicTypeDlg::getGraphicType()
{
	return this->ui.comboBox->currentText();
}

TitleDlg::TitleDlg(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent,flags)
{
	this->ui.setupUi(this);
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	this->ui.fireSim->setAttribute(Qt::WA_TranslucentBackground, true);
	this->ui.dataSim->setAttribute(Qt::WA_TranslucentBackground, true);
	this->ui.analyse->setAttribute(Qt::WA_TranslucentBackground, true);
	this->ui.systemSetting->setAttribute(Qt::WA_TranslucentBackground, true);
	this->ui.exitSystem->setAttribute(Qt::WA_TranslucentBackground, true);
	this->startTimer(500);
}

TitleDlg::~TitleDlg()
{

}

void TitleDlg::timerEvent( QTimerEvent * event )
{
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString date = current_date_time.toString("yyyy.MM.dd");
	QString time = current_date_time.toString("hh:mm:ss");//ap
	QString d = current_date_time.toString("ddd");
	QStringList dateStr = date.split('.');
	date = dateStr.at(0) 
		+ QString::fromLocal8Bit("��") 
		+ dateStr.at(1) + QString::fromLocal8Bit("��")
		+ dateStr.at(2) + QString::fromLocal8Bit("��");
	this->ui.ny->setText(date);
	this->ui.xq->setText(d);
	this->ui.time->setText(time);
}

void TitleDlg::slotExit()
{
	this->close();
	this->Owner->close();
}

bool isDisplayPoint = false;
void TitleDlg::slotFireSim()
{
	if (isDisplayPoint == false)
	{
		this->Owner->slotActionCreatePoint();
		isDisplayPoint = true;
	}
	else
	{
		this->Owner->slotActionDeletePoint();
		isDisplayPoint = false;
	}
}


void TitleDlg::slotDataSim()
{
	this->Owner->slotAddDynamicObjects();
	this->Owner->slotActionChangeTrackLineColor();
	this->Owner->slotActionChangeTrackLineLength();
	this->Owner->slotActionControlTrackLineVisible();
}

bool isAnalyseOn = false;
void TitleDlg::slotAnalyse()
{
	if (isAnalyseOn == false)
	{
		this->Owner->slotBeginFlatDistance();
		isAnalyseOn = true;
	}
	else
	{
		this->Owner->slotEndFlatDistance();
		isAnalyseOn = false;
	}
}

bool isEditGeometryOn = false;
void TitleDlg::slotSetting()
{
	if (isEditGeometryOn == false)
	{
		this->Owner->slotActionBeginGraphicEdit();
		isEditGeometryOn = true;
	}
	else
	{
		this->Owner->slotActionEndGraphicEdit();
		isEditGeometryOn = false;
	}
}