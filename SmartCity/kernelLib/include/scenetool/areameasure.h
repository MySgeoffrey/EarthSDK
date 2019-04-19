#if !defined(_AREAMEASURE_H)
#define _AREAMEASURE_H

#include "scenetool/measurementconfig.h"
#include "geometryeditor/polygoneditor.h"
#include "framework/tool/itoollistener.h"
#include "geographic/geopoint.h"
#include "geometryeditor/geographiceditor.h"

namespace SceneTool
{
	class VR_SCENETOOL_EXPORT CAreaMeasure : public GeometryEditor::IPolygonEditor, public Tool::IToolListener
	{
	public:
		CAreaMeasure(osgEarth::MapNode* pRefMapNode,MeasureType measureType);
		
		~CAreaMeasure();

		static osgEarth::Symbology::Style& getTextStyle();

		static osgEarth::Symbology::Style& getGeometryStyle();
	public:
		virtual void onEvent(Common::IObject* pResult);
	protected:
		virtual bool handle(const osgGA::GUIEventAdapter& ea,
			osgGA::GUIActionAdapter& aa);
	private:
		Graphic::CGeoPoint* mpAreaContent;
		MeasureType mMeasureType;
		
	};


	class VR_SCENETOOL_EXPORT CAreaMeasureTool 
		: public GeometryEditor::IGeoGraphicEditor
	{
	public:
		CAreaMeasureTool(osgEarth::MapNode* pRefMapNode,MeasureType measureType);
		
		~CAreaMeasureTool();

		virtual void beginEdit(const std::string& graphicType,
			Common::ISceneObject* pGraphicObject,
			const bool& hideOriginGraphicObject = true);

		virtual bool endEdit(
			const bool& clearCurrentGeometry = true,
			const bool& needSaveEditingResult = true,
			const bool& hideOriginGraphicObject = true);

		// 经纬度信息
		virtual void performPicked(osg::Vec3d pos);

		virtual void performMoving(osg::Vec3d pos);

		virtual void performMovingXYZ(osg::Vec3d pos);

		virtual void performRightPicked();
	protected:
		void updateResult();

		void createEditorCallBack();

	protected:
		MeasureType mMeasureType;
		bool mNeedResetResult;
	};
}

#endif