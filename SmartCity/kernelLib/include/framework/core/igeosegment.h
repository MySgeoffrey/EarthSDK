//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : 蚂蚁空间信息平台
//  @ File Name : igeosegment.h
//  @ Date : 2017/8/21
//  @ Author : 
//
//


#if !defined(_IGEOSEGMENT_H)
#define _IGEOSEGMENT_H

#include "framework/common/isceneobject.h"

// 地理线段
namespace Core
{
	/// <summary>
	/// 线段接口类
	/// </summary>
	class VR_PIPEFRAMEWORK_EXPORT IGeoSegment : public Common::ISceneObject
    {
    public:
		IGeoSegment();

		IGeoSegment(osgEarth::MapNode* pRefMapNode);

		virtual ~IGeoSegment();

		virtual std::vector<osg::Vec3d>& getGeometry();

		virtual osg::Vec3d& getStartPoint();

		virtual osg::Vec3d& getEndPoint();

	protected:
		std::vector<osg::Vec3d> mPoints;
    };
}

#endif  //_IGEOSEGMENT_H
