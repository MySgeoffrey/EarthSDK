#if !defined(_DISTANCEMEASURE_H)
#define _DISTANCEMEASURE_H

#include "scenetool/measurementconfig.h"
#include "geometryeditor/polylineeditor.h"
#include "framework/tool/itoollistener.h"
#include "geographic/geopoint.h"
#include "geometryeditor/geographiceditor.h"

namespace SceneTool
{
	class VR_SCENETOOL_EXPORT CDistanceMeasure : public GeometryEditor::IPolylineEditor, public Tool::IToolListener
	{
	public:
		CDistanceMeasure(osgEarth::MapNode* pRefMapNode,MeasureType measureType);
		
		~CDistanceMeasure();
		
		static osgEarth::Symbology::Style& getTextStyle();

		static osgEarth::Symbology::Style& getGeometryStyle();

		virtual void onEvent(Common::IObject* pResult);
	protected:
		virtual bool handle(const osgGA::GUIEventAdapter& ea,
			osgGA::GUIActionAdapter& aa);

		virtual void disposeDistanceContents();

		virtual void renderDistanceContents();
	private:
		Graphic::CGeoPoint* mpDistanceContent;
		std::vector<Graphic::CGeoPoint*> mpDistanceContents;
		MeasureType mMeasureType;
		
	};

	class VR_SCENETOOL_EXPORT CDistanceMeasureTool 
		: public GeometryEditor::IGeoGraphicEditor
	{
	public:
		CDistanceMeasureTool(osgEarth::MapNode* pRefMapNode,MeasureType measureType);
		
		~CDistanceMeasureTool();

		void setShowAzimuth(const bool& showAzimuth);

		bool getShowAzimuth();

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
		void updateResult(const bool& rebuildAllText = true);

		void createEditorCallBack();

		virtual void disposeDistanceContents();

		virtual void renderDistanceContents();

		void updateTotalLengthText();

	protected:
		bool mShowAzimuth;
		MeasureType mMeasureType;
		bool mNeedResetResult;
		std::vector<Graphic::CGeoPoint*> mpDistanceContents;
		double mLastDistance;
	};
}

#endif