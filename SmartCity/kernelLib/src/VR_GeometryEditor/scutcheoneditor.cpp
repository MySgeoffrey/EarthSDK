#include "geometryeditor/scutcheoneditor.h"
#include <math.h>


namespace GeometryEditor
{

	IScutcheonEditor::IScutcheonEditor(osgEarth::MapNode* pRefMapNode)
		: IGeometryEditor(pRefMapNode)
	{
		
		this->mpScutcheon = NULL;
	}


	IScutcheonEditor::~IScutcheonEditor()
	{
	}

	void IScutcheonEditor::performDoublePicked(osg::Vec3d pos)
	{
		IGeometryEditor::performDoublePicked(pos);
		if (this->mpScutcheon)
		{
			this->mpScutcheon->deRenderFromScene();
			this->mpScutcheon->unLoadFromScene();
			delete this->mpScutcheon;
			this->mpScutcheon = NULL;
		}
	}

	void IScutcheonEditor::initial()
	{

	}

	void IScutcheonEditor::performPicked(osg::Vec3d pos)
	{
		if (NULL != this->mpScutcheon)
		{
			this->mpScutcheon->deRenderFromScene();
			this->mpScutcheon->unLoadFromScene();
			delete this->mpScutcheon;
			this->mpScutcheon = NULL;
		}
		Geo::CGeoScutcheon* pGeoPoint = new Geo::CGeoScutcheon(this->mpRefMapNode);
		pGeoPoint->setGeoPosition(pos);
		pGeoPoint->getStyle() = this->getGeometryStyle();
		pGeoPoint->loadToScene();
		pGeoPoint->renderToScene();
		this->mpScutcheon = pGeoPoint;
	}

	void IScutcheonEditor::performMoving(osg::Vec3d pos)
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

	Common::ISceneObject* IScutcheonEditor::generateSceneObject()
	{
		Geo::CGeoScutcheon* pGeoPoint = NULL;
		if (this->mpScutcheon)
		{
			pGeoPoint = new Geo::CGeoScutcheon(this->mpRefMapNode);
			pGeoPoint->setGeoPosition(this->mpScutcheon->getGeoPosition());
		}
		return pGeoPoint;
	}

	void IScutcheonEditor::performPickedXYZ(osg::Vec3d pos)
	{

	}

	void IScutcheonEditor::performMovingXYZ(osg::Vec3d pos)
	{

	}

	void IScutcheonEditor::performRightPicked()
	{

	}

	void IScutcheonEditor::performDeleteDrawing()
	{

	}
}