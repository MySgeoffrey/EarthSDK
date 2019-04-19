#include "ModelCreator.h"


CModelCreator::CModelCreator(void)
{
}


CModelCreator::~CModelCreator(void)
{
}



bool createPipelineModel(CPipelineStyle& style,
	osgEarth::GeoPoint& startPoint,osgEarth::GeoPoint& endPoint,
	osg::Vec3d referencePoint,
	double& currentLength,double totalLength,CPipelineData* pData)
{
	bool r = false;

	int originVertexCount = pData->Vertexes.size();
	/*当已知3D坐标点时求的点在earth上的经纬度。
	osgEarth::SpatialReference *MapWGS84SRS = osgEarth::SpatialReference::create("EPSG:4326");
	osgEarth::GeoPoint ConvertPoint;
	ConvertPoint.fromWorld(MapWGS84SRS,3D坐标点);
	ConvertPoint 就是求得的经纬度坐标*/

	osg::Vec3d startWorldPos;
	osg::Vec3d toWorldPos;
	r = startPoint.toWorld(startWorldPos);
	r = endPoint.toWorld(toWorldPos);

	//求旋转方向
	osg::Vec3 tempUpDir = startWorldPos;

	startWorldPos -= referencePoint;
	toWorldPos -= referencePoint;
	osg::Vec3d forwardDir = toWorldPos - startWorldPos;
	double length = forwardDir.length();
	forwardDir.normalize();

	tempUpDir.normalize();
	osg::Vec3d leftDir = forwardDir ^ tempUpDir;
	leftDir.normalize();

	double radius = pData->Style.Radius;
	osg::Vec3d startRotatePoint(0,0,0);
	startRotatePoint = leftDir * radius;

	double pointCount = 20;
	double angle = 360 / pointCount;
	//第一个点
	for (int i = 0; i <= (int)pointCount; ++i)
	{
		float radAngle = osg::DegreesToRadians(angle*i);
		osg::Vec3d curPoint = startRotatePoint * osg::Matrix::rotate(radAngle,forwardDir);

		PositionNormalColorTexture* vertex = new PositionNormalColorTexture();
		vertex->Position = curPoint + startWorldPos;
		vertex->TextureCoor = osg::Vec2d(i / pointCount,currentLength / totalLength);
		vertex->Normal = curPoint;
		vertex->Normal.normalize();
		pData->Vertexes.push_back(vertex);
	}
	
	////第二个点
	//for (int i = 0; i <= (int)pointCount; ++i)
	//{
	//	float radAngle = osg::DegreesToRadians(angle*i);
	//	osg::Vec3d curPoint = startRotatePoint * osg::Matrix::rotate(radAngle,forwardDir);

	//	PositionNormalColorTexture* vertex = new PositionNormalColorTexture();
	//	vertex->Position = curPoint  + toWorldPos;
	//	vertex->TextureCoor = osg::Vec2d(i / pointCount,(currentLength + length) / totalLength);
	//	vertex->Normal = curPoint;
	//	vertex->Normal.normalize();
	//	pData->Vertexes.push_back(vertex);
	//}

	/*int realColCount = 20;
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

			pData->Indices.push_back(index1 + originVertexCount);
			pData->Indices.push_back(index2 + originVertexCount);
			pData->Indices.push_back(index3 + originVertexCount);

			pData->Indices.push_back(index4 + originVertexCount);
			pData->Indices.push_back(index5 + originVertexCount);
			pData->Indices.push_back(index6 + originVertexCount);
		}
	}*/
	currentLength += length;
	return r;
}

bool createPipelineModel(CPipelineData* pData)
{
	bool r = false;
	if (NULL != pData)
	{
		//////////////////////////////////////////////////////////////////////////
		std::vector<osgEarth::GeoPoint>& points = pData->CenterPoints;

		double totalLength = 0;
		double curLength = 0;
		for (int i = 0; i < points.size() - 1;++i)
		{
			osgEarth::GeoPoint& startPoint = points[i];
			osgEarth::GeoPoint& endPoint = points[i+1];

			osg::Vec3d startWorldPoint(0,0,0);
			osg::Vec3d endWorldPoint(0,0,0);
			startPoint.toWorld(startWorldPoint);
			endPoint.toWorld(endWorldPoint);

			totalLength += (endWorldPoint - startWorldPoint).length();

		}
		
		osg::Vec3d referencePoint(0,0,0);
		points[0].toWorld(referencePoint);

		osgEarth::GeoPoint p ;
		p.fromWorld(points[0].getSRS(),referencePoint);

		pData->ReferenceWorldPos = referencePoint;
		for (int i = 0; i < points.size();++i)
		{
			osgEarth::GeoPoint& startPoint = points[i];
			osgEarth::GeoPoint endPoint;
			if (i == points.size()-1)
			{
				endPoint = points[i-1];
			}
			else
			{
				endPoint = points[i+1];
			}
			createPipelineModel(pData->Style,startPoint,endPoint,
				referencePoint,curLength,totalLength,pData);
		}
		int realColCount = 20;
		for (int row = 0; row < points.size()-1; ++row)
		{
			for (int col = 0; col < realColCount; ++col)
			{
				unsigned int index1 = row * (realColCount + 1) + col;
				unsigned int index2 = (row + 1) * (realColCount + 1) + col;
				unsigned int index3 = row * (realColCount + 1) + col + 1;

				unsigned int index4 = row * (realColCount + 1) + col + 1;
				unsigned int index5 = (row + 1) * (realColCount + 1) + col;
				unsigned int index6 = (row + 1) * (realColCount + 1) + col + 1;

				pData->Indices.push_back(index1 + 0);
				pData->Indices.push_back(index2 + 0);
				pData->Indices.push_back(index3 + 0);

				pData->Indices.push_back(index4 + 0);
				pData->Indices.push_back(index5 + 0);
				pData->Indices.push_back(index6 + 0);
			}
		}
	}

	return r;
}

osg::Node* CModelCreator::createPipeLineNode(CPipelineData* pData)
{    
	::createPipelineModel(pData);
	// we'll create a texture mapped quad to sit behind the Geometry 
	/*osg::Image* image = osgDB::readImageFile("D:/Pipe.jpg");
	if (!image) return NULL;*/


	// create Geometry object to store all the vertices and lines primitive.
	osg::Geometry* polyGeom = new osg::Geometry();

	// note, anticlockwise ordering.
	float length = 1;
	float upOffset = length;
#if 0
	osg::Vec3 myCoords[] =
	{
		osg::Vec3(-length + upOffset,0.0f,length + upOffset),
		osg::Vec3(-length + upOffset,0.0f,-length + upOffset),
		osg::Vec3(length + upOffset,0.0f,-length + upOffset),
		osg::Vec3(length + upOffset,0.0f,length + upOffset)
	};
#else
	osg::Vec3* myCoords = new osg::Vec3[pData->Vertexes.size()];
	for (int i = 0; i < pData->Vertexes.size();++i)
	{
		myCoords[i] = pData->Vertexes[i]->Position;
	}
#endif
	int numCoords = sizeof(myCoords)/sizeof(osg::Vec3);
	numCoords = pData->Vertexes.size();

	// pass the created vertex array to the points geometry object.
	polyGeom->setVertexArray(new osg::Vec3Array(numCoords,myCoords));

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
	polyGeom->setColorArray(colors);
	polyGeom->setColorBinding(osg::Geometry::BIND_OVERALL);


	// set the normal in the same way color.
	osg::Vec3Array* normals = new osg::Vec3Array;
	//normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
	for (int i =0; i < pData->Vertexes.size();++i)
	{
		normals->push_back(pData->Vertexes[i]->Normal);
	}
	polyGeom->setNormalArray(normals);
	polyGeom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
#if 0
	osg::Vec2 myTexCoords[] =
	{
		osg::Vec2(0,1),
		osg::Vec2(0,0),
		osg::Vec2(1,0),
		osg::Vec2(1,1)
	};
#else
	osg::Vec2* myTexCoords = new osg::Vec2[pData->Vertexes.size()];
	for (int i = 0; i < pData->Vertexes.size();++i)
	{
		myTexCoords[i] = pData->Vertexes[i]->TextureCoor;
	}
#endif

	int numTexCoords = sizeof(myTexCoords)/sizeof(osg::Vec2);
	numTexCoords = pData->Vertexes.size();

	// pass the created tex coord array to the points geometry object,
	// and use it to set texture unit 0.
	polyGeom->setTexCoordArray(0,new osg::Vec2Array(numTexCoords,myTexCoords));

	// well use indices and DrawElements to define the primitive this time.
	/*unsigned short myIndices[] =
	{
		0,
		1,
		3,
		2
	};*/

#if 0
	unsigned short myIndices[] =
	{
		0,
		1,
		3,
		1,
		3,
		2
	};
#else
	unsigned short* myIndices = new unsigned short[pData->Indices.size()];
	for (int i = 0; i < pData->Indices.size();++i)
	{
		myIndices[i] = pData->Indices[i];
	}

#endif

	int numIndices = sizeof(myIndices)/sizeof(unsigned short);
	numIndices = pData->Indices.size();
	// There are three variants of the DrawElements osg::Primitive, UByteDrawElements which
	// contains unsigned char indices, UShortDrawElements which contains unsigned short indices,
	// and UIntDrawElements which contains ... unsigned int indices.  
	// The first parameter to DrawElements is 
	/*polyGeom->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLE_STRIP,numIndices,myIndices));*/
	polyGeom->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLES,numIndices,myIndices));

	// new we need to add the texture to the Drawable, we do so by creating a 
	// StateSet to contain the Texture2D StateAttribute.
	//osg::StateSet* stateset = new osg::StateSet;

	//// set up the texture.
	//osg::Texture2D* texture = new osg::Texture2D;
	//texture->setImage(image);
	//texture->setWrap(osg::Texture::WRAP_R,osg::Texture::REPEAT);

	//stateset->setTextureAttributeAndModes(0, texture,osg::StateAttribute::ON);

	//polyGeom->setStateSet(stateset);


	// create the Geode (Geometry Node) to contain all our osg::Geometry objects.
	osg::Geode* geode = new osg::Geode();

	// add the points geometry to the geode.
	geode->addDrawable(polyGeom);

	//return geode;

	// create a transform to move the background back and forward with.

	osg::MatrixTransform* transform = new osg::MatrixTransform();
	//transform->setUpdateCallback(new MyTransformCallback(1.0f));
	transform->addChild(geode);

	return transform;
}
