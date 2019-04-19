#if !defined(_AZIMUTHMEASURE_H)
#define _AZIMUTHMEASURE_H

#include "scenetool/measurementconfig.h"
#include "geometryeditor/polylineeditor.h"
#include "framework/tool/itoollistener.h"
#include "geographic/geopoint.h"

namespace SceneTool
{
	class VR_SCENETOOL_EXPORT CAzimuthMeasure : public GeometryEditor::IPolylineEditor, public Tool::IToolListener
	{
	public:
		CAzimuthMeasure(osgEarth::MapNode* pRefMapNode);
		~CAzimuthMeasure();
		static double computeAzimuth(
			const osg::Vec3d& beginPoint,
			const osg::Vec3d& endPoint);

		static osgEarth::Symbology::Style& getTextStyle();

		virtual void onEvent(Common::IObject* pResult);
	protected:
		virtual bool handle(const osgGA::GUIEventAdapter& ea,
			osgGA::GUIActionAdapter& aa);

		virtual void performPicked(osg::Vec3d pos);

		virtual void performMoving(osg::Vec3d pos);

	private:
		Graphic::CGeoPoint* mpAzimuthContent;
		
		bool mIsEnd;
	};
}

#endif