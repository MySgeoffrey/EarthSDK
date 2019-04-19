#ifndef _MODELCREATOR_H
#define _MODELCREATOR_H

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
#include "vr_globewidget.h"

class VR_GLOBEWIDGET_DLL CPipelineStyle
{
public:
	CPipelineStyle()
	{
		Radius = 3;
		Color = osg::Vec4(1.0,0.0,0.0,1.0);
	}

	~CPipelineStyle()
	{

	}
	float Radius;
	osg::Vec4 Color;
};

class VR_GLOBEWIDGET_DLL PositionNormalColorTexture
{
public:
	PositionNormalColorTexture()
	{
		Position = osg::Vec3(0,0,0);
		Normal = osg::Vec3(1.0,1.0,1.0);
		Color = osg::Vec4(1.0,0,0,1.0);
		TextureCoor = osg::Vec2(0,0);

	}

	~PositionNormalColorTexture()
	{

	}
	osg::Vec3 Position;
	osg::Vec3 Normal;
	osg::Vec4 Color;
	osg::Vec2 TextureCoor;
};

class VR_GLOBEWIDGET_DLL CPipelineData
{
public:
	CPipelineData()
	{
		
	}

	~CPipelineData()
	{

	}
public:
	osg::Vec3d ReferenceWorldPos;
	std::vector<PositionNormalColorTexture*> Vertexes;
	std::vector<unsigned int> Indices;
	std::vector<osgEarth::GeoPoint> CenterPoints;
	CPipelineStyle Style;
};

class VR_GLOBEWIDGET_DLL CModelCreator
{
public:
	CModelCreator(void);

	~CModelCreator(void);

	static osg::Node* createPipeLineNode(CPipelineData* pData);
};


#endif
