#include "scenetool/gridguieventhandle.h"
#include <osgViewer/Viewer>
#include "geoobject/geogrid.h"
#include <osgEarth/GeoMath>
#include "scenetool/azimuthmeasure.h"
#include <osgEarthSymbology/GeometryFactory>
#include <osgEarth/GeoMath>
#include "framework/common/languageconfig.h"

using namespace osgEarth::Symbology;

namespace SceneTool
{
	CGridMeasure::CGridMeasure(osgEarth::MapNode* pRefMapNode,Common::ISceneObject* pRefGridObject)
		:mpRefMapNode(pRefMapNode),mpRefGridObject(pRefGridObject)
	{
		this->mpContent = new Graphic::CGeoPoint(this->mpRefMapNode);
		this->mpContent->getStyle().getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 15.0;//设置字体大小
		this->mpContent->getStyle().getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_LEFT_BOTTOM;
		this->mpContent->getStyle().getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral("");
		this->mDirectionType = 0;
	}

	CGridMeasure::~CGridMeasure()
	{
		if (this->mpContent)
		{
			this->mpContent->deRenderFromScene();
			this->mpContent->unLoadFromScene();
			delete this->mpContent;
			this->mpContent = NULL;
		}
	}

	int& CGridMeasure::getDirectionType()
	{
		return this->mDirectionType;
	}

	osgEarth::Symbology::Style& CGridMeasure::getStyle()
	{
		return this->mpContent->getStyle();
	}

	void CGridMeasure::handleEvent(const osg::Vec3d& mouseGeoPos)
	{
		if (this->mpContent)
		{
			this->mpContent->deRenderFromScene();
			this->mpContent->unLoadFromScene();
		}
		if (mouseGeoPos != osg::Vec3d(0,0,0))
		{
			bool inGrid = false;
			osg::Vec3d center = this->mpRefGridObject->getGeoPosition();
			double distance = osgEarth::GeoMath::distance(center,mouseGeoPos,this->mpRefMapNode->getMapSRS());
			double azimuth = 0;
			azimuth = SceneTool::CAzimuthMeasure::computeAzimuth(center,mouseGeoPos);
			if (this->getDirectionType() == 1)
			{
				azimuth = azimuth / 30.0;
			}
			char distanceStr[200];
			char azimuthStr[200];
			std::string disM = "";
			if (this->getDirectionType() == 0)
			{
				disM = CLanguageConfig::getValue(CLanguageConfig::GeoRectGrid_DistanceMeter);
				sprintf(distanceStr, "%.0f%s", distance,disM.c_str());
				std::string azimuthDegree = CLanguageConfig::getValue(CLanguageConfig::GeoRectGrid_AzimuthDegree);
				sprintf(azimuthStr, "%.0f%s", azimuth,azimuthDegree.c_str());
			}
			else if (this->getDirectionType() == 1)
			{
				disM = CLanguageConfig::getValue(CLanguageConfig::GeoRingGrid_DistanceMeter);
				sprintf(distanceStr, "%.0f%s", distance,disM.c_str());
				std::string azimuthTime = CLanguageConfig::getValue(CLanguageConfig::GeoRingGrid_AzimuthTime);
				sprintf(azimuthStr, "%.0f%s", azimuth,azimuthTime.c_str());
			}
			if (Geo::CGeoRectGrid* pRectGrid = 
				dynamic_cast<Geo::CGeoRectGrid*>(this->mpRefGridObject))
			{
				GeometryFactory factory = GeometryFactory(
					this->mpRefMapNode->getMapSRS()->getGeographicSRS());
				osgEarth::Symbology::Geometry* geometry = factory.createRectangle(
					pRectGrid->getGeoPosition(),
					osgEarth::Distance(pRectGrid->getWidth()),osgEarth::Distance(pRectGrid->getHeight()));
				if (geometry)
				{
					osgEarth::Symbology::Ring geometryRing;
					geometryRing.insert(geometryRing.begin(),geometry->begin(),geometry->end());
					geometryRing.close();
					if (geometryRing.contains2D(mouseGeoPos.x(),mouseGeoPos.y()))
					{
						inGrid = true;
					}
					geometry->clear();
					delete geometry;
					geometry = NULL;
				}
			}
			else if (Geo::CGeoRingGrid* pRingGrid = 
				dynamic_cast<Geo::CGeoRingGrid*>(this->mpRefGridObject))
			{
				inGrid = (distance <= pRingGrid->getRadius());
			}
			if (inGrid)
			{
				std::string disTitle = CLanguageConfig::getValue(CLanguageConfig::GeoRectGrid_DistanceTitle);
				if (this->getDirectionType() == 1)
				{
					disTitle = CLanguageConfig::getValue(CLanguageConfig::GeoRingGrid_DistanceTitle);
				}
				std::string content = disTitle + std::string(distanceStr);
				content += "\n";
				std::string azimuthTitle = CLanguageConfig::getValue(CLanguageConfig::GeoRectGrid_AzimuthTitle);
				if (this->getDirectionType() == 1)
				{
					azimuthTitle = CLanguageConfig::getValue(CLanguageConfig::GeoRingGrid_AzimuthTitle);
				}
				content += azimuthTitle + std::string(azimuthStr);
				this->mpContent->setName(content);
				this->mpContent->setGeoPosition(mouseGeoPos);
				this->mpContent->loadToScene();
				this->mpContent->renderToScene();
			}
		}
	}

	bool CGridMeasure::handle(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& aa)
	{
		if (this->mpRefGridObject == NULL 
			|| this->mpRefMapNode == NULL)
			return false;
		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::MOVE://鼠标移动
			{
				osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
				osg::Vec3d curGeoPos = osg::Vec3d(0, 0, 0);
				// 获取当前点
				osgUtil::LineSegmentIntersector::Intersections intersection;
				double x = ea.getX();
				double y = ea.getY();
				pViewer->computeIntersections(ea.getX(), ea.getY(), intersection);
				osgUtil::LineSegmentIntersector::Intersections::iterator iter
					= intersection.begin();
				if (iter != intersection.end())
				{
					this->mpRefMapNode->getMapSRS()->getEllipsoid()->convertXYZToLatLongHeight(
						iter->getWorldIntersectPoint().x(), iter->getWorldIntersectPoint().y(), iter->getWorldIntersectPoint().z(),
						curGeoPos.y(), curGeoPos.x(), curGeoPos.z());
					curGeoPos.x() = osg::RadiansToDegrees(curGeoPos.x());
					curGeoPos.y() = osg::RadiansToDegrees(curGeoPos.y());
					curGeoPos.z() = 0;
				}
				this->handleEvent(curGeoPos);
				break;
			}
		}
		return false;
	}

}
