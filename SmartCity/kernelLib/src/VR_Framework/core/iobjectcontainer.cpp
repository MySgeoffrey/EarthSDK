//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : 蚂蚁空间信息平台
//  @ File Name : iobjectcontainer.cpp
//  @ Date : 2017/8/21
//  @ Author : 
//
//


#include "framework/core/iobjectcontainer.h"

namespace Core
{
	IObjectContainer::IObjectContainer()
	{

	}

	IObjectContainer::~IObjectContainer()
	{


	}

	bool IObjectContainer::addObject(Common::IObject* pObject)
    {
		ObjectMap::iterator itr = this->mObjects.find(pObject->getID());
		if (itr == this->mObjects.end())
		{
			this->mObjects[pObject->getID()] = pObject;
			return true;
		}
		return false;
    }
    
	Common::IObject* IObjectContainer::getObject(const std::string& id)
    {
		ObjectMap::iterator itr = this->mObjects.find(id);
		if (itr != this->mObjects.end())
		{
			return itr->second;
		}
		return NULL;
    }

	int IObjectContainer::getNumOfObjects()
	{
		return this->mObjects.size();
	}

	bool IObjectContainer::getKeys(std::vector<std::string>& keys)
	{
		if (!this->mObjects.empty())
		{
			ObjectMap::iterator bItr = this->mObjects.begin();
			ObjectMap::iterator eItr = this->mObjects.end();
			while (bItr != eItr)
			{
				keys.push_back(bItr->first);
				++bItr;
			}
		}
		return (keys.size() > 0);
	}
    
	Common::IObject* IObjectContainer::removeObject(const std::string& id)
    {
		Common::IObject* pResult = NULL;
		ObjectMap::iterator itr = this->mObjects.find(id);
		if (itr != this->mObjects.end())
		{
			pResult = itr->second;
			this->mObjects.erase(itr);
		}
		return pResult;
    }
    
	bool IObjectContainer::removeAllObjects()
    {
		if (!this->mObjects.empty())
		{
			this->mObjects.clear();//主要耗效率的接口
			return true;
		}
		return false;
    }
    
	bool IObjectContainer::removeAndDestroyAllObjects()
    {
		if (!this->mObjects.empty())
		{
			ObjectMap::iterator bItr = this->mObjects.begin();
			ObjectMap::iterator eItr = this->mObjects.end();
			while (bItr != eItr)
			{
				if (bItr->second)
					delete bItr->second;
				++bItr;
			}
			this->mObjects.clear();//主要耗效率的接口
			return true;
		}
		return false;
    }
    
}
