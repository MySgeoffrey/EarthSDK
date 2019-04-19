//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : 蚂蚁空间信息平台
//  @ File Name : geopoint.cpp
//  @ Date : 2017/8/21
//  @ Author : 
//
//


#include "geographic/geopoint.h"
#include <osg/Geometry>
#include <osgEarthSymbology/Geometry>
#include <osgEarthFeatures/Feature>
#include <osgEarth/ElevationQuery>
#include <osgEarthAnnotation/PlaceNode>
#include "framework/common/globesetting.h"

using namespace osg;
using namespace osgEarth::Symbology;

namespace Graphic
{
	CGeoPoint::CGeoPoint()
	{
		this->mpNode = NULL;
		this->mMaxGeometryPointNum = 1;
		this->mType = Graphic::GT_POINT;
		osgEarth::Symbology::Style& style = this->getStyle();
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()= "fonts/simhei.ttf";//指定中文字体路径
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 20.0;//设置字体大小
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = osgEarth::Symbology::Color::Yellow;//设置字体颜色
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_LEFT_CENTER;
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->declutter() = false;
		style.getOrCreate<osgEarth::Symbology::IconSymbol>()->declutter() = false;
		style.getOrCreate<osgEarth::Symbology::IconSymbol>()->alignment() = osgEarth::Symbology::IconSymbol::ALIGN_CENTER_CENTER;
		style.getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral(
			"systemImages/resources/image/icon.png");//设置点图标显示的图片
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()= "fonts/simhei.ttf";//指定中文字体路径
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->declutter() = false;
		style.getOrCreate<osgEarth::Symbology::IconSymbol>()->declutter() = false;
	}

	CGeoPoint::CGeoPoint(osgEarth::MapNode* pRefMapNode)
		: Core::IGeoPoint(pRefMapNode)
	{
		this->mpNode = NULL;
		this->mMaxGeometryPointNum = 1;
		this->mType = Graphic::GT_POINT;
		osgEarth::Symbology::Style& style = this->getStyle();
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()= "fonts/simhei.ttf";//指定中文字体路径
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 20.0;//设置字体大小
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = osgEarth::Symbology::Color::Yellow;//设置字体颜色
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_LEFT_CENTER;
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->declutter() = false;
		style.getOrCreate<osgEarth::Symbology::IconSymbol>()->declutter() = false;
		style.getOrCreate<osgEarth::Symbology::IconSymbol>()->alignment() = osgEarth::Symbology::IconSymbol::ALIGN_CENTER_CENTER;
		style.getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral(
			"systemImages/resources/image/icon.png");//设置点图标显示的图片
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()= "fonts/simhei.ttf";//指定中文字体路径
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->declutter() = false;
		style.getOrCreate<osgEarth::Symbology::IconSymbol>()->declutter() = false;
	}

	CGeoPoint::~CGeoPoint()
	{
	
	}

	bool CGeoPoint::loadToScene()
	{
		bool r = false;
		if (this->mpLod)
			return r;
		Style& pointStyle = this->getStyle();
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->declutter() = false;
		pointStyle.getOrCreate<osgEarth::Symbology::IconSymbol>()->declutter() = false;
		std::string text(this->getName());
	
		text = CGlobeSetting::instance()->gbkToUTF8(text);
		osg::Vec3d geoPosition = osg::Vec3d(this->getGeoPosition().x(), this->getGeoPosition().y(),
			this->getGeoPosition().z());

		osgEarth::Annotation::PlaceNode* pPlaceNode = 
			new osgEarth::Annotation::PlaceNode(
			this->mpRefMapNode, 
			GeoPoint(this->mpRefMapNode->getMapSRS()->getGeographicSRS(),
			geoPosition),
			text, pointStyle);
		if (this->getSelectable())
		{
			this->mObjectId = osgEarth::Registry::objectIndex()->tagNode(pPlaceNode, this);
		}
		this->mpNode = pPlaceNode;
		if (NULL == this->mpLod)
		{
			this->mpLod = new osg::LOD();
		}
		this->mpLod->addChild(pPlaceNode, 0, 10000000000);
		setVisible(getVisible());
		r = true;
		return r;
	}

	bool CGeoPoint::renderToScene()
	{
		bool r = false;
		if (this->mpLod && this->mpLod->getNumParents() == 0)
		{
			this->mpRefMapNode->addChild(this->mpLod);
			r = true;
		}
		return r;
	}

	bool CGeoPoint::deRenderFromScene()
	{
		bool r = false;

		return r;
	}

	bool CGeoPoint::unLoadFromScene()
	{
		bool r = false;
		if (this->mpLod && this->mpLod->getNumParents() > 0)
		{
			this->mpLod->getParent(0)->removeChild(this->mpLod);
			this->mpLod = NULL;
			r = true;
		}
		return r;
	}

	void CGeoPoint::updateSceneObject()
	{
		if (this->mpLod && this->mpNode)
		{
			osg::Vec3d geoPosition = this->getGeoPosition();
			if (this->getGeometry().size() >= 1)
			{
				geoPosition = this->getGeometry().at(0);
			}
			osg::Vec3d worldPosition;
			osgEarth::GeoPoint geoPoint(this->getMapNode()->getMapSRS(),geoPosition);
			geoPoint.toWorld(worldPosition);
			osgEarth::Annotation::PlaceNode* pPlaceNode =
				dynamic_cast<osgEarth::Annotation::PlaceNode*>(this->mpNode);
			pPlaceNode->getGeoTransform()->setMatrix(osg::Matrix::translate(worldPosition));
		}
	}

	osg::Node* CGeoPoint::getNode()
	{
		return mpNode;
	}

}
