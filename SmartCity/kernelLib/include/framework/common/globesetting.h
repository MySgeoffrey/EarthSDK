#ifndef _GLOBESETTINGS_H
#define _GLOBESETTINGS_H

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
#include <iostream>
#include "framework/vr_globeframework_global.h"

using namespace std;
class VR_PIPEFRAMEWORK_EXPORT CGlobeSetting
{
public:
	CGlobeSetting();

	virtual ~CGlobeSetting();

	static CGlobeSetting* instance();

	std::string getDataServiceUrl();
	void unicodeToUTF8(const wstring &src, string& result);
	void gb2312ToUnicode(const string& src, wstring& result);

	void setDataServiceUrl(const std::string& dataServiceUrl);
	void gb2312ToUtf8(const string& src, string& result);
	std::string getAxFullPath();
	std::string gbkToUTF8(const std::string &strGBK)
	{
		std::string strOutUTF8 = "";
		int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
		WCHAR * str1 = new WCHAR[n * 2];
		MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
		n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
		std::wstring wstr(str1);
		//strOutUTF8 = WStrToStr(wstr);
		char* str2 = new char[n];
		WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
		strOutUTF8 = str2;

		delete[] str1;
		delete[] str2;
		str1 = NULL;
		str2 = NULL;

		return strOutUTF8;
	}
private:
	std::string mDataServiceUrl;
};

#endif
