#if !defined(_SCENEOBJECTLAYER_H)
#define _SCENEOBJECTLAYER_H

#include "dynamicobject/dynamicobjectconfig.h"
#include "framework/core/ilayer.h"
#include "geoobject/geocircle.h"

namespace DynamicObject
{
	class VR_DYNAMICOBJECT_EXPORT CSceneObjectLayer : public Core::ILayer
	{
	public:
		CSceneObjectLayer(const std::string& layerType);
		~CSceneObjectLayer();
		bool renderToScene();
		bool deRenderFromScene();
	private:
		osgEarth::MapNode* mpMapNode;
	};
}

#endif