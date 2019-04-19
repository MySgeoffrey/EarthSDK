#include "dynamicobject/dynamicobjectlayer.h"

using namespace DynamicObject;

DynamicObject::CDynamicObjectLayer::CDynamicObjectLayer(osgEarth::MapNode* pMapNode)
{
	mpMapNode = pMapNode;
	if(NULL != mpMapNode)
	{
		mpGroup = new osg::Group;
		mpMapNode->addChild(mpGroup);
	}
	mpObjectContainer = new Core::IObjectContainer;
}

DynamicObject::CDynamicObjectLayer::~CDynamicObjectLayer()
{
	if(NULL != mpGroup)
	{
		mpMapNode->removeChild(mpGroup);
		mpGroup = NULL;
	}

	if(NULL != mpObjectContainer)
	{
		mpObjectContainer->removeAndDestroyAllObjects();
		delete mpObjectContainer;
		mpObjectContainer = NULL;
	}
}

CDynamicObject* DynamicObject::CDynamicObjectLayer::addDynamicObject( const std::string& id,const std::string& name )
{
	CDynamicObject* pDynamicObject = new CDynamicObject(mpMapNode);
	pDynamicObject->setName(name);
	pDynamicObject->setID(id);
	osg::Node* pNode = pDynamicObject->getDynamicObjctNode();
	if(NULL != pNode)
		mpGroup->addChild(pNode);
	mpObjectContainer->addObject(pDynamicObject);
	return pDynamicObject;
}

CDynamicObject* DynamicObject::CDynamicObjectLayer::getDynamicObjectByName( const std::string& name )
{
	std::vector<std::string> keys;
	mpObjectContainer->getKeys(keys);
	Common::IObject* pObject = NULL;
	for (int index = 0; index < keys.size(); ++index)
	{
		pObject = mpObjectContainer->getObject(keys.at(index));
		if(NULL == pObject)
			continue;
		if(name == pObject->getName())
			return dynamic_cast<CDynamicObject*>(pObject);
	}
	return NULL;
}

CDynamicObject* DynamicObject::CDynamicObjectLayer::getDynamicObjectByID( const std::string& id )
{
	return dynamic_cast<CDynamicObject*>(mpObjectContainer->getObject(id));
}

bool DynamicObject::CDynamicObjectLayer::removeDynamicObejctByName( const std::string& name )
{
	CDynamicObject* pDynamicObject = getDynamicObjectByName(name);
	if(NULL == pDynamicObject)
		return false;
	return removeDynamicObejctByID(pDynamicObject->getID());
}

bool DynamicObject::CDynamicObjectLayer::removeDynamicObejctByID( const std::string& id )
{
	Common::IObject* pObject = mpObjectContainer->removeObject(id);
	if(NULL == pObject)
		return false;
	delete pObject;
	pObject = NULL;
	return true;
}

std::vector<std::string> DynamicObject::CDynamicObjectLayer::getDynamicNames()
{
	std::vector<std::string> keys;
	mpObjectContainer->getKeys(keys);
	std::vector<std::string> names;
	Common::IObject* pObject = NULL;
	for (int index = 0; index < keys.size(); ++index)
	{
		pObject = mpObjectContainer->getObject(keys.at(index));
		if(NULL == pObject)
			continue;
		names.push_back(pObject->getName());
	}
	return names;
}

std::vector<std::string> DynamicObject::CDynamicObjectLayer::getDynamicIDs()
{
	std::vector<std::string> keys;
	mpObjectContainer->getKeys(keys);
	return keys;
}

int DynamicObject::CDynamicObjectLayer::getDynamicCount()
{
	return mpObjectContainer->getNumOfObjects();
}
