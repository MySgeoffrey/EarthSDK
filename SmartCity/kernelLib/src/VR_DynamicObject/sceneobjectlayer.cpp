#include "dynamicobject/sceneobjectlayer.h"

using namespace DynamicObject;

DynamicObject::CSceneObjectLayer::CSceneObjectLayer(const std::string& layerType)
{
	this->setType(layerType);
}

DynamicObject::CSceneObjectLayer::~CSceneObjectLayer()
{
	removeAndDestroyAllObjects();
}

bool DynamicObject::CSceneObjectLayer::renderToScene()
{
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return false;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		ISceneObject* pObject = dynamic_cast<ISceneObject*>(getObject(id));
		if (NULL == pObject)
			continue;
		pObject->loadToScene();
		pObject->renderToScene();
	}
	return true;
}

bool DynamicObject::CSceneObjectLayer::deRenderFromScene()
{
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return false;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		ISceneObject* pObject = dynamic_cast<ISceneObject*>(getObject(id));
		if (NULL == pObject)
			continue;
		pObject->deRenderFromScene();
		pObject->unLoadFromScene();
	}
	return true;
}
