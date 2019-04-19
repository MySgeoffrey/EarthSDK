#include "geometryeditor/pointeditor.h"
#include <math.h>


namespace GeometryEditor
{

	IPointEditor::IPointEditor(osgEarth::MapNode* pRefMapNode)
		: IGeometryEditor(pRefMapNode)
	{
		osgEarth::Symbology::Style& pointStyle = this->getGeometryStyle();
		pointStyle.getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral(
			"data/placemark32.png");
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloImplementation() = (osgText::Text::BackdropImplementation::STENCIL_BUFFER);
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = (osg::Vec4(1, 1, 0, 1));
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->halo()->color() = (osg::Vec4(0, 0, 0, 1));
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->halo()->width() = 20;
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloBackdropType() = (osgText::Text::BackdropType::OUTLINE);
		this->mpGeoPoint = NULL;
	}


	IPointEditor::~IPointEditor()
	{
		if (this->mpGeoPoint)
		{
			this->mpGeoPoint->deRenderFromScene();
			this->mpGeoPoint->unLoadFromScene();
			delete this->mpGeoPoint;
			this->mpGeoPoint = NULL;
		}
	}

	void IPointEditor::performDoublePicked(osg::Vec3d pos)
	{
		IGeometryEditor::performDoublePicked(pos);
		if (this->mpGeoPoint)
		{
			this->mpGeoPoint->deRenderFromScene();
			this->mpGeoPoint->unLoadFromScene();
			delete this->mpGeoPoint;
			this->mpGeoPoint = NULL;
		}
	}

	void IPointEditor::initial()
	{

	}

	void IPointEditor::performPicked(osg::Vec3d pos)
	{
		if (NULL != this->mpGeoPoint)
		{
			this->mpGeoPoint->deRenderFromScene();
			this->mpGeoPoint->unLoadFromScene();
			delete this->mpGeoPoint;
			this->mpGeoPoint = NULL;
		}
		Graphic::CGeoPoint* pGeoPoint = new Graphic::CGeoPoint(this->mpRefMapNode);
		pGeoPoint->setGeoPosition(pos);
		pGeoPoint->getStyle() = this->getGeometryStyle();
		pGeoPoint->loadToScene();
		pGeoPoint->renderToScene();
		this->mpGeoPoint = pGeoPoint;
	}

	void IPointEditor::performMoving(osg::Vec3d pos)
	{
		/*if (NULL != this->mpGeoPoint)
		{
			this->mpGeoPoint->deRenderFromScene();
			this->mpGeoPoint->unLoadFromScene();
			delete this->mpGeoPoint;
			this->mpGeoPoint = NULL;
		}
		Geo::CGeoPoint* pGeoPoint = new Geo::CGeoPoint(this->mpRefMapNode);
		pGeoPoint->setPosition(Core::IVec3(
			pos.x(), pos.y(), pos.z()));
		pGeoPoint->getStyle() = this->getGeometryStyle();
		pGeoPoint->loadToScene();
		pGeoPoint->renderToScene();
		this->mpGeoPoint = pGeoPoint;*/
	}

	Common::ISceneObject* IPointEditor::generateSceneObject()
	{
		return NULL;
	}

	void IPointEditor::performPickedXYZ(osg::Vec3d pos)
	{

	}

	void IPointEditor::performMovingXYZ(osg::Vec3d pos)
	{

	}

	void IPointEditor::performRightPicked()
	{

	}

	void IPointEditor::performDeleteDrawing()
	{

	}
}