#include "geometryeditor/geometryeditor.h"
#include <math.h>
#include <osgEarthAnnotation/Draggers>

namespace GeometryEditor
{
	IEPointSet::IEPointSet()
	{
	
	}

	IEPointSet::~IEPointSet()
	{
		if (!this->Data.empty())
		{
			for (int i = 0; i < this->Data.size(); ++i)
			{
				delete this->Data.at(i);
			}
			this->Data.clear();
		}
	}

	class MoveFeatureDraggerCallbackEx :
		public osgEarth::Annotation::Dragger::PositionChangedCallback
	{
	public:
		MoveFeatureDraggerCallbackEx(
			osgEarth::Annotation::FeatureNode* featureNode, int point) :
			_featureNode(featureNode),_point(point)
			{

			}

			virtual void onPositionChanged(
			const osgEarth::Annotation::Dragger* sender,
			const osgEarth::GeoPoint& position)
			{
				SphereDraggerEx* pOwnerDragger =
					dynamic_cast<SphereDraggerEx*>(const_cast<osgEarth::Annotation::Dragger*>(sender));

				if (pOwnerDragger
					&& pOwnerDragger->mpRefOwnerPoint 
					&& pOwnerDragger->mpRefOwnerEditor
					&& pOwnerDragger->mpRefOwnerEditor->isEditingState())
				{
					osgEarth::Annotation::Dragger::PositionChangedCallback::onPositionChanged(
						sender, position);
					(*_featureNode->getFeature()->getGeometry())[_point] = osg::Vec3d(position.x(), position.y(), position.z());
					_featureNode->init();

					if (pOwnerDragger->mpRefOwnerEditor->getGeometry().size() > _point)
					{
						_point = pOwnerDragger->mpRefOwnerPoint->getIndex();
						pOwnerDragger->mpRefOwnerEditor->getGeometry()[_point]->setGeoPosition(
							osg::Vec3d(position.x(), position.y(), position.z()));
					}
				}
			}

			osg::ref_ptr< osgEarth::Annotation::FeatureNode > _featureNode;

			int _point;

	};

	SphereDraggerEx::SphereDraggerEx(osgEarth::MapNode* mapNode)
		: osgEarth::Annotation::SphereDragger(mapNode),
		mpRefOwnerPoint(NULL), mpRefOwnerEditor(NULL), mNeedHandleGUIEvent(true)
	{

	}

	bool SphereDraggerEx::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		if (mNeedHandleGUIEvent && mpRefOwnerEditor)
		{
			if (mpRefOwnerEditor->isEditingState()
				|| osgGA::GUIEventAdapter::MOVE == ea.getEventType())
			{
				return osgEarth::Annotation::SphereDragger::handle(ea, aa);
			}
		}
		return false;
	}

	void SphereDraggerEx::enter()
	{
		if (mNeedHandleGUIEvent)
		{
			osgEarth::Annotation::SphereDragger::enter();
			if (mpRefOwnerPoint && mpRefOwnerEditor)
			{
				this->mpRefOwnerEditor->setCurrentPointEnter(mpRefOwnerPoint);
			}
		}
		
	}

	void SphereDraggerEx::leave()
	{
		if (mNeedHandleGUIEvent)
		{
			osgEarth::Annotation::SphereDragger::leave();
			if (mpRefOwnerEditor)
			{
				this->mpRefOwnerEditor->setCurrentPointEnter(NULL);
			}
		}
	}

	void SphereDraggerEx::setNeedHandleGUIEvent(const bool& needHandleGUIEvent)
	{
		this->mNeedHandleGUIEvent = needHandleGUIEvent;
	}

	bool SphereDraggerEx::getNeedHandleGUIEvent()
	{
		return this->mNeedHandleGUIEvent;
	}

	IEditPoint::IEditPoint()
	{
		this->mGeoposition = osg::Vec3d(0, 0, 0);
		this->mIndex = -1;
		this->mpRefUserData = NULL;
	}

	int& IEditPoint::getIndex()
	{
		return this->mIndex;
	}

	void IEditPoint::setIndex(const int& index)
	{
		this->mIndex = index;
	}

	IEditPoint::~IEditPoint()
	{

	}


	osg::Vec3d& IEditPoint::getGeoPosition()
	{
		return this->mGeoposition;
	}

	void IEditPoint::setGeoPosition(const osg::Vec3d& geoPosition)
	{
		this->mGeoposition = geoPosition;
	}

	void IEditPoint::setUserData(osg::ref_ptr<osg::Referenced> pUserData)
	{
		this->mpRefUserData = pUserData;
	}

	osg::ref_ptr<osg::Referenced>& IEditPoint::getUserData()
	{
		return this->mpRefUserData;
	}

	IGeometryEditor::IGeometryEditor(osgEarth::MapNode* pRefMapNode)
		: mpRefMapNode(pRefMapNode), mpPointsEdit(NULL),
		mpGeometryNode(NULL), mpRefViewer(NULL),mpRefCurrentPoint(NULL)
	{
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
			->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
			->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
		this->mGeometryStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()
			->verticalOffset() = 0.1;

		//可编辑点的风格
		this->getPointsEditStyle().getOrCreateSymbol<osgEarth::Symbology::PointSymbol>()->size() = 8.0;
		this->getPointsEditStyle().getOrCreateSymbol<osgEarth::Symbology::PointSymbol>()->fill()->color() =
			osgEarth::Symbology::Color(osg::Vec4(1, 0, 0, 1));
		this->mIsEditState = false;
		this->mIsInsertPointState = false;
		this->mCanQueryPoint = false;
		this->mVisible = true;
		this->mPreInsertPointIndex = -1;
		this->mNextInsertPointIndex = -1;
		this->mpCurrentEditingGeometry = NULL;
		this->mIsDragState = false;
		this->mCurGeoPostion = osg::Vec3d(0,0,0);
		this->mMouseDownAppendPointMode = false;
		this->mNeedMoveGraphic = true;
	}


	IGeometryEditor::~IGeometryEditor()
	{
		this->clearRenderResource();
		if (!mOverEditedGeometrySet.empty())
		{
			for (int i = 0; i < mOverEditedGeometrySet.size(); ++i)
			{
				delete mOverEditedGeometrySet.at(i);
			}
			this->mOverEditedGeometrySet.clear();
		}
	}

	void IGeometryEditor::setVisible(const bool& visible)
	{
		this->mVisible = visible;
		if (this->mpGeometryNode != NULL)
		{
			int mask = this->getVisible() ? 1 : 0;
			this->mpGeometryNode->setNodeMask(mask);
			if (this->mpPointsEdit)
			{
				this->mpPointsEdit->setNodeMask(mask);
			}
			
		}
	}

	bool IGeometryEditor::getVisible()
	{
		return this->mVisible;
	}

	void IGeometryEditor::initial()
	{

	}

	void IGeometryEditor::setCurrentPointEnter(IEditPoint* pCurrentPoint)
	{
		this->mpRefCurrentPoint = pCurrentPoint;
	}

	void IGeometryEditor::createEditorCallBack()
	{
#if 1
		if (mpPointsEdit
			&& this->mpPointsEdit->getNumChildren() > 0)
		{
			this->mpPointsEdit->removeChildren(0, this->mpPointsEdit->getNumChildren());
			//Create a dragger for each point
			for (unsigned int i = 0;
				i < this->getGeometry().size();
				i++)
			{
				IEditPoint* pEditorPoint = this->getGeometry().at(i);
				if (NULL == pEditorPoint)
				{
					throw std::exception("");
				}
				SphereDraggerEx* dragger = new SphereDraggerEx(this->mpRefMapNode);
				dragger->mpRefOwnerPoint = pEditorPoint;
				dragger->mpRefOwnerEditor = this;
				dragger->setColor(this->mpPointsEdit->getColor());
				dragger->setPickColor(this->mpPointsEdit->getPickColor());
				dragger->setSize(this->mpPointsEdit->getSize());
				dragger->setPosition(
					osgEarth::GeoPoint(
					this->mpGeometryNode->getFeature()->getSRS(),
					pEditorPoint->getGeoPosition().x(),
					pEditorPoint->getGeoPosition().y()));
				MoveFeatureDraggerCallbackEx*
					pEditorCallBack = new MoveFeatureDraggerCallbackEx(this->mpGeometryNode.get(), i);
				dragger->addPositionChangedCallback(pEditorCallBack);
				this->mpPointsEdit->addChild(dragger);
			}
		}
#endif
	}

	bool IGeometryEditor::beginEdit()
	{
		try
		{
			//this->clearRenderResource();
			if (this->mpCurrentEditingGeometry)
			{
				delete this->mpCurrentEditingGeometry;
			}
			this->mpCurrentEditingGeometry = new IEPointSet();
		}
		catch (...)
		{

		}
		this->mIsEditState = true;
		return true;
	}

	void IGeometryEditor::clearRenderResource()
	{
		if (this->mpGeometryNode)
		{
			osg::ref_ptr<osgEarth::Symbology::Geometry> pGeometry = this->mpGeometryNode->getFeature()->getGeometry();
			for (int i = 0; i < this->getGeometry().size(); ++i)
			{
				IEditPoint* pPoint = this->getGeometry().at(i);
				if (pPoint)
				{
					delete pPoint;
				}
			}
			this->getGeometry().clear();
			pGeometry->clear();
		}

		if (this->mpPointsEdit != NULL)
		{
			int count = this->mpPointsEdit->getNumChildren();
			this->mpPointsEdit->removeChildren(0, count);
			this->mpPointsEdit->getParent(0)->removeChild(this->mpPointsEdit);
			this->mpPointsEdit = NULL;
		}

		if (this->mpGeometryNode != NULL)
		{
			int count = this->mpGeometryNode->getNumChildren();
			this->mpGeometryNode->removeChildren(0, count);
			this->mpGeometryNode->getParent(0)->removeChild(this->mpGeometryNode);
			this->mpGeometryNode = NULL;
		}
	}

	bool IGeometryEditor::endEdit(
		const bool& clearCurrentGeometry ,
		const bool& needSaveEditingResult ,
		const bool& hideOriginGraphicObject)
	{
		if (clearCurrentGeometry)
		{
			try
			{
				this->clearRenderResource();
				if (this->mpCurrentEditingGeometry)
				{
					if (this->mpCurrentEditingGeometry->Data.size() > 1)
					{
						this->mOverEditedGeometrySet.push_back(this->mpCurrentEditingGeometry);
						this->mpCurrentEditingGeometry = NULL;
					}
					else
					{
						delete this->mpCurrentEditingGeometry;
						this->mpCurrentEditingGeometry = NULL;
					}
					
				}
			}
			catch (...)
			{

			}
		}
		this->mIsEditState = false;

		return (this->mIsEditState == false);
	}

	std::vector<IEditPoint*>& IGeometryEditor::getGeometry()
	{
		if (NULL == this->mpCurrentEditingGeometry)
			this->mpCurrentEditingGeometry = new IEPointSet();
		return this->mpCurrentEditingGeometry->Data;
	}

	IEPointSet* IGeometryEditor::getCurrentEditingPointSet()
	{
		return this->mpCurrentEditingGeometry;
	}

	std::vector<IEPointSet*>& IGeometryEditor::getAllGemetries()
	{
		return this->mOverEditedGeometrySet;
	}

	osgEarth::Symbology::Style& IGeometryEditor::getPointsEditStyle()
	{
		return this->mPointsEditStyle;
	}

	osgEarth::Symbology::Style& IGeometryEditor::getGeometryStyle()
	{
		return this->mGeometryStyle;
	}

	void IGeometryEditor::setViewer(osgViewer::Viewer* pRefViewer)
	{
		this->mpRefViewer = pRefViewer;
	}

	bool IGeometryEditor::getCanQueryPoint()
	{
		return this->mCanQueryPoint;
	}

	void IGeometryEditor::setCanQueryPoint(const bool& canQueryPoint)
	{
		this->mCanQueryPoint = canQueryPoint;
	}

	bool IGeometryEditor::isEditingState()
	{
		return this->mIsEditState;
	}

	Common::ISceneObject* IGeometryEditor::generateSceneObject()
	{
		return NULL;
	}

	void IGeometryEditor::notifyStyleChanged()
	{
		if (this->mpGeometryNode)
		{
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

	void IGeometryEditor::removeRenderResource()
	{
		this->clearRenderResource();
	}

#if 0
	bool IGeometryEditor::handle(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& aa)
	{
		// 存储经纬度信息
		osg::Vec3d vecPos;
		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::PUSH:
		{
			if (this->mIsEditState)
			{
				osg::Vec3d pos = getGeoPosition(ea, aa, vecPos);

				if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
				{
					mCurGeoPostion = vecPos;
				}

			}
			mIsDragState = false;
			break;
		}
		case osgGA::GUIEventAdapter::KEYDOWN:
		{
			if (this->mIsEditState)
			{
				if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Delete)
				{
					this->performDeleteDrawing();
				}
				else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_P)//捕捉点
				{
					if (NULL != this->mpRefCurrentPoint)
					{
						if (this->mPreInsertPointIndex == -1)
						{
							this->mPreInsertPointIndex = this->mpRefCurrentPoint->getIndex();
						}
						else if (this->mNextInsertPointIndex == -1
							&& 1 == std::abs(this->mPreInsertPointIndex - this->mpRefCurrentPoint->getIndex()))
						{
							this->mNextInsertPointIndex = this->mpRefCurrentPoint->getIndex();
						}
					}

				}
				else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_U)//捕捉点
				{
					this->mPreInsertPointIndex = -1;
					this->mNextInsertPointIndex = -1;
				}
			}
		}
		case osgGA::GUIEventAdapter::MOVE:
		{
			if (this->mIsEditState && !mIsDragState)
			{
				osg::Vec3d pos = getGeoPosition(ea, aa, vecPos);
				if (pos != osg::Vec3d(0,0,0))
				{
					this->performMovingXYZ(vecPos);
				}
				
			}

			break;
		}
		case osgGA::GUIEventAdapter::DRAG:
		{
			if (this->mIsEditState)
			{
				mIsDragState = true;
				osg::Vec3d pos = getGeoPosition(ea, aa, vecPos);
				if (pos != osg::Vec3d(0,0,0))
				{
					this->performMoving(vecPos);
					if (this->mpRefCurrentPoint)
					{
						return true;
					}
					return this->performMoveGraphic(vecPos);
				}
			}
			break;
		}
		case osgGA::GUIEventAdapter::RELEASE:
		{
			if (this->mIsEditState)
			{
				osg::Vec3d pos = getGeoPosition(ea, aa, vecPos);
				if (!mIsDragState)
				{
					if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
					{
#if 0
						// 如果释放的点和点击的点同一，则发送单击事件发生的位置
						if (abs(mCurGeoPostion.x() - pos.x()) < 1e-5 &&
							abs(mCurGeoPostion.y() - pos.y()) < 1e-5 &&
							abs(mCurGeoPostion.z() - pos.z()) < 1e-5 &&
							mCurGeoPostion != osg::Vec3d(0, 0, 0))
#else
						if (pos != osg::Vec3d(0, 0, 0))
#endif
						{
							this->performPicked(vecPos);
							this->performPickedXYZ(pos);
							this->mPreInsertPointIndex = -1;
							this->mNextInsertPointIndex = -1;
						}

					}
					else if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
					{
						this->performRightPicked();
					}
				}
				else
				{
					this->performEndMoveGraphic(vecPos);
				}
			}
			else if (this->getCanQueryPoint())
			{
				if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
				{
					this->forceListenerEvents(this->mpRefCurrentPoint);
				}
			}
			this->mCurGeoPostion = osg::Vec3d(0,0,0);
			this->mIsDragState = false;
			break;
		}
		case osgGA::GUIEventAdapter::DOUBLECLICK:
		{
			if (this->mIsEditState)
			{
				osg::Vec3d pos = getGeoPosition(ea, aa, vecPos);
				this->performDoublePicked(pos);
				return true;
			}
			break;
		}
		}
		return false;
	}
#else
	bool IGeometryEditor::handle(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& aa)
	{
		// 存储经纬度信息
		osg::Vec3d vecPos;
		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::PUSH:
		{
			if (this->mIsEditState)
			{
				osg::Vec3d pos = getGeoPosition(ea, aa, vecPos);

				if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
				{
					mCurGeoPostion = vecPos;
					if (this->mMouseDownAppendPointMode)
					{
						if (pos != osg::Vec3d(0,0,0))
						{
							this->performPicked(vecPos);
							this->performPickedXYZ(pos);
							this->mPreInsertPointIndex = -1;
							this->mNextInsertPointIndex = -1;
						}	
					}
				}

			}
			mIsDragState = false;
			break;
		}
		case osgGA::GUIEventAdapter::KEYDOWN:
		{
			if (this->mIsEditState)
			{
				if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Delete)
				{
					this->performDeleteDrawing();
				}
				else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_P)//捕捉点
				{
					if (NULL != this->mpRefCurrentPoint)
					{
						if (this->mPreInsertPointIndex == -1)
						{
							this->mPreInsertPointIndex = this->mpRefCurrentPoint->getIndex();
						}
						else if (this->mNextInsertPointIndex == -1
							&& 1 == std::abs(this->mPreInsertPointIndex - this->mpRefCurrentPoint->getIndex()))
						{
							this->mNextInsertPointIndex = this->mpRefCurrentPoint->getIndex();
						}
					}

				}
				else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_U)//捕捉点
				{
					this->mPreInsertPointIndex = -1;
					this->mNextInsertPointIndex = -1;
				}
			}
		}
		case osgGA::GUIEventAdapter::MOVE:
		{
			if (this->mIsEditState && !mIsDragState)
			{
				osg::Vec3d pos = getGeoPosition(ea, aa, vecPos);
				if (pos != osg::Vec3d(0,0,0))
				{
					this->performMovingXYZ(vecPos);
				}
				
			}

			break;
		}
		case osgGA::GUIEventAdapter::DRAG:
		{
			if (this->mIsEditState)
			{
				mIsDragState = true;
				osg::Vec3d pos = getGeoPosition(ea, aa, vecPos);
				if (pos != osg::Vec3d(0,0,0))
				{
					this->performMoving(vecPos);
					if (this->mpRefCurrentPoint)
					{
						return true;
					}
					if (this->mNeedMoveGraphic)
					{
						return this->performMoveGraphic(vecPos);
					}
				}
			}
			break;
		}
		case osgGA::GUIEventAdapter::RELEASE:
		{
			if (this->mIsEditState)
			{
				osg::Vec3d pos = getGeoPosition(ea, aa, vecPos);
				if (!mIsDragState)
				{
					if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
					{
#if 0
						// 如果释放的点和点击的点同一，则发送单击事件发生的位置
						if (abs(mCurGeoPostion.x() - pos.x()) < 1e-5 &&
							abs(mCurGeoPostion.y() - pos.y()) < 1e-5 &&
							abs(mCurGeoPostion.z() - pos.z()) < 1e-5 &&
							mCurGeoPostion != osg::Vec3d(0, 0, 0))
#else
						if (pos != osg::Vec3d(0, 0, 0))
#endif
						{
							if (!this->mMouseDownAppendPointMode)
							{
								this->performPicked(vecPos);
								this->performPickedXYZ(pos);
								this->mPreInsertPointIndex = -1;
								this->mNextInsertPointIndex = -1;
							}
						}

					}
					else if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
					{
						this->performRightPicked();
					}
				}
				else
				{
					this->performEndMoveGraphic(vecPos);
				}
			}
			else if (this->getCanQueryPoint())
			{
				if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
				{
					this->forceListenerEvents(this->mpRefCurrentPoint);
				}
			}
			this->mCurGeoPostion = osg::Vec3d(0,0,0);
			this->mIsDragState = false;
			break;
		}
		case osgGA::GUIEventAdapter::DOUBLECLICK:
		{
			if (this->mIsEditState)
			{
				osg::Vec3d pos = getGeoPosition(ea, aa, vecPos);
				this->performDoublePicked(pos);
				return true;
			}
			break;
		}
		}
		return false;
	}
#endif

	osg::Vec3d IGeometryEditor::getGeoPosition(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter& aa, osg::Vec3d& pos)
	{
#if 1
		pos = osg::Vec3d(0, 0, 0);
		osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		if (pViewer == NULL)
		{
			return osg::Vec3d(0, 0, 0);
		}
		// 获取当前点
		osgUtil::LineSegmentIntersector::Intersections intersection;
		double x = ea.getX();
		double y = ea.getY();
		pViewer->computeIntersections(ea.getX(), ea.getY(), intersection);
		osgUtil::LineSegmentIntersector::Intersections::iterator iter
			= intersection.begin();
		while (iter != intersection.end())
		{
			if (iter->indexList.size() > 0 && iter->getWorldIntersectPoint().length() > 100000)
			{
				this->mpRefMapNode->getMapSRS()->getEllipsoid()->convertXYZToLatLongHeight(
					iter->getWorldIntersectPoint().x(), iter->getWorldIntersectPoint().y(), iter->getWorldIntersectPoint().z(),
					pos.y(), pos.x(), pos.z());
				pos.x() = osg::RadiansToDegrees(pos.x());
				pos.y() = osg::RadiansToDegrees(pos.y());
				pos.z() = 0;

				return iter->getWorldIntersectPoint();
			}
			++iter;
		}
		return osg::Vec3d(0, 0, 0);
#else
		pos = osg::Vec3d(0, 0, 0);
		osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		if (pViewer == NULL)
		{
			return osg::Vec3d(0, 0, 0);
		}
		osg::Vec3d geoPosition(-1,-1,0);
		if (pViewer == NULL)
			return geoPosition;
		osg::Vec3d world;
		if ( this->mpRefMapNode->getTerrain()->getWorldCoordsUnderMouse(
			pViewer, ea.getX(), ea.getY(), world) )
		{
			osgEarth::GeoPoint map;
			map.fromWorld( this->mpRefMapNode->getMapSRS(), world );
			geoPosition = map.vec3d();
		}
		geoPosition.z() = 0;
		return geoPosition;
#endif
	}

	void IGeometryEditor::performDoublePicked(osg::Vec3d pos)
	{
		//this->forceListenerEvents(NULL);
	}

	void IGeometryEditor::performPicked(osg::Vec3d pos)
	{

	}

	void IGeometryEditor::performMoving(osg::Vec3d pos)
	{

	}

	void IGeometryEditor::performPickedXYZ(osg::Vec3d pos)
	{

	}

	bool IGeometryEditor::performMoveGraphic(osg::Vec3d pos)
	{
		return false;
	}

	bool IGeometryEditor::performEndMoveGraphic(osg::Vec3d pos)
	{
		return false;
	}

	void IGeometryEditor::performMovingXYZ(osg::Vec3d pos)
	{

	}

	void IGeometryEditor::performRightPicked()
	{
		this->mIsEditState = false;
	}

	void IGeometryEditor::performDeleteDrawing()
	{
		/*if (this->mpRefCurrentPoint)
		{
			if (this->mpRefCurrentPoint->getIndex()
				< this->getGeometry().size() - 1)
			{
				for (int i = this->mpRefCurrentPoint->getIndex();
					i < this->getGeometry().size(); ++i)
				{
					int ogriginIndex = this->getGeometry().at(i)->getIndex() - 1;
					this->getGeometry().at(i)->setIndex(ogriginIndex);
				}
			}
			this->getGeometry().erase(this->getGeometry().begin()
				+ this->mpRefCurrentPoint->getIndex());
		}*/

	}

	void IGeometryEditor::onPositionChanged(const osgEarth::Annotation::Dragger* sender, const osgEarth::GeoPoint& position)
	{}
}