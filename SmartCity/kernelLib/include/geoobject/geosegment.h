//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : 蚂蚁空间信息平台
//  @ File Name : igeoline.h
//  @ Date : 2017/8/21
//  @ Author : 
//
//


#if !defined(_CGEOSEGMENT_H)
#define _CGEOSEGMENT_H

#include "framework/core/igeosegment.h"
#include "geoobject/geoobject_global.h"
#include <osgEarthSymbology/Style>

// 地理线串
namespace Geo
{
	/// <summary>
	/// CGeoSegment类，继承IGeoSegment类，几何线组织与渲染对象，实现IGeoSegment的接口，
	/// 用来表达空间地理线段,由两个点坐标构成。
	/// </summary>
	class VR_GEOOBJECT_EXPORT CGeoSegment
		: public Core::IGeoSegment
    {
    public:
		/// <summary>
		/// 构造函数
		/// </summary>
		CGeoSegment();
		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="pRefMapNode">关联的地图节点</param>
		CGeoSegment(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~CGeoSegment();
		/// <summary>
		/// 加载直场景
		/// </summary>
		virtual bool loadToScene();
		/// <summary>
		/// 渲染至场景
		/// </summary>
		virtual bool renderToScene();
		/// <summary>
		/// 从场景中反挂接
		/// </summary>
		virtual bool deRenderFromScene();
		/// <summary>
		/// 从场景中卸载
		/// </summary>
		virtual bool unLoadFromScene();
		/// <summary>
		/// 获取风格
		/// </summary>
		osgEarth::Symbology::Style& getStyle();

		virtual void updateSceneObject();

		void setColor(osg::Vec4f color);

		osg::Vec4f getColor();

		void setLineWidth(float width);

		float getLineWidth();

		//0表示实线、1表示虚线
		void setLineType(int type);

		int getLineType();

	protected:
		osg::Matrix computeWorldMatrix();

	protected:
		osgEarth::Symbology::Style mStyle;
		osg::Node* mpNode;
		osg::ref_ptr<osg::Geometry> mpGeometry;
		int mLineType;
		float mLineWidth;
		osg::Vec4f mLineColor;
    };
}

#endif  //_CGEOSEGMENT_H
