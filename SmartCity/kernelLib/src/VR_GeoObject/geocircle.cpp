//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : ���Ͽռ���Ϣƽ̨
//  @ File Name : geopoint.cpp
//  @ Date : 2017/8/21
//  @ Author : 
//
//


#include "geoobject/geocircle.h"
#include <osg/Geometry>
#include <osgEarthSymbology/Geometry>
#include <osgEarthFeatures/Feature>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarth/ElevationQuery>
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/AnnotationEditing>
#include <osgEarthSymbology/GeometryFactory>
#include <osgEarthSymbology/Geometry>
#include <osgEarth/Registry>
#include <osgEarthFeatures/Feature>
#include <osgEarthAnnotation/FeatureNode>
#include <osgEarth/DrapeableNode>
//#include "earth/DrapeableNode.h"

using namespace osg;
using namespace osgEarth::Symbology;

#define  d2r 3.1415926535898 / 180.0

namespace Geo
{
	class CGeoCircleExCallback
		: public osg::NodeCallback
	{
    public:

		CGeoCircleExCallback()
		{
			Owner = NULL;
		}

		~CGeoCircleExCallback()
		{
		
		}

		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
		{
			if (Owner)
			{
				Owner->update(node,nv);
			}
			traverse(node,nv);
		}
	public:
		CGeoCircleEx* Owner;
    };

	IGeoCircle::IGeoCircle()
	{
		this->mRadius = 1000;
		this->mBeginAngle = 0;
		this->mEndAngle = 360;
		Style& circleStyle = this->getStyle();
		circleStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Red, 0.5);
		//circleStyle.getOrCreate<LineSymbol>()->stroke()->color() = Color::Red;
		//circleStyle.getOrCreate<LineSymbol>()->stroke()->width() = 3.0f;
		circleStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;// CLAMP_TO_TERRAIN;// ;
		circleStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;//DRAPE
		circleStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->verticalOffset() = 0.1;
	}

	IGeoCircle::IGeoCircle(osgEarth::MapNode* pRefMapNode)
		:  IGeoPoint(pRefMapNode)
	{
		this->mRadius = 1000;
		this->mBeginAngle = 0;
		this->mEndAngle = 360;
		Style& circleStyle = this->getStyle();
		circleStyle.getOrCreate<PolygonSymbol>()->fill()->color() = Color(Color::Red, 0.5);
	/*	circleStyle.getOrCreate<LineSymbol>()->stroke()->color() = Color::Red;
		circleStyle.getOrCreate<LineSymbol>()->stroke()->width() = 3.0f;*/
		circleStyle.getOrCreate<AltitudeSymbol>()->clamping() = AltitudeSymbol::CLAMP_TO_TERRAIN;// CLAMP_TO_TERRAIN;// ;
		circleStyle.getOrCreate<AltitudeSymbol>()->technique() = AltitudeSymbol::TECHNIQUE_DRAPE;//DRAPE
		circleStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->verticalOffset() = 0.1;
	}

	IGeoCircle::~IGeoCircle()
	{

	}

	
	void IGeoCircle::setBeginAngle(const double& beginAngle)
	{
		this->mBeginAngle = beginAngle;
	}

	void IGeoCircle::setEndAngle(const double& endAngle)
	{
		this->mEndAngle = endAngle;
	}

	double IGeoCircle::getBeginAngle()
	{
		return this->mBeginAngle;
	}

	double IGeoCircle::getEndAngle()
	{
		return this->mEndAngle;
	}

	bool IGeoCircle::loadToScene()
	{
		bool r = false;

		GeometryFactory factory = GeometryFactory(this->getMapNode()->getMapSRS()->getGeographicSRS());
		
		osgEarth::Symbology::Geometry* geometry = NULL;
		if (std::abs(this->mEndAngle - this->mBeginAngle) >= 360.0)
		{
			geometry = factory.createCircle(this->getGeoPosition(), this->getRadius(), 0L);
		}
		else
		{
			geometry = factory.createArc(this->getGeoPosition(), this->getRadius(),
				this->mBeginAngle, this->mEndAngle, 0L, 0L, true);
		}
		osgEarth::Symbology::Geometry* path = new osgEarth::Symbology::Polygon();
		for (int i = 0; i < geometry->size(); ++i)
		{
			path->push_back(geometry->at(i));
		}
		delete geometry;
		geometry = NULL;
		Style& geomStyle = this->getStyle();
		osgEarth::Features::Feature* pathFeature = new osgEarth::Features::Feature(path, this->mpRefMapNode->getMapSRS(), geomStyle);
		pathFeature->geoInterp() = GEOINTERP_GREAT_CIRCLE;
		osgEarth::Annotation::FeatureNode* pathNode = 
			new osgEarth::Annotation::FeatureNode(this->mpRefMapNode, pathFeature,
			geomStyle, osgEarth::Features::GeometryCompilerOptions(),NULL);
		if (NULL == this->mpLod)
		{
			this->mpLod = new osg::LOD();
		}
		this->mpLod->addChild(pathNode, 0, 20000000);
		r = true;
		return r;
	}

	bool IGeoCircle::renderToScene()
	{
		bool r = false;
		if (this->mpLod && this->mpLod->getNumParents() == 0)
		{
			this->mpRefMapNode->addChild(this->mpLod);
			r = true;
		}
		return r;
	}

	bool IGeoCircle::deRenderFromScene()
	{

		return false;
	}

	bool IGeoCircle::unLoadFromScene()
	{
		bool r = false;
		if (this->mpLod && this->mpLod->getNumParents() > 0)
		{
			this->mpLod->getParent(0)->removeChild(this->mpLod);
			this->mpLod = NULL;
			r = true;
		}
		return r;
	}

	void IGeoCircle::setRadius(const double& radius)
	{
		this->mRadius = radius;
	}

	double IGeoCircle::getRadius()
	{
		return this->mRadius;
	}


	osgEarth::Symbology::Style& IGeoCircle::getStyle()
	{
		return this->mStyle;
	}

	CGeoCircleEx::CGeoCircleEx()
		:IGeoCircle()
	{
		this->mType = "geoCircleex";
		this->mUseGradualTransparent = false;
		this->mStartTransparent = 0.2;
		this->mEndTransparent = 0.8;
		this->mWorldMatrix = osg::Matrix::identity();
		this->mRotateAngle = 0;
		this->mRotateFactor = 1.0;
		this->mpUpdateCallback= NULL;
		this->mRotateEnable = false;
		this->mDirectionType = CGeoCircleEx::DT_ClockWise;
		this->mClampGroundMode = false;
	}

	CGeoCircleEx::CGeoCircleEx(osgEarth::MapNode* pRefMapNode)
		: IGeoCircle(pRefMapNode)
	{
		this->mType = "geoCircleex";
		this->mUseGradualTransparent = false;
		this->mStartTransparent = 0.2;
		this->mEndTransparent = 0.8;
		this->mWorldMatrix = osg::Matrix::identity();
		this->mRotateAngle = 0;
		this->mRotateFactor = 1.0;
		this->mpUpdateCallback= NULL;
		this->mRotateEnable = false;
		this->mDirectionType = CGeoCircleEx::DT_ClockWise;
		this->mClampGroundMode = false;
	}

	CGeoCircleEx::~CGeoCircleEx()
	{
	
	}
	

	bool& CGeoCircleEx::useGradualTransparent()
	{
		return this->mUseGradualTransparent;
	}

	double& CGeoCircleEx::startTransparent()
	{
		return this->mStartTransparent;
	}

	double CGeoCircleEx::endTransparent()
	{
		return this->mEndTransparent;
	}

	bool CGeoCircleEx::loadToScene()
	{
		bool r = false;
		if (this->mpLod)
			return r;
        double segLen = this->getRadius() / 100.0;
        double circumference = 2 * osg::PI * this->getRadius();
        double numSegments = (unsigned)::ceil(circumference / segLen);
		osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;//���ζ���
		osg::ref_ptr<osg::Vec3Array> centerVertices = new osg::Vec3Array;//���ζ���
		double startAngle = this->getBeginAngle() * d2r;
		double endAngle = this->getEndAngle() * d2r;
		double deltaAngle = (endAngle - startAngle) / numSegments;
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
		
		osg::Vec4 color(1,0,0,0.5);
		if (this->getStyle().get<osgEarth::Symbology::PolygonSymbol>())
		{
			color = this->getStyle().getOrCreate<osgEarth::Symbology::PolygonSymbol>()->fill()->color();
		}
	
		for (int i = 0; i <= numSegments; ++i)
		{
			double radAngle = startAngle + i * deltaAngle;
			osg::Vec3d top = osg::Vec3(sin(radAngle), cos(radAngle), 0);
			vertices->push_back(top);
			if (useGradualTransparent())
			{
				double alphaFactor = (endTransparent() - startTransparent()) / numSegments;
				color.a() = startTransparent() + i * alphaFactor;
			}
			
			colors->push_back(color);
		}
		for (int i = 0; i <= numSegments; ++i)
		{
			vertices->push_back(osg::Vec3(0,0,0));
			if (useGradualTransparent())
			{
				double alphaFactor = (endTransparent() - startTransparent()) / numSegments;
				color.a() = startTransparent() + i * alphaFactor;
			}
			colors->push_back(color);
		}
		//
		std::vector<unsigned short> indices;
		int realColCount = numSegments;
		for (int row = 0; row < 1; ++row)
		{
			for (int col = 0; col < numSegments; ++col)
			{
				unsigned int index1 = row * (realColCount + 1) + col;
				unsigned int index2 = (row + 1) * (realColCount + 1) + col;
				unsigned int index3 = row * (realColCount + 1) + col + 1;

				unsigned int index4 = row * (realColCount + 1) + col + 1;
				unsigned int index5 = (row + 1) * (realColCount + 1) + col;
				unsigned int index6 = (row + 1) * (realColCount + 1) + col + 1;

				indices.push_back(index1 + 0);
				indices.push_back(index2 + 0);
				indices.push_back(index3 + 0);
				indices.push_back(index4 + 0);
				indices.push_back(index5 + 0);
				indices.push_back(index6 + 0);
			}
		}
		unsigned short* pIndices = new unsigned short[indices.size()];
		for (int i = 0; i < indices.size(); ++i)
		{
			pIndices[i] = indices[i];
		}
		int numIndices = sizeof(pIndices) / sizeof(unsigned short);
		numIndices = indices.size();
		osg::ref_ptr<osg::Geometry>geomTri = new osg::Geometry;
		geomTri->setColorArray(colors.get());
		geomTri->setVertexArray(vertices.get());
		geomTri->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

		geomTri->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLES, numIndices, pIndices));
		osg::StateSet* stateset = new osg::StateSet;  //͸����������
		stateset->setMode(GL_BLEND, osg::StateAttribute::ON); //Alpha��Ͽ��� 
		stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF); //ȡ����Ȳ��� 
		stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF /*| osg::StateAttribute::PROTECTED*/);
		stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		stateset->setRenderBinDetails(5, "RenderBin");

		osg::Geode* circle = new osg::Geode;//����ڵ�
		geomTri->setStateSet(stateset);
		circle->addDrawable(geomTri.get());
		circle->addDescription(std::string("Circle"));

		osg::ref_ptr<osg::MatrixTransform> pRealCircle = new osg::MatrixTransform;
		pRealCircle->addChild(circle);
		if (this->getRotateEnable())
		{
			this->mpUpdateCallback = new CGeoCircleExCallback();
			this->mpUpdateCallback->Owner = this;
			pRealCircle->setUpdateCallback(this->mpUpdateCallback);
		}
		osg::Matrixd worldMatrix;
		this->getMapNode()->getMap()->getSRS()->getEllipsoid()->computeLocalToWorldTransformFromLatLongHeight(
			osg::DegreesToRadians(this->getGeoPosition().y()),
			osg::DegreesToRadians(this->getGeoPosition().x()),
			this->getGeoPosition().z(), worldMatrix);
		this->mWorldMatrix = osg::Matrix::scale(this->getRadius(),this->getRadius(),1) * worldMatrix;
		pRealCircle->setMatrix(this->mWorldMatrix);
		
		osg::ref_ptr<CGeoCircleEx> object = this;
		this->mObjectId = osgEarth::Registry::objectIndex()->tagNode(pRealCircle, object);

		if (this->getClampGroundMode())
		{
			osgEarth::DrapeableNode* d = new osgEarth::DrapeableNode();
			d->addChild( pRealCircle );
			osg::LOD* pLod = new osg::LOD();
			pLod->addChild(d, 0, 10000000000);
			this->mpLod = pLod;
		}
		else
		{	
			osg::LOD* pLod = new osg::LOD();
			pLod->addChild(pRealCircle, 0, 10000000000);
			this->mpLod = pLod;
		}
		r = true;
		return r;
	}

	bool CGeoCircleEx::renderToScene()
	{
		bool r = false;
		if (this->mpLod && this->mpLod->getNumParents() == 0)
		{
			this->mpRefMapNode->addChild(this->mpLod);
			r = true;
		}
		return r;
	}

	bool CGeoCircleEx::deRenderFromScene()
	{
		return false;
	}

	bool CGeoCircleEx::unLoadFromScene()
	{
		bool r = false;
		if (this->mpLod && this->mpLod->getNumParents() > 0)
		{
			this->mpLod->getParent(0)->removeChild(this->mpLod);
			this->mpLod = NULL;
			r = true;
		}
		return r;
	}

	void CGeoCircleEx::setRotateFactor(const double& rotateFactor)
	{
		this->mRotateFactor = rotateFactor; 
	}

	double CGeoCircleEx::getRotateFactor()
	{
		return this->mRotateFactor;
	}

	void CGeoCircleEx::setRotateDirection(const CGeoCircleEx::DirectionType& value)
	{
		this->mDirectionType = value;
	}

	CGeoCircleEx::DirectionType CGeoCircleEx::getRotateDirection()
	{
		return this->mDirectionType;
	}

	void CGeoCircleEx::setClampGroundMode(const bool& value)
	{
		this->mClampGroundMode = value;
	}

	bool CGeoCircleEx::getClampGroundMode()
	{
		return this->mClampGroundMode;
	}

	void CGeoCircleEx::setRotateEnable(const bool& rotateEnable)
	{
		this->mRotateEnable = rotateEnable;
	}

	bool CGeoCircleEx::getRotateEnable()
	{
		return this->mRotateEnable;
	}

	void CGeoCircleEx::update(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::ref_ptr<osg::MatrixTransform> pRealCircle = 
			dynamic_cast<osg::MatrixTransform*>(node);
		if (pRealCircle)
		{
			int directionFactor = 1;
			if (this->mDirectionType == CGeoCircleEx::DT_ClockWise)
			{
				directionFactor = 1;
			}
			else if (this->mDirectionType == CGeoCircleEx::DT_AntiClockWise)
			{
				directionFactor = -1;
			}
			this->mRotateAngle -= directionFactor * 1 * this->getRotateFactor();
			if (this->mRotateAngle < -360)
				this->mRotateAngle += 360;
			if (this->mRotateAngle > 360)
				this->mRotateAngle -= 360;
			double angle = this->mRotateAngle * 3.1415926 / 180;
			osg::Quat q;
			q.makeRotate(angle,osg::Vec3d(0,0,1));
			osg::Matrix worldMatrix = osg::Matrix::rotate(q) * this->mWorldMatrix;
			pRealCircle->setMatrix(worldMatrix);
		}
		
	}
	
}