#if !defined(_WARNINGOBJECTLAYER_H)
#define _WARNINGOBJECTLAYER_H

#include "dynamicobject/dynamicobjectconfig.h"
#include "framework/core/ilayer.h"
#include "geoobject/geocircle.h"

namespace DynamicObject
{
	class VR_DYNAMICOBJECT_EXPORT CWarnningObjectLayer : public Core::ILayer
	{
	public:
		CWarnningObjectLayer(osgEarth::MapNode* pMapNode);
		~CWarnningObjectLayer();
		Geo::CGeoCircle* addWarnningObject(const std::string& id,const std::string& name);
		bool renderToScene();
		bool deRenderFromScene();
	private:
		osgEarth::MapNode* mpMapNode;
	};
}

#endif