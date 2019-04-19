#if !defined(_GRIDMEASURE_H)
#define _GRIDMEASURE_H

#include "scenetool/measurementconfig.h"
#include <osgGA/GUIEventHandler>
#include "geographic/geopoint.h"

namespace SceneTool
{
	class VR_SCENETOOL_EXPORT CGridMeasure 
		: public osgGA::GUIEventHandler
	{
	public:
		CGridMeasure(osgEarth::MapNode* pRefMapNode,
			Common::ISceneObject* mpRefGridObject);

		~CGridMeasure();

		virtual osgEarth::Symbology::Style& getStyle();

		int& getDirectionType();
	protected:
		void handleEvent(const osg::Vec3d& mouseGeoPos);

		virtual bool handle(const osgGA::GUIEventAdapter& ea,
			osgGA::GUIActionAdapter& aa);
	private:
		Graphic::CGeoPoint* mpContent;
		osgEarth::MapNode* mpRefMapNode;
		Common::ISceneObject* mpRefGridObject;
		int mDirectionType;//0代表方位角，1代表时钟方向
	};
}

#endif