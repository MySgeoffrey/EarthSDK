#include "citybuilder/instancemodellayer.h"
#include "mycharts57datalib/mycharts57datalib.h"
#include "mychartmaplib/imyshaperecord.h"
#include "mychartmaplib/imyproperty.h"
#include "mychartmaplib/imylayer.h"
#include "mycharts57datalib/imys57mapdata.h"
#include "citybuilder/modelcreator.h"
#include <osg/ComputeBoundsVisitor>
#include <osgEarth/DrawInstanced>
#include "pipenet/pipeline.h"
#include "pipenet/pipepoint.h"
#include "pipenet/pipelinedataset.h"

#pragma execution_character_set("utf-8")

using namespace CityBuilder;

void
createDAIGeometry( osg::Geometry& geom, int nInstances=1 )
{
    const float halfDimX( .5 );
    const float halfDimZ( .5 );

    osg::Vec3Array* v = new osg::Vec3Array;
    v->resize( 4 );
    geom.setVertexArray( v );

    // Geometry for a single quad.
    (*v)[ 0 ] = osg::Vec3( -halfDimX, 0., -halfDimZ );
    (*v)[ 1 ] = osg::Vec3( halfDimX, 0., -halfDimZ );
    (*v)[ 2 ] = osg::Vec3( halfDimX, 0., halfDimZ );
    (*v)[ 3 ] = osg::Vec3( -halfDimX, 0., halfDimZ );

    // Use the DrawArraysInstanced PrimitiveSet and tell it to draw 1024 instances.
    geom.addPrimitiveSet( new osg::DrawArrays( GL_QUADS, 0, 4, nInstances ) );
}

osg::Node* createGraphicUnit(const double& radius, int count = 1)
{
	osg::BoundingBox box;
	osg::Vec3d forwardDir(0,0,1);
	double length = forwardDir.length();
	forwardDir.normalize();

	osg::Vec3d leftDir(1,0,0);
	leftDir.normalize();

	osg::Vec3d startRotatePoint = leftDir * radius;

	double pointCount = 20;
	double angle = 360 / pointCount;

	CPipelineData* pData = new CPipelineData();
	//第一个点
	for (int i = 0; i <= (int)pointCount; ++i)
	{
		float radAngle = osg::DegreesToRadians(angle*i);
		osg::Vec3d curPoint = startRotatePoint * osg::Matrix::rotate(radAngle,forwardDir);

		PositionNormalColorTexture* vertex = new PositionNormalColorTexture();
		vertex->Position = curPoint;
		vertex->Normal = curPoint;
		vertex->Normal.normalize();
		pData->Vertexes.push_back(vertex);

		box.expandBy(curPoint);
	}
	
	//第一个点
	for (int i = 0; i <= (int)pointCount; ++i)
	{
		float radAngle = osg::DegreesToRadians(angle*i);
		osg::Vec3d curPoint = startRotatePoint * osg::Matrix::rotate(radAngle,forwardDir);

		PositionNormalColorTexture* vertex = new PositionNormalColorTexture();
		vertex->Position = curPoint + osg::Vec3d(0,0,1);
		vertex->Normal = curPoint;
		vertex->Normal.normalize();
		pData->Vertexes.push_back(vertex);
		box.expandBy(curPoint);
	}

	int realColCount = 20;
	for (int row = 0; row < 1; ++row)
	{
		for (int col = 0; col < realColCount; ++col)
		{
			unsigned int index1 = row * (realColCount + 1) + col;
			unsigned int index2 = (row + 1) * (realColCount + 1) + col;
			unsigned int index3 = row * (realColCount + 1) + col + 1;

			unsigned int index4 = row * (realColCount + 1) + col + 1;
			unsigned int index5 = (row + 1) * (realColCount + 1) + col;
			unsigned int index6 = (row + 1) * (realColCount + 1) + col + 1;

			pData->Indices.push_back(index1);
			pData->Indices.push_back(index2);
			pData->Indices.push_back(index3);

			pData->Indices.push_back(index4);
			pData->Indices.push_back(index5);
			pData->Indices.push_back(index6);
		}
	}
	osg::Geometry* pGeometry = new osg::Geometry();

	osg::Vec3* pVertexes = new osg::Vec3[pData->Vertexes.size()];
	for (int i = 0; i < pData->Vertexes.size();++i)
	{
		pVertexes[i] = pData->Vertexes[i]->Position;
	}
	// pass the created vertex array to the points geometry object.
	pGeometry->setVertexArray(new osg::Vec3Array(pData->Vertexes.size(),pVertexes));

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
	pGeometry->setColorArray(colors);
	pGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	// set the normal in the same way color.
	osg::Vec3Array* normals = new osg::Vec3Array;
	//normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
	for (int i =0; i < pData->Vertexes.size();++i)
	{
		normals->push_back(pData->Vertexes[i]->Normal);
	}
	pGeometry->setNormalArray(normals);
	pGeometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

	unsigned short* pIndices = new unsigned short[pData->Indices.size()];
	for (int i = 0; i < pData->Indices.size();++i)
	{
		pIndices[i] = pData->Indices[i];
	}
	pGeometry->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLES,pData->Indices.size(),pIndices,count));
	osg::Geode* pGeode = new osg::Geode();

	pGeometry->setUseDisplayList( false );
	pGeometry->setUseVertexBufferObjects( true );
	//pGeometry->setInitialBound( box );

	// add the points geometry to the geode.
	pGeode->addDrawable(pGeometry);
	return pGeode;
}

CPipeLayerDriver::CPipeLayerDriver()
{

}

CPipeLayerDriver::~CPipeLayerDriver()
{

}

bool CPipeLayerDriver::loadEx(const std::string& filePath, PipeNet::CPipeLineDataSet*& inout_pDataSet)
{
	bool success = false;
	QString layerDataPath = QString::fromStdString(filePath);
	MyChart::CMyShpDataDriver driver;
	MyChart::IMyLayer* pDataSet = driver.readS57Map(layerDataPath)->getLayers().at(0);
	for (int i = 0; i < pDataSet->getRecords().size(); ++i)
	{
		MyChart::IMyShapeRecord* pRecord = pDataSet->getRecords().at(i);
		if (pRecord && pRecord->Geometry)
		{
			MyChart::IMyPolyline* pLine =
				dynamic_cast<MyChart::IMyPolyline*>(pRecord->Geometry);
			if (pLine->Points.size() >= 2)
			{
				//获取管线ID
				int objectIdIndex = pDataSet->getProperty()->getFieldIndex("OBJECTID");
				int radiusIndex = pDataSet->getProperty()->getFieldIndex("PipeWidth");

				int start84ZIndex = pDataSet->getProperty()->getFieldIndex("StartDeep");
				int end84ZIndex = pDataSet->getProperty()->getFieldIndex("EndDeep");
				//QString objectID = pDataSet->getProperty()->RecordDBFValues[i]->at(objectIdIndex);
				QString radius = pDataSet->getProperty()->RecordDBFValues[i]->at(radiusIndex);
				double startZ = pDataSet->getProperty()->RecordDBFValues[i]->at(start84ZIndex).toDouble();
				double endZ = pDataSet->getProperty()->RecordDBFValues[i]->at(end84ZIndex).toDouble();

				PipeNet::CPipeLine* pPipeLine = new PipeNet::CPipeLine();
				//pPipeLine->setID(objectID.toStdString());
				pPipeLine->getStartGeoPosition() =
					osg::Vec3d(pLine->Points.at(0).X, pLine->Points.at(0).Y, startZ);
				pPipeLine->getEndGeoPosition() =
					osg::Vec3d(pLine->Points.at(1).X, pLine->Points.at(1).Y, endZ);

				/*std::string startID = startPointID.toStdString();
				std::string endID = endPointID.toStdString();
				pPipeLine->getStartPointID() = startID;
				pPipeLine->getEndPointID() = endID;*/
				double r = radius.toDouble() / 1000.0;
				pPipeLine->getRadius() = r;

				if (NULL == inout_pDataSet)
				{
					inout_pDataSet = new PipeNet::CPipeLineDataSet();
				}
				inout_pDataSet->getPipeLines().push_back(pPipeLine);
				success = true;
			}
		}
	}
	return success;
}


bool CPipeLayerDriver::load(const std::string& filePath,PipeNet::CPipeLineDataSet*& inout_pDataSet)
{
	QString layerDataPath = QString::fromStdString(filePath);
	MyChart::CMyShpDataDriver driver;
	MyChart::IMyLayer* pDataSet = driver.readS57Map(layerDataPath)->getLayers().at(0);
	for (int i = 0; i < pDataSet->getRecords().size(); ++i)
	{
		MyChart::IMyShapeRecord* pRecord = pDataSet->getRecords().at(i);
		if (pRecord && pRecord->Geometry)
		{
			MyChart::IMyPolyline* pLine = 
				dynamic_cast<MyChart::IMyPolyline*>(pRecord->Geometry);
			if (pLine->Points.size() >= 2)
			{
				//获取管线ID
				int objectIdIndex = pDataSet->getProperty()->getFieldIndex("OBJECTID");
				int startPointIndex = 1;// pDataSet->getProperty()->getFieldIndex("起点点号");
				int endPointIndex = 2;// pDataSet->getProperty()->getFieldIndex("终点点号");
				int materialIndex = 7;// pDataSet->getProperty()->getFieldIndex("材质");
				int typeIndex = 9;// pDataSet->getProperty()->getFieldIndex("线型");
				int radiusIndex = 10;// pDataSet->getProperty()->getFieldIndex("管径");

				int start84XIndex = 33;// pDataSet->getProperty()->getFieldIndex("START84X");
				int start84YIndex = 34;//pDataSet->getProperty()->getFieldIndex("START84Y");
				int start84ZIndex = 35;//pDataSet->getProperty()->getFieldIndex("START84Z");
				int end84XIndex = 36;//pDataSet->getProperty()->getFieldIndex("END84X");
				int end84YIndex = 37;//pDataSet->getProperty()->getFieldIndex("END84Y");
				int end84ZIndex = 38;//pDataSet->getProperty()->getFieldIndex("END84Z");
				QString objectID = pDataSet->getProperty()->RecordDBFValues[i]->at(objectIdIndex);
				QString startPointID = pDataSet->getProperty()->RecordDBFValues[i]->at(startPointIndex);
				QString endPointID = pDataSet->getProperty()->RecordDBFValues[i]->at(endPointIndex);

				QString material = pDataSet->getProperty()->RecordDBFValues[i]->at(materialIndex);
				QString type = pDataSet->getProperty()->RecordDBFValues[i]->at(typeIndex);
				QString radius = pDataSet->getProperty()->RecordDBFValues[i]->at(radiusIndex);
				double startX = pDataSet->getProperty()->RecordDBFValues[i]->at(start84XIndex).toDouble();
				double startY = pDataSet->getProperty()->RecordDBFValues[i]->at(start84YIndex).toDouble();
				double startZ = 4;// pDataSet->getProperty()->RecordDBFValues[i]->at(start84ZIndex).toDouble();
				double endX = pDataSet->getProperty()->RecordDBFValues[i]->at(end84XIndex).toDouble();
				double endY = pDataSet->getProperty()->RecordDBFValues[i]->at(end84YIndex).toDouble();
				double endZ = 4;//pDataSet->getProperty()->RecordDBFValues[i]->at(end84ZIndex).toDouble();

				PipeNet::CPipeLine* pPipeLine = new PipeNet::CPipeLine();
				pPipeLine->setID(objectID.toStdString());
				pPipeLine->getStartGeoPosition() = 
					osg::Vec3d(startX, startY, startZ);
				pPipeLine->getEndGeoPosition() = 
					osg::Vec3d(endX,endY,endZ);
				
				std::string startID = startPointID.toStdString();
				std::string endID = endPointID.toStdString();
				pPipeLine->getStartPointID() = startID;
				pPipeLine->getEndPointID() = endID;
				double r = radius.toDouble() / 1000.0;
				pPipeLine->getRadius() = r;

				if (NULL == inout_pDataSet)
				{
					inout_pDataSet = new PipeNet::CPipeLineDataSet();
				}
				inout_pDataSet->getPipeLines().push_back(pPipeLine);
			}
		}
	}
	return false;
}

bool CPipeLayerDriver::load(const std::string& filePath,PipeNet::CPipePointDataSet*& inout_pDataSet)
{
	QString layerDataPath = QString::fromStdString(filePath);
	MyChart::CMyShpDataDriver driver;
	MyChart::IMyLayer* pDataSet = driver.readS57Map(layerDataPath)->getLayers().at(0);
	for (int i = 0; i < pDataSet->getRecords().size(); ++i)
	{
		MyChart::IMyShapeRecord* pRecord = pDataSet->getRecords().at(i);
		if (pRecord && pRecord->Geometry)
		{
			MyChart::IMyPoint* pPoint = 
				dynamic_cast<MyChart::IMyPoint*>(pRecord->Geometry);
			if (pPoint)
			{
				int objectIdIndex = 0;//pDataSet->getProperty()->getFieldIndex("OBJECTID");
				int pointIDIndex = 1;// pDataSet->getProperty()->getFieldIndex("管点编号");
				int featureIndex = 6;// pDataSet->getProperty()->getFieldIndex("特征");
				int attachmentIndex = 7;// pDataSet->getProperty()->getFieldIndex("附属物");

				int wgs84XIndex = 31;// pDataSet->getProperty()->getFieldIndex("WGS84X");
				int wgs84YIndex = 32;//pDataSet->getProperty()->getFieldIndex("WGS84Y");
				int wgs84ZIndex = 33;//pDataSet->getProperty()->getFieldIndex("WGS84Z");
				
				QString objectID = pDataSet->getProperty()->RecordDBFValues[i]->at(objectIdIndex);
				QString pointID = pDataSet->getProperty()->RecordDBFValues[i]->at(pointIDIndex);
				QString feature = pDataSet->getProperty()->RecordDBFValues[i]->at(featureIndex);
				QString attachment = pDataSet->getProperty()->RecordDBFValues[i]->at(attachmentIndex);

				double wgs84X = pDataSet->getProperty()->RecordDBFValues[i]->at(wgs84XIndex).toDouble();
				double wgs84Y = pDataSet->getProperty()->RecordDBFValues[i]->at(wgs84YIndex).toDouble();
				double wgs84Z = 4;// pDataSet->getProperty()->RecordDBFValues[i]->at(wgs84ZIndex).toDouble();

				PipeNet::CPipePoint* pPipePoint = new PipeNet::CPipePoint();
				pPipePoint->setID(pointID.toStdString());
				pPipePoint->getGeoPosition() = osg::Vec3d(
					wgs84X, wgs84Y, wgs84Z);
				pPipePoint->getAttachmentType() = attachment.toStdString();
				if (NULL == inout_pDataSet)
				{
					inout_pDataSet = new PipeNet::CPipePointDataSet();
				}
				inout_pDataSet->getPipePoints().push_back(pPipePoint);
			}
		}
	}
	return false;
}

bool CPipeLayerDriver::loadEx(const std::string& filePath, PipeNet::CPipePointDataSet*& inout_pDataSet)
{
	bool r = false;
	QString layerDataPath = QString::fromStdString(filePath);
	MyChart::CMyShpDataDriver driver;
	MyChart::IMyLayer* pDataSet = driver.readS57Map(layerDataPath)->getLayers().at(0);
	for (int i = 0; i < pDataSet->getRecords().size(); ++i)
	{
		MyChart::IMyShapeRecord* pRecord = pDataSet->getRecords().at(i);
		if (pRecord && pRecord->Geometry)
		{
			MyChart::IMyPolyline* pPolyline =
				dynamic_cast<MyChart::IMyPolyline*>(pRecord->Geometry);
			if (pPolyline && pPolyline->Points.size() >= 02)
			{
				int objectIdIndex = pDataSet->getProperty()->getFieldIndex("OBJECTID");
				int radiusIndex = pDataSet->getProperty()->getFieldIndex("PipeWidth");

				int start84ZIndex = pDataSet->getProperty()->getFieldIndex("StartDeep");
				int end84ZIndex = pDataSet->getProperty()->getFieldIndex("EndDeep");
				//QString objectID = pDataSet->getProperty()->RecordDBFValues[i]->at(objectIdIndex);
				QString radius = pDataSet->getProperty()->RecordDBFValues[i]->at(radiusIndex);
				double startZ = pDataSet->getProperty()->RecordDBFValues[i]->at(start84ZIndex).toDouble();
				double endZ = pDataSet->getProperty()->RecordDBFValues[i]->at(end84ZIndex).toDouble();


				PipeNet::CPipePoint* pPipePoint1 = new PipeNet::CPipePoint();
				pPipePoint1->getGeoPosition() = osg::Vec3d(
					pPolyline->Points.at(0).X, pPolyline->Points.at(0).Y,startZ);

				PipeNet::CPipePoint* pPipePoint2 = new PipeNet::CPipePoint();
				pPipePoint2->getGeoPosition() = osg::Vec3d(
					pPolyline->Points.at(1).X, pPolyline->Points.at(1).Y, endZ);
				if (NULL == inout_pDataSet)
				{
					inout_pDataSet = new PipeNet::CPipePointDataSet();
				}
				inout_pDataSet->getPipePoints().push_back(pPipePoint1);
				inout_pDataSet->getPipePoints().push_back(pPipePoint2);
				r = true;
			}
		}
	}
	return r;
}


CInstanceModelLayer::CInstanceModelLayer()
	: Core::ILayer()
{
	this->mpRenderObject = NULL;
	//MyChart::CMyShpDataDriver::initial();
}

CInstanceModelLayer::CInstanceModelLayer(osgEarth::MapNode* pRefMapNode)
	: Core::ILayer(pRefMapNode)
{
	this->mpRenderObject = NULL;
	//MyChart::CMyShpDataDriver::initial();
}

bool CInstanceModelLayer::loadDataFromFile(const std::string& filePath)
{
	QString layerDataPath = QString::fromStdString(filePath);
	MyChart::CMyShpDataDriver driver;
	MyChart::IMyLayer* pDataSet = driver.readS57Map(layerDataPath)->getLayers().at(0);
	this->mpDataSet = pDataSet;
	return this->mpDataSet != NULL;
}

bool CInstanceModelLayer::loadToScene(PipeNet::CPipeLineDataSet* pPipeLineDataSet)
{
#if 1
	osg::Vec4d layerColor(
		rand() % 255 / 255.0,
		rand() % 255 / 255.0,
		rand() % 255 / 255.0,
		1
		);

#else
	osg::Vec4d layerColor(
		1,0,0,1
		);
#endif
	osg::Node* pGraphicUnit = createGraphicUnit(0.5,pPipeLineDataSet->getPipeLines().size());
	osg::BoundingBox layerBox;
	// calculate the overall bounding box for the model:
	osg::ComputeBoundsVisitor cbv;
	pGraphicUnit->accept( cbv );
	const osg::BoundingBox& nodeBox = cbv.getBoundingBox();
	double altOffset = 1;
	osg::Vec3d referenceCenter(0,0,0);
	for (int i = 0; i < pPipeLineDataSet->getPipeLines().size(); ++i)
	{
		PipeNet::CPipeLine* pPipeLine = pPipeLineDataSet->getPipeLines().at(i);
		if (pPipeLine)
		{
			int startDeepthIndex = 3;//this->mpDataSet->getProperty()->getFieldIndex(QString("起点埋深"));
			//double startDeep = this->mpDataSet->getProperty()->RecordDBFValues[i]->at(startDeepthIndex).toDouble();
			int endtDeepthIndex = 4;//this->mpDataSet->getProperty()->getFieldIndex(QString("终点埋深"));
			//double endDeep = this->mpDataSet->getProperty()->RecordDBFValues[i]->at(endtDeepthIndex).toDouble();

			osg::Vec3d worldStartPoint,worldEndPoint;
			osgEarth::GeoPoint geoStartPoint(this->getMapNode()->getMapSRS(),
				pPipeLine->getStartGeoPosition());
			geoStartPoint.toWorld(worldStartPoint);
			osgEarth::GeoPoint geoEndPoint(this->getMapNode()->getMapSRS(),
				pPipeLine->getEndGeoPosition());
			geoEndPoint.toWorld(worldEndPoint);

			if (referenceCenter == osg::Vec3d(0,0,0))
			{
				referenceCenter = worldStartPoint;
			}

			osg::Vec3d worldPosition = worldStartPoint - referenceCenter;
			osg::Vec3d worldDir = worldEndPoint - worldStartPoint;
			double length = worldDir.normalize();
			osg::Quat q;
			q.makeRotate(osg::Vec3d(0,0,1),worldDir);
			osg::Matrix matrix;
			matrix.makeRotate(q);
			double radius = pPipeLine->getRadius();
			matrix = osg::Matrix::scale(osg::Vec3d(radius,radius,length)) * matrix * osg::Matrix::translate(worldPosition);

			if (NULL == mpRenderObject)
			{
				mpRenderObject = new Render::CInstanceObject(this->getMapNode());
			}

			Render::InstanceMatrix instanceMatrix;
			instanceMatrix.Matrix = matrix;
			instanceMatrix.Color = layerColor;

			if (!layerBox.valid())
			{
				layerBox.set(nodeBox.corner(0) * matrix,nodeBox.corner(0) * matrix);
			}
			layerBox.expandBy(nodeBox.corner(0) * matrix);
			layerBox.expandBy(nodeBox.corner(1) * matrix);
			layerBox.expandBy(nodeBox.corner(2) * matrix);
			layerBox.expandBy(nodeBox.corner(3) * matrix);
			layerBox.expandBy(nodeBox.corner(4) * matrix);
			layerBox.expandBy(nodeBox.corner(5) * matrix);
			layerBox.expandBy(nodeBox.corner(6) * matrix);
			layerBox.expandBy(nodeBox.corner(7) * matrix);

			mpRenderObject->getMatrixes().push_back(instanceMatrix);
		}
	}

	osg::MatrixTransform* pMatrixNode = new osg::MatrixTransform();
	pMatrixNode->addChild(pGraphicUnit);
	pGraphicUnit->asGeode()->getDrawable(0)->asGeometry()->setInitialBound(layerBox);
	pGraphicUnit->setInitialBound(layerBox);
	pMatrixNode->setMatrix(osg::Matrix::translate(referenceCenter));
	mpRenderObject->setNode(pMatrixNode);
	mpRenderObject->initial();

	return false;
}

bool CInstanceModelLayer::loadToScene()
{
	if (this->mpDataSet == NULL)
		return false;
#if 1
	osg::Vec4d layerColor(
		rand() % 255 / 255.0,
		rand() % 255 / 255.0,
		rand() % 255 / 255.0,
		1
		);

#else
	osg::Vec4d layerColor(
		1,0,0,1
		);
#endif
	//osg::Node* pGraphicUnit = createGraphicUnit(0.5,this->mpDataSet->getRecords().size());
	//osg::BoundingBox layerBox;
	//// calculate the overall bounding box for the model:
	//osg::ComputeBoundsVisitor cbv;
	//pGraphicUnit->accept( cbv );
	//const osg::BoundingBox& nodeBox = cbv.getBoundingBox();
	//double altOffset = 1;
	//osg::Vec3d referenceCenter(0,0,0);
	//for (int i = 0; i < this->mpDataSet->getRecords().size(); ++i)
	//{
	//	MyChart::IMyShapeRecord* pShapeRecord = this->mpDataSet->getRecords().at(i);
	//	if (pShapeRecord->getGeometryType() == MyChart::GeometryType::GT_POLYLINE)
	//	{
	//		MyChart::IMyPolyline* pGeometry = 
	//			dynamic_cast<MyChart::IMyPolyline*>(pShapeRecord->Geometry);
	//		if (pGeometry && pGeometry->Points.size() >= 2)
	//		{
	//			MyChart::IVector3& startPoint = pGeometry->Points.at(0);
	//			MyChart::IVector3& endPoint = pGeometry->Points.at(1);

	//			int startDeepthIndex = 3;//this->mpDataSet->getProperty()->getFieldIndex(QString("起点埋深"));
	//			double startDeep = this->mpDataSet->getProperty()->RecordDBFValues[i]->at(startDeepthIndex).toDouble();
	//			int endtDeepthIndex = 4;//this->mpDataSet->getProperty()->getFieldIndex(QString("终点埋深"));
	//			double endDeep = this->mpDataSet->getProperty()->RecordDBFValues[i]->at(endtDeepthIndex).toDouble();

	//			osg::Vec3d worldStartPoint,worldEndPoint;
	//			osgEarth::GeoPoint geoStartPoint(this->getMapNode()->getMapSRS(),
	//				osg::Vec3d(startPoint.X,startPoint.Y,startPoint.Z + startDeep + altOffset));
	//			geoStartPoint.toWorld(worldStartPoint);
	//			osgEarth::GeoPoint geoEndPoint(this->getMapNode()->getMapSRS(),
	//				osg::Vec3d(endPoint.X,endPoint.Y,endPoint.Z + endDeep + altOffset));
	//			geoEndPoint.toWorld(worldEndPoint);

	//			if (referenceCenter == osg::Vec3d(0,0,0))
	//			{
	//				referenceCenter = worldStartPoint;
	//			}

	//			osg::Vec3d worldPosition = worldStartPoint - referenceCenter;
	//			osg::Vec3d worldDir = worldEndPoint - worldStartPoint;
	//			double length = worldDir.normalize();
	//			osg::Quat q;
	//			q.makeRotate(osg::Vec3d(0,0,1),worldDir);
	//			osg::Matrix matrix;
	//			matrix.makeRotate(q);
	//			matrix = osg::Matrix::scale(osg::Vec3d(1,1,length)) * matrix * osg::Matrix::translate(worldPosition);
	//			
	//			if (NULL == mpRenderObject)
	//			{
	//				mpRenderObject = new Render::CInstanceObject(this->getMapNode());
	//			}

	//			Render::InstanceMatrix instanceMatrix;
	//			instanceMatrix.Matrix = matrix;
	//			instanceMatrix.Color = layerColor;
	//			
	//			if (!layerBox.valid())
	//			{
	//				layerBox.set(nodeBox.corner(0) * matrix,nodeBox.corner(0) * matrix);
	//			}
	//			layerBox.expandBy(nodeBox.corner(0) * matrix);
	//			layerBox.expandBy(nodeBox.corner(1) * matrix);
	//			layerBox.expandBy(nodeBox.corner(2) * matrix);
	//			layerBox.expandBy(nodeBox.corner(3) * matrix);
	//			layerBox.expandBy(nodeBox.corner(4) * matrix);
	//			layerBox.expandBy(nodeBox.corner(5) * matrix);
	//			layerBox.expandBy(nodeBox.corner(6) * matrix);
	//			layerBox.expandBy(nodeBox.corner(7) * matrix);

	//			mpRenderObject->getMatrixes().push_back(instanceMatrix);
	//		}
	//	}
	//}

	//osg::MatrixTransform* pMatrixNode = new osg::MatrixTransform();
	//pMatrixNode->addChild(pGraphicUnit);
	//pGraphicUnit->asGeode()->getDrawable(0)->asGeometry()->setInitialBound(layerBox);
	//pGraphicUnit->setInitialBound(layerBox);
	//pMatrixNode->setMatrix(osg::Matrix::translate(referenceCenter));
	//mpRenderObject->setNode(pMatrixNode);
	//mpRenderObject->initial();

	return false;
}

bool CInstanceModelLayer::renderToScene()
{
	if (this->mpRenderObject)
	{
		this->mpRenderObject->render();
	}
	return false;
}

bool CInstanceModelLayer::deRenderFromScene()
{
	return false;
}

bool CInstanceModelLayer::unLoadFromScene()
{
	return false;
}