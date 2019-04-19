#include "dynamicobject/dynamicobject.h"
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthSymbology/Style>
#include <osg/LineWidth>
#include <osg/LineStipple>
#include <osg/BlendColor>
#include <osg/BlendFunc>
#include <osgEarthAnnotation/AnnotationUtils>
#include <osg/Point>

using namespace DynamicObject;

#define MAX_DESTROY_TIME 1000000000
class CObjectBlinkCallback : public osg::NodeCallback
{
public:
	CObjectBlinkCallback(const double& time,
		const double& timeInterval)
	{
		mTime = time;
		mIsBegin = true;
		mBeginTime = 0.0;
		mTimeInterval = timeInterval;
		mTimeRecord = 0.0;
	}

	~CObjectBlinkCallback()
	{

	}

	void operator()(osg::Node *node,osg::NodeVisitor *nv)
	{
		double time = nv->getFrameStamp()->getSimulationTime();
		if(mIsBegin)
		{
			mBeginTime = time;
			mIsBegin = false;
		}

		if(time - mBeginTime < mTime)
		{
			if(time - mTimeRecord > mTimeInterval)
			{
				mTimeRecord = time;
				if(isHighLight(node))
					wipeHighLight(node);
				else
					highLight(node);
			}
		}
		else if(time - mBeginTime > mTime)
		{
			wipeHighLight(node);
			node->removeUpdateCallback(this);
		}
	}

private:
	void highLight( osg::Node* node )
	{
		if(isHighLight(node))
			return;
		osg::StateSet* pStateset = node->getOrCreateStateSet();
		osg::Program* pProgram = dynamic_cast<osg::Program*>(pStateset->getAttribute(osg::StateAttribute::PROGRAM));
		if(NULL == pProgram)
		{
			pProgram = new osg::Program;
			std::string fragShaderPath = "shader/highlight.frag";
			std::string vertexShaderPath = "shader/highlight.vert";
			osg::Shader* fragShader = osg::Shader::readShaderFile(osg::Shader::FRAGMENT, fragShaderPath);
			osg::Shader* vertexShader = osg::Shader::readShaderFile(osg::Shader::VERTEX, vertexShaderPath);
			pStateset->addUniform( new osg::Uniform("TextureMap", 0) );
			pProgram->addShader(fragShader);
			pProgram->addShader(vertexShader);
		}
		pStateset->setAttributeAndModes(pProgram,osg::StateAttribute::ON | 
			osg::StateAttribute::OVERRIDE | osg::StateAttribute::PROTECTED);
	}
	
	bool isHighLight( osg::Node* node )
	{
		bool ishigh = false;
		osg::StateSet* pState = node->getOrCreateStateSet();
		ishigh = pState->getAttribute(osg::StateAttribute::PROGRAM);
		return ishigh;
	}

	bool wipeHighLight( osg::Node* node )
	{
		if(!isHighLight(node))
			return true;
		osg::StateSet* pState = node->getOrCreateStateSet();
		pState->removeAttribute(osg::StateAttribute::PROGRAM);
		return true;
	}
private:
	double mTime;
	bool mIsBegin;
	double mBeginTime;
	double mTimeInterval;//多少秒闪一下
	double mTimeRecord;
};

DynamicObject::CDynamicObject::CDynamicObject(osgEarth::MapNode* mMapNode)
	: Common::ISceneObject(mMapNode)
{
	mpGeoPoint = new Graphic::CGeoPoint(mMapNode);
	mpGeoScutcheon = new Geo::CGeoScutcheon(mMapNode);
	mpLineGeode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> lineGeom = new osg::Geometry;
	osg::ref_ptr<osg::Vec3dArray> vertices = new osg::Vec3dArray;
	lineGeom->setVertexArray(vertices.get());
	osg::ref_ptr<osg::DrawArrays> drawArrays = new osg::DrawArrays(GL_LINE_STRIP, 0, vertices->getNumElements());
	lineGeom->addPrimitiveSet(drawArrays.get());
	mTrackLineColor = osg::Vec4f(1,0,0,1);
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
	colors->push_back(mTrackLineColor);
	lineGeom->setColorArray(colors.get());
	lineGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	mTrackLineWitdh = 5.0;
	lineGeom->getOrCreateStateSet()->setAttributeAndModes(new osg::LineWidth(mTrackLineWitdh));
	lineGeom->getOrCreateStateSet()->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
	lineGeom->getOrCreateStateSet()->setMode(GL_POINT_SMOOTH, osg::StateAttribute::ON);
	mpLineGeode->addChild(lineGeom.get());
	osg::ref_ptr<osg::MatrixTransform> pLineMatrixTransform = new osg::MatrixTransform;
	pLineMatrixTransform->addChild(mpLineGeode.get());
	mpLineNode = new osg::LOD;
	mpLineNode->addChild(pLineMatrixTransform.get());
	mpLineNode->setRange(0,0,50000000);
	//mMapNode->addChild(mpLineNode);
	mTrackLinePointSize = 500;
	mTrackLineColor = osg::Vec4f(1,0,0,1);
	mPathPointSize = -1;
	mIsTrackLineVisible = false;
	mIsScutcheonVisible = false;
	mIsIconVisible = true;
	mpMapNode = mMapNode;
	mpAnimationPathCallback = NULL;
	mHasLoadResource = false;
	mIsFirstPos = true;
	mFirstPos = osg::Vec3d(0,0,0);
	mFirstWPos = osg::Vec3d(0,0,0);
	mTrackLineType = SOLID_LT;
	mDirectionalLineColor = osg::Vec4f(1,1,0,1);
	mDirectionalLineWitdh = 5.0;
	mDirectionalLineType = SOLID_LT;
	mIsDirectionallineVisible = false;
	mIsInWarnningRegion = false;
	mIsInScanningRegion = false;
	mIsInNoFlyingRegion = false;
	mNearestUpdateTime = 0.0;
	mTrackLineFadingTimeInterval = MAX_DESTROY_TIME;
	mIconUnVisibleTimeInterval = MAX_DESTROY_TIME;
	mDestroyTimeInterval = MAX_DESTROY_TIME;
	mAssociatedFacilityID = "";
	mIconScale = 1.0;
	mScutcheonUnVisibleTimeInterval = MAX_DESTROY_TIME;
	mScutcheonHeaderColor = osgEarth::Symbology::Color::Red;
	mObejctBlinkTime = 5.0;
	mObejctBlinkTimeInterval = 1.0;
	mIsFieldsChanged = false;
	mIsScutcheonHeaderColorChanged = false;
	mIsIconImageChanged = false;
	mIsBlink = false;

	mIsIconStyleChanged = false;
	mIsScutcheonStyleChanged = false;

	mIsTrackPointPattern = false;
	mIsTrackLinePatternChanged = false;
	mIsTrackLineColorChanged = false;
	mIsTrackLinePointSizeChanged = false;

	mScutcheonScreenCoordinatesOffset = osg::Vec2s(0.0,0.0);
	mIsScutcheonScreenCoordinatesChanged = false;

	mScutcheonSeparativeSign = " : ";
	mScutcheonVerticalSpacing = 10;
	mIntervalBetweenTitleAndContent = 20;
	mTitleImageOffset = 0;
	this->mIntervalBetweenBottomAndContent = 10;
	this->mContentHorizontalOffset = -65;
	this->mBackImageWidth = 175;
	this->mTitleImageSize = osg::Vec2s(120,40);

	mScuLineColor = osg::Vec4f(1,0,0,1);
	mScuLineType = SOLID_LT;
	mScuLineWitdh = 5.0;
	mIsScuLineVisible = true;
	mIsScuLineStyleChanged = false;
	this->mpScreenCamera = NULL;
	//ScuLine
	{
		mpScuLineGeode = new osg::Geode;
		mpScuLineGeode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF); //取消深度测试 
		mpScuLineGeode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		osg::ref_ptr<osg::Geometry> lineGeom = new osg::Geometry;
		osg::ref_ptr<osg::Vec3dArray> vertices = new osg::Vec3dArray;
		lineGeom->setVertexArray(vertices.get());
		lineGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, vertices->getNumElements()));
		osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array();
		colors->push_back(mScuLineColor);
		lineGeom->setColorArray(colors.get());
		lineGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
		lineGeom->getOrCreateStateSet()->setAttributeAndModes(new osg::LineWidth(mScuLineWitdh));
		mpScuLineGeode->addChild(lineGeom.get());
		osg::ref_ptr<osg::MatrixTransform> pScuLineMatrixTransform = new osg::MatrixTransform;
		pScuLineMatrixTransform->addChild(mpScuLineGeode.get());
		mpScuLineNode = new osg::LOD;
		mpScuLineNode->addChild(pScuLineMatrixTransform.get());
		mpScuLineNode->setRange(0,0,50000000);
		mScuLineMatrix = osg::Matrixd::identity();
	}

	mpRefViewer = NULL;
	mIsIconTextVisible = false;
	mIsScutcheonRenderToScene = false;
	mIsIconRenderToScene = false;
	mIsScutcheonContentChanged = false;

	mIsCurrentFieldsChanged = false;
	mIsScutcheonCurrentContentChanged = false;

	mCurrentWorldPos = osg::Vec3d(0.0,0.0,0.0);
	mTrackLineOriginalColor = mTrackLineColor;
	mNeedUpdateIconVisible = true;
	mIsTrackLineColorFad = false;
}

std::map<std::string,std::string>& DynamicObject::CDynamicObject::getUserProperty()
{
	return this->mUserProperty;
}

DynamicObject::CDynamicObject::~CDynamicObject()
{
	if(NULL != mpGeoPoint)
	{
		mpGeoPoint->deRenderFromScene();
		mpGeoPoint->unLoadFromScene();
		delete mpGeoPoint;
		mpGeoPoint = NULL;
	}

	if(NULL != mpGeoScutcheon)
	{
		mpGeoScutcheon->deRenderFromScene();
		mpGeoScutcheon->unLoadFromScene();
		delete mpGeoScutcheon;
		mpGeoScutcheon = NULL;
	}

	if(NULL != mpLineGeode)
	{
		this->getMapNode()->removeChild(mpLineNode);
		mpLineNode = NULL;
	}

	if(NULL != mpScuLineNode)
	{
		getMapNode()->removeChild(mpScuLineNode);
		mpScuLineNode = NULL;	
	}

	if(mDirectionalLines.size() > 0)
	{
		std::map<std::string,Geo::CGeoSegment*>::iterator nodeIter = mDirectionalLines.begin();
		while(nodeIter != mDirectionalLines.end())
		{
			if(nodeIter->second)
			{
				nodeIter->second->deRenderFromScene();
				nodeIter->second->unLoadFromScene();
				delete nodeIter->second;
			}
			nodeIter++;
		}
		mDirectionalLines.clear();
	}
}


bool DynamicObject::CDynamicObject::aSynLoad()
{
	if (this->asynLock())
	{
		mpGeoPoint->setID(this->getID());
		mpGeoScutcheon->setID(this->getID());
		mpGeoScutcheon->setScreenCamera(this->getScreenCamera());
		mpGeoScutcheon->setViewer(mpRefViewer);
		//加载目标图标
		osgEarth::Symbology::Style style = mpGeoPoint->getStyle();
		if(mIsIconTextVisible)
			mpGeoPoint->setName(this->getName());
		/*this->setName("目标" + this->getID());*/
		mpGeoPoint->setID(this->getID());
		mpGeoPoint->setType("Target");//按需求设置type的字符串类型
		osgEarth::Symbology::Style& pointStyle = this->mpGeoPoint->getStyle();
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()= "fonts/simhei.ttf";//指定中文字体路径
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 20.0;//设置字体大小
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = osgEarth::Symbology::Color::Yellow;//设置字体颜色
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_LEFT_CENTER;
		pointStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->declutter() = false;
		pointStyle.getOrCreate<osgEarth::Symbology::IconSymbol>()->declutter() = false;
		pointStyle.getOrCreate<osgEarth::Symbology::IconSymbol>()->alignment() = osgEarth::Symbology::IconSymbol::ALIGN_CENTER_TOP;
		std::string iconPath = "systemImages/resources/image/" + mIconImagePath;
		pointStyle.getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral(iconPath);//设置点图标显示的图片
		pointStyle.getOrCreate<osgEarth::Annotation::IconSymbol>()->alignment() = osgEarth::Annotation::IconSymbol::ALIGN_CENTER_CENTER;
		pointStyle.getOrCreate<osgEarth::Annotation::IconSymbol>()->scale() = mIconScale;
		mpGeoPoint->loadToScene();
		mpGeoPoint->setVisible(mIsIconVisible);

		mpGeoScutcheon->getFields().clear();
		for (int index = 0; index < mFields.size(); ++index)
		{
			if(true == mFields.at(index).valid)
				mpGeoScutcheon->getFields().push_back(Geo::Field(mFields.at(index).name,mFields.at(index).value));
		}

		mpGeoScutcheon->setName(this->getName());
		mpGeoScutcheon->setID(this->getID());
		mpGeoScutcheon->loadToScene();
		mpGeoScutcheon->setVisible(mIsScutcheonVisible);

		//加载航迹线
		mHasLoadResource = true;
		this->asynUnlock();
	}
	return false;
}

void buildTrackLinePoints(
	osgEarth::MapNode* pMapNode,
	osg::Vec3dArray* pLineDrawArray,
	std::map<double,osg::Vec3d> pathPoints,
	int pointCount)
{
	if(NULL == pLineDrawArray)
		return;
	pLineDrawArray->clear();
	std::map<double,osg::Vec3d>::reverse_iterator rBeginItr = pathPoints.rbegin();
	if(pathPoints.size() < pointCount)
		pointCount = pathPoints.size();
	else
		pointCount = pointCount - 2;
	for (int index = 0; index < pointCount; ++index)
	{
		osg::Vec3d pos = rBeginItr->second;
		osgEarth::GeoPoint geoStartPoint(pMapNode->getMapSRS(), pos, osgEarth::ALTMODE_ABSOLUTE);
		osg::Matrixd local2world;
		geoStartPoint.createLocalToWorld( local2world );
		pLineDrawArray->push_back(local2world.getTrans());
		rBeginItr--;
	}
}


bool DynamicObject::CDynamicObject::synUpdate(osg::Camera* pCamera)
{
	osg::Vec3d currentGeoPosition(0,0,0);
	if (mPathMutex.lock() == 0)
	{
		if (!this->mPathPoints.empty())
		{
			std::map<double,osg::Vec3d>::iterator itr = this->mPathPoints.begin();
			currentGeoPosition = itr->second;
			mTrackLastPos = currentGeoPosition;
			if(mIsFirstPos)
			{
				mFirstPos = currentGeoPosition;
				osgEarth::GeoPoint firstGeoPoint(mpMapNode->getMapSRS(), mFirstPos, osgEarth::ALTMODE_ABSOLUTE);
				firstGeoPoint.toWorld(mFirstWPos);
				mIsFirstPos = false;
			}
			this->mPathPoints.erase(itr);
		}
		mPathMutex.unlock();
	}

	if(this->getTrackLineVisible())
	{
		if(mpLineNode->getNumParents() == 0)
			getMapNode()->addChild(mpLineNode.get());
	}

	if (mpGeoPoint && getIconVisible() && false == mIsIconRenderToScene)
	{
		mIsIconRenderToScene = true;
		mpGeoPoint->renderToScene();
		mpGeoPoint->getOrCreateLod(false)->setRange(0,0,50000000);
	}
#if 1
	//解决图标在没有接收到信号后隐藏，然后再接收到信号后的显示的问题
	if (this->mNeedUpdateIconVisible)
	{
		this->mNeedUpdateIconVisible = false;
		this->setIconVisible(this->getIconVisible());
	}
#endif
	if(mpGeoScutcheon && getScutcheonVisible() && false == mIsScutcheonRenderToScene)
	{
		if(mpGeoPoint->getGeoPosition() != osg::Vec3d(0,0,0))
		{
			mIsScutcheonRenderToScene = true;
			mpGeoScutcheon->setGeoPosition(mpGeoPoint->getGeoPosition());
			mpGeoScutcheon->renderToScene();
			mScuteonFieldMutex.lock();
			mpGeoScutcheon->updateSceneObject();
			mScuteonFieldMutex.unlock();
		}
	}

	if (currentGeoPosition != osg::Vec3d(0,0,0))
	{
		osg::Vec3d currentWPoint;
		osgEarth::GeoPoint currentGeoPoint(mpMapNode->getMapSRS(), currentGeoPosition, osgEarth::ALTMODE_ABSOLUTE);
		currentGeoPoint.toWorld(currentWPoint);
		mCurrentWorldPos = currentWPoint;
		//if(this->getTrackLineVisible())//不管显示不显示，轨迹线都要更新，不然GL_POINTS模式下，会出现点缺失
		{
			osg::MatrixTransform* pLineMatrixTransform = dynamic_cast<osg::MatrixTransform*>(mpLineNode->getChild(0));
			if(pLineMatrixTransform)
			{
				osg::Matrixd lineMatrix;
				lineMatrix.setTrans(mFirstWPos);
				pLineMatrixTransform->setMatrix(lineMatrix);
				osg::Geometry* pLineGeom = dynamic_cast<osg::Geometry *>(mpLineGeode->getDrawable(0));
				osg::Vec3dArray* pLineDrawArray = dynamic_cast<osg::Vec3dArray*>(pLineGeom->getVertexArray());
				osg::DrawArrays* pLinePrimitiveSet = dynamic_cast<osg::DrawArrays *>(pLineGeom->getPrimitiveSet(0));
				if(pLineDrawArray->size() > mTrackLinePointSize && mTrackLinePointSize != -1)
				{
					osg::Vec3dArray::iterator beginItr = pLineDrawArray->begin();
					pLineDrawArray->erase(beginItr);
				}
				pLineDrawArray->push_back(currentWPoint - mFirstWPos);
				pLineGeom->setVertexArray(pLineDrawArray);
				pLinePrimitiveSet->setFirst(0);
				pLinePrimitiveSet->setCount(pLineDrawArray->size());
				pLineGeom->dirtyDisplayList();
				pLineGeom->dirtyBound();
			}
		}
		if (mpGeoPoint && mIsIconRenderToScene)
		{
			std::map<double,osg::Vec3d>::iterator itr = this->mPathPoints.begin();
			mpGeoPoint->setGeoPosition(currentGeoPosition);
			mpGeoPoint->updateSceneObject();
		}
		mScuteonFieldMutex.lock();
		if(mpGeoScutcheon && mIsScutcheonRenderToScene)
		{
			mpGeoScutcheon->setGeoPosition(currentGeoPosition);
			mpGeoScutcheon->updateSceneObject();
		}
		if(mpGeoScutcheon && mIsScutcheonContentChanged && mIsScutcheonCurrentContentChanged)
		{
			if(mScutcheonScreenCoordinatesOffset != osg::Vec2s(0.0,0.0))
			{
				//mpGeoScutcheon->setScreenCoordinatesOffset(osg::Vec2s(0.0,0.0));
				//mpGeoScutcheon->setScreenCoordinatesOffset(mScutcheonScreenCoordinatesOffset);
			}
			mpGeoScutcheon->updateSceneObject();
			mIsScutcheonContentChanged = false;
			mIsScutcheonCurrentContentChanged = false;
		}
		mScuteonFieldMutex.unlock();
		if(mIsDirectionallineVisible && mDirectionalLines.size() > 0)
		{
			std::map<std::string,Geo::CGeoSegment*>::iterator nodeIter = mDirectionalLines.begin();
			while (nodeIter != mDirectionalLines.end())
			{
				Geo::CGeoSegment* pGeoSegment = nodeIter->second;
				if(pGeoSegment)
				{
					pGeoSegment->getEndPoint() = currentGeoPosition;
					pGeoSegment->updateSceneObject();
				}
				nodeIter++;
			}
		}//mIsDirectionallineVisible
	}//currentGeoPosition != osg::Vec3d(0,0,0)
	mScuteonFieldMutex.lock();
	if(true == mIsFieldsChanged || true == mIsScutcheonHeaderColorChanged ||
		true == mIsScutcheonStyleChanged || true == mIsCurrentFieldsChanged)
	{
		mpGeoScutcheon->deRenderFromScene();
		mpGeoScutcheon->unLoadFromScene();
		mpGeoScutcheon->loadToScene();
		mpGeoScutcheon->renderToScene();
		if(mScutcheonScreenCoordinatesOffset != osg::Vec2s(0.0,0.0))
		{
			//mpGeoScutcheon->setScreenCoordinatesOffset(osg::Vec2s(0.0,0.0));
			//mpGeoScutcheon->setScreenCoordinatesOffset(mScutcheonScreenCoordinatesOffset);
		}
		mIsFieldsChanged = false;
		mIsScutcheonHeaderColorChanged = false;
		mIsScutcheonStyleChanged = false;
		mIsCurrentFieldsChanged = false;
	}
	mScuteonFieldMutex.unlock();
	if(true == mIsIconImageChanged || true == mIsIconStyleChanged)
	{
		mIsIconImageChanged = false;
		mIsIconStyleChanged = false;
		mpGeoPoint->deRenderFromScene();
		mpGeoPoint->unLoadFromScene();
		mpGeoPoint->loadToScene();
		mpGeoPoint->renderToScene();
	}

	if(mIsBlink)
	{
		mIsBlink = false;
		if(mpGeoPoint)
		{
			osg::Group* geoGroup = dynamic_cast<osg::Group*>(mpGeoPoint->getNode());
			if(geoGroup && geoGroup->getNumChildren() > 0)
			{
				osg::ref_ptr<CObjectBlinkCallback> pCObjectBlinkCallback = 
					new CObjectBlinkCallback(mObejctBlinkTime,mObejctBlinkTimeInterval);
				geoGroup->getChild(0)->addUpdateCallback(pCObjectBlinkCallback.get());
			}
		}//mpGeoPoint
	}//mIsBlink

	if(mIsTrackLinePatternChanged)
	{
		osg::Geometry* pLineGeom = dynamic_cast<osg::Geometry *>(mpLineGeode->getDrawable(0));
		osg::Vec3dArray* pLineDrawArray = dynamic_cast<osg::Vec3dArray*>(pLineGeom->getVertexArray());
		if(true == mIsTrackPointPattern)
		{
			pLineGeom->removePrimitiveSet(0);
			pLineGeom->addPrimitiveSet(new osg::DrawArrays(GL_POINTS, 0, pLineDrawArray->getNumElements()));
		}
		else
		{
			pLineGeom->removePrimitiveSet(0);
			pLineGeom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, pLineDrawArray->getNumElements()));
		}
		setTrackLineWidth(mTrackLineWitdh);
		pLineGeom->dirtyDisplayList();
		mIsTrackLinePatternChanged = false;
	}

	if(mIsTrackLineColorChanged)
	{
		osg::Geometry* pLineGeom = dynamic_cast<osg::Geometry *>(mpLineGeode->getDrawable(0));
		osg::ref_ptr<osg::Vec4Array> colors = dynamic_cast<osg::Vec4Array*>(pLineGeom->getColorArray());
		colors->clear();
		colors->push_back(mTrackLineColor);
		pLineGeom->dirtyDisplayList();
		mIsTrackLineColorChanged = false;
	}

	if(mIsTrackLinePointSizeChanged)
	{
		osg::Geometry* pLineGeom = dynamic_cast<osg::Geometry *>(mpLineGeode->getDrawable(0));
		osg::Vec3dArray* pLineDrawArray = dynamic_cast<osg::Vec3dArray*>(pLineGeom->getVertexArray());
		osg::DrawArrays* pLinePrimitiveSet = dynamic_cast<osg::DrawArrays *>(pLineGeom->getPrimitiveSet(0));
		if(pLineDrawArray->size() > mTrackLinePointSize)
		{
			int needSubCounnt = pLineDrawArray->size() - mTrackLinePointSize;
			for (int index = 0; index < needSubCounnt; ++index)
			{
				osg::Vec3dArray::iterator beginItr = pLineDrawArray->begin();
				pLineDrawArray->erase(beginItr);
			}
		}
		pLineGeom->setVertexArray(pLineDrawArray);
		pLinePrimitiveSet->setFirst(0);
		pLinePrimitiveSet->setCount(pLineDrawArray->size());
		pLineGeom->dirtyDisplayList();
		pLineGeom->dirtyBound();
		mIsTrackLinePointSizeChanged = false;
	}

	//更新scuLine
	{
		if(mpScuLineNode->getNumParents() == 0)
			getMapNode()->addChild(mpScuLineNode.get());

		bool needUpdateScuLine = mIsScutcheonScreenCoordinatesChanged;
		if(mIsScutcheonScreenCoordinatesChanged)
		{
			mpGeoScutcheon->setScreenCoordinatesOffset(mScutcheonScreenCoordinatesOffset);
			mIsScutcheonScreenCoordinatesChanged = false;
		}

		osg::MatrixTransform* pScuLineMatrixTransform = dynamic_cast<osg::MatrixTransform*>(mpScuLineNode->getChild(0));
		if(pScuLineMatrixTransform)
		{
			osg::Matrixd lineMatrix;
			lineMatrix.setTrans(mFirstWPos);
			pScuLineMatrixTransform->setMatrix(lineMatrix);
			osg::Geometry* pLineGeom = dynamic_cast<osg::Geometry *>(mpScuLineGeode->getDrawable(0));
			osg::Vec3dArray* pLineDrawArray = dynamic_cast<osg::Vec3dArray*>(pLineGeom->getVertexArray());
			osg::DrawArrays* pLinePrimitiveSet = dynamic_cast<osg::DrawArrays *>(pLineGeom->getPrimitiveSet(0));
			
			pLineDrawArray->clear();
			osg::Vec3d currentWPoint;
			osgEarth::GeoPoint firstGeoPoint(mpMapNode->getMapSRS(), mTrackLastPos, osgEarth::ALTMODE_ABSOLUTE);
			firstGeoPoint.toWorld(currentWPoint);

			osg::Vec3d offsetWorldPoint(0,0,0);
			if ((std::abs(this->mScutcheonScreenCoordinatesOffset.x()) > 0
				|| std::abs(this->mScutcheonScreenCoordinatesOffset.y()) > 0)
				&& mpGeoScutcheon 
				&& mpGeoScutcheon->getVisible())
			{
				offsetWorldPoint = mpGeoScutcheon->getNearWorldPointToOrigin(pCamera);
			}
			else
			{
				osg::Vec3d currentScreenPoint = worldToScreen(currentWPoint);
				osg::Vec3d offsetScreenPoint = currentScreenPoint + osg::Vec3d(
					mScutcheonScreenCoordinatesOffset.x(),mScutcheonScreenCoordinatesOffset.y(),0.0);
				offsetWorldPoint = screenToWorld(offsetScreenPoint);
			}
			if (offsetWorldPoint == osg::Vec3d(0,0,0))
			{
				offsetWorldPoint = currentWPoint;
			}
			pLineDrawArray->push_back(currentWPoint - mFirstWPos);
			pLineDrawArray->push_back(offsetWorldPoint - mFirstWPos);
		

			if(mIsScuLineStyleChanged)
			{
				osg::ref_ptr<osg::Vec4Array> colors = dynamic_cast<osg::Vec4Array*>(pLineGeom->getColorArray());
				colors->clear();
				colors->push_back(mScuLineColor);
				pLineGeom->dirtyDisplayList();
				mIsScuLineStyleChanged = false;
			}

			pLineGeom->setVertexArray(pLineDrawArray);
			pLinePrimitiveSet->setFirst(0);
			pLinePrimitiveSet->setCount(pLineDrawArray->size());
			pLineGeom->dirtyDisplayList();
			pLineGeom->dirtyBound();
		}
	}
	return false;
}

void DynamicObject::CDynamicObject::clearSceneTrackLines()
{
	osg::Geometry* pLineGeom = dynamic_cast<osg::Geometry *>(mpLineGeode->getDrawable(0));
	if (pLineGeom)
	{
		osg::Vec3dArray* pLineDrawArray = dynamic_cast<osg::Vec3dArray*>(pLineGeom->getVertexArray());
		osg::DrawArrays* pLinePrimitiveSet = dynamic_cast<osg::DrawArrays *>(pLineGeom->getPrimitiveSet(0));
		if (pLineDrawArray && pLinePrimitiveSet)
		{
			pLineDrawArray->clear();
			pLineGeom->setVertexArray(pLineDrawArray);
			pLinePrimitiveSet->setFirst(0);
			pLinePrimitiveSet->setCount(pLineDrawArray->size());
			pLineGeom->dirtyDisplayList();
			pLineGeom->dirtyBound();
		}
	}
}

bool DynamicObject::CDynamicObject::hasLoadResource()
{
	bool r = false;
	if (this->asynLock())
	{
		r = mHasLoadResource;
		this->asynUnlock();
	}
	return r;
}

void DynamicObject::CDynamicObject::setTrackLinePointSize(const int& trackLinePointSize)
{
	mTrackLinePointSize = trackLinePointSize;
	mIsTrackLinePointSizeChanged = true;
}

int DynamicObject::CDynamicObject::getTrackLinePointSize()
{
	return mTrackLinePointSize;
}

void DynamicObject::CDynamicObject::setTrackLineColor( const osg::Vec4f& color )
{
	mTrackLineColor = color;
	mTrackLineOriginalColor = color;
	mIsTrackLineColorChanged = true;
}

osg::Vec4f DynamicObject::CDynamicObject::getTrackLineColor()
{
	return mTrackLineColor;
}

void DynamicObject::CDynamicObject::setTrackLineVisible( const bool& isVisible )
{
	mIsTrackLineVisible = isVisible;
	if(mIsTrackLineVisible)
	{
		mpLineGeode->setNodeMask(1);
	}
	else
	{
		mpLineGeode->setNodeMask(0);
	}
}

bool DynamicObject::CDynamicObject::getTrackLineVisible()
{
	return mIsTrackLineVisible;
}

void DynamicObject::CDynamicObject::setPathPointSize(const int& pathPointSize)
{
	mPathPointSize = pathPointSize;
}

int DynamicObject::CDynamicObject::getPathPointSize()
{
	return mPathPointSize;
}

void DynamicObject::CDynamicObject::setModelPath( const std::string& modelPath )
{
	mModelPath = modelPath;
}

void DynamicObject::CDynamicObject::setScutcheonContent( const std::string& content )
{
	mpGeoScutcheon->setName(content);
}

std::string DynamicObject::CDynamicObject::getScutcheonContent()
{
	return mpGeoScutcheon->getName();
}

void DynamicObject::CDynamicObject::setScutcheonVisible( const bool& isVisible )
{
	mIsScutcheonVisible = isVisible;
	if (mpGeoScutcheon)
		mpGeoScutcheon->setVisible(mIsScutcheonVisible);
	setScuLineVisible(isVisible);
}

bool DynamicObject::CDynamicObject::getScutcheonVisible()
{
	return mIsScutcheonVisible;
}

void DynamicObject::CDynamicObject::setObjectIcon( const std::string& imagePath )
{
	osgEarth::Symbology::Style& stype = mpGeoPoint->getStyle();
	mIconImagePath = imagePath;
	std::string iconPath = "systemImages/resources/image/" + mIconImagePath;
	stype.getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral(iconPath);
	mIsIconImageChanged = true;
}

std::string DynamicObject::CDynamicObject::getObjectIcon()
{
	return mIconImagePath;
}

void DynamicObject::CDynamicObject::notifyHideScutcheon()
{
	if (mpGeoScutcheon)
		mpGeoScutcheon->setVisible(false);
	setScuLineVisible(false);
}

void DynamicObject::CDynamicObject::notifyHideIcon()
{
	if (mpGeoPoint)
		mpGeoPoint->setVisible(false);
}

void DynamicObject::CDynamicObject::setIconVisible( const bool& isVisible )
{
	mIsIconVisible = isVisible;
	if (mpGeoPoint)
		mpGeoPoint->setVisible(mIsIconVisible);
}

bool DynamicObject::CDynamicObject::getIconVisible()
{
	return mIsIconVisible;
}

bool DynamicObject::CDynamicObject::reveiveSignal(DynamicObject::CDataSingal* pRefSignal)
{
	bool r = false;
	double time = 2;
	osg::Vec3d geoPosition = osg::Vec3d(pRefSignal->longitude,pRefSignal->latitude,pRefSignal->altitude); 
	if (mPathMutex.lock() == 0)
	{
		this->addModelPathPoint(geoPosition,time);
		//if (this->getIsTrackLineFadState())
		{
			recoverTrackLineColor();
			//this->mIsTrackLineColorFad = false;
		}
#if 1 //恢复图标
		this->mNeedUpdateIconVisible = true;		
#endif
		mPathMutex.unlock();
	}
	return r;
}

void DynamicObject::CDynamicObject::addModelPathPoint( const osg::Vec3d& pos,const double& time )
{
	double rightTime = time;
	if(mPathPoints.size() != 0)
	{
		rightTime += mPathPoints.rbegin()->first;
		if (mPathPoints.size() > 100)
		{
			mPathPoints.clear();
		}
	}
	mPathPoints.insert(std::make_pair(rightTime,pos));
}

osg::Vec3d DynamicObject::CDynamicObject::getModelPathPoint( const double& time )
{
	return osg::Vec3d();
}

osg::Vec3d DynamicObject::CDynamicObject::getModelCurrentPoint()
{
	osg::Vec3d resultPos(0,0,0);
	if (mPathMutex.lock() == 0)
	{
		std::map<double,osg::Vec3d>::iterator itr = this->mPathPoints.begin();
		if(itr == this->mPathPoints.end())
			resultPos = mTrackLastPos;
		else
			resultPos = itr->second;
		mPathMutex.unlock();
	}
	return resultPos;
}

osg::Node* DynamicObject::CDynamicObject::getDynamicObjctNode()
{
	return NULL;
}

void DynamicObject::CDynamicObject::setPosition( osg::Vec3d pos )
{
	return;
}

void DynamicObject::CDynamicObject::setTrackLineType( LineType lineType )
{
	mTrackLineType = lineType;
	if(NULL == mpLineGeode)
		return;
	osg::Geometry* pLineGeom = dynamic_cast<osg::Geometry *>(mpLineGeode->getDrawable(0));
	if(NULL == pLineGeom)
		return;
	if(LineType::SOLID_LT == mTrackLineType)
	{
		osg::ref_ptr<osg::LineStipple> pLineStipple = dynamic_cast<osg::LineStipple*>(
			pLineGeom->getOrCreateStateSet()->getAttribute(osg::StateAttribute::LINESTIPPLE));
		if(pLineStipple)
		{
			//pLineGeom->getOrCreateStateSet()->setAttribute(pLineStipple.get(),osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
			pLineStipple->setFactor(1);
			pLineStipple->setPattern(0xFFFF);
			pLineGeom->getOrCreateStateSet()->setAttributeAndModes(pLineStipple.get(),osg::StateAttribute::ON);
		}
	}
	else
	{
		osg::ref_ptr<osg::LineStipple> pLineStipple = new osg::LineStipple;
		pLineStipple->setFactor(1);
		if(LineType::DOTTED_LT == mTrackLineType)
			pLineStipple->setPattern(0x3F3F);
		else if(LineType::POINT_LT == mTrackLineType)
			pLineStipple->setPattern(0x0101);
		else if(LineType::DOTDASH_LT == mTrackLineType)
			pLineStipple->setPattern(0x27FF);
		else if(LineType::CENTRELINE_LT == mTrackLineType)
			pLineStipple->setPattern(0x33FF);
		else if(LineType::DOUBLEDOT_LT == mTrackLineType)
			pLineStipple->setPattern(0x24FF);
		else if(LineType::TRIDOT_LT == mTrackLineType)
			pLineStipple->setPattern(0x55FF);
		pLineGeom->getOrCreateStateSet()->setAttributeAndModes(pLineStipple.get(),osg::StateAttribute::ON);
	}
}

DynamicObject::LineType DynamicObject::CDynamicObject::getTrackLineType()
{
	return mTrackLineType;
}

osg::Vec3d DynamicObject::CDynamicObject::getPosition()
{
	return mModelPos;
}

void DynamicObject::CDynamicObject::setScreenCamera(osg::Camera* pScreenCamera)
{
	this->mpScreenCamera = pScreenCamera;
}

osg::Camera* DynamicObject::CDynamicObject::getScreenCamera()
{
	return this->mpScreenCamera;
}

void DynamicObject::CDynamicObject::addDirectionalPoint( const std::string& id,const osg::Vec3d& point )
{
	//std::map<std::string,Geo::CGeoSegment*>::iterator nodeIter = mDirectionalLines.find(id);
	//if(nodeIter != mDirectionalLines.end())
	//	return;
	//Geo::CGeoSegment* pGeoSegment = new Geo::CGeoSegment(mpMapNode);
	//pGeoSegment->getStartPoint() = point;
	//pGeoSegment->getEndPoint() = point;
	//pGeoSegment->setColor(mDirectionalLineColor);
	//pGeoSegment->setLineWidth(mTrackLineWitdh);
	//if(mDirectionalLineType == SOLID_LT)
	//	pGeoSegment->setLineType(0);
	//else
	//	pGeoSegment->setLineType(1);
	//pGeoSegment->loadToScene();
	//pGeoSegment->renderToScene();
	//mDirectionalLines.insert(std::make_pair(id,pGeoSegment));
}

int DynamicObject::CDynamicObject::getDirectionalPointsSize()
{
	return mDirectionalLines.size();
}

void DynamicObject::CDynamicObject::setDirectionallineVisible( const bool& isVisible )
{
	mIsDirectionallineVisible = isVisible;
	std::map<std::string,Geo::CGeoSegment*>::iterator nodeIter = mDirectionalLines.begin();
	while(nodeIter != mDirectionalLines.end())
	{
		if(nodeIter->second)
		{
			nodeIter->second->setVisible(mIsDirectionallineVisible);
		}
		nodeIter++;
	}
}

bool DynamicObject::CDynamicObject::getDirectionallineVisible()
{
	return mIsDirectionallineVisible;
}

void DynamicObject::CDynamicObject::removeDirectionalPoint( const std::string& id )
{
	std::map<std::string,Geo::CGeoSegment*>::iterator nodeIter = mDirectionalLines.find(id);
	if(nodeIter == mDirectionalLines.end())
		return;
	nodeIter->second->deRenderFromScene();
	nodeIter->second->unLoadFromScene();
	mDirectionalLines.erase(nodeIter);
}

void DynamicObject::CDynamicObject::setTrackLineWidth( const float& lineWidth )
{
	mTrackLineWitdh = lineWidth;
	if(NULL == mpLineGeode)
		return;
	osg::Geometry* pLineGeom = dynamic_cast<osg::Geometry *>(mpLineGeode->getDrawable(0));
	if(NULL == pLineGeom)
		return;
	if(mIsTrackPointPattern)
		pLineGeom->getOrCreateStateSet()->setAttributeAndModes(new osg::Point(mTrackLineWitdh));
	else
		pLineGeom->getOrCreateStateSet()->setAttributeAndModes(new osg::LineWidth(mTrackLineWitdh));
}

float DynamicObject::CDynamicObject::getTrackLineWidth()
{
	return mTrackLineWitdh;
}

void DynamicObject::CDynamicObject::setDirectionallineColor( const osg::Vec4f& color )
{
	mDirectionalLineColor = color;
	std::map<std::string,Geo::CGeoSegment*>::iterator nodeIter = mDirectionalLines.begin();
	while(nodeIter != mDirectionalLines.end())
	{
		if(nodeIter->second)
		{
			nodeIter->second->setColor(color);
		}
		nodeIter++;
	}
}

osg::Vec4f DynamicObject::CDynamicObject::getDirectionallineColor()
{
	return mDirectionalLineColor;
}

void DynamicObject::CDynamicObject::setDirectionallineType( LineType lineType )
{
	mDirectionalLineType = lineType;
	std::map<std::string,Geo::CGeoSegment*>::iterator nodeIter = mDirectionalLines.begin();
	while(nodeIter != mDirectionalLines.end())
	{
		if(nodeIter->second)
		{
			if(mDirectionalLineType == SOLID_LT)
				nodeIter->second->setLineType(0);
			else
				nodeIter->second->setLineType(1);
		}
		nodeIter++;
	}
}

DynamicObject::LineType DynamicObject::CDynamicObject::getDirectionallineType()
{
	return mDirectionalLineType;
}

void DynamicObject::CDynamicObject::setDirectionallineWidth( const float& lineWidth )
{
	mDirectionalLineWitdh = lineWidth;
	std::map<std::string,Geo::CGeoSegment*>::iterator nodeIter = mDirectionalLines.begin();
	while(nodeIter != mDirectionalLines.end())
	{
		if(nodeIter->second)
		{
			nodeIter->second->setLineWidth(mDirectionalLineWitdh);
		}
		nodeIter++;
	}
}

float DynamicObject::CDynamicObject::getDirectionallineWidth()
{
	return mDirectionalLineWitdh;
}

bool DynamicObject::CDynamicObject::exsitDirectionalPoint( const std::string& id )
{
	std::map<std::string,Geo::CGeoSegment*>::iterator nodeIter = mDirectionalLines.find(id);
	if(nodeIter == mDirectionalLines.end())
		return false;
	return true;
}

void DynamicObject::CDynamicObject::addRegion( const std::string& id,const std::string& regionType )
{
	mRegions.insert(std::make_pair(id,regionType));
}

bool DynamicObject::CDynamicObject::exsitRegion( const std::string& id )
{
	std::map<std::string,std::string>::iterator regionIter = mRegions.find(id);
	if(regionIter == mRegions.end())
		return false;
	return true;
}

void DynamicObject::CDynamicObject::removeRegion( const std::string& id )
{
	std::map<std::string,std::string>::iterator regionIter = mRegions.find(id);
	if(regionIter == mRegions.end())
		return;
	mRegions.erase(regionIter);
}

void DynamicObject::CDynamicObject::setIsInWarnningRegion( const bool& isInRegion )
{
	mIsInWarnningRegion = isInRegion;
}

bool DynamicObject::CDynamicObject::isInWarnningRegion()
{
	return mIsInWarnningRegion;
}

void DynamicObject::CDynamicObject::setIsInNoFlyingRegion( const bool& isInRegion )
{
	mIsInNoFlyingRegion = isInRegion;
}

bool DynamicObject::CDynamicObject::isInNoFlyingRegion()
{
	return mIsInNoFlyingRegion;
}

int DynamicObject::CDynamicObject::getRegionSize()
{
	return mRegions.size();
}

bool DynamicObject::CDynamicObject::exsitRegionType( const std::string& regionType )
{
	std::map<std::string,std::string>::iterator regionIter = mRegions.begin();
	while(regionIter != mRegions.end())
	{
		if(regionIter->second == regionType)
			return true;
		regionIter++;
	}
	return false;
}

void DynamicObject::CDynamicObject::setNearestUpdateTime( const double& nearestUpdateTime )
{
	mNearestUpdateTime = nearestUpdateTime;
}

double DynamicObject::CDynamicObject::getNearestUpdateTime()
{
	return mNearestUpdateTime;
}

void DynamicObject::CDynamicObject::setTrackLineFadingTimeInterval( const double& timeInterval )
{
	mTrackLineFadingTimeInterval = timeInterval;
}

double DynamicObject::CDynamicObject::getTrackLineFadingTimeInterval()
{
	return mTrackLineFadingTimeInterval;
}

void DynamicObject::CDynamicObject::setIconUnVisibleTimeInterval( const double& timeInterval )
{
	mIconUnVisibleTimeInterval = timeInterval;
}

double DynamicObject::CDynamicObject::getIconUnVisibleTimeInterval()
{
	return mIconUnVisibleTimeInterval + getTrackLineFadingTimeInterval();
}

void DynamicObject::CDynamicObject::setDestroyTimeInterval( const double& timeInterval )
{
	mDestroyTimeInterval = timeInterval;
}

double DynamicObject::CDynamicObject::getDestroyTimeInterval()
{
	return mDestroyTimeInterval + getIconUnVisibleTimeInterval();
}

int DynamicObject::CDynamicObject::getNeedUpatePathPointSize()
{
	int result = 0;
	if (mPathMutex.lock() == 0)
	{
		result = mPathPoints.size();
		mPathMutex.unlock();
	}
	return result;
}

void DynamicObject::CDynamicObject::setAssociatedFacilityID( const std::string& facilityID )
{
	mAssociatedFacilityID = facilityID;
}

std::string DynamicObject::CDynamicObject::getAssociatedFacilityID()
{
	return mAssociatedFacilityID;
}

void DynamicObject::CDynamicObject::setIconScale( const double& scale )
{
	mIconScale = scale;
}

double DynamicObject::CDynamicObject::getIconScale()
{
	return mIconScale;
}

void DynamicObject::CDynamicObject::setScutcheonUnVisibleTimeInterval( const double& timeInterval )
{
	mScutcheonUnVisibleTimeInterval = timeInterval;
}

double DynamicObject::CDynamicObject::getScutcheonUnVisibleTimeInterval()
{
	return mScutcheonUnVisibleTimeInterval + getTrackLineFadingTimeInterval();
}

void DynamicObject::CDynamicObject::changeScutcheonHeaderColor( const osgEarth::Symbology::Color& headerColor )
{
	if(NULL == mpGeoScutcheon)
		return;
	mScutcheonHeaderColor = mpGeoScutcheon->getTitleSymbol()->fill()->color();
	mpGeoScutcheon->getTitleSymbol()->fill()->color() = headerColor;
	mIsScutcheonHeaderColorChanged = true;
}

bool DynamicObject::CDynamicObject::getNeedRecoverTrackLineFadState()
{
	bool hasData = false;
	if (mPathMutex.lock() == 0)
	{
		hasData = !this->mPathPoints.empty();
		mPathMutex.unlock();
	}
	return mIsTrackLineColorFad && hasData;
}

bool DynamicObject::CDynamicObject::getIsTrackLineFadState()
{
	return this->mIsTrackLineColorFad;
}

void DynamicObject::CDynamicObject::setIsTrackLineFadState(const bool& value)
{
	this->mIsTrackLineColorFad = value;
}

void DynamicObject::CDynamicObject::setTrackLineFadColor(const osg::Vec4f& trackLineColor)
{
	mIsTrackLineColorFad = true;
	this->changeTrackLineColor(trackLineColor);
}

void DynamicObject::CDynamicObject::changeTrackLineColor( const osg::Vec4f& trackLineColor )
{
	mTrackLineColor = trackLineColor;
	mIsTrackLineColorChanged = true;
}

void DynamicObject::CDynamicObject::recoverScutcheonHeaderColor()
{
	if(NULL == mpGeoScutcheon)
		return;
	mpGeoScutcheon->getTitleSymbol()->fill()->color() = mScutcheonHeaderColor;
	mIsScutcheonHeaderColorChanged = true;
}

void DynamicObject::CDynamicObject::recoverTrackLineColor()
{
	setTrackLineColor(mTrackLineOriginalColor);
}

void DynamicObject::CDynamicObject::setScutcheonVisibleFields( const Geo::Fields fields )
{
	mFields = fields;
	if(NULL == mpGeoScutcheon)
		return;
	this->mScuteonFieldMutex.lock();
	mpGeoScutcheon->getFields().clear();
	for (int index = 0; index < mFields.size(); ++index)
	{
		if(true == mFields.at(index).valid)
			mpGeoScutcheon->getFields().push_back(Geo::Field(mFields.at(index).name,mFields.at(index).value));
	}
	mIsFieldsChanged = true;
	mScuteonFieldMutex.unlock();
}

Geo::Fields DynamicObject::CDynamicObject::getScutcheonVisibleFields()
{
	return mFields;
}

void DynamicObject::CDynamicObject::setObjectVisible( const bool& isVisible )
{
	setTrackLineVisible(isVisible);
	setIconVisible(isVisible);
	setScutcheonVisible(isVisible);
	setDirectionallineVisible(isVisible);
}

void DynamicObject::CDynamicObject::setIsInScanningRegion( const bool& isInRegion )
{
	mIsInScanningRegion = isInRegion;
}

bool DynamicObject::CDynamicObject::isInScanningRegion()
{
	return mIsInScanningRegion;
}

void DynamicObject::CDynamicObject::setObejctBlinkInterval( const double& timeInterval )
{
	mObejctBlinkTimeInterval = timeInterval;
	mIsBlink = true;
}

double DynamicObject::CDynamicObject::getObejctBlinkInterval()
{
	return mObejctBlinkTimeInterval;
}

void DynamicObject::CDynamicObject::setObejctBlink( const double& time )
{
	mObejctBlinkTime = time;
	mIsBlink = true;
}

double DynamicObject::CDynamicObject::getObjectBlink()
{
	return mObejctBlinkTimeInterval;
}

void DynamicObject::CDynamicObject::setTrackLinePointPattern(const bool& isPointPattern)
{
	if(mIsTrackPointPattern == isPointPattern)
		return;

	mIsTrackPointPattern = isPointPattern;
	mIsTrackLinePatternChanged = true;
}

osgEarth::Symbology::Style& DynamicObject::CDynamicObject::getIconStyle()
{
	return this->mpGeoPoint->getStyle();
}

osgEarth::Symbology::TextSymbol* DynamicObject::CDynamicObject::getScutcheonTitleSymbol()
{
	return mpGeoScutcheon->getTitleSymbol();
}

osgEarth::Symbology::TextSymbol* DynamicObject::CDynamicObject::getScutcheonContentSymbol()
{
	return mpGeoScutcheon->getContentSymbol();
}

osgEarth::Symbology::Style& DynamicObject::CDynamicObject::getScutcheonStyle()
{
	return this->mpGeoScutcheon->getStyle();
}

osgEarth::Symbology::Style& DynamicObject::CDynamicObject::getScutcheonTitleImageStyle()
{
	return mpGeoScutcheon->getTitleImageStyle();
}

void DynamicObject::CDynamicObject::updateIconStyle()
{
	mIsIconStyleChanged = true;
}

void DynamicObject::CDynamicObject::updateScutcheonStyle()
{
	mIsScutcheonStyleChanged = true;
}

void DynamicObject::CDynamicObject::setScutcheonScreenCoordinatesOffset( const osg::Vec2s& offset )
{
	mScutcheonScreenCoordinatesOffset = offset;
	//mpGeoScutcheon->setScreenCoordinatesOffset(offset);
	mIsScutcheonScreenCoordinatesChanged = true;
}

osg::Vec2s DynamicObject::CDynamicObject::getScutcheonScreenCoordinatesOffset()
{
	if(mpGeoScutcheon)
		return mpGeoScutcheon->getScreenCoordinatesOffset();
	return osg::Vec2s(0.0,0.0);
}

void DynamicObject::CDynamicObject::setScutcheonSeparativeSign( const std::string& separativeSign )
{
	mScutcheonSeparativeSign = separativeSign;
	mpGeoScutcheon->setSeparativeSign(separativeSign);
	mIsScutcheonStyleChanged = true;
}

std::string DynamicObject::CDynamicObject::getScutcheonSeparativeSign()
{
	return mScutcheonSeparativeSign;
}

void DynamicObject::CDynamicObject::setScutcheonVerticalSpacing( const int& verticalSpacing )
{
	mScutcheonVerticalSpacing = verticalSpacing;
	mpGeoScutcheon->setVerticalSpacing(verticalSpacing);
	mIsScutcheonStyleChanged = true;
}

int DynamicObject::CDynamicObject::getScutcheonVerticalSpacing()
{
	return mScutcheonVerticalSpacing;
}

int DynamicObject::CDynamicObject::getIntervalBetweenTitleAndContent()
{
	return this->mIntervalBetweenTitleAndContent;
}

void DynamicObject::CDynamicObject::setIntervalBetweenTitleAndContent(const int& intervalBetweenTitleAndContent)
{
	this->mIntervalBetweenTitleAndContent = intervalBetweenTitleAndContent;
	mpGeoScutcheon->setIntervalBetweenTitleAndContent(this->mIntervalBetweenTitleAndContent);
	mIsScutcheonStyleChanged = true;
}

void DynamicObject::CDynamicObject::setBackImageWidth(const int& backImageWidth)
{
	this->mBackImageWidth = backImageWidth;
	mpGeoScutcheon->setBackImageWidth(this->mBackImageWidth);
	mIsScutcheonStyleChanged = true;
}

int DynamicObject::CDynamicObject::getBackImageWidth()
{
	return this->mBackImageWidth;
}

int DynamicObject::CDynamicObject::getIntervalBetweenBottomAndContent()
{
	return this->mIntervalBetweenBottomAndContent;
}

void DynamicObject::CDynamicObject::setIntervalBetweenBottomAndContent(const int& intervalBetweenBottomAndContent)
{
	this->mIntervalBetweenBottomAndContent = intervalBetweenBottomAndContent;
	mpGeoScutcheon->setIntervalBetweenBottomAndContent(this->mIntervalBetweenBottomAndContent);
	mIsScutcheonStyleChanged = true;
}

int DynamicObject::CDynamicObject::getContentHorizontalOffset()
{
	return this->mContentHorizontalOffset;
}

void DynamicObject::CDynamicObject::setContentHorizontalOffset(const int& contentHorizontalOffset)
{
	this->mContentHorizontalOffset = contentHorizontalOffset;
	mpGeoScutcheon->setContentHorizontalOffset(this->mContentHorizontalOffset);
	mIsScutcheonStyleChanged = true;
}

int DynamicObject::CDynamicObject::getTitleImageOffset()
{
	return this->mTitleImageOffset;
}

void DynamicObject::CDynamicObject::setTitleImageOffset(const int& titleImageOffset)
{
	this->mTitleImageOffset = titleImageOffset;
	mpGeoScutcheon->setTitleImageOffset(this->mTitleImageOffset);
	mIsScutcheonStyleChanged = true;
}

void DynamicObject::CDynamicObject::setTitleImageSize(const osg::Vec2s& titleImageSize)
{
	this->mTitleImageSize = titleImageSize;
	mpGeoScutcheon->setTitleImageSize(this->mTitleImageSize);
	mIsScutcheonStyleChanged = true;
}

osg::Vec2s DynamicObject::CDynamicObject::getTitleImageSize()
{
	return this->mTitleImageSize;
}

double DynamicObject::CDynamicObject::contain(const int& x, const int& y ,osg::Camera* pCamera)
{
	double r = 0;
	if (mpGeoScutcheon)
	{
		r = mpGeoScutcheon->contain(x,y,pCamera);
	}
	return r;
}

void DynamicObject::CDynamicObject::initial( osgViewer::Viewer* pRefViewer )
{
	mpRefViewer = pRefViewer;
}

osg::Vec3d DynamicObject::CDynamicObject::worldToScreen(const osg::Vec3d& worldPoint)
{
	osg::Camera* p_camera = mpRefViewer->getCamera();
	osg::Matrixd VPW = p_camera->getViewMatrix() *
		p_camera->getProjectionMatrix() *
		p_camera->getViewport()->computeWindowMatrix();
	osg::Vec3d window = worldPoint * VPW;
	return window;
}

osg::Vec3d DynamicObject::CDynamicObject::screenToWorld(const osg::Vec3d& screenPoint)
{// 屏幕-〉世界 
	osg::Camera* p_camera = mpRefViewer->getCamera();
	osg::Matrixd VPW = p_camera->getViewMatrix() *
		p_camera->getProjectionMatrix() *
		p_camera->getViewport()->computeWindowMatrix();
	osg::Matrixd inverseVPW;
	inverseVPW.invert(VPW);
	osg::Vec3d world = screenPoint * inverseVPW;
	return world;
}

void DynamicObject::CDynamicObject::setIconTextVisible( const bool& isVisible )
{
	mIsIconTextVisible = isVisible;
	if(isVisible)
		mpGeoPoint->setName("目标" + this->getID());
	else
		mpGeoPoint->setName("");
}

bool DynamicObject::CDynamicObject::getIconTextVisible()
{
	return mIsIconTextVisible;
}

void DynamicObject::CDynamicObject::setScuLineColor( const osg::Vec4f& color )
{
	mScuLineColor = color;
	mIsScuLineStyleChanged = true;
}

osg::Vec4f DynamicObject::CDynamicObject::getScuLineColor()
{
	return mScuLineColor;
}

void DynamicObject::CDynamicObject::setScuLineType( LineType lineType )
{
	mScuLineType = lineType;
	if(NULL == mpScuLineGeode)
		return;
	osg::Geometry* pLineGeom = dynamic_cast<osg::Geometry *>(mpScuLineGeode->getDrawable(0));
	if(NULL == pLineGeom)
		return;
	if(LineType::SOLID_LT == mScuLineType)
	{
		osg::ref_ptr<osg::LineStipple> pLineStipple = dynamic_cast<osg::LineStipple*>(
			pLineGeom->getOrCreateStateSet()->getAttribute(osg::StateAttribute::LINESTIPPLE));
		if(pLineStipple)
		{
			//pLineGeom->getOrCreateStateSet()->setAttribute(pLineStipple.get(),osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
			pLineStipple->setFactor(1);
			pLineStipple->setPattern(0xFFFF);
			pLineGeom->getOrCreateStateSet()->setAttributeAndModes(pLineStipple.get(),osg::StateAttribute::ON);
		}
	}
	else
	{
		osg::ref_ptr<osg::LineStipple> pLineStipple = new osg::LineStipple;
		pLineStipple->setFactor(1);
		if(LineType::DOTTED_LT == mScuLineType)
			pLineStipple->setPattern(0x3F3F);
		else if(LineType::POINT_LT == mScuLineType)
			pLineStipple->setPattern(0x0101);
		else if(LineType::DOTDASH_LT == mScuLineType)
			pLineStipple->setPattern(0x27FF);
		else if(LineType::CENTRELINE_LT == mScuLineType)
			pLineStipple->setPattern(0x33FF);
		else if(LineType::DOUBLEDOT_LT == mScuLineType)
			pLineStipple->setPattern(0x24FF);
		else if(LineType::TRIDOT_LT == mScuLineType)
			pLineStipple->setPattern(0x55FF);
		pLineGeom->getOrCreateStateSet()->setAttributeAndModes(pLineStipple.get(),osg::StateAttribute::ON);
	}
}

DynamicObject::LineType DynamicObject::CDynamicObject::getScuLineType()
{
	return mScuLineType;
}

void DynamicObject::CDynamicObject::setScuLineVisible( const bool& isVisible )
{
	mIsScuLineVisible = isVisible;
	if(mIsScuLineVisible)
	{
		mpScuLineGeode->setNodeMask(1);
	}
	else
	{
		mpScuLineGeode->setNodeMask(0);
	}
}

bool DynamicObject::CDynamicObject::getScuineVisible()
{
	return mIsScuLineVisible;
}

void DynamicObject::CDynamicObject::setScuLineWidth( const float& lineWidth )
{
	mScuLineWitdh = lineWidth;
	if(NULL == mpScuLineGeode)
		return;
	osg::Geometry* pLineGeom = dynamic_cast<osg::Geometry *>(mpScuLineGeode->getDrawable(0));
	if(NULL == pLineGeom)
		return;
	pLineGeom->getOrCreateStateSet()->setAttributeAndModes(new osg::LineWidth(mScuLineWitdh));
}

float DynamicObject::CDynamicObject::getScuLineWidth()
{
	return mScuLineWitdh;
}

void DynamicObject::CDynamicObject::updateScutcheonContent(const Geo::Fields fields)
{
	if(NULL == mpGeoScutcheon)
		return;
	mpGeoScutcheon->getFields().clear();
	mFields = fields;
	for (int index = 0; index < mFields.size(); ++index)
	{
		if(true == mFields.at(index).valid)
			mpGeoScutcheon->getFields().push_back(Geo::Field(fields.at(index).name,fields.at(index).value));
	}
	mIsScutcheonContentChanged = true;
}

void DynamicObject::CDynamicObject::listenObjectCull(osg::NodeVisitor* nv)
{
	if(NULL == mpGeoScutcheon)
		return;
	//进行包围盒裁剪
	osgUtil::CullVisitor* cv = osgEarth::Culling::asCullVisitor(nv);
	std::vector<osg::Vec3> vertices;
	vertices.push_back(mCurrentWorldPos);
	bool isOutOfView = cv->isCulled(vertices);
	if(isOutOfView)
	{
		if(mIsScutcheonVisible)
			mpGeoScutcheon->setVisible(false);

		if(mIsScuLineVisible)
			mpScuLineGeode->setNodeMask(0);
	}
	else
	{
		if(mIsScutcheonVisible)
			mpGeoScutcheon->setVisible(true);

		if(mIsScuLineVisible)
			mpScuLineGeode->setNodeMask(1);
	}
}

void DynamicObject::CDynamicObject::setCurrentScutcheonVisibleFields( const Geo::Fields fields )
{
	mCurrentFields = fields;
	if(NULL == mpGeoScutcheon)
		return;
	this->mScuteonFieldMutex.lock();
	mpGeoScutcheon->getFields().clear();
	for (int index = 0; index < mCurrentFields.size(); ++index)
	{
		mpGeoScutcheon->getFields().push_back(Geo::Field(mCurrentFields.at(index).name,mCurrentFields.at(index).value));
	}
	mIsCurrentFieldsChanged = true;
	this->mScuteonFieldMutex.unlock();
}

Geo::Fields DynamicObject::CDynamicObject::getCurrentScutcheonVisibleFields()
{
	return mCurrentFields;
}

void DynamicObject::CDynamicObject::updateCurrentScutcheonContent( const Geo::Fields fields )
{
	mCurrentFields = fields;
	if(NULL == mpGeoScutcheon)
		return;
	this->mScuteonFieldMutex.lock();
	mpGeoScutcheon->getFields().clear();
	for (int index = 0; index < mCurrentFields.size(); ++index)
	{
		mpGeoScutcheon->getFields().push_back(Geo::Field(mCurrentFields.at(index).name,mCurrentFields.at(index).value));
	}
	mIsScutcheonCurrentContentChanged = true;
	this->mScuteonFieldMutex.unlock();
}

const std::string DynamicObject::CDynamicObjectType::UAV = "CDynamicObjectType::UAV";

const std::string DynamicObject::CDynamicObjectType::BIRD = "CDynamicObjectType::BIRD";

const std::string DynamicObject::CDynamicObjectGroupType::RADARTARGET = "CDynamicObjectGroupType::RADARTARGET";

const std::string DynamicObject::CDynamicObjectGroupType::WIRELESSTARGET = "CDynamicObjectGroupType::WIRELESSTARGET";
