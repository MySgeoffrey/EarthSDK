#if !defined(_DYNAMICOBJECTLAYER_H)
#define _DYNAMICOBJECTLAYER_H

#include "dynamicobject/dynamicobjectconfig.h"
#include "framework/common/iobject.h"
#include "dynamicobject/dynamicobject.h"
#include "framework/common/iobjectcontainer.h"

namespace DynamicObject
{
	class VR_DYNAMICOBJECT_EXPORT CDynamicObjectLayer : public Common::IObject
	{
	public:
		CDynamicObjectLayer(osgEarth::MapNode* pMapNode);
		~CDynamicObjectLayer();
		CDynamicObject* addDynamicObject(const std::string& id,const std::string& name);
		CDynamicObject* getDynamicObjectByName(const std::string& name);
		CDynamicObject* getDynamicObjectByID(const std::string& id);
		bool removeDynamicObejctByName(const std::string& name);
		bool removeDynamicObejctByID(const std::string& id);
		std::vector<std::string> getDynamicNames();
		std::vector<std::string> getDynamicIDs();
		int getDynamicCount();

	private:
		Core::IObjectContainer* mpObjectContainer;
		osgEarth::MapNode* mpMapNode;
		osg::Group* mpGroup;
	};
}

#endif