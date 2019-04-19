#include "dynamicobject/objectcontainerex.h"


CSafeObjectContainer::CSafeObjectContainer()
{

}

CSafeObjectContainer::~CSafeObjectContainer()
{

}

bool CSafeObjectContainer::addObject(Common::IObject* pObject)
{
	bool r = false;
	if (0 == this->mObjectMutex.lock())
	{
		Core::IObjectContainer::addObject(pObject);
		this->mObjectMutex.unlock();
	}
	return r;
}

Common::IObject* CSafeObjectContainer::getObject(const std::string& id)
{
	Common::IObject* pObject = NULL;
	if (0 == this->mObjectMutex.lock())
	{
		pObject = Core::IObjectContainer::getObject(id);
		this->mObjectMutex.unlock();
	}
	return pObject;
}

int CSafeObjectContainer::getNumOfObjects()
{
	int objectCount = 0;
	if (0 == this->mObjectMutex.lock())
	{
		objectCount = Core::IObjectContainer::getNumOfObjects();
		this->mObjectMutex.unlock();
	}
	return objectCount;
}

bool CSafeObjectContainer::getKeys(std::vector<std::string>& keys)
{
	bool r = false;
	if (0 == this->mObjectMutex.lock())
	{
		Core::IObjectContainer::getKeys(keys);
		this->mObjectMutex.unlock();
	}
	return r;
}


Common::IObject* CSafeObjectContainer::removeObject(const std::string& id)
{
	Common::IObject* pObject = NULL;
	if (0 == this->mObjectMutex.lock())
	{
		pObject = Core::IObjectContainer::removeObject(id);
		this->mObjectMutex.unlock();
	}
	return pObject;
}

bool CSafeObjectContainer::removeAllObjects()
{
	bool r = false;
	if (0 == this->mObjectMutex.lock())
	{
		Core::IObjectContainer::removeAllObjects();
		this->mObjectMutex.unlock();
	}
	return r;
}

bool CSafeObjectContainer::removeAndDestroyAllObjects()
{
	bool r = false;
	if (0 == this->mObjectMutex.lock())
	{
		Core::IObjectContainer::removeAndDestroyAllObjects();
		this->mObjectMutex.unlock();
	}
	return r;
}
