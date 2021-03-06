//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : 蚂蚁空间信息平台
//  @ File Name : irasterlayer.cpp
//  @ Date : 2017/8/21
//  @ Author : 
//
//


#include "framework/core/irasterlayer.h"

namespace Core
{
	IRasterLayer::IRasterLayer()
		: mpRefMap(NULL)
	{
		
	}

	IRasterLayer::IRasterLayer(osgEarth::Map* pRefMap)
		: mpRefMap(pRefMap)
	{
		
	}

	IRasterLayer::~IRasterLayer()
	{
	
	}

	void IRasterLayer::setTransparet(const double& transparent)
	{
		this->mTransparent = transparent;
	}

	double IRasterLayer::getTransparet()
	{
		return this->mTransparent;
	}
    
}
