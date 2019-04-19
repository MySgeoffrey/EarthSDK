#include "scenetool/distancemeasure.h"
#include "geographic/geoline.h"
#include "framework/common/languageconfig.h"

namespace SceneTool
{
	static osgEarth::Symbology::Style mTextStyle;
	static osgEarth::Symbology::Style g_DistanceGeometryStyle;
	CDistanceMeasure::CDistanceMeasure(
		osgEarth::MapNode* pRefMapNode, MeasureType measureType) :IPolylineEditor(pRefMapNode)
	{
		mpDistanceContent = NULL;
		mMeasureType = measureType;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->color() = osgEarth::Symbology::Color::Blue;
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

	CDistanceMeasure::~CDistanceMeasure()
	{
		if (mpDistanceContent)
		{
			mpDistanceContent->unLoadFromScene();
			delete mpDistanceContent;
		}
		mpDistanceContent = NULL;
		this->disposeDistanceContents();
	}

	bool CDistanceMeasure::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		bool result = true;
		bool needUpdate = false;
		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::PUSH:
			{
				this->mIsDragState = false;
				break;
			}
		case osgGA::GUIEventAdapter::DRAG:
		case osgGA::GUIEventAdapter::RELEASE:
			{
				if (this->mIsEditState == false)
				{
#if 0
					if (mpDistanceContent)
					{
						mpDistanceContent->unLoadFromScene();
						delete mpDistanceContent;
					}
					mpDistanceContent = NULL;
#else
					this->disposeDistanceContents();
#endif
					this->endEdit();
					this->beginEdit();
				}
				needUpdate = true;
				break;
			}
		}
		result = IPolylineEditor::handle(ea, aa);
		if (needUpdate)
		{
			this->forceListenerEvents(mpCurrentEditingGeometry);
		}
		
		return result;
	}

	void CDistanceMeasure::renderDistanceContents()
	{
		for (int i = 0 ; i < this->mpDistanceContents.size(); ++i)
		{
			this->mpDistanceContents.at(i)->loadToScene();
			this->mpDistanceContents.at(i)->renderToScene();
		}
	}

	void CDistanceMeasure::disposeDistanceContents()
	{
		for (int i = 0 ; i < this->mpDistanceContents.size(); ++i)
		{
			this->mpDistanceContents.at(i)->deRenderFromScene();
			this->mpDistanceContents.at(i)->unLoadFromScene();
			delete this->mpDistanceContents.at(i);
			this->mpDistanceContents.at(i) = NULL;
		}
		this->mpDistanceContents.clear();
	}

	osgEarth::Symbology::Style& CDistanceMeasure::getTextStyle()
	{
		if (mTextStyle.get<osgEarth::Symbology::TextSymbol>() == NULL)
		{
			mTextStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()= "fonts/simhei.ttf";//指定中文字体路径
			mTextStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
			mTextStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = osgEarth::Symbology::Color::Yellow;
			mTextStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 20;
			mTextStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloBackdropType() = osgText::Text::BackdropType::OUTLINE;
			mTextStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloImplementation() =  osgText::Text::BackdropImplementation::STENCIL_BUFFER;
			mTextStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->halo()->color() =osgEarth::Symbology::Color(0,0,0,1);
			mTextStyle.getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral("");//设置点图标显示的图片
		}
		return mTextStyle;
	}

	osgEarth::Symbology::Style& CDistanceMeasure::getGeometryStyle()
	{
		if (g_DistanceGeometryStyle.get<osgEarth::Symbology::LineSymbol>() == NULL)
		{
			g_DistanceGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
				->stroke()->color() = osgEarth::Symbology::Color::Blue;
			g_DistanceGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
				->stroke()->width() = 5.0;
			g_DistanceGeometryStyle.getOrCreate<osgEarth::Symbology::LineSymbol>()
				->tessellation() = 20.0;
			g_DistanceGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
				->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
			g_DistanceGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
				->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;// TECHNIQUE_DRAPE;
			g_DistanceGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
				->verticalOffset() = 0.1;
		}
		return g_DistanceGeometryStyle;
	}

	void CDistanceMeasure::onEvent(Common::IObject* pResult)
	{
#if 0
		GeometryEditor::IEPointSet* pPointSet = dynamic_cast<GeometryEditor::IEPointSet*>(pResult);
		if (NULL == pPointSet)
			return;
		std::vector<GeometryEditor::IEditPoint*> editorPointVector = pPointSet->Data;
		if (editorPointVector.size() < 2)
			return;
		double distance = 0.0;
		for (int index = 0; index < editorPointVector.size(); ++index)
		{
			GeometryEditor::IEditPoint* pEditorPoint = editorPointVector.at(index);
			if (NULL == pEditorPoint)
				continue;
			if (index == 0)
				continue;
			distance += osgEarth::GeoMath::distance(
				pEditorPoint->getGeoPosition(), editorPointVector.at(index - 1)->getGeoPosition(), mpRefMapNode->getMap()->getSRS());

			if (mMeasureType == SPHERE_MT)
				distance *= 1.1;
		}
		if (NULL == mpDistanceContent)
		{
			mpDistanceContent = new Geo::CGeoPoint(mpRefMapNode);osgEarth::Symbology::Style& style = mpDistanceContent->getStyle();
			style.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = osgEarth::Symbology::Color::Yellow;
			style.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 20;
			style.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloBackdropType() = osgText::Text::BackdropType::OUTLINE;
			style.getOrCreate<osgEarth::Symbology::TextSymbol>()->haloImplementation() =  osgText::Text::BackdropImplementation::STENCIL_BUFFER;
			style.getOrCreate<osgEarth::Symbology::TextSymbol>()->halo()->color() =osgEarth::Symbology::Color(0,0,0,1);
			style.getOrCreate<osgEarth::Symbology::TextSymbol>()->pixelOffset() = osg::Vec2s(10,10);
		}
		else
		{
			mpDistanceContent->unLoadFromScene();
		}
		
		char str[200];
		double distance2 =  distance / 1000.0;
		if ( distance2 > 1)
		{
			sprintf(str, "总长:%.1f(公里)", distance2);
		}
		else
		{
			sprintf(str, "总长:%.0f(米)", distance);
		}
		

		mpDistanceContent->setName(str);
		mpDistanceContent->setGeoPosition(editorPointVector.at(editorPointVector.size() - 1)->getGeoPosition());
		mpDistanceContent->loadToScene();
		mpDistanceContent->renderToScene();
#else
		GeometryEditor::IEPointSet* pPointSet = dynamic_cast<GeometryEditor::IEPointSet*>(pResult);
		if (NULL == pPointSet)
			return;
		std::vector<GeometryEditor::IEditPoint*> editorPointVector = pPointSet->Data;
		if (editorPointVector.size() < 2)
			return;
		osgEarth::Symbology::Style& style = getTextStyle();
		double distance = 0.0;
		this->disposeDistanceContents();
		for (int index = 0; index < editorPointVector.size(); ++index)
		{
			GeometryEditor::IEditPoint* pEditorPoint = editorPointVector.at(index);
			if (NULL == pEditorPoint)
				continue;
			if (index == 0)
				continue;
			distance += osgEarth::GeoMath::distance(
				pEditorPoint->getGeoPosition(), editorPointVector.at(index - 1)->getGeoPosition(), mpRefMapNode->getMap()->getSRS());

			if (mMeasureType == SPHERE_MT)
				distance *= 1.1;

			Graphic::CGeoPoint* pDistanceContent = new Graphic::CGeoPoint(mpRefMapNode);
			pDistanceContent->getStyle() = style;
			char str[200];
			double distance2 =  distance / 1000.0;
			if (index == editorPointVector.size() - 1)
			{
				if ( distance2 > 1)
				{
					sprintf(str, "总长:%.1f(公里)", distance2);
				}
				else
				{
					sprintf(str, "总长:%.0f(米)", distance);
				}
			}
			else
			{
				if ( distance2 > 1)
				{
					sprintf(str, "%.1f(公里)", distance2);
				}
				else
				{
					sprintf(str, "%.0f(米)", distance);
				}
			}
			
			pDistanceContent->setName(str);
			pDistanceContent->setGeoPosition(pEditorPoint->getGeoPosition());
			this->mpDistanceContents.push_back(pDistanceContent);
		}
		this->renderDistanceContents();
#endif
	}

	CDistanceMeasureTool::CDistanceMeasureTool(osgEarth::MapNode* pRefMapNode,MeasureType measureType)
		: IGeoGraphicEditor(pRefMapNode),mMeasureType(measureType)
	{
		this->mNeedResetResult = false;
		this->mMouseDownAppendPointMode = true;
		this->mNeedMoveGraphic = false;
		this->setNeedInsertPointByMouse(false);
		this->mLastDistance = 0;
		this->mShowAzimuth = false;
		this->getPointsEditStyle().getOrCreateSymbol<osgEarth::Symbology::PointSymbol>()->size() = 0;
	}

	CDistanceMeasureTool::~CDistanceMeasureTool()
	{

	}

	void CDistanceMeasureTool::setShowAzimuth(const bool& showAzimuth)
	{
		this->mShowAzimuth = showAzimuth;
	}

	bool CDistanceMeasureTool::getShowAzimuth()
	{
		return this->mShowAzimuth;
	}

	void CDistanceMeasureTool::beginEdit(const std::string& graphicType,
		Common::ISceneObject* pGraphicObject,
		const bool& hideOriginGraphicObject)
	{
		this->getGeometryStyle() = CDistanceMeasure::getGeometryStyle();
		this->mHighLightColor = CDistanceMeasure::getGeometryStyle().getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->color();
		this->mHighLightWidth = CDistanceMeasure::getGeometryStyle().getOrCreate<osgEarth::Symbology::LineSymbol>()
			->stroke()->width().get();
		IGeoGraphicEditor::beginEdit(
			Graphic::GT_POLYLINE,NULL);
		Graphic::CGeoLine* pLine = 
			dynamic_cast<Graphic::CGeoLine*>(this->mpEditingObject);
		if (pLine)
		{
			pLine->setUseCustomRenderMode(true);
		}
		if (this->mpEditContent)
		{
			this->mpEditContent->setSelectable(false);
			this->mpEditContent->deRenderFromScene();
			this->mpEditContent->unLoadFromScene();
			int x = 0;
		}
	}

	bool CDistanceMeasureTool::endEdit(
		const bool& clearCurrentGeometry,
		const bool& needSaveEditingResult,
		const bool& hideOriginGraphicObject)
	{
		bool r = IGeoGraphicEditor::endEdit(
			clearCurrentGeometry,needSaveEditingResult,false);
		if (this->mpEditingObject)
		{
			this->mpEditingObject->deRenderFromScene();
			this->mpEditingObject->unLoadFromScene();
			delete this->mpEditingObject;
			this->mpEditingObject = NULL;
		}
		this->disposeDistanceContents();
		if (this->mpEditContent)
		{
			this->mpEditContent->deRenderFromScene();
			this->mpEditContent->unLoadFromScene();
			delete this->mpEditContent;
			this->mpEditContent = NULL;
		}
		return r;
	}

	void CDistanceMeasureTool::performPicked(osg::Vec3d pos)
	{
		if (this->mNeedResetResult)
		{
			if (this->mpPointsEdit != NULL)
			{
				int count = this->mpPointsEdit->getNumChildren();
				this->mpPointsEdit->removeChildren(0, count);
				this->mpPointsEdit->getParent(0)->removeChild(this->mpPointsEdit);
				this->mpPointsEdit = NULL;
			}
			osg::ref_ptr<osgEarth::Symbology::Geometry> pGeometry = this->mpGeometryNode->getFeature()->getGeometry();
			pGeometry->clear();
			this->mpEditingObject->getGeometry().clear();
			this->mNeedResetResult = false;
			IGeoGraphicEditor::performPicked(pos);
			this->updateResult();
		}
		else
		{
			IGeoGraphicEditor::performPicked(pos);
			this->updateResult(false);
		}
	}

	void CDistanceMeasureTool::performMoving(osg::Vec3d pos)
	{
		IGeoGraphicEditor::performMoving(pos);
		//this->updateResult(false);
	}

	void CDistanceMeasureTool::performMovingXYZ(osg::Vec3d pos)
	{
		if (!this->mNeedResetResult)
		{
			IGeoGraphicEditor::performMovingXYZ(pos);
			//this->updateResult(false);
			this->updateTotalLengthText();
		}
	}

	void CDistanceMeasureTool::updateTotalLengthText()
	{
		if (this->mpEditContent)
		{
			this->mpEditContent->deRenderFromScene();
			this->mpEditContent->unLoadFromScene();
			delete this->mpEditContent;
			this->mpEditContent = NULL;
		}
		if (this->mpEditingObject->getGeometry().size() >= 2)
		{
			osg::Vec3d textGeoPosition = osg::Vec3d(0,0,0);
			double distance = 0.0;
			for (int index = 0; 
				index < this->mpEditingObject->getGeometry().size(); ++index)
			{
				if (index == 0)
					continue;
				osg::Vec3d position = this->mpEditingObject->getGeometry().at(index);
				osg::Vec3d prePosition = this->mpEditingObject->getGeometry().at(index-1);
				distance += osgEarth::GeoMath::distance(
					position, prePosition, mpRefMapNode->getMap()->getSRS());
				if (mMeasureType == SPHERE_MT)
					distance *= 1.1;
				textGeoPosition = position;
			}
			double distance2 =  distance / 1000.0;
			char str[200];
			if ( distance2 > 1)
			{
				std::string disTitle = CLanguageConfig::getValue(CLanguageConfig::DistanceTextType_Total);
				std::string disKM = CLanguageConfig::getValue(CLanguageConfig::DistanceTextType_KiloMetre);
				sprintf(str, "%s%.1f%s", disTitle.c_str(),distance2,disKM.c_str());
			}
			else
			{
				std::string disTitle = CLanguageConfig::getValue(CLanguageConfig::DistanceTextType_Total);
				std::string disM = CLanguageConfig::getValue(CLanguageConfig::DistanceTextType_Meter);
				sprintf(str, "%s%.0f%s", disTitle.c_str(),distance,disM.c_str());
			}
			if (this->getShowAzimuth())
			{
				osg::Vec3d beginPoint = this->mpEditingObject->getGeometry().at(this->mpEditingObject->getGeometry().size() - 2);
				osg::Vec3d endPoint = this->mpEditingObject->getGeometry().at(this->mpEditingObject->getGeometry().size() - 1);
				double azimuth = osgEarth::GeoMath::bearing(
					beginPoint.y() * 3.1415926/180,
					beginPoint.x() * 3.1415926/180,
					endPoint.y() * 3.1415926/180,
					endPoint.x() * 3.1415926/180) * 180 / 3.1415926;
				if (azimuth < 0)
				{
					azimuth += 360;
				}
				std::string azimuthTitle = CLanguageConfig::getValue(CLanguageConfig::DistanceTextType_AzimuthLabel);
				std::string azimuthUnit = CLanguageConfig::getValue(CLanguageConfig::DistanceTextType_AzimuthUnit);
				sprintf(str, "%s,%s%.0f%s", str,azimuthTitle.c_str(),azimuth,azimuthUnit.c_str());
			}
			Graphic::CGeoPoint* pDistanceContent = new Graphic::CGeoPoint(mpRefMapNode);
			pDistanceContent->setSelectable(false);
			pDistanceContent->getStyle() = CDistanceMeasure::getTextStyle();
			pDistanceContent->setName(str);
			pDistanceContent->setGeoPosition(textGeoPosition);
			pDistanceContent->loadToScene();
			pDistanceContent->renderToScene();
			this->mpEditContent = pDistanceContent;
		}
	}
	
	void CDistanceMeasureTool::performRightPicked()
	{
		if (this->mNeedResetResult)
		{
			if (this->mpPointsEdit != NULL)
			{
				int count = this->mpPointsEdit->getNumChildren();
				this->mpPointsEdit->removeChildren(0, count);
				this->mpPointsEdit->getParent(0)->removeChild(this->mpPointsEdit);
				this->mpPointsEdit = NULL;
			}
			osg::ref_ptr<osgEarth::Symbology::Geometry> pGeometry = this->mpGeometryNode->getFeature()->getGeometry();
			pGeometry->clear();
			this->mpGeometryNode->init();
			this->mpEditingObject->getGeometry().clear();
			this->mpEditingObject->deRenderFromScene();
			this->mpEditingObject->unLoadFromScene();
			if (this->mpEditContent)
			{
				this->mpEditContent->deRenderFromScene();
				this->mpEditContent->unLoadFromScene();
				delete this->mpEditContent;
				this->mpEditContent = NULL;
			}
			this->disposeDistanceContents();
			this->mNeedResetResult = false;
			this->mLastDistance = 0;
			this->updateResult();
		}
		else
		{
			IGeoGraphicEditor::performRightPicked();
			//this->updateResult(true);
			if (this->mpDistanceContents.size() > 0)
			{
				int index = this->mpDistanceContents.size() - 1;
				this->mpDistanceContents.at(index)->deRenderFromScene();
				this->mpDistanceContents.at(index)->unLoadFromScene();
				delete this->mpDistanceContents.at(index);
				this->mpDistanceContents.erase(
					this->mpDistanceContents.begin() + index);
			}
			this->updateTotalLengthText();
			this->mLastDistance = 0;
			this->mIsEditState = true;
			this->mNeedResetResult = true;
		}
		
	}

	void CDistanceMeasureTool::createEditorCallBack()
	{
		
	}

	void CDistanceMeasureTool::renderDistanceContents()
	{
		for (int i = 0 ; i < this->mpDistanceContents.size(); ++i)
		{
			this->mpDistanceContents.at(i)->loadToScene();
			this->mpDistanceContents.at(i)->renderToScene();
		}
	}

	void CDistanceMeasureTool::disposeDistanceContents()
	{
		for (int i = 0 ; i < this->mpDistanceContents.size(); ++i)
		{
			this->mpDistanceContents.at(i)->deRenderFromScene();
			this->mpDistanceContents.at(i)->unLoadFromScene();
			delete this->mpDistanceContents.at(i);
			this->mpDistanceContents.at(i) = NULL;
		}
		this->mpDistanceContents.clear();
	}

	void CDistanceMeasureTool::updateResult(const bool& rebuildAllText)
	{
		if (this->mpEditContent)
		{
			this->mpEditContent->deRenderFromScene();
			this->mpEditContent->unLoadFromScene();
			delete this->mpEditContent;
			this->mpEditContent = NULL;
		}
		if (this->mpEditingObject->getGeometry().size() >= 2)
		{
			if (rebuildAllText)
			{
				double distance = 0.0;
				this->disposeDistanceContents();
				for (int index = 0; 
					index < this->mpEditingObject->getGeometry().size(); ++index)
				{
					if (index == 0)
						continue;
					osg::Vec3d position = this->mpEditingObject->getGeometry().at(index);
					osg::Vec3d prePosition = this->mpEditingObject->getGeometry().at(index-1);
					distance += osgEarth::GeoMath::distance(
						position, prePosition, mpRefMapNode->getMap()->getSRS());
					if (mMeasureType == SPHERE_MT)
						distance *= 1.1;

					Graphic::CGeoPoint* pDistanceContent = new Graphic::CGeoPoint(mpRefMapNode);
					pDistanceContent->setSelectable(false);
					pDistanceContent->getStyle() = CDistanceMeasure::getTextStyle();
					char str[200];
					double distance2 =  distance / 1000.0;
					if ( distance2 > 1)
					{
						std::string disTitle = CLanguageConfig::getValue(CLanguageConfig::DistanceTextType_Total);
						std::string disKM = CLanguageConfig::getValue(CLanguageConfig::DistanceTextType_KiloMetre);
						sprintf(str, "%s%.1f%s",disTitle.c_str(), distance2,disKM.c_str());
					}
					else
					{
						std::string disTitle = CLanguageConfig::getValue(CLanguageConfig::DistanceTextType_Total);
						std::string disM = CLanguageConfig::getValue(CLanguageConfig::DistanceTextType_Meter);
						sprintf(str, "%s%.0f%s",disTitle.c_str(), distance,disM.c_str());
					}

					pDistanceContent->setName(str);
					pDistanceContent->setGeoPosition(position);
					this->mpDistanceContents.push_back(pDistanceContent);
				}
				this->renderDistanceContents();
			}
			else
			{
				int index = this->mpEditingObject->getGeometry().size() - 1;
		
				osg::Vec3d position = this->mpEditingObject->getGeometry().at(index);
				osg::Vec3d prePosition = this->mpEditingObject->getGeometry().at(index-1);
				double curDistance = osgEarth::GeoMath::distance(
					position, prePosition, mpRefMapNode->getMap()->getSRS());
				this->mLastDistance += curDistance;

				double distance = this->mLastDistance;
				if (mMeasureType == SPHERE_MT)
					distance *= 1.1;

				Graphic::CGeoPoint* pDistanceContent = new Graphic::CGeoPoint(mpRefMapNode);
				pDistanceContent->setSelectable(false);
				pDistanceContent->getStyle() = CDistanceMeasure::getTextStyle();
				char str[200];
				double distance2 =  distance / 1000.0;
				if ( distance2 > 1)
				{
					std::string disTitle = "";
					std::string disKM = CLanguageConfig::getValue(CLanguageConfig::DistanceTextType_KiloMetre);
					sprintf(str, "%s%.1f%s",disTitle.c_str(), distance2,disKM.c_str());
				}
				else
				{
					std::string disTitle = "";
					std::string disM = CLanguageConfig::getValue(CLanguageConfig::DistanceTextType_Meter);
					sprintf(str, "%s%.0f%s",disTitle.c_str(), distance,disM.c_str());
				}
				if (this->getShowAzimuth())
				{
					osg::Vec3d beginPoint = this->mpEditingObject->getGeometry().at(this->mpEditingObject->getGeometry().size() - 1);
					osg::Vec3d endPoint = this->mpEditingObject->getGeometry().at(this->mpEditingObject->getGeometry().size() - 2);
					double azimuth = osgEarth::GeoMath::bearing(
						beginPoint.y() * 3.1415926/180,
						beginPoint.x() * 3.1415926/180,
						endPoint.y() * 3.1415926/180,
						endPoint.x() * 3.1415926/180) * 180 / 3.1415926;
					if (azimuth < 0)
					{
						azimuth += 360;
					}
					std::string azimuthTitle = CLanguageConfig::getValue(CLanguageConfig::DistanceTextType_AzimuthLabel);
					std::string azimuthUnit = CLanguageConfig::getValue(CLanguageConfig::DistanceTextType_AzimuthUnit);
					sprintf(str, "%s,%s%.0f%s", str,azimuthTitle.c_str(),azimuth,azimuthUnit.c_str());
				}
				pDistanceContent->setName(str);
				pDistanceContent->setGeoPosition(position);
				this->mpDistanceContents.push_back(pDistanceContent);

				this->renderDistanceContents();
			}
		}
		else
		{
			this->disposeDistanceContents();
		}
	}
}