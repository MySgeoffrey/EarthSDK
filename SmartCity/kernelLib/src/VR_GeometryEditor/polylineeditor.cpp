#include "geometryeditor/polylineeditor.h"
#include <math.h>

namespace GeometryEditor
{
	IPolylineEditor::IPolylineEditor(osgEarth::MapNode* pRefMapNode)
		: IGeometryEditor(pRefMapNode)
	{
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->color() = osgEarth::Symbology::Color::Red;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->width() = 5.0;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->tessellation() = 20.0;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
			->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
			->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;// TECHNIQUE_DRAPE;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
			->verticalOffset() = 0.1;
	}


	IPolylineEditor::~IPolylineEditor()
	{
	}

	Common::ISceneObject* IPolylineEditor::generateSceneObject()
	{
		return NULL;
	}

	void IPolylineEditor::initial()
	{
		if (this->mpGeometryNode == NULL)
		{
			osgEarth::Features::Feature* pFeature = new osgEarth::Features::Feature(
				new osgEarth::Symbology::LineString,
				this->mpRefMapNode->getMapSRS());

			this->mpGeometryNode = new osgEarth::Annotation::FeatureNode(
				this->mpRefMapNode, pFeature);
			this->mpGeometryNode->getFeature()->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;

			this->mpRefMapNode->addChild(this->mpGeometryNode);
		}

		osg::ref_ptr<osgEarth::Symbology::Geometry> pGeometry = this->mpGeometryNode->getFeature()->getGeometry();
		pGeometry->clear();
		if (!this->getGeometry().empty())
		{
			for (int i = 0; i < this->getGeometry().size(); ++i)
			{
				IEditPoint* pPoint = this->getGeometry().at(i);
				pGeometry->push_back(pPoint->getGeoPosition());
			}
		}
		

		this->mpGeometryNode->setStyle(this->mGeometryStyle);
		this->mpGeometryNode->init();

		if (this->mpPointsEdit != NULL)
		{
			int count = this->mpPointsEdit->getNumChildren();
			this->mpPointsEdit->removeChildren(0, count);
			this->mpPointsEdit->getParent(0)->removeChild(this->mpPointsEdit);
			this->mpPointsEdit = NULL;
		}
		this->mpPointsEdit = new osgEarth::Annotation::FeatureEditor(this->mpGeometryNode);
		this->mpPointsEdit->setSize(
			this->getPointsEditStyle().getOrCreateSymbol<osgEarth::Symbology::PointSymbol>()->size().get());
		this->mpPointsEdit->setColor(
			this->getPointsEditStyle().getOrCreateSymbol<osgEarth::Symbology::PointSymbol>()->fill()->color());
		this->mpRefMapNode->addChild(this->mpPointsEdit);

		this->createEditorCallBack();
	}

	void IPolylineEditor::performDoublePicked(osg::Vec3d pos)
	{
		/*this->mIsEditState = false;
		IGeometryEditor::performDoublePicked(pos);*/
	}

	void IPolylineEditor::performPicked(osg::Vec3d pos)
	{
		IEditPoint* pEditPoint = new IEditPoint();
		pEditPoint->setGeoPosition(pos);
		pEditPoint->setIndex(this->getGeometry().size());
		if (this->mPreInsertPointIndex != -1
			&& this->mNextInsertPointIndex != -1)
		{
			int maxIndex =
				this->mPreInsertPointIndex > this->mNextInsertPointIndex ? this->mPreInsertPointIndex : this->mNextInsertPointIndex;
			this->getGeometry().insert(
				this->getGeometry().begin() + maxIndex, pEditPoint);
			for (int i = 0; i < this->getGeometry().size(); ++i)
			{
				this->getGeometry().at(i)->setIndex(i);
			}
		}
		else
		{
			this->getGeometry().push_back(pEditPoint);
		}
		if (this->mpGeometryNode == NULL)
		{
			osgEarth::Features::Feature* pFeature = new osgEarth::Features::Feature(
				new osgEarth::Symbology::LineString,
				this->mpRefMapNode->getMapSRS());

			this->mpGeometryNode = new osgEarth::Annotation::FeatureNode(
				this->mpRefMapNode, pFeature);
			this->mpGeometryNode->getFeature()->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;

			this->mpRefMapNode->addChild(this->mpGeometryNode);
		}

		osg::ref_ptr<osgEarth::Symbology::Geometry> pGeometry = this->mpGeometryNode->getFeature()->getGeometry();
		if (this->mPreInsertPointIndex != -1
			&& this->mNextInsertPointIndex != -1)
		{
			int maxIndex =
				this->mPreInsertPointIndex > this->mNextInsertPointIndex ? this->mPreInsertPointIndex : this->mNextInsertPointIndex;
			pGeometry->insert(
				pGeometry->begin() + maxIndex, pos);
		}
		else
		{
			pGeometry->push_back(pos);
		}
		
		this->mpGeometryNode->setStyle(this->mGeometryStyle);
		this->mpGeometryNode->init();

		if (this->mpPointsEdit != NULL)
		{
			int count = this->mpPointsEdit->getNumChildren();
			this->mpPointsEdit->removeChildren(0, count);
			this->mpPointsEdit->getParent(0)->removeChild(this->mpPointsEdit);
			this->mpPointsEdit = NULL;
		}
		this->mpPointsEdit = new osgEarth::Annotation::FeatureEditor(this->mpGeometryNode);
		this->mpPointsEdit->setSize(
			this->getPointsEditStyle().getOrCreateSymbol<osgEarth::Symbology::PointSymbol>()->size().get());
		this->mpPointsEdit->setColor(this->getPointsEditStyle().getOrCreateSymbol<osgEarth::Symbology::PointSymbol>()->fill()->color());
		this->mpRefMapNode->addChild(this->mpPointsEdit);

		this->createEditorCallBack();
	}

	void IPolylineEditor::performMoving(osg::Vec3d pos)
	{

	}

	void IPolylineEditor::performPickedXYZ(osg::Vec3d pos)
	{

	}

	void IPolylineEditor::performMovingXYZ(osg::Vec3d pos)
	{

	}

	void IPolylineEditor::performRightPicked()
	{
		this->mIsEditState = false;
		IGeometryEditor::performRightPicked();
	}

	void IPolylineEditor::performDeleteDrawing()
	{
		if (this->mpRefCurrentPoint)
		{
			this->getGeometry().erase(this->getGeometry().begin() + this->mpRefCurrentPoint->getIndex());
			for (int i = 0; i < this->getGeometry().size(); ++i)
			{
				IEditPoint* pPoint = this->getGeometry().at(i);
				if (pPoint)
				{
					pPoint->setIndex(i);
				}
			}
			delete this->mpRefCurrentPoint;
			this->mpRefCurrentPoint = NULL;
			this->initial();
		}
	}
}