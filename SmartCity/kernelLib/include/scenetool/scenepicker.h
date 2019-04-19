#ifndef CModelPickerTool_H
#define CModelPickerTool_H


#include <osgGA/GUIEventHandler>
#include "framework/tool/itool.h"
#include <osgEarth/MapNode>
#include <osgEarthUtil/RTTPicker>
#include "scenetool/measurementconfig.h"


namespace SceneTool
{
	class VR_SCENETOOL_EXPORT CModelPickerTool
		: public osgEarth::Util::RTTPicker,public Tool::ITool
	{
	public:
		CModelPickerTool(osg::ref_ptr<osgEarth::MapNode> refMapNode);

		~CModelPickerTool();

		bool getEnable();

		void setEnable(const bool& enable);

		void forceListeners(Common::IObject* pResult);

		virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	protected:
		osgEarth::Util::RTTPicker* mpPicker;
		osg::ref_ptr<osgEarth::MapNode> mRefMapNode;
		bool mEnable;
	};

	class VR_SCENETOOL_EXPORT CPickerResult : public Common::IObject
	{
	public:
		CPickerResult();

		virtual ~CPickerResult();

		osg::Vec2d getScreenPosition();

		void setScreenPosition(const osg::Vec2d& screenPosition);

		Common::IObject* getPickerObject();

		void setPickerObject(Common::IObject* pPickerObject);

		void setMouseButton(const int& mouseButton);

		int getMouseButton();

	protected:
		osg::Vec3d mGeoPosition;
		osg::Vec2d mScreenPosition;
		int mMouseButton;
		Common::IObject* mPickerObject;
	};
}

#endif