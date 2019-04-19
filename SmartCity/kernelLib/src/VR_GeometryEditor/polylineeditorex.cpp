#include "geometryeditor/polylineeditorex.h"
#include <math.h>

namespace GeometryEditor
{
	osg::Vec3d getBezierPointByRate(osg::Vec3d curvePoint[], float t)
	{
		osg::Vec3d newPoint;
		newPoint.x() = ((1 - t) * (1 - t) * curvePoint[0].x() + 2 * t * (1 - t) * curvePoint[1].x() + t * t * curvePoint[2].x());
		newPoint.y() = ((1 - t) * (1 - t) * curvePoint[0].y() + 2 * t * (1 - t) * curvePoint[1].y() + t * t * curvePoint[2].y());
		return newPoint;
	}

	void getBezierPointsLeft(osg::Vec3d bezierPoints[], std::vector<osg::Vec3d>& linePoints)
	{
		int rate = 20;
		double t, t1, t2, xt, yt;
		for (t = 0; t <= 1; t += 1.0 / rate)
		{
			yt = 1 - t;
			t1 = yt * yt;
			t2 = 3 * yt * t;
			xt = bezierPoints[0].x() * t1 * yt + bezierPoints[1].x() * t2 * yt + bezierPoints[2].x() * t2 * t + bezierPoints[3].x() * t * t * t;
			yt = bezierPoints[0].y() * t1 * yt + bezierPoints[1].y() * t2 * yt + bezierPoints[2].y() * t2 * t + bezierPoints[3].y() * t * t * t;
			linePoints.push_back(osg::Vec3d(xt, yt,0));
		}
	}

	void getBezierPointsRight(osg::Vec3d bezierPoints[], std::vector<osg::Vec3d>& linePoints)
	{
		int rate = 20;
		double t, t1, t2, xt, yt;
		for (t = 1; t >= 0; t -= 1.0 / rate)
		{
			yt = 1 - t;
			t1 = yt * yt;
			t2 = 3 * yt * t;
			xt = bezierPoints[0].x() * t1 * yt + bezierPoints[1].x() * t2 * yt + bezierPoints[2].x() * t2 * t + bezierPoints[3].x() * t * t * t;
			yt = bezierPoints[0].y() * t1 * yt + bezierPoints[1].y() * t2 * yt + bezierPoints[2].y() * t2 * t + bezierPoints[3].y() * t * t * t;
			linePoints.push_back(osg::Vec3d(xt, yt,0));
		}
	}


	IArrowEditor::IArrowEditor(osgEarth::MapNode* pRefMapNode)
		: IPolygonEditor(pRefMapNode)
	{

	}
	
	IArrowEditor::~IArrowEditor()
	{

	}

	void IArrowEditor::performMoving(osg::Vec3d pos)
	{
		//this->getGeometry().push_back(pEditPoint);
		if (this->mpRefCurrentPoint)
		{
			this->mpRefCurrentPoint->getGeoPosition() = pos;
			if (this->getGeometry().size() == 4)
			{
				std::vector<osg::Vec3d> arrowPoints;
				this->createArrow(this->getGeometry(), arrowPoints);
				if (this->mpGeometryNode)
				{
					osg::ref_ptr<osgEarth::Symbology::Geometry> pGeometry = this->mpGeometryNode->getFeature()->getGeometry();
					pGeometry->clear();
					for (int i = 0; i < arrowPoints.size(); ++i)
					{
						pGeometry->push_back(arrowPoints.at(i));
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
			}
		}
		
	}

	bool IArrowEditor::createArrow(
		std::vector<IEditPoint*>& originPoints,
		std::vector<osg::Vec3d>& results)
	{
		osg::Vec3d polyline[4];
		for (int i = 0; i < 4; ++i)
		{
			polyline[i] = originPoints.at(i)->getGeoPosition();
		}
		std::swap(polyline[0], polyline[1]);
		osg::Vec3d curvePoint[4];
		curvePoint[0].x() = (polyline[0].x() + polyline[1].x()) / 2;
		curvePoint[0].y() = (polyline[0].y() + polyline[1].y()) / 2;

		osg::Vec3d pointsLeft[4];
		osg::Vec3d pointsRight[4];
		//左侧贝尔斯曲线起点
		pointsLeft[0] = polyline[0];
		//右侧贝尔斯曲线起点
		pointsRight[0] = polyline[1];

		curvePoint[1].x() = 2 * polyline[2].x() - 0.5 * (curvePoint[0].x() + polyline[3].x());
		curvePoint[1].y() = 2 * polyline[2].y() - 0.5 * (curvePoint[0].y() + polyline[3].y());
		curvePoint[2] = polyline[3];

		osg::Vec3d controlPoints[10];
		//生成箭头控制点
		controlPoints[0] = getBezierPointByRate(curvePoint, 0.900000F);
		controlPoints[1] = getBezierPointByRate(curvePoint, 0.900005F);
		controlPoints[2] = getBezierPointByRate(curvePoint, 0.930000F);
		controlPoints[3] = getBezierPointByRate(curvePoint, 0.930005F);
		controlPoints[4] = getBezierPointByRate(curvePoint, 0.999995F);
		controlPoints[5] = getBezierPointByRate(curvePoint, 1.000000F);
		controlPoints[6] = getBezierPointByRate(curvePoint, 0.333333F);
		controlPoints[7] = getBezierPointByRate(curvePoint, 0.333338F);
		controlPoints[8] = getBezierPointByRate(curvePoint, 0.600000F);
		controlPoints[9] = getBezierPointByRate(curvePoint, 0.600005F);

		double k1 = (controlPoints[1].y() - controlPoints[0].y()) / (controlPoints[1].x() - controlPoints[0].x());
		k1 = -(1 / k1);
		double b1 = controlPoints[0].y() - k1 * controlPoints[0].x();
		double k2 = (controlPoints[3].y() - controlPoints[2].y()) / (controlPoints[3].x() - controlPoints[2].x());
		k2 = -(1 / k2);
		double b2 = controlPoints[2].y() - k2 * controlPoints[2].x();
		double k3 = (controlPoints[5].y() - controlPoints[4].y()) / (controlPoints[5].x() - controlPoints[4].x());

		double angle1 = 30;
		double radians1 = angle1 * (3.1415926 / 180.0);
		double result1_d = std::tan(radians1);
		double result1 = (double)result1_d;

		//斜率为30度的两条直线
		double k31 = (k3 + result1) / (1 - k3 * result1);
		double b31 = controlPoints[5].y() - k31 * controlPoints[5].x();

		double k32 = (k3 - result1) / (1 + k3 * result1);
		double b32 = controlPoints[5].y() - k32 * controlPoints[5].x();

		double angle2 = 15;
		double radians2 = angle2 * (3.1415926 / 180.0);
		double result2_d = std::tan(radians2);
		double result2 = (double)result2_d;

		//斜率为15度的两条直线
		double k33 = (k3 + result2) / (1 - k3 * result2);
		double b33 = controlPoints[5].y() - k33 * controlPoints[5].x();

		double k34 = (k3 - result2) / (1 + k3 * result2);
		double b34 = controlPoints[5].y() - k34 * controlPoints[5].x();

		osg::Vec3d drawPoints[5];
		drawPoints[0].x() = (controlPoints[5].x());
		drawPoints[0].y() = (controlPoints[5].y());

		//30度夹角直线的交点
		drawPoints[1].x() = ((b31 - b1) / (k1 - k31));
		drawPoints[1].y() = ((b1 * k31 - b31 * k1) / (k31 - k1));

		drawPoints[2].x() = ((b32 - b1) / (k1 - k32));
		drawPoints[2].y() = ((b1 * k32 - b32 * k1) / (k32 - k1));

		//15度夹角直线的交点
		drawPoints[3].x() = ((b33 - b2) / (k2 - k33));
		drawPoints[3].y() = ((b2 * k33 - b33 * k2) / (k33 - k2));

		drawPoints[4].x() = ((b34 - b2) / (k2 - k34));
		drawPoints[4].y() = ((b2 * k34 - b34 * k2) / (k34 - k2));

		//添加贝尔斯曲线所需的点
		pointsLeft[3] = drawPoints[4];

		pointsRight[3] = drawPoints[3];

		pointsLeft[1] = curvePoint[1];
		pointsLeft[2] = pointsLeft[3];
		pointsRight[1] = curvePoint[1];
		pointsRight[2] = pointsRight[3];

		std::vector<osg::Vec3d> linePoints;

		//左侧曲线顶点
		getBezierPointsLeft(pointsLeft, linePoints);

		//箭头头部顶点
		linePoints.push_back(drawPoints[2]);
		linePoints.push_back(drawPoints[0]);
		linePoints.push_back(drawPoints[1]);

		//右侧曲线顶点
		getBezierPointsRight(pointsRight, linePoints);

		linePoints.push_back(pointsRight[0]);
		linePoints.push_back(linePoints.at(0));
		results = linePoints;
		return true;
	}

	void IArrowEditor::performPicked(osg::Vec3d pos)
	{
		IEditPoint* pEditPoint = new IEditPoint();
		pEditPoint->setGeoPosition(pos);
		pEditPoint->setIndex(this->getGeometry().size());
		this->getGeometry().push_back(pEditPoint);
		if (this->mpGeometryNode == NULL)
		{
			osgEarth::Features::Feature* pFeature = new osgEarth::Features::Feature(
				new osgEarth::Symbology::Polygon,
				this->mpRefMapNode->getMapSRS());

			this->mpGeometryNode = new osgEarth::Annotation::FeatureNode(
				this->mpRefMapNode, pFeature);
			this->mpGeometryNode->getFeature()->geoInterp() = osgEarth::GEOINTERP_GREAT_CIRCLE;

			this->mpRefMapNode->addChild(this->mpGeometryNode);
		}
		osg::ref_ptr<osgEarth::Symbology::Geometry> pGeometry = this->mpGeometryNode->getFeature()->getGeometry();
		pGeometry->push_back(pos);
		if (this->getGeometry().size() >= 4)
		{
			std::vector<osg::Vec3d> arrowPoints;
			this->createArrow(this->getGeometry(), arrowPoints);
			pGeometry->clear();
			for (int i = 0; i < arrowPoints.size(); ++i)
			{
				pGeometry->push_back(arrowPoints.at(i));
			}


			this->mpGeometryNode->setStyle(this->mGeometryStyle);
			this->mpGeometryNode->init();
		}

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
}