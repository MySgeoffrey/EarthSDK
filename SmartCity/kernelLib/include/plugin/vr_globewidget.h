#ifndef VR_GLOBEWIDGET_H
#define VR_GLOBEWIDGET_H


#include <QtGui/QWidget>
#include <QtOpenGL/QGLWidget>
#include "vr_globewidgetconfig.h"
#include <osgViewer/CompositeViewer>
#include <osgViewer/viewer>  
#include <osgEarth/Viewpoint>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/Common>
#include <osgEarthUtil/GeodeticGraticule>
#include <osgEarth/Cache>
#include <osgEarthUtil/LatLongFormatter>
#include <osgEarthUtil/MouseCoordsTool>
#include <osgEarthUtil/EarthManipulator>
//#include "earthmanipulator.h"
#include <osgEarth\MapNode>
#include <osgEarthQt/ViewerWidget>


#define _OLD_GLOBEWIDGET_VALID 1

using namespace std;
class CRenderThread;
class CGlobeNavigation;

class VR_GLOBEWIDGET_DLL ViewerEx : public osgViewer::Viewer
{
public:
	ViewerEx();

	virtual ~ViewerEx();

	virtual void frame(double simulationTime = USE_REFERENCE_TIME);

	virtual void stopRender();

	virtual void startRender();

protected:
	bool mFrameState;
};

class VR_GLOBEWIDGET_DLL CMouseCoordsTool 
	: public osgEarth::Util::MouseCoordsTool
{

public:
	CMouseCoordsTool( osgEarth::MapNode* mapNode,
		osgEarth::Util::Controls::LabelControl* label =0L,
		osgEarth::Util::Formatter* formatter =0L );

	virtual ~CMouseCoordsTool();

	 void removedCallback( Callback* callback );
};
		

class VR_GLOBEWIDGET_DLL VRGlobeWidget
	: public osgEarth::QtGui::ViewerWidget
{
public :
	VRGlobeWidget(osgViewer::ViewerBase* viewer = NULL);

	~VRGlobeWidget();

	osg::ref_ptr<osg::Camera> getScreenCamera();

	bool initial(const bool& is3DMode, const QString& configPath);

	bool addEventHandler(osgGA::EventHandler* handler);

	bool removeEventHandler(osgGA::EventHandler* handler);

	bool insertEventHandlerFront(osgGA::GUIEventHandler *handler);

	osg::Vec3d getGeoPosition(osg::View* pView, const double& screenX,const double& screenY);

	void initialGlobe();

	osg::Vec3d getCameraPosition();
	
	void flyTo(const osgEarth::Viewpoint& viewPoint, double duration_s = 0);

	void flyTo(const double& longtitude,const double& latitude,const double& radius,double duration_s = 0);

	inline osgGA::CameraManipulator* getCameraManipulator()
	{
		return mpViewer->getCameraManipulator();
	}
	osg::ref_ptr<osgEarth::Util::EarthManipulator> getEarthManipulator()
	{
		return m_earthManip;
	}
	inline void setCameraManipulator(osgGA::CameraManipulator* camera)
	{
		mpViewer->setCameraManipulator(camera);
	}

	inline void resetCameraEathManipulator()
	{
		mpViewer->setCameraManipulator(m_earthManip);
	}

	inline osg::Camera* getCamera()
	{
		return mpViewer->getCamera();
	}

	osgEarth::Map* getMapRef();

	osg::Group* getRootRef();

	osgEarth::MapNode* getMapNodeRef();

	osg::Node* getSceneData();

	void runEarth();

	std::string getAxFullPath();

	void setUse3DCameraMode(const bool& use3DCameraMode);

	bool getUse3DCameraMode();

	void setCanPanMap(const bool& canPanMap);

	bool getCanPanMap();

	void stopRender();

	void restartRender();

	bool getNavigationVisible();

	void setNavigationVisible(const bool& visible);

	bool getEagleVisible();

	void setEagleVisible(const bool& visible);

	bool getStateInfoVisible();

	void setStateInfoVisible(const bool& visible);

	void setNavigationPosition(const int& x, const int& y);

	void getNavigationPosition(int& x, int& y);

	void setGlobeScalePosition(const int& x ,const int& y);

	void getGlobeScalePosition(int& x ,int& y);

	bool getGlobeScaleVisible();

	void setGlobeScaleVisible(const bool& visible);

	int getStateTextSize();

	void setStateTextSize(const int& size);

	CGlobeNavigation* getGlobeNavigation();

	osg::ref_ptr<CMouseCoordsTool> getMouseCoordsTool();

	virtual void resizeEvent(QResizeEvent * event);

	virtual void paintEvent(QPaintEvent * event);

	QPaintEngine * paintEngine() const;

	void timerEvent(QTimerEvent * event);

	void refresh();

	void getCameraHeight(
		double& cameraMinHeight,
		double& cameraMaxHeight);

	void setCameraHeight(
		const double& cameraMinHeight,
		const double& cameraMaxHeight);

	void getCameraLevel(
		double& cameraMinLevel,
		double& cameraMaxLevel);

	void setCameraLevel(
		const double& cameraMinLevel,
		const double& cameraMaxLevel);
protected:
	bool mInitialised;
	bool mUse3DCameraMode;
	bool mCanPanMap;
	QString m_axPath;
	osg::ref_ptr<osg::Camera> mpScreenCamera;
	osg::ref_ptr<osgEarth::Util::LabelControl> mpLabelControl;
	osg::ref_ptr<osgEarth::Util::EarthManipulator> m_earthManip;
	osg::ref_ptr<osgEarth::Map> mpMap;
	osg::ref_ptr<osgEarth::MapNode> mpMapNode;
	osg::ref_ptr<osg::Group> mpRoot;

	CRenderThread* mpRenderThread;
	osg::ref_ptr<CGlobeNavigation> mpGlobeNavigation;
	osg::ref_ptr<CMouseCoordsTool> mpMouseCoordsTool;

	bool mRenderThreadState;/*äÖÈ¾Ïß³Ì×´Ì¬£¬trueÎªäÖÈ¾×´Ì¬£¬falseÎªÍ£Ö¹×´Ì¬,Ä¬ÈÏÎªtrue*/
	osg::ref_ptr<ViewerEx> mpViewer;
	std::string m_glSupporName;
	std::string m_glRender;
	std::string m_glVersion;
	std::string m_gluVersion;
	bool mIs3DMode;
	QString mConfigPath;
	int mCameraMaxLevel;
	int mCameraMinLevel;
};

class VR_GLOBEWIDGET_DLL CVREarthManipulator //: public osgEarth::Util::VREarthManipulator
{
public:
	CVREarthManipulator()
	{
		GlobeWidget = NULL;
	}

	~CVREarthManipulator()
	{}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

public:
	VRGlobeWidget* GlobeWidget;
};
#endif // VR_GLOBEWIDGET_H
