#include "vr_globewidget.h"
#include <qpainter.h>
#include <qcoreevent.h>
#include <qevent.h>
#include <osg/Node>  
#include <osg/geode>  
#include <osg/group>  
#include <osgDB/readfile>  
#include <osgDB/writefile>  
#include <osgUtil\optimizer>  
#include <osgEarth\Map>
#include <osgEarth/URI>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgEarth/Viewpoint>
#include <QThread>
#include <osgViewer/ViewerEventHandlers>
#include <osgWidget/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgWidget/WindowManager>
#include <WinDef.h>
#include <qcoreapplication.h>
#include <osgEarthUtil/Sky>
#include <osg/Multisample>
#include <osgEarthUtil/FeatureQueryTool>
#include <osgEarthUtil/Controls>
#include <osgEarthFeatures/FeatureSource>
//#include <osgEarthAnnotation/Decluttering>
#include <osgDB/FileNameUtils>
#include <osgEarth/MapNode>
#include <osgEarth/ECEF>
#include <osgEarthUtil/AnnotationEvents>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/ExampleResources>
#include <osgEarthAnnotation/AnnotationEditing>
#include <osgEarthAnnotation/AnnotationRegistry>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/RectangleNode>
#include <osgEarthAnnotation/EllipseNode>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/LocalGeometryNode>
#include <osgEarthAnnotation/FeatureNode>
//#include <osgEarthAnnotation/Decluttering>
#include <osgEarthAnnotation/HighlightDecoration>
#include <osgEarthAnnotation/ScaleDecoration>

#include <osgEarthSymbology/GeometryFactory>

#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgGA/EventVisitor>
#include <osgDB/WriteFile>
#include <osgEarth/Picker>  
#include <QtGui/QMessageBox>
#include "framework/common/globesetting.h"
#include "globenavigation.h"
#include <osg/Notify>
#include <osgViewer/CompositeViewer>
#include <osgViewer/Viewer>
#include <osgEarthUtil/ExampleResources>
#include <osgEarthQt/ViewerWidget>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <iomanip>
#include "commontool.h"
#include <osgEarthDrivers/sky_simple/SimpleSkyOptions>

using namespace std;
using namespace osgEarth::Util;
using namespace osgEarth::Util::Controls;
using namespace osgEarth::Annotation;
using namespace osgEarth;
using namespace osgEarth::Util;
using namespace osgEarth::QtGui;

const unsigned int MASK_2D = 0xF0000000;

#define _USE_MESSAGE_INFO 0



#define _USE_PARENT_WINDOW_ 1

//osg::Camera* CreateHUDCamera()
//{
//	//osg::ref_ptr<osg::Camera> camera = new osg::Camera;
//	//camera->setProjectionResizePolicy(osg::Camera::FIXED);
//	//camera->getOrCreateStateSet()->setMode(
//	//	GL_LIGHTING,
//	//	osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF
//	//	);
//	////二维相机，设置观察矩阵始终是一个单位矩阵
//	//camera->setViewMatrix(osg::Matrix::identity());
//	////最后渲染
//	//camera->setRenderOrder(osg::Camera::POST_RENDER);
//	////在其他相机的基础上渲染，清楚缓存
//	//camera->setClearMask(GL_DEPTH_BUFFER_BIT);
//	//////不接受事件
//	////camera->setAllowEventFocus(false);
//	////始终显示，不受其他节点影响
//	////设置参考帧
//	//camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
//	////设置视口
//	//camera->setProjectionMatrixAsOrtho2D(0,1366,0,768);
//	//return camera.release();
//
//	const static int width = 1280, height = 1024;
//	osg::Camera* camera = new osg::Camera();
//
//	camera->getOrCreateStateSet()->setMode(
//		GL_LIGHTING,
//		osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF
//		);
//
//	osg::Matrix m = osg::Matrix::ortho2D(0.0f, width, 0.0f, height);
//
//	camera->setProjectionMatrix(m);
//	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
//	camera->setViewMatrix(osg::Matrix::identity());
//	camera->setClearMask(GL_DEPTH_BUFFER_BIT); 
//	camera->setRenderOrder(osg::Camera::POST_RENDER);
//
//	return camera;
//}

osg::Camera* createOrthoCamera(double width, double height)
{
	osg::Camera* camera = new osg::Camera();

	camera->getOrCreateStateSet()->setMode(
		GL_LIGHTING,
		osg::StateAttribute::PROTECTED | osg::StateAttribute::OFF
		);

	osg::Matrix m = osg::Matrix::ortho2D(0.0f, width, 0.0f, height);

	camera->setProjectionMatrix(m);
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrix(osg::Matrix::identity());
	camera->setClearMask(GL_DEPTH_BUFFER_BIT); 
	camera->setRenderOrder(osg::Camera::POST_RENDER);

	return camera;
}


CMouseCoordsTool::CMouseCoordsTool( osgEarth::MapNode* mapNode,
	osgEarth::Util::Controls::LabelControl* label,
	osgEarth::Util::Formatter* formatter )
	:osgEarth::Util::MouseCoordsTool(mapNode,label,formatter)
{

}

CMouseCoordsTool::~CMouseCoordsTool()
{

}

void CMouseCoordsTool::removedCallback( Callback* callback )
{
	if (!this->_callbacks.empty())
	{
		Callbacks::iterator bItr = this->_callbacks.begin();
		Callbacks::iterator eItr = this->_callbacks.end();
		while (bItr != eItr)
		{
			if (*bItr == callback)
			{
				this->_callbacks.erase(bItr);
				break;
			}
			++bItr;
		}
	}
}


ViewerEx::ViewerEx()
{
	this->mFrameState = true;
	//osg::DisplaySettings::instance()->setNumMultiSamples(16);
	osg::ref_ptr< osg::DisplaySettings > displaySettings = new osg::DisplaySettings;

	displaySettings->setNumMultiSamples(16);

	this->setDisplaySettings( displaySettings.get());
}

ViewerEx::~ViewerEx()
{

}

void ViewerEx::stopRender()
{
	this->mFrameState = false;
}

void ViewerEx::startRender()
{
	this->mFrameState = true;
}

void ViewerEx::frame(double simulationTime)
{
	if (_done) return;

	// OSG_NOTICE<<std::endl<<"CompositeViewer::frame()"<<std::endl<<std::endl;

	if (_firstFrame)
	{
		viewerInit();

		if (!isRealized())
		{
			realize();
		}

		_firstFrame = false;
	}

	if (this->mFrameState)
	{
		advance(simulationTime);
		eventTraversal();
		updateTraversal();
		renderingTraversals();
	}
}

VRGlobeWidget::VRGlobeWidget(osgViewer::ViewerBase* viewer)
	:osgEarth::QtGui::ViewerWidget(new ViewerEx())
{
	this->mInitialised = false;
	this->mRenderThreadState = true;
	this->mpGlobeNavigation = NULL;
	this->mpRoot = NULL;
	this->m_earthManip = NULL;
	this->mpRoot = new osg::Group;
	mIs3DMode = true;
	this->mConfigPath = "";
	this->mpMouseCoordsTool = NULL;
	this->mUse3DCameraMode = true;
	this->mCanPanMap = true;
	this->mCameraMaxLevel = 22;
	this->mCameraMinLevel = 0;
	this->mpScreenCamera = NULL;
	/*setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_PaintOnScreen);
	setAutoFillBackground(true);*/
	//this->startTimer(2);
}

VRGlobeWidget::~VRGlobeWidget()
{

}

osg::ref_ptr<osg::Camera> VRGlobeWidget::getScreenCamera()
{
	return this->mpScreenCamera;
}

bool VRGlobeWidget::initial(const bool& is3DMode, const QString& configPath)
{
	bool r = false;
	this->mIs3DMode = is3DMode;
	this->mConfigPath = configPath;
	this->initialGlobe();

	this->runEarth();
	setTimerInterval(1);
	return r;
}

bool VRGlobeWidget::addEventHandler(osgGA::EventHandler* handler)
{
	if (handler && this->mpViewer.valid())
	{
		this->mpViewer->addEventHandler(handler);
		return true;
	}
	return false;
}

bool VRGlobeWidget::removeEventHandler(osgGA::EventHandler* handler)
{
	if (handler && this->mpViewer.valid())
	{
		this->mpViewer->removeEventHandler(handler);
		return true;
	}
	return false;
}

osg::Vec3d VRGlobeWidget::getGeoPosition(
	osg::View* pView,
	const double& screenX,const double& screenY)
{
	osg::Vec3d geoPosition(-1,-1,0);
	if (pView == NULL)
		return geoPosition;
	osg::Vec3d world;
	if ( this->getMapNodeRef()->getTerrain()->getWorldCoordsUnderMouse(
		pView, (float)screenX, (float)screenY, world) )
	{
		GeoPoint map;
		map.fromWorld( this->getMapNodeRef()->getMapSRS(), world );
		geoPosition = map.vec3d();
	}
	geoPosition.z() = 0;
	return geoPosition;
}

bool VRGlobeWidget::insertEventHandlerFront(osgGA::GUIEventHandler *handler)
{
	if (handler && this->mpViewer.valid())
	{
		this->mpViewer->getEventHandlers().push_front(handler);
		return true;
	}
	return false;
}

void VRGlobeWidget::initialGlobe()
{
	osgViewer::Viewer* pViewer =
		(osgViewer::Viewer*)(this->getViewer());
	this->mpViewer = dynamic_cast<ViewerEx*>(pViewer);
	pViewer->setRunFrameScheme(pViewer->CONTINUOUS);
	pViewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);	
	
	m_earthManip = new osgEarth::Util::EarthManipulator;
	//m_earthManip->GlobeWidget = this;
	pViewer->setCameraManipulator(m_earthManip);//必须在setViewpoint之前
	pViewer->addEventHandler(new osgGA::StateSetManipulator(pViewer->getCamera()->getOrCreateStateSet()));//显示网格
	//pViewer->addEventHandler(new osgViewer::StatsHandler);//状态信息
	//pViewer->addEventHandler(new osgViewer::WindowSizeHandler);///全屏，有问题
	osgEarth::Viewpoint vp("", 0, 0, 10000.0, -0, -90.0, 1.5e4);
	
	m_earthManip->setViewpoint(vp, 0);
	osgEarth::Viewpoint vp2("", 107.85, 32.35, 0.0, -0, -90.0, 1.5e7);
	m_earthManip->setHomeViewpoint(vp2, 2.0);
	m_earthManip->getSettings()->setScrollSensitivity(1.6);
	m_earthManip->getSettings()->setThrowingEnabled(true);
	//m_earthManip->getSettings()->setThrowDecayRate(m_earthManip->getSettings()->getThrowDecayRate() / 1.5);
	if (this->mIs3DMode)
	{
		m_earthManip->getSettings()->setArcViewpointTransitions(true);
		if (this->getCanPanMap())
		{
			m_earthManip->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_EARTH_DRAG,
			osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON);
		}
	
		this->setUse3DCameraMode(this->mUse3DCameraMode);
	
		m_earthManip->getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_ZOOM_OUT,
			osgGA::GUIEventAdapter::SCROLL_DOWN);
		m_earthManip->getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_ZOOM_IN,
			osgGA::GUIEventAdapter::SCROLL_UP);
		//m_earthManip->getSettings()->bindMouseDoubleClick(osgEarth::Util::EarthManipulator::ACTION_NULL,//GOTO
		//	osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON);
	}
	else
	{
		m_earthManip->getSettings()->setArcViewpointTransitions(false);
		/*m_earthManip->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_EARTH_DRAG,
			osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON);*/
		m_earthManip->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_NULL,
			osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON);
		m_earthManip->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_NULL,
			osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON);
	}

	m_earthManip->getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_ZOOM_IN,
		osgGA::GUIEventAdapter::SCROLL_UP, EarthManipulator::OPTION_DURATION);
	m_earthManip->getSettings()->bindScroll(osgEarth::Util::EarthManipulator::ACTION_ZOOM_OUT,
		osgGA::GUIEventAdapter::SCROLL_DOWN, EarthManipulator::OPTION_DURATION);
}

osg::Vec3d VRGlobeWidget::getCameraPosition()
{
	osg::Vec3d eye, up, center;
	((osgViewer::Viewer*)(this->getViewer()))->getCamera()->getViewMatrixAsLookAt(eye, center, up);
	double lon = 0.0, lat = 0.0,alt = 0.0;
	const osg::EllipsoidModel* pellipsoidmodel = this->getMapNodeRef()->getMapSRS()->getEllipsoid();
	pellipsoidmodel->convertXYZToLatLongHeight(eye.x(), eye.y(), eye.z(),
		lat, lon, alt);
	lon = osg::RadiansToDegrees(lon);
	lat = osg::RadiansToDegrees(lat);
	return osg::Vec3d(lon,lat,alt);
}

void VRGlobeWidget::flyTo(const osgEarth::Viewpoint& viewPoint, double duration_s)
{
	osgEarth::Util::EarthManipulator* pCameraManipulator = dynamic_cast<osgEarth::Util::EarthManipulator*>(this->mpViewer->getCameraManipulator());
	if (pCameraManipulator)
	{
		pCameraManipulator->setViewpoint(viewPoint, duration_s);
	}
}

void VRGlobeWidget::flyTo(const double& longtitude,const double& latitude,const double& radius, double duration_s)
{
	osgEarth::Util::EarthManipulator* pCameraManipulator = dynamic_cast<osgEarth::Util::EarthManipulator*>(this->mpViewer->getCameraManipulator());
	if (pCameraManipulator)
	{
		double halfAngle = 45 * 3.1415926 / 180;
		double height = radius / std::tan(halfAngle);
		pCameraManipulator->setViewpoint(osgEarth::Viewpoint("", longtitude, latitude, height, 0, -90, radius), duration_s);
	}
}

osgEarth::Map* VRGlobeWidget::getMapRef()
{
	return this->mpMap;
}

osg::Group* VRGlobeWidget::getRootRef()
{
	return this->mpRoot;
}

osgEarth::MapNode* VRGlobeWidget::getMapNodeRef()
{
	return this->mpMapNode;
}

osg::Node* VRGlobeWidget::getSceneData()
{
	return this->mpViewer->getSceneData();
}

void VRGlobeWidget::setUse3DCameraMode(const bool& use3DCameraMode)
{
	this->mUse3DCameraMode = use3DCameraMode;
	if (this->getUse3DCameraMode())
	{
		m_earthManip->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_ROTATE,
			osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON);
		m_earthManip->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_NULL,
			osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON);
	}
	else
	{
		m_earthManip->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_NULL,
			osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON);
		m_earthManip->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_NULL,
			osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON);
		m_earthManip->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_NULL,
			osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON | osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON);
		m_earthManip->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_PAN,
			osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON);
	}
}

bool VRGlobeWidget::getUse3DCameraMode()
{
	return this->mUse3DCameraMode;
}

void VRGlobeWidget::setCanPanMap(const bool& canPanMap)
{
	this->mCanPanMap = canPanMap;
	if (this->getCanPanMap())
	{
		m_earthManip->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_EARTH_DRAG,
		osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON);
	}
	else
	{
		m_earthManip->getSettings()->bindMouse(osgEarth::Util::EarthManipulator::ACTION_NULL,
		osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON);
	}
}

bool VRGlobeWidget::getCanPanMap()
{
	return this->mCanPanMap;
}

void VRGlobeWidget::stopRender()
{
	this->mpViewer->stopRender();
}

void VRGlobeWidget::restartRender()
{
	this->mpViewer->startRender();

}

bool VRGlobeWidget::getNavigationVisible()
{
	return this->mpGlobeNavigation->getNavigationVisible();
}

void VRGlobeWidget::setNavigationVisible(const bool& visible)
{
	this->mpGlobeNavigation->setNavigationVisible(visible);
}

bool VRGlobeWidget::getStateInfoVisible()
{
	return this->mpGlobeNavigation->getStateInfoVisible();
}

void VRGlobeWidget::setStateInfoVisible(const bool& visible)
{
	this->mpGlobeNavigation->setStateInfoVisible(visible);
}

void VRGlobeWidget::setNavigationPosition(const int& x, const int& y)
{
	if (this->mpGlobeNavigation)
	{
		this->mpGlobeNavigation->setNavigationPosition(x, y);
	}
}

void VRGlobeWidget::getNavigationPosition(int& x, int& y)
{
	if (this->mpGlobeNavigation)
	{
		this->mpGlobeNavigation->getNavigationPosition(x, y);
	}
}


void VRGlobeWidget::setGlobeScalePosition(const int& x ,const int& y)
{
	if (this->mpGlobeNavigation)
	{
		this->mpGlobeNavigation->setGlobeScalePosition(x,y);
	}
}

void VRGlobeWidget::getGlobeScalePosition(int& x ,int& y)
{
	if (this->mpGlobeNavigation)
	{
		this->mpGlobeNavigation->getGlobeScalePosition(x,y);
	}
}

bool VRGlobeWidget::getGlobeScaleVisible()
{
	if (this->mpGlobeNavigation)
	{
		return this->mpGlobeNavigation->getGlobeScaleVisible();
	}
	return false;
}

void VRGlobeWidget::setGlobeScaleVisible(const bool& visible)
{
	if (this->mpGlobeNavigation)
	{
		this->mpGlobeNavigation->setGlobeScaleVisible(visible);
	}
}

int VRGlobeWidget::getStateTextSize()
{
	if (this->mpGlobeNavigation)
	{
		return this->mpGlobeNavigation->getStateTextSize();
	}
	return 0;
}

void VRGlobeWidget::setStateTextSize(const int& size)
{
	if (this->mpGlobeNavigation)
	{
		return this->mpGlobeNavigation->setStateTextSize(size);
	}
}

CGlobeNavigation* VRGlobeWidget::getGlobeNavigation()
{
	return this->mpGlobeNavigation;
}

bool VRGlobeWidget::getEagleVisible()
{
	return this->mpGlobeNavigation->getEagleVisible();
}


void VRGlobeWidget::setEagleVisible(const bool& visible)
{
	this->mpGlobeNavigation->setEagleVisible(visible);
}

void VRGlobeWidget::timerEvent(QTimerEvent * event)
{
	/*if (this->mInitialised)
	{
		this->getViewer()->frame();
	}*/
}

void VRGlobeWidget::getCameraHeight(
	double& cameraMinHeight,
	double& cameraMaxHeight)
{
	if (this->m_earthManip)
	{
		cameraMaxHeight = m_earthManip->getSettings()->getMaxDistance();
		cameraMinHeight = m_earthManip->getSettings()->getMinDistance();
	}
}

void VRGlobeWidget::setCameraHeight(
	const double& cameraMinHeight,
	const double& cameraMaxHeight)
{
	if (this->m_earthManip)
	{
		m_earthManip->getSettings()->setMinMaxDistance(cameraMinHeight,cameraMaxHeight);
	}
}

void VRGlobeWidget::getCameraLevel(
	double& cameraMinLevel,
	double& cameraMaxLevel)
{
	cameraMinLevel = this->mCameraMinLevel;
	cameraMaxLevel = this->mCameraMaxLevel;
}

void VRGlobeWidget::setCameraLevel(
	const double& cameraMinLevel,
	const double& cameraMaxLevel)
{
	this->mCameraMinLevel = cameraMinLevel;
	this->mCameraMaxLevel = cameraMaxLevel;
	static std::map<int,double> levelData;
	if (!levelData.size())
	{
#if 0
		levelData[0] = 20000000;
		levelData[1] = 10000000;
		levelData[2] = 5000000;
		levelData[3] = 3000000;
		levelData[4] = 2000000;
		levelData[5] = 1000000;
		levelData[6] = 500000;
		levelData[7] = 200000;
		levelData[8] = 100000;
		levelData[9] = 50000;
		levelData[10] = 20000;
		levelData[11] = 10000;
		levelData[12] = 5000;
		levelData[13] = 3000;
		levelData[14] = 2000;
		levelData[15] = 1000;
		levelData[16] = 500;
		levelData[17] = 200;
		levelData[18] = 100;
		levelData[19] = 50;
		levelData[20] = 20;
#else
		levelData[0] = 50000000;
		levelData[1] = 30000000;
		levelData[2] = 20000000;
		levelData[3] = 10000000;
		levelData[4] = 5000000;
		levelData[5] = 3000000;
		levelData[6] = 2000000;
		levelData[7] = 1000000;
		levelData[8] = 500000;
		levelData[9] = 200000;
		levelData[10] = 100000;
		levelData[11] = 50000;
		levelData[12] = 20000;
		levelData[13] = 10000;
		levelData[14] = 5000;
		levelData[15] = 3000;
		levelData[16] = 2000;
		levelData[17] = 1000;
		levelData[18] = 500;
		levelData[19] = 200;
		levelData[20] = 100;
		levelData[21] = 50;
		levelData[22] = 0;
#endif
		double maxCameraHeight = levelData[this->mCameraMinLevel];
		double minCameraHeight = levelData[this->mCameraMaxLevel];
		this->setCameraHeight(minCameraHeight,maxCameraHeight);
	}
}

void VRGlobeWidget::refresh()
{
	osgViewer::Viewer* pViewer =
		dynamic_cast<osgViewer::Viewer*>(this->getViewer());
	if (pViewer)
	{
		pViewer->requestRedraw();
		pViewer->frame();
	}
}

void VRGlobeWidget::paintEvent(QPaintEvent * event)
{
	/*if (this->getViewer())
	{
		this->getViewer()->frame();
	}*/
	osgEarth::QtGui::ViewerWidget::paintEvent(event);
	//event->accept();
}

QPaintEngine * VRGlobeWidget::paintEngine() const
{
	return osgEarth::QtGui::ViewerWidget::paintEngine();
	//return NULL;
}

void VRGlobeWidget::resizeEvent(QResizeEvent * event)
{
	osgEarth::QtGui::ViewerWidget::resizeEvent(event);
	if (this->mInitialised)
	{
		this->mpGlobeNavigation->resize(this->width(), this->height());
		if (this->mpScreenCamera)
		{
			//this->mpScreenCamera->resize(this->width(), this->height());
			//this->mpScreenCamera->setViewport(this->x(),this->y(),this->width(),this->height());
		}
		this->getViewer()->frame();
	}
}

osg::ref_ptr<CMouseCoordsTool> VRGlobeWidget::getMouseCoordsTool()
{
	return this->mpMouseCoordsTool;
}

std::string VRGlobeWidget::getAxFullPath()
{
	// 使用Windows API 获取到ActiveX控件在磁盘上的绝对路径
	CHAR ch[MAX_PATH];
	CHAR longPath[MAX_PATH];
#if QT_NO_DEBUG
	GetModuleFileNameA(::GetModuleHandleA("VR_GlobeWidget.dll"), ch, MAX_PATH);
#else
	GetModuleFileNameA(::GetModuleHandleA("VR_GlobeWidgetd.dll"), ch, MAX_PATH);
#endif
	GetLongPathNameA(ch, longPath, MAX_PATH);

	return osgDB::getFilePath(longPath);
}

void VRGlobeWidget::runEarth()
{
	QString curRunPath = "";
	if (this->mConfigPath != "")
	{
		curRunPath = this->mConfigPath;
	}
	else
	{

#if 0
		curRunPath = QCoreApplication::applicationDirPath();
#else
		string strpath = getAxFullPath();
		curRunPath = QString::fromLocal8Bit(strpath.c_str());
#endif
		QString dataServiceConfigPath = curRunPath;
		curRunPath += "\\VR-GlobeConfig.earth";
	}
	if (QFile::exists(curRunPath))
	{
		osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(curRunPath.toLocal8Bit().constData());
		this->mpRoot->addChild(node);
		this->mpViewer->setSceneData(this->mpRoot);
		this->mpMapNode = osgEarth::MapNode::findMapNode(node.get());
		this->mpMap = this->mpMapNode->getMap();
		m_earthManip->setNode(mpMapNode);
#if 1
		osg::Multisample* pms=new osg::Multisample;
		pms->setSampleCoverage(5,true);
		mpRoot->getOrCreateStateSet()->setAttributeAndModes(pms,osg::StateAttribute::ON);
		//缓存

		//pMap->getCache()->apply(&cacheOptions);
		////近地面自动裁剪AutoClipPlaneCullCallback
		this->getCamera()->addCullCallback( new osgEarth::Util::AutoClipPlaneCullCallback(this->mpMapNode));

#if 0
		/////解决镜头拉近地面，地面穿透的问题
		this->mpViewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);
		//this->getCamera()->setNearFarRatio(0.00001f);//设置阻尼1.0为全看不见及全阻
		this->mpViewer->getCamera()->setNearFarRatio(0.00000001f);//设置阻尼1.0为全看不见及全阻
		this->mpViewer->getCamera()->setSmallFeatureCullingPixelSize(-1.0f);
#else
		//ChenPeng20181013
		/////解决镜头拉近地面，地面穿透的问题
		this->mpViewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);
		this->mpViewer->getCamera()->setNearFarRatio(0.0000000001f);//设置阻尼1.0为全看不见及全阻//0.00000001f
		/*this->mpViewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);
		this->mpViewer->getCamera()->setNearFarRatio(0.0000000001f);*///设置阻尼1.0为全看不见及全阻//0.00000001f
		m_earthManip->getSettings()->setMinMaxDistance(10.0,m_earthManip->getSettings()->getMaxDistance());
		this->mpViewer->getCamera()->setSmallFeatureCullingPixelSize(-1.0f);
#endif

		//this->mpViewer->getDatabasePager()->setUpThreads(10, 8);
		//////////////////////////////////////////////////////////////////////////
		//关闭光照
		this->mpMapNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
		
		//osg::DisplaySettings::instance()->setNumMultiSamples(4);
		/*osg::ref_ptr< osg::DisplaySettings > displaySettings = new osg::DisplaySettings;
		displaySettings->setNumMultiSamples(16);
		this->mpViewer->setDisplaySettings( displaySettings.get());*/

		//osg::ref_ptr<osgEarth::Util::SkyNode> pSkyNode = osgEarth::Util::SkyNode::create(this->mpMapNode);
		osgEarth::SimpleSky::SimpleSkyOptions options;
		options.setDriver("simple");
		//options.setDriver("gl");
		options.atmosphericLighting() = true;
		osg::ref_ptr<osgEarth::Util::SkyNode> pSkyNode = osgEarth::Util::SkyNode::create(options,this->mpMapNode);

		//pSkyNode->setDateTime(osgEarth::DateTime(2015, 12, 12, 8));
		osgEarth::DateTime t;
		t = osgEarth::DateTime("2018-7-7 05:00:00");
		pSkyNode->setDateTime(t);
		pSkyNode->setName("mySkyNode");
		//this->mpRoot->addChild(pSkyNode);
		pSkyNode->setMoonVisible(false);
		pSkyNode->attach(this->mpViewer);
		pSkyNode->setSunVisible(true);
		/*pSkyNode->setMinimumAmbient(osg::Vec4f(0.3, 0.3, 0.3, 1.0));*/
		pSkyNode->setMinimumAmbient(osg::Vec4f(0.9, 0.9, 0.9, 1.0));
		osgEarth::insertGroup(pSkyNode.get(),this->mpMapNode);
		//pSkyNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

		//QString editTimeStr = "2017-08-15 12:36:04";
		//QString tempHour = editTimeStr.mid(11, 2);
		//QString tempDay = editTimeStr.mid(9, 2);
		//QString tempMonth = editTimeStr.mid(6, 2);
		//QString tempYear = editTimeStr.left(4);
		//pSkyNode->setDateTime(osgEarth::DateTime(tempYear.toInt(), tempMonth.toInt(), tempDay.toInt(), 20 - 8));
#endif
		this->mpMouseCoordsTool = new CMouseCoordsTool(this->getMapNodeRef());
		this->addEventHandler(this->mpMouseCoordsTool);

		if (this->mpScreenCamera == NULL)
		{
			mpScreenCamera = createOrthoCamera(1280 * 2,1024 * 2);//CreateHUDCamera();
			mpScreenCamera->setViewport(0,0,1280 * 2,1024  * 2);
			this->getRootRef()->addChild(mpScreenCamera);
		}
		osg::Camera* pCamera = this->mpScreenCamera;
		this->mpGlobeNavigation = new CGlobeNavigation(this);
		this->mpGlobeNavigation->inital(pCamera);
		mInitialised = true;

	}
}



bool CVREarthManipulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	bool r = false;//
	//if (this->GlobeWidget == NULL)
	//	return r;
	osgGA::GUIEventAdapter::EventType type = ea.getEventType();
	switch (type)
	{
		case osgGA::GUIEventAdapter::SCROLL:
		{
			/*osgViewer::View* view = static_cast<osgViewer::View*>(aa.asView());
			osg::Vec3d eye, up, center;
			view->getCamera()->getViewMatrixAsLookAt(eye, center, up);
			double distanceToGround = 0;
			double xxlon = 0.0, yylan = 0.0;
			const osg::EllipsoidModel* pEllipsoidModel = 
				this->GlobeWidget->getMapNodeRef()->getMapSRS()->getEllipsoid();
			pEllipsoidModel->convertXYZToLatLongHeight(eye.x(), eye.y(), eye.z(),
				yylan, xxlon, distanceToGround);*/
			

			//osg::Vec3d pos = osg::Vec3d(0, 0, 0);
			//// 获取当前点
			//osgUtil::LineSegmentIntersector::Intersections intersection;
			//double x = ea.getX();
			//double y = ea.getY();
			//view->computeIntersections(ea.getX(), ea.getY(), intersection);
			//osgUtil::LineSegmentIntersector::Intersections::iterator iter
			//	= intersection.begin();
			//if (iter != intersection.end())
			//{
			//	this->_mapNode->getMapSRS()->getGeodeticSRS()->getEllipsoid()->convertXYZToLatLongHeight(
			//		iter->getWorldIntersectPoint().x(), iter->getWorldIntersectPoint().y(), iter->getWorldIntersectPoint().z(),
			//		pos.y(), pos.x(), pos.z());
			//	pos.x() = osg::RadiansToDegrees(pos.x());
			//	pos.y() = osg::RadiansToDegrees(pos.y());
			//	osgEarth::Viewpoint viewPoint("", pos.x(), pos.y(), pos.z(), -0, -90.0, 1.5e7);
			//	this->setViewpoint(viewPoint, 2);
			//}
			//break;
		}
	}
	return false;
	//return osgEarth::Util::EarthManipulator::handle(ea,aa);
}

