//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : 蚂蚁空间信息平台
//  @ File Name : igeoline.cpp
//  @ Date : 2017/8/21
//  @ Author : 
//
//


#include "framework/core/igeoline.h"

namespace Core
{
	IGeoLine::IGeoLine()
	{
	
	}

	IGeoLine::IGeoLine(osgEarth::MapNode* pRefMapNode)
		: Common::ISceneObject(pRefMapNode)
	{
	
	}

	IGeoLine::~IGeoLine()
	{
		this->m_vecPoint.clear();
	}

	std::vector<osg::Vec3d>& IGeoLine::getGeometry()
	{
		return this->m_vecPoint;
	}
    
}
