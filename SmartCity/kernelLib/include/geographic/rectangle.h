//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : crectangle.h
//  @ Date : 2018/7/28
//  @ Author : sgeoffrey
//  @platform:蚂蚁空间信息平台
//


#if !defined(_CRECTANGLE_H)
#define _CRECTANGLE_H

#include "geographic/geopolygon.h"
#include "geographic/geographicconfig.h"

namespace Graphic
{
	class VR_GEOGRAPHIC_EXPORT CGeoRectangle 
		: public CGeoPolygon
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		CGeoRectangle();
		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="pRefMapNode">关联的地图节点</param>
		CGeoRectangle(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~CGeoRectangle();

		virtual void setWidth(const double& width);

		virtual double getWidth();

		virtual void setLength(const double& length);

		virtual double getLength();

		virtual void updateSceneObject();

		virtual void copyTo(Common::ISceneObject* pDestObject);
	protected:
		virtual bool transformRenderGeometry(std::vector<osg::Vec3d>& outRenderGeometry);
	protected:
		double mWidth;
		double mLength;
	};
}
#endif  //_CRECTANGLE_H
