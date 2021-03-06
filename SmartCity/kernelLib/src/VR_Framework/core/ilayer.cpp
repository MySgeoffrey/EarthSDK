//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : 蚂蚁空间信息平台
//  @ File Name : ilayer.cpp
//  @ Date : 2017/8/21
//  @ Author : 
//
//


#include "framework/core/ilayer.h"

namespace Core
{
	ILayer::ILayer()
	{
	
	}

	ILayer::ILayer(osgEarth::MapNode* pRefMapNode)
		: Common::ISceneObject(pRefMapNode)
	{
	
	}

	ILayer::~ILayer()
	{
	
	}

	void ILayer::setVisible(const bool& visible)
	{
		ISceneObject::setVisible(visible);
		if (this->mpLod)
		{
			this->mpLod->setNodeMask(visible);
		}
		std::vector<std::string> keys;
		this->getKeys(keys);
		if (keys.size() > 0)
		{
			for (int i = 0; i < keys.size(); ++i)
			{
				std::string key = keys.at(i);
				ISceneObject* pModelObject =
					dynamic_cast<ISceneObject*>(this->getObject(key));
				if (pModelObject)
				{
					if (visible == false)
					{
						pModelObject->setVisible(visible);
					}
				}
			}
		}
	}
    
}
