#ifndef _COMMONTOOL_H
#define _COMMONTOOL_H

#include <osgEarth/SpatialReference>
#include <osgViewer/viewer>  
#include <osg/Node>  
#include <osg/geode>  
#include <osg\group>  
#include <osgDB/readfile>  
#include <osgDB/writefile>  
#include <osgUtil\optimizer>  
#include <osgEarth\Map>
#include <osgEarth\MapNode>
#include <osgEarth/URI>
#include <osgViewer/api/win32/GraphicsWindowWin32>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarth/Viewpoint>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgText/Text>
#include <osgEarth/Registry>
#include <osgEarth/Utils>
#include <osg/PositionAttitudeTransform>
#include <osg/Math>
#include <osg/ImageStream>
#include <osgDB/FileNameUtils>
#include <osgEarthSymbology/Geometry>
#include <osgEarthSymbology/Style>
#include <osgEarthSymbology/LineSymbol>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarthFeatures/Feature>
#include <osgEarthAnnotation/ImageOverlay>
#include <osgEarthAnnotation/ImageOverlayEditor>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthUtil/AnnotationEvents>
#include <osgEarthAnnotation/HighlightDecoration>
#include <osgEarthAnnotation/ScaleDecoration>
#include <osgGA/EventVisitor>
#include <osgEarthUtil/ObjectLocator>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarth/Terrain>
#include <osgEarth/Picker>
#include <osgEarthUtil/FeatureQueryTool>
#include <osgEarthUtil/MeasureTool>
#include "vr_globewidgetconfig.h"
#include <qimage>

class VR_GLOBEWIDGET_DLL CCommonTool
{
public:
	CCommonTool()
	{}

	virtual ~CCommonTool()
	{}

	static void unicodeToUTF8(const std::wstring &src, std::string& result)
	{
		int n = WideCharToMultiByte( CP_UTF8, 0, src.c_str(), -1, 0, 0, 0, 0 );
		result.resize(n);
		::WideCharToMultiByte( CP_UTF8, 0, src.c_str(), -1, (char*)result.c_str(), result.length(), 0, 0 );
	}

	static void gb2312ToUnicode(const std::string& src, std::wstring& result)
	{
		int n = MultiByteToWideChar( CP_ACP, 0, src.c_str(), -1, NULL, 0 );
		result.resize(n);
		::MultiByteToWideChar( CP_ACP, 0, src.c_str(), -1, (LPWSTR)result.c_str(), result.length());
	}

	static void gb2312ToUtf8(const std::string& src, std::string& result)
	{
		std::wstring strWideChar;
		gb2312ToUnicode(src, strWideChar);
		unicodeToUTF8(strWideChar, result);
	}
};

class VR_GLOBEWIDGET_DLL CUtility
{
public:
	static osgText::Text* createText(const osg::Vec3& pos, const std::string& content, float size, const std::string &font);

	static std::string GBKToUTF8(const std::string &strGBK);

	static void setUTF8Text(osgText::Text* text, const std::string& content);

	static osg::Camera* createHUDCamera(double left, double right, double bottom, double top);

	static osg::Node* CreatScreenText(const float &x, const float &y, const float &width,
		const float &height, const float &size, const std::string &content, const osg::Vec4 &color, const std::string &font);
};

enum ImageSourceType
{
	IST_GOOGLEMAP
};

class VR_GLOBEWIDGET_DLL CImageCreator
{
public:
	CImageCreator();

	virtual ~CImageCreator();

	QImage createImage(
		osgEarth::GeoPoint& leftBottomGeoPoint,
		osgEarth::GeoPoint& rightTopGeoPoint,
		const ImageSourceType& imageSourceType);
};
#endif
