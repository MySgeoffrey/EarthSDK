//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : 蚂蚁空间信息平台
//  @ File Name : igeopoint.h
//  @ Date : 2017/8/21
//  @ Author : 
//
//


#if !defined(_CGEOCIRLCE_H)
#define _CGEOCIRLCE_H


#include "framework/core/igeopoint.h"
#include "geoobject/geoobject_global.h"
#include <osgEarthSymbology/Style>

// 地理圆
namespace Geo
{
	class CGeoCircleExCallback;
	class VR_GEOOBJECT_EXPORT IGeoCircle
		: public Core::IGeoPoint
	{
	public:

		IGeoCircle();

		IGeoCircle(osgEarth::MapNode* pRefMapNode);

		virtual ~IGeoCircle();

		virtual void setRadius(const double& radius);

		virtual double getRadius();

		virtual void setBeginAngle(const double& beginAngle);

		virtual void setEndAngle(const double& endAngle);

		virtual double getBeginAngle();

		virtual double getEndAngle();

		virtual bool loadToScene();

		virtual bool renderToScene();

		virtual bool deRenderFromScene();

		virtual bool unLoadFromScene();

		osgEarth::Symbology::Style& getStyle();
	protected:
		osgEarth::Symbology::Style mStyle;
		double mRadius;
		double mBeginAngle;
		double mEndAngle;
	};

	class VR_GEOOBJECT_EXPORT CGeoCircleEx 
		: public IGeoCircle
	{
	public:
	enum DirectionType
	{
		DT_ClockWise,//顺时针
		DT_AntiClockWise//逆时针
	};
	public:

		CGeoCircleEx();

		CGeoCircleEx(osgEarth::MapNode* pRefMapNode);

		virtual ~CGeoCircleEx();

		virtual bool loadToScene();

		virtual bool renderToScene();

		virtual bool deRenderFromScene();

		virtual bool unLoadFromScene();

		void setRotateEnable(const bool& rotateEnable);

		bool getRotateEnable();

		bool& useGradualTransparent();

		double& startTransparent();

		double endTransparent();

		void setRotateFactor(const double& rotateFactor);

		double getRotateFactor();

		void setRotateDirection(const DirectionType& value);

		DirectionType getRotateDirection();

		void setClampGroundMode(const bool& value);

		bool getClampGroundMode();

		virtual void update(osg::Node* node, osg::NodeVisitor* nv);

	protected:
		bool mClampGroundMode;
		DirectionType mDirectionType;
		double mStartTransparent;
		double mEndTransparent;
		bool mUseGradualTransparent;
		osg::Matrix mWorldMatrix;
		double mRotateAngle;
		double mRotateFactor;
		bool mRotateEnable;
		CGeoCircleExCallback* mpUpdateCallback;
    };

}

#endif  //_CGEOCIRLCE_H
