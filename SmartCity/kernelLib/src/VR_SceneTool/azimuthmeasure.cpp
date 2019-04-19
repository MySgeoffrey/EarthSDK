#include "scenetool/azimuthmeasure.h"
#include <osgEarthSymbology/GeometryFactory>
#include "framework/common/languageconfig.h"

using namespace GeometryEditor;

namespace SceneTool
{
	static osgEarth::Symbology::Style mTextStyle;
	CAzimuthMeasure::CAzimuthMeasure(
		osgEarth::MapNode* pRefMapNode) :IPolylineEditor(pRefMapNode)
	{
		mpAzimuthContent = NULL;
		mIsEnd = false;
		this->mMouseDownAppendPointMode = true;
		this->mNeedMoveGraphic = false;
	}

	CAzimuthMeasure::~CAzimuthMeasure()
	{
		if (mpAzimuthContent)
		{
			mpAzimuthContent->unLoadFromScene();
			delete mpAzimuthContent;
		}
		mpAzimuthContent = NULL;
	}

	double CAzimuthMeasure::computeAzimuth(
			const osg::Vec3d& beginPoint,
			const osg::Vec3d& endPoint)
	{
		double azimuth = osgEarth::GeoMath::bearing(
			beginPoint.y() * 3.1415926/180,
			beginPoint.x() * 3.1415926/180,
			endPoint.y() * 3.1415926/180,
			endPoint.x() * 3.1415926/180);
		azimuth = azimuth * 180 / 3.1415926;//将方位角从弧度转换成角度
		if (azimuth < 0)
		{
			azimuth += 360;
		}
		return azimuth;
	}

	void CAzimuthMeasure::performPicked(osg::Vec3d pos)
	{
		if (this->getGeometry().size() >= 2)
			return ;
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

#if 1 //多加一条线
		osg::Vec3d startPoint(this->getGeometry().at(0)->getGeoPosition());
		osg::Vec3d startWorldPos;
		osg::Vec3d endWorldPos;
		osgEarth::GeoPoint geoStartPoint(mpRefMapNode->getMapSRS(), startPoint, osgEarth::ALTMODE_ABSOLUTE);
		geoStartPoint.toWorld(startWorldPos);
		osg::Vec3d zAxis(0, 0, 1);
		osg::Vec3d upDir = startWorldPos;
		upDir.normalize();
		osg::Vec3d leftDir = upDir ^ zAxis;
		leftDir.normalize();
		osg::Vec3d northDir = leftDir ^ upDir;
		northDir.normalize();

		osg::Vec3d northPoint = startWorldPos + northDir * 100000;
		osgEarth::GeoPoint geoNorhPoint;
		geoNorhPoint.fromWorld(mpRefMapNode->getMapSRS(),northPoint);

		if (this->getGeometry().size() == 1)
		{
			pGeometry->clear();
			pGeometry->push_back(geoNorhPoint.vec3d());
			pGeometry->push_back(startPoint);
		}
		else if (this->getGeometry().size() == 2)
		{
			osg::Vec3d endPoint(this->getGeometry().at(1)->getGeoPosition());
			osgEarth::GeoPoint geoEndPoint(mpRefMapNode->getMapSRS(), endPoint, osgEarth::ALTMODE_ABSOLUTE);
			geoEndPoint.toWorld(endWorldPos);

			osg::Vec3d curDir = endWorldPos - startWorldPos;
			double length = curDir.normalize();
			northPoint = startWorldPos + northDir * length;
			geoNorhPoint.fromWorld(mpRefMapNode->getMapSRS(),northPoint);

			pGeometry->clear();

			double azimuth = osgEarth::GeoMath::bearing(
				this->getGeometry().at(0)->getGeoPosition().y() * 3.1415926/180,
				this->getGeometry().at(0)->getGeoPosition().x() * 3.1415926/180,
				this->getGeometry().at(1)->getGeoPosition().y() * 3.1415926/180,
				this->getGeometry().at(1)->getGeoPosition().x() * 3.1415926/180) * 180 / 3.1415926;
			if (azimuth < 0)
			{
				azimuth += 360;
			}
			//计算arc上点
			double arcRadius = length / 10.0;
			osgEarth::Symbology::GeometryFactory factory = osgEarth::Symbology::GeometryFactory(this->mpRefMapNode->getMapSRS()->getGeographicSRS());
			osgEarth::Symbology::Geometry* pArc = factory.createArc(
				this->getGeometry().at(0)->getGeoPosition(), arcRadius,
					0, azimuth, 0L, 0L, true);

			pGeometry->push_back(geoNorhPoint.vec3d());
			pGeometry->push_back(startPoint);
			if (pArc)
			{
				for (int n = 0 ; n < pArc->size(); ++n)
				{
					pGeometry->push_back(pArc->at(n));
				}
				pGeometry->push_back(startPoint);
				pArc->clear();
				delete pArc;
				pArc = NULL;
			}
			pGeometry->push_back(endPoint);
		}
#endif

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

	void CAzimuthMeasure::performMoving(osg::Vec3d pos)
	{
		if (NULL == this->mpGeometryNode)
			return ;
		if (this->getGeometry().size() < 1)
			return;
#if 1 //多加一条线
		osgEarth::Symbology::Geometry* pGeometry = this->mpGeometryNode->getFeature()->getGeometry();
		osg::Vec3d startPoint(this->getGeometry().at(0)->getGeoPosition());
		osg::Vec3d startWorldPos;
		osg::Vec3d endWorldPos;
		osgEarth::GeoPoint geoStartPoint(mpRefMapNode->getMapSRS(), startPoint, osgEarth::ALTMODE_ABSOLUTE);
		geoStartPoint.toWorld(startWorldPos);
		osg::Vec3d zAxis(0, 0, 1);
		osg::Vec3d upDir = startWorldPos;
		upDir.normalize();
		osg::Vec3d leftDir = upDir ^ zAxis;
		leftDir.normalize();
		osg::Vec3d northDir = leftDir ^ upDir;
		northDir.normalize();

		osg::Vec3d northPoint = startWorldPos + northDir * 100000;
		osgEarth::GeoPoint geoNorhPoint;
		geoNorhPoint.fromWorld(mpRefMapNode->getMapSRS(),northPoint);

		if (this->getGeometry().size() == 1)
		{
			pGeometry->clear();
			pGeometry->push_back(geoNorhPoint.vec3d());
			pGeometry->push_back(startPoint);
		}
		else if (this->getGeometry().size() == 2)
		{
			osg::Vec3d endPoint(this->getGeometry().at(1)->getGeoPosition());
			osgEarth::GeoPoint geoEndPoint(mpRefMapNode->getMapSRS(), endPoint, osgEarth::ALTMODE_ABSOLUTE);
			geoEndPoint.toWorld(endWorldPos);

			osg::Vec3d curDir = endWorldPos - startWorldPos;
			double length = curDir.normalize();
			northPoint = startWorldPos + northDir * length;
			geoNorhPoint.fromWorld(mpRefMapNode->getMapSRS(),northPoint);

			pGeometry->clear();

			double azimuth = osgEarth::GeoMath::bearing(
				this->getGeometry().at(0)->getGeoPosition().y() * 3.1415926/180,
				this->getGeometry().at(0)->getGeoPosition().x() * 3.1415926/180,
				this->getGeometry().at(1)->getGeoPosition().y() * 3.1415926/180,
				this->getGeometry().at(1)->getGeoPosition().x() * 3.1415926/180) * 180 / 3.1415926;
			if (azimuth < 0)
			{
				azimuth += 360;
			}
			//计算arc上点
			double arcRadius = length / 10.0;
			osgEarth::Symbology::GeometryFactory factory = osgEarth::Symbology::GeometryFactory(this->mpRefMapNode->getMapSRS()->getGeographicSRS());
			osgEarth::Symbology::Geometry* pArc = factory.createArc(
				this->getGeometry().at(0)->getGeoPosition(), arcRadius,
					0, azimuth, 0L, 0L, true);

			pGeometry->push_back(geoNorhPoint.vec3d());
			pGeometry->push_back(startPoint);
			if (pArc)
			{
				for (int n = 0 ; n < pArc->size(); ++n)
				{
					pGeometry->push_back(pArc->at(n));
				}
				pGeometry->push_back(startPoint);
				pArc->clear();
				delete pArc;
				pArc = NULL;
			}
			pGeometry->push_back(endPoint);
		}
#endif
		
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

	osgEarth::Symbology::Style& CAzimuthMeasure::getTextStyle()
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

	bool CAzimuthMeasure::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		if (this->getGeometry().size() > 2)
			return false;
		bool result = true;
		bool needUpdate = false;
		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::PUSH:
			{
				if (this->mIsEditState == false)
				{
					if (mpAzimuthContent)
					{
						mpAzimuthContent->unLoadFromScene();
						delete mpAzimuthContent;
					}
					mpAzimuthContent = NULL;
					this->endEdit();
					this->beginEdit();
				}
				break;
			}
		case osgGA::GUIEventAdapter::DRAG:
		case osgGA::GUIEventAdapter::RELEASE:
			{
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

	void CAzimuthMeasure::onEvent(Common::IObject* pResult)
	{
		GeometryEditor::IEPointSet* pPointSet = dynamic_cast<GeometryEditor::IEPointSet*>(pResult);
		if (NULL == pPointSet)
			return;
		std::vector<GeometryEditor::IEditPoint*> editorPointVector = pPointSet->Data;
		if (editorPointVector.size() < 2)
			return;

		double azimuth = 0;
		
		osg::Vec3d beginWPoint, endWPoint;
		GeometryEditor::IEditPoint* pBeginEditorPoint = editorPointVector.at(0);
		osgEarth::GeoPoint beginPoint(mpRefMapNode->getMapSRS(), pBeginEditorPoint->getGeoPosition(), osgEarth::ALTMODE_ABSOLUTE);
		beginPoint.toWorld(beginWPoint);
		GeometryEditor::IEditPoint* pEndEditorPoint = editorPointVector.at(1);
		osgEarth::GeoPoint endPoint(mpRefMapNode->getMapSRS(), pEndEditorPoint->getGeoPosition(), osgEarth::ALTMODE_ABSOLUTE);
		endPoint.toWorld(endWPoint);
		osg::Vec3d dir = endWPoint - beginWPoint;
		double length = dir.normalize();
		azimuth = osgEarth::GeoMath::bearing(
			pBeginEditorPoint->getGeoPosition().y() * 3.1415926/180,
			pBeginEditorPoint->getGeoPosition().x() * 3.1415926/180,
			pEndEditorPoint->getGeoPosition().y() * 3.1415926/180,
			pEndEditorPoint->getGeoPosition().x() * 3.1415926/180) * 180 / 3.1415926;
		if (azimuth < 0)
		{
			azimuth += 360;
		}
		osg::Vec3d zAxis(0, 0, 1);
		osg::Vec3d upDir = beginWPoint;
		upDir.normalize();
		osg::Vec3d leftDir = upDir ^ zAxis;
		leftDir.normalize();
		osg::Vec3d northDir = leftDir ^ upDir;
		northDir.normalize();

		if (NULL == mpAzimuthContent)
		{
			mpAzimuthContent = new Graphic::CGeoPoint(mpRefMapNode);
			osgEarth::Symbology::Style& style = mpAzimuthContent->getStyle();
			style = getTextStyle();
		}
		else
		{
			mpAzimuthContent->unLoadFromScene();
		}
		
		std::string azimuthTitle = CLanguageConfig::getValue(CLanguageConfig::AzimuthTextType_Title);
		std::string azimuthDegree = CLanguageConfig::getValue(CLanguageConfig::AzimuthTextType_Degree);

		char str[200];
		sprintf(str, "%s%.0f%s", azimuthTitle.c_str(),azimuth,azimuthDegree.c_str());
	
		mpAzimuthContent->setName(str);
		mpAzimuthContent->setGeoPosition(editorPointVector.at(0)->getGeoPosition());
		mpAzimuthContent->loadToScene();
		mpAzimuthContent->renderToScene();
	}
}
