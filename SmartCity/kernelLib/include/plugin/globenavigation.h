#ifndef _GLOBENAVIGATION_H
#define _GLOBENAVIGATION_H
#include <osg/Node>  
#include <osg/geode>  
#include <osg\group>  
#include <osgDB/readfile>  
#include <osgDB/writefile>  
#include <osgUtil\optimizer>  
#include <osgEarth\Map>
#include <osgEarth/URI>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgEarth/Viewpoint>
#include <osgViewer/ViewerEventHandlers>
#include <osgWidget/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgWidget/WindowManager>
#include <WinDef.h>
#include <qcoreapplication.h>
#include <osgEarthUtil/FeatureQueryTool>
#include <osgEarthUtil/Controls>
#include <osgEarthFeatures/FeatureSource>
#include <osgDB/FileNameUtils>
#include <osgEarth/MapNode>
#include <osgEarth/ECEF>
#include <osgEarthUtil/EarthManipulator>
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
#include <osgEarthAnnotation/HighlightDecoration>
#include <osgEarthAnnotation/ScaleDecoration>
#include <osgEarthSymbology/GeometryFactory>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgGA/EventVisitor>
#include <osgDB/WriteFile>
#include "vr_globewidget.h"
#include "osgGA/GUIEventHandler"


using namespace osgEarth::Util;
using namespace osgEarth::Util::Controls;
class MouseCoordsCallback;
class CCameraCallBack;



class MouseCoordsCallback;

class VR_GLOBEWIDGET_DLL CGlobeNavigation
	: public osgGA::GUIEventHandler
{
public:

	CGlobeNavigation(VRGlobeWidget* pRefGlobeWidgetEx);

	~CGlobeNavigation();
	
	void setStateTextMode(const bool& longitudeVisible,const bool& lantitudeVisible,
		const bool& terrainAltitudeVisible,const bool& viewAltitudeVisible);

	void getStateTextMode(bool& longitudeVisible,bool& lantitudeVisible,
		bool& terrainAltitudeVisible,bool& viewAltitudeVisible);

	bool getNavigationVisible();

	void setNavigationVisible(const bool& visible);

	bool getStateInfoVisible();

	void setStateInfoVisible(const bool& visible);

	bool getEagleVisible();

	void setEagleVisible(const bool& visible);

	void setNavigationPosition(const int& x ,const int& y);

	void getNavigationPosition(int& x ,int& y);

	void setGlobeScalePosition(const int& x ,const int& y);

	void getGlobeScalePosition(int& x ,int& y);

	bool getGlobeScaleVisible();

	void setGlobeScaleVisible(const bool& visible);

	int getStateTextSize();

	void setStateTextSize(const int& size);

	//获取状态栏的文字对象
	osgText::Text* getStateLabelText();

	//获取比例尺的文字对象
	osgText::Text* getGlobeScaleText();

	//获取比例尺距离的文字对象
	osgText::Text* getScaleDisText();

	void inital(osg::Camera*& pScreenCamera);

	void resize(const int& width,const int& height);

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

	double getCurrentAltitude();

	int getPrecision();

	void setPrecision(const int& precision);
//public:
	//void setEagleEyeVisible(bool bVisible);
protected:
	bool contains(ImageControl* pControl, int x, int y);

	bool contains(ImageControl* pControl, float dy, int x, int y);

	osg::Camera* createOrthoCamera(double width, double height);

	//void createEagleCamera();

	void textInfo(osgText::Text* text);

	osgText::Text* createLabel(const std::string& l, const char* f, unsigned int size);

	void updateCompass();

	void updateGlobeScaleInfo();
protected:
	osgText::Text* mStateLabelText;
	osgText::Text* mGlobeScaleText;
	osgText::Text* mScaleDisText;
	osg::Camera* mLabelCamera;
	//osg::ref_ptr<osg::Camera> mRadarCamera;
	Grid* mStateInfoControl;
	VRGlobeWidget* mpRefGlobeWidgetEx;
	ImageControl* mCompassControl;
	ImageControl* mLevelControl;
	ImageControl* mLevelSelectControl;
	ImageControl* mZoomInLevelControl;
	ImageControl* mZoomOutLevelControl;
	ImageControl* mEyeDisableControl;
	ImageControl* mEyeViewControl;
	ImageControl* mEagleImageControl;
	ImageControl* mEaglePositionControl;
	ImageControl* mGlobeScaleControl;
	

	ImageControl* mCameraMoveUpControl;
	ImageControl* mCameraMoveDownControl;
	ImageControl* mCameraMoveLeftControl;
	ImageControl* mCameraMoveRightControl;
	bool mHasInitialised;
	bool mCanSelectLevel;
	bool mNavigationVisible;
	bool mStateInfoVisible;
	bool mEagleVisible;
	int mMouseDownX;
	int mMouseDownY;
	float mLevelButtonX;
	float mLevelButtonY;
	int mMouseButton;
	double mX;
	double mY;
	double mGlobeScalePositionX;
	double mGlobeScalePositionY;
	bool mGlobeScaleVisible;
	bool mLongitudeVisible;
	bool mLantitudeVisible;
	bool mTerrainAltitudeVisible;
	bool mViewAltitudeVisible;
	int mPrecision;
	MouseCoordsCallback* mMouseCoordsCallback;
	CCameraCallBack* mCameraCallBack;
	friend class MouseCoordsCallback;
	friend class CCameraCallBack;
};

#endif //_GLOBENAVIGATION_H