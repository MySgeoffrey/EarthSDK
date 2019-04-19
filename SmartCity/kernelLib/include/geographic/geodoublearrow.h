//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : cgeodoublearrow.h
//  @ Date : 2018/7/28
//  @ Author : sgeoffrey
//  @platform:蚂蚁空间信息平台
//


#if !defined(_CGEODOUBLEARROW_H)
#define _CGEODOUBLEARROW_H

#include "geographic/geopolygon.h"
#include "geographic/geographicconfig.h"

namespace Graphic
{
	class CGeoDoubleArrow : public CGeoPolygon
	{
	public:
		/// <summary>
		/// 构造函数
		/// </summary>
		CGeoDoubleArrow();
		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="pRefMapNode">关联的地图节点</param>
		CGeoDoubleArrow(osgEarth::MapNode* pRefMapNode);
		/// <summary>
		/// 析构函数
		/// </summary>
		virtual ~CGeoDoubleArrow();

	protected:
		virtual bool transformRenderGeometry(std::vector<osg::Vec3d>& outRenderGeometry);
	};
}
#endif  //_CGEODOUBLEARROW_H
