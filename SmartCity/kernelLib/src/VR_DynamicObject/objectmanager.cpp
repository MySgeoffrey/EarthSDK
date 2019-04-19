#include "dynamicobject/objectmanager.h"
#include "dynamicobject/backgroundthread.h"
#include <osgEarth/GeoMath>
#include "dynamicobject/wirelessobject.h"
#include "geographic/geopolygon.h"
#include "geographic/geocirlce.h"
#include "dynamicobject/calculatorhelper.h"
#include <osgEarth/Registry>
#include <osgEarthUtil/EarthManipulator>

using namespace DynamicObject;

#define USEWIRELESSOBJECT 1
#define MAX_DESTROY_TIME 1000000000
#define DEALFIELDSREDUCE 1
#define _USE_NEW_SCUTEON_ 1
#define _USE_SYN_SIGNAL_ 1

osg::Camera* g_RefCamera = NULL;
bool sortFunc(CDynamicObject* pSceneObject1, CDynamicObject* pSceneObject2)
{
	bool r = false;
	if (g_RefCamera)
	{
		////进行距离判断
		//osg::Vec3d eye, up, center;
		//g_RefCamera->getViewMatrixAsLookAt(eye, center, up);
		//double distance1 = (eye - pSceneObject1->pSceneO).length();
		//double distance2 = (eye - pSceneObject2->getBoundingSphere().center()).length();
		//r = distance1 < distance2;
	}
	return r;
}

#if _USE_NEW_SCUTEON_ == 1
class ModelPickerEx :
	public osgGA::GUIEventHandler
#else
class ModelPickerEx :
	public osgEarth::Util::RTTPicker
#endif
{
public:
	ModelPickerEx()
	{
		mpDynamicObject = NULL;
		mBeginScreenX = 0.0;
		mBeginScreenY = 0.0;
		mObjectOffset = osg::Vec2s(0.0,0.0);
		mLastOffset = osg::Vec2s(0.0,0.0);
		mpOwner = NULL;
	}
	~ModelPickerEx(void)
	{

	}

	void notifyObjectDestroy(Common::IObject* pDynamicObject)
	{
		if ((DynamicObject::CDynamicObject*)pDynamicObject == this->mpDynamicObject)
		{
			this->mpDynamicObject = NULL;
		}
	}

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		bool r = false;
		if (this->mpOwner
			&& !this->mpOwner->getEnableDragScutcheon())
			return r;
#if _USE_NEW_SCUTEON_ == 1

#else
		r = osgEarth::Util::RTTPicker::handle(ea,aa);
		if (NULL == this->mpDynamicObject)
			return false;
#endif
		bool result = false;
		switch(ea.getEventType())
		{
		case osgGA::GUIEventAdapter::PUSH :
			{
				mpDynamicObject = NULL;
				mBeginScreenX = 0.0;
				mBeginScreenY = 0.0;
				mObjectOffset = osg::Vec2s(0.0,0.0);
				mLastOffset = osg::Vec2s(0.0,0.0);
#if _USE_NEW_SCUTEON_ == 1
				if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON
					&& this->mpOwner)
				{
					osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
					this->mpDynamicObject = this->mpOwner->getDynamicObject(
						ea.getX(),ea.getY(),pViewer->getCamera());
				}
#endif
				break;
			}
		case osgGA::GUIEventAdapter::RELEASE :
			{
				mpDynamicObject = NULL;
				mBeginScreenX = 0.0;
				mBeginScreenY = 0.0;
				mObjectOffset = osg::Vec2s(0.0,0.0);
				mLastOffset = osg::Vec2s(0.0,0.0);
				break;
			}
		case osgGA::GUIEventAdapter::DRAG :
			{
				if(mpDynamicObject)
				{
					if (mBeginScreenX == 0
						&& mBeginScreenY == 0)
					{
						mBeginScreenX = ea.getX();
						mBeginScreenY = ea.getY();
						mLastOffset = osg::Vec2s(0.0,0.0);
						mObjectOffset = mpDynamicObject->getScutcheonScreenCoordinatesOffset();
					}

					osg::Vec2s screenOffset =  osg::Vec2s(ea.getX() - mBeginScreenX,ea.getY() - mBeginScreenY);
					mObjectOffset += screenOffset - mLastOffset;
					mLastOffset = screenOffset;
					mpDynamicObject->setScutcheonScreenCoordinatesOffset(mObjectOffset);
					result = true;
				}
				break;
			}
		case osgGA::GUIEventAdapter::DOUBLECLICK:
			{
#if _USE_NEW_SCUTEON_ == 1
				if (this->mpOwner
					&& ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON
					&& this->mpOwner->getDynamicObjectListener())
				{
					osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
					this->mpDynamicObject = this->mpOwner->getDynamicObject(
						ea.getX(),ea.getY(),pViewer->getCamera());
					this->mpOwner->getDynamicObjectListener()->onDoubleClickScutcheon(
						this->mpDynamicObject);
				}
#endif
			}
		default:
			break;
		}
		return result;
	}

public:
	float mBeginScreenX;
	float mBeginScreenY;
	osg::Vec2s mObjectOffset;
	osg::Vec2s mLastOffset;
	DynamicObject::CDynamicObject* mpDynamicObject;
	bool mResult;
	CObjectManager* mpOwner;
};

class ModelPickerExCallback : public osgEarth::Util::RTTPicker::Callback
{
public:
	ModelPickerExCallback(ModelPickerEx* pModelPickerEx)
	{
		mpModelPickerEx = pModelPickerEx;
		mEventType = osgGA::GUIEventAdapter::EventType::NONE;
	}

	void onHit(osgEarth::ObjectID id)
	{
		if (this->mpModelPickerEx->mpDynamicObject == NULL)
		{
			//this->mpModelPickerEx->mpDynamicObject = NULL;
			Common::IObject* pUserData =
				osgEarth::Registry::objectIndex()->get<Common::IObject>(id);
			Geo::CGeoScutcheon* pGeoScutcheon = dynamic_cast<Geo::CGeoScutcheon*>(pUserData);
			if(NULL == pGeoScutcheon)
				return;
			this->mpModelPickerEx->mpDynamicObject = dynamic_cast<DynamicObject::CDynamicObject*>(CObjectManager::instance()->getObject(pGeoScutcheon->getID()));
		}
#if 1 //双击回调
		if (this->mEventType == osgGA::GUIEventAdapter::DOUBLECLICK)
		{
			if (this->mpModelPickerEx->mpOwner
				&& this->mpModelPickerEx->mpOwner->getDynamicObjectListener())
			{
				this->mpModelPickerEx->mpOwner->getDynamicObjectListener()->onDoubleClickScutcheon(
					this->mpModelPickerEx->mpDynamicObject);
			}
		}
		mEventType = osgGA::GUIEventAdapter::NONE;
#endif
	}

	void onMiss()
	{
		this->mpModelPickerEx->mpDynamicObject = NULL;
		mEventType = osgGA::GUIEventAdapter::NONE;
	}

	bool accept(const osgGA::GUIEventAdapter& ea, const osgGA::GUIActionAdapter& aa)
	{
		bool r = false;
		if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH
				|| ea.getEventType() == osgGA::GUIEventAdapter::DOUBLECLICK)
			{
				r = true;
				this->mEventType = osgGA::GUIEventAdapter::NONE;
			}
			if (ea.getEventType() == osgGA::GUIEventAdapter::DOUBLECLICK)
			{
				r = true;
				this->mEventType = ea.getEventType();
			}
		}
		return r;
	}
private:
	osgGA::GUIEventAdapter::EventType mEventType;
	ModelPickerEx* mpModelPickerEx;
};

CObjectManagerCallBack::CObjectManagerCallBack()
{
	this->Owner = NULL;
}

CObjectManagerCallBack::~CObjectManagerCallBack()
{

}

void CObjectManagerCallBack::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	if (this->Owner)
	{
		this->Owner->frame(node,nv);
	}
}

CObjectManager::CObjectManager()
	: mpRefRoot(NULL), mpRefMapNode(NULL), mpRefViewer(NULL)
{
	this->mpBackgroundProcessor = new CBackgroundProcessor();
	this->mpBackgroundProcessor->start();
	mpDynamicObjectListener = NULL;
	mDirectionallineColor = osg::Vec4f(1,1,0,1);
	mDirectionallineType = SOLID_LT;
	mDirectionallineWidth = 5.0;

	mIsTrackLineVisible = false;
	mIsIconVisible = true;
	mIsScutcheonVisible = false;
	mIsDirectionallineVisible = true;
	mIsWholeProprietaryVisibles = false;
	mTrackLineColor = osg::Vec4f(1,0,0,1);
	mTrackLineType = SOLID_LT;
	mTrackLineWidth = 5.0;
	mTrackLinePointSize = 500;
	mDirectionallineDestroyTimeInterval = 1.0;
	mTrackLineFadingTimeInterval = MAX_DESTROY_TIME;
	mIconUnVisibleTimeInterval = MAX_DESTROY_TIME;
	mTrackLineDestroyTimeInterval = MAX_DESTROY_TIME;
	mScutcheonUnVisibleTimeInterval = MAX_DESTROY_TIME;

	mIconFont = "fonts/simhei.ttf";
	mIconTextSize = 20.0;
	mIconTextColor = osgEarth::Symbology::Color::Yellow;
	mScutcheonTitleFont = "fonts/simhei.ttf";
	mScutcheonTitleTextSize = 17.0;
	mScutcheonTitleTextColor = osgEarth::Symbology::Color::Red;
	mScutcheonTitleBackgroundImage = "systemImages/resources/image/pannelbackground_title.png";
	mScutcheonContentFont = "fonts/simhei.ttf";
	mScutcheonContentSize = 14.0;
	mScutcheonContentColor = osgEarth::Symbology::Color::White;
	mScutcheonContentBackgroundImage = "systemImages/resources/image/pannelbackground.png";

	mIsTrackPointPattern = false;
	mIsScutcheonContentUseShadow = true;
	mIsScutcheonTitleUseShadow = true;

	mEnableDragScutcheon = false;
	mpModelPickerExCallback = NULL;
	mpModelPickerEx = NULL;
	this->mpRefScreenCamera = NULL;
	this->mpRefViewer = NULL;

	mScutcheonScreenCoordinatesOffset = osg::Vec2s(0.0,0.0);
	mScutcheonSeparativeSign = " : ";
	mScutcheonVerticalSpacing = 10;
	mIntervalBetweenTitleAndContent = 20;
	mTitleImageOffset = 0;
	mIsIconTextVisible = false;

	mScuLineColor = osg::Vec4f(1,0,0,1);
	mScuLineType = SOLID_LT;
	mScuLineWitdh = 5.0;
	mIsScuLineVisible = true;
	this->mpObjectManagerCallBack = NULL;
	this->mIntervalBetweenBottomAndContent = 10;
	this->mContentHorizontalOffset = -65;
	this->mBackImageWidth = 175;
	this->mTitleImageSize = osg::Vec2s(120,40);
}

CObjectManager::~CObjectManager()
{
	if(this->mpBackgroundProcessor)
	{
		//加了这两句之后关闭程序之后，带vs跑程序，关闭程序，vs不能正常退出
		//this->mpBackgroundProcessor->cancel();
		//delete this->mpBackgroundProcessor;
		this->mpBackgroundProcessor = NULL;
	}

	if(NULL != mpDynamicObjectListener)
	{
		delete mpDynamicObjectListener;
		mpDynamicObjectListener = NULL;
	}
}

void CObjectManager::initial(osg::Group* pRefRoot
	, osgEarth::MapNode* pRefMapNode,
	osgViewer::Viewer* pRefViewer,
	osg::Camera* pRefScreenCamera)
{
	this->mpRefRoot = pRefRoot;
	this->mpRefMapNode = pRefMapNode;
	this->mpRefViewer = pRefViewer;
	this->mpRefScreenCamera = pRefScreenCamera;
	if (this->mpRefViewer)
	{
		this->mpObjectManagerCallBack = new CObjectManagerCallBack();
		this->mpObjectManagerCallBack->Owner = this;
		this->mpRefViewer->getCamera()->addCullCallback(this->mpObjectManagerCallBack);

		mpModelPickerEx = new ModelPickerEx;
		if(NULL == mpModelPickerExCallback)
			mpModelPickerExCallback = new ModelPickerExCallback(mpModelPickerEx);
		mpModelPickerEx->mpOwner = this;
#if _USE_NEW_SCUTEON_ == 0
		mpModelPickerEx->setDefaultCallback(mpModelPickerExCallback);
#endif
		this->mpRefViewer->addEventHandler(mpModelPickerEx);
	}
}

void CObjectManager::frame(osg::Node* node, osg::NodeVisitor* nv)
{
	synHandleSignals();//modified by sjl->2018-11-3
	double minVisibleDistance = 0;
	double maxVisibleDistance = 500000;
	osg::Camera* pCamera = node->asCamera();
	g_RefCamera = pCamera;
	//找出当前可见的场景对象
	std::vector<Common::IObject*> needRenderObjects;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (!keys.empty())
	{
		for (int i = 0; i < keys.size(); ++i)
		{
			std::string id = keys.at(i);
			Common::IObject* pObject = this->getObject(id);
			if (pObject)
			{
				bool needToRender = true;

				if (needToRender)
				{
					needRenderObjects.push_back(pObject);
				}
			}
		}
	}
	
	this->findVisibleSceneObject(needRenderObjects, pCamera, nv, true);
	nv->traverse(*node);
}

void CObjectManager::clearSceneTrackLines()
{
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (!keys.empty())
	{
		for (int i = 0; i < keys.size(); ++i)
		{
			std::string id = keys.at(i);
			//CDynamicObject * pRadarObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
			//if(NULL != pRadarObject)
			//{
			//	//清除雷达对象航迹
			//	pRadarObject->clearSceneTrackLines();
			//}
			//else
			{
				//清除无线电对象方向线
				Common::IObject* pObject = this->removeObject(id);
				if(pObject)
				{
					delete pObject;
					pObject = NULL;
				}
			}
		}
	}
}

void CObjectManager::findVisibleSceneObject(std::vector<Common::IObject*>& sceneObjects,
	osg::Camera* pCamera, osg::NodeVisitor* nv, const bool& needToRender)
{
	double time = nv->getFrameStamp()->getSimulationTime();
	if (needToRender)
	{
		//处于当前未加载的目标
		std::vector<Common::IObject*>::iterator bItr = sceneObjects.begin();
		while (bItr != sceneObjects.end())
		{
			CDynamicObject* pDyObject = dynamic_cast<CDynamicObject*>(*bItr);
			if(pDyObject)
			{
				if (!pDyObject->hasLoadResource())
				{
					//添加至后台线程进行资源加载
					mpBackgroundProcessor->addObject(pDyObject);
				}
				else
				{
					if(pDyObject->getNeedUpatePathPointSize() > 0)
					{
						pDyObject->setNearestUpdateTime(time);
					}
					pDyObject->listenObjectCull(nv);
#if 1
					if (pDyObject->getNeedRecoverTrackLineFadState())
					{
						if (this->mpDynamicObjectListener)
						{
							//航迹变灰后恢复的回调
							this->mpDynamicObjectListener->onTrackLineRecoverEvent(pDyObject);
						}
						pDyObject->setIsTrackLineFadState(false);
					}

#endif
					pDyObject->synUpdate(pCamera);

					//处理对象进出区域
					dealDynamicObjectInOutRegion(pDyObject);
					//trackLineFading
					if(time - pDyObject->getNearestUpdateTime() > pDyObject->getTrackLineFadingTimeInterval()
						&& !pDyObject->getIsTrackLineFadState())
					{
						//pDyObject->changeTrackLineColor(osg::Vec4f(96/255.0,96/255.0 ,96/255.0,1.0));
						pDyObject->setTrackLineFadColor(osg::Vec4f(96/255.0,96/255.0 ,96/255.0,1.0));
						if (this->mpDynamicObjectListener)
						{
							//航迹变灰的回调
							this->mpDynamicObjectListener->onTrackLineFadEvent(pDyObject);
						}
					}

#if 0
					//iconUnvisible
					if(time - pDyObject->getNearestUpdateTime() > pDyObject->getIconUnVisibleTimeInterval())
						pDyObject->setIconVisible(false);
					//ScutcheonUnvisible
					if(time - pDyObject->getNearestUpdateTime() > pDyObject->getScutcheonUnVisibleTimeInterval())
						pDyObject->setScutcheonVisible(false);
#else
					//iconUnvisible
					if(time - pDyObject->getNearestUpdateTime() > pDyObject->getIconUnVisibleTimeInterval())
						pDyObject->notifyHideIcon();
					//ScutcheonUnvisible
					if(time - pDyObject->getNearestUpdateTime() > pDyObject->getScutcheonUnVisibleTimeInterval())
						pDyObject->notifyHideScutcheon();
#endif
					//destrop
					if(time - pDyObject->getNearestUpdateTime() > pDyObject->getDestroyTimeInterval())
					{
						dealDynamicObjectDestory(pDyObject);
						Common::IObject* pObject = this->removeObject(pDyObject->getID());
						if(pObject)
						{
							if (this->mpModelPickerEx)
							{
								this->mpModelPickerEx->notifyObjectDestroy(pObject);
							}
							if (this->mpDynamicObjectListener)
							{
								//对象销毁的回调
								this->mpDynamicObjectListener->onDynamicObjectDestoryEvent(pDyObject);
							}
							delete pObject;
							pObject = NULL;
						}
					}//destrop
				}
			}
			else
			{
				CWirelessObject* pWirelessObject = dynamic_cast<CWirelessObject*>(*bItr);
				if(USEWIRELESSOBJECT && pWirelessObject)
				{
					if (!pWirelessObject->hasLoadResource())
					{
						//添加至后台线程进行资源加载
						pWirelessObject->setCreateTime(time);
						mpBackgroundProcessor->addObject(pWirelessObject);
					}
					else
					{
						//处理动态对象方向线
						pWirelessObject->synUpdate();
						if(time - pWirelessObject->getCreateTime() > pWirelessObject->getDestroyTimeInterval())
						{
							Common::IObject* pObject = this->removeObject(pWirelessObject->getID());
							if(pObject)
							{
								delete pObject;
								pObject = NULL;
							}
						}
					}
				}
			}

			++bItr;
		}

		//更新目标的位置


		//挂接目标到场景
	}
}

DynamicObject::CDynamicObject* CObjectManager::getDynamicObject(const int& x, const int& y ,osg::Camera* pCamera)
{
	DynamicObject::CDynamicObject* pResultObject = NULL;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (!keys.empty())
	{
		osg::Vec3d eye, up, center;
		pCamera->getViewMatrixAsLookAt(eye, center, up);
		std::map<double,DynamicObject::CDynamicObject*> sortedObjects;
		for (int i = 0; i < keys.size(); ++i)
		{
			std::string id = keys.at(i);
			Common::IObject* pObject = this->getObject(id);
			if(pObject)
			{
				DynamicObject::CDynamicObject* pCurObject =
					dynamic_cast<DynamicObject::CDynamicObject*>(pObject);
				if (pCurObject)
				{
					osgEarth::GeoPoint p(this->mpRefMapNode->getMapSRS(),
						pCurObject->getModelCurrentPoint());

					osg::Vec3d worldPos;
					p.toWorld(worldPos);
					double squareDis = (worldPos - eye).length2();
					sortedObjects[squareDis] = pCurObject;
				}
				
			}
		}
#if 0
		for (int i = 0; i < keys.size(); ++i)
		{
			std::string id = keys.at(i);
			Common::IObject* pObject = this->getObject(id);
			if(pObject)
			{
				DynamicObject::CDynamicObject* pCurObject =
					dynamic_cast<DynamicObject::CDynamicObject*>(pObject);
				if (pCurObject && pCurObject->contain(x,y,pCamera) != 0 )
				{
					pResultObject = pCurObject;
					break;
				}
			}
		}
#else
		std::map<double,DynamicObject::CDynamicObject*>::iterator bItr = sortedObjects.begin();
		std::map<double,DynamicObject::CDynamicObject*>::iterator eItr = sortedObjects.end();
		while (bItr != eItr)
		{
			DynamicObject::CDynamicObject* pCurObject = bItr->second;
			if (pCurObject && pCurObject->contain(x,y,pCamera) != 0 )
			{
				pResultObject = pCurObject;
				break;
			}
			++bItr;
		}
#endif
	}
	return pResultObject;
}

void CObjectManager::closeDataProcessThread()
{
	if (this->mpBackgroundProcessor)
	{
		this->mpBackgroundProcessor->setState(false);
		bool hasRequest = true;
		while (hasRequest)
		{
			std::vector<std::string> keys;
			this->mpBackgroundProcessor->getKeys(keys);
			if (keys.size() == 0)
			{
				hasRequest = false;
			}
			OpenThreads::Thread::microSleep(1);
		}

		this->mpBackgroundProcessor->cancel();
	}
}

void CObjectManager::synHandleSignals()
{
	DynamicObject::CDataSingal* pDataSignal = NULL;
	this->mSignalMutex.lock();
	{
		if (!this->mReceiveSignals.empty())
		{
			std::vector<DynamicObject::CDataSingal*>::iterator itr = this->mReceiveSignals.begin();
			pDataSignal = *itr;
			this->mReceiveSignals.erase(itr);


			/*if (this->mReceiveSignals.size() > 5000)
			{
				std::vector<DynamicObject::CDataSingal*> usefulSignals;
				for (int i = 0; i < this->mReceiveSignals.size(); ++i)
				{
					if (i < 2500)
					{
						DynamicObject::CDataSingal* pSignal = this->mReceiveSignals.at(i);
						delete pSignal;
					}
					else
					{
						usefulSignals.push_back(this->mReceiveSignals.at(i));
					}
				}
				this->mReceiveSignals.clear();
				this->mReceiveSignals= usefulSignals;
			}*/
		}
	}
	this->mSignalMutex.unlock();
	if (pDataSignal)
	{
		std::string id = pDataSignal->id;
		std::string name = pDataSignal->name;
		Common::IObject* pObject = this->getObject(id);
		CDynamicObject*pDynamicObject = NULL;
		CWirelessObject* pWirelessObject = NULL;
		bool isNewObject = false;
		if (pObject == NULL)
		{
			isNewObject = true;
			if(USEWIRELESSOBJECT && pDataSignal->group == DynamicObject::CDynamicObjectGroupType::WIRELESSTARGET)
			{
				pWirelessObject = new CWirelessObject(this->mpRefMapNode);
				//pWirelessObject->setID(id);
				pWirelessObject->setName(name);
				pWirelessObject->setLineType(mDirectionallineType);
				pWirelessObject->setLineWidth(mDirectionallineWidth);
				pWirelessObject->setColor(mDirectionallineColor);
				pWirelessObject->setAssociatedFacilityID(pDataSignal->deviceID);
				pWirelessObject->setGroup(pDataSignal->group);
				pWirelessObject->setVisible(mIsDirectionallineVisible);
				std::map<std::string,FacilityDetectionTargetsState>::iterator iter = mFacilityDetectionTargetsState.find(pDataSignal->deviceID);
				if(iter != mFacilityDetectionTargetsState.end())
					pWirelessObject->setVisible(iter->second.mIsDirectionallineVisible);
				pWirelessObject->setDestroyTimeInterval(mDirectionallineDestroyTimeInterval);
				pObject = pDynamicObject;
				this->addObject(pWirelessObject);
			}
			else if(pDataSignal->group == DynamicObject::CDynamicObjectGroupType::RADARTARGET)
			{
				//新目标接收信号
				pDynamicObject = new CDynamicObject(this->mpRefMapNode);
				pDynamicObject->setName(name);
				pDynamicObject->setScreenCamera(this->mpRefScreenCamera);
				pDynamicObject->initial(mpRefViewer);
				pDynamicObject->setType(pDataSignal->type);
				pDynamicObject->setID(id);
				pDynamicObject->setModelPath("earthData/model/fly.ive");
				pDynamicObject->setObjectIcon(pDataSignal->iconImageName);
				pDynamicObject->setIconScale(pDataSignal->iconscale);
				pDynamicObject->setAssociatedFacilityID(pDataSignal->deviceID);
				pDynamicObject->setTrackLineVisible(mIsTrackLineVisible);
				pDynamicObject->setIconVisible(mIsIconVisible);
				pDynamicObject->setScutcheonVisible(mIsScutcheonVisible);
				pDynamicObject->setTrackLinePointPattern(mIsTrackPointPattern);
				pDynamicObject->setIntervalBetweenTitleAndContent(mIntervalBetweenTitleAndContent);
#if 1
				pDynamicObject->setIntervalBetweenBottomAndContent(this->getIntervalBetweenBottomAndContent());
				pDynamicObject->setBackImageWidth(this->getBackImageWidth());
				pDynamicObject->setContentHorizontalOffset(this->getContentHorizontalOffset());
				pDynamicObject->setTitleImageSize(this->getTitleImageSize());
				pDynamicObject->setTitleImageOffset(this->getTitleImageOffset());
#endif
				std::map<std::string,FacilityDetectionTargetsState>::iterator iter = mFacilityDetectionTargetsState.find(pDataSignal->deviceID);
				if(iter != mFacilityDetectionTargetsState.end())
				{
					pDynamicObject->setTrackLineVisible(iter->second.mIsTrackLineVisible);
					pDynamicObject->setIconVisible(iter->second.mIsIconVisible);
					pDynamicObject->setScutcheonVisible(iter->second.mIsScutcheonVisible);
				}
				pDynamicObject->setTrackLineColor(mTrackLineColor);
				pDynamicObject->setTrackLineType(mTrackLineType);
				pDynamicObject->setTrackLineWidth(mTrackLineWidth);
				pDynamicObject->setTrackLinePointSize(mTrackLinePointSize);
				pDynamicObject->setTrackLineFadingTimeInterval(mTrackLineFadingTimeInterval);
				pDynamicObject->setIconUnVisibleTimeInterval(mIconUnVisibleTimeInterval);
				pDynamicObject->setDestroyTimeInterval(mTrackLineDestroyTimeInterval);
				pDynamicObject->setScutcheonUnVisibleTimeInterval(mScutcheonUnVisibleTimeInterval);
				pDynamicObject->setScuLineColor(mScuLineColor);
				pDynamicObject->setScuLineType(mScuLineType);
				pDynamicObject->setScuLineVisible(mIsScuLineVisible);
				pDynamicObject->setScuLineWidth(mScuLineWitdh);

				osgEarth::Symbology::Style& style = pDynamicObject->getIconStyle();
				style.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()= mIconFont;
				style.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = mIconTextSize;
				style.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = mIconTextColor;
				pDynamicObject->setIconTextVisible(mIsIconTextVisible);
				pDynamicObject->updateIconStyle();

				osgEarth::Symbology::TextSymbol* titleSymbol = pDynamicObject->getScutcheonTitleSymbol();
				titleSymbol->font() = mScutcheonTitleFont;
				titleSymbol->size() = mScutcheonTitleTextSize;
				titleSymbol->fill()->color() = mScutcheonTitleTextColor;
				osgEarth::Symbology::TextSymbol* contentSymbol = pDynamicObject->getScutcheonContentSymbol();
				contentSymbol->font() = mScutcheonContentFont;
				contentSymbol->size() = mScutcheonContentSize;
				contentSymbol->fill()->color() = mScutcheonContentColor;
				pDynamicObject->getScutcheonTitleImageStyle().getOrCreate<
					osgEarth::Annotation::IconSymbol>()->url()->setLiteral(mScutcheonTitleBackgroundImage);
				pDynamicObject->getScutcheonStyle().getOrCreate<
					osgEarth::Annotation::IconSymbol>()->url()->setLiteral(mScutcheonContentBackgroundImage);
				if(mIsScutcheonTitleUseShadow)
					pDynamicObject->getScutcheonTitleSymbol()->haloBackdropType() = osgText::Text::BackdropType::OUTLINE;
				else
					pDynamicObject->getScutcheonTitleSymbol()->haloBackdropType() = osgText::Text::BackdropType::NONE;

				if(mIsScutcheonContentUseShadow)
					pDynamicObject->getScutcheonContentSymbol()->haloBackdropType() = osgText::Text::BackdropType::OUTLINE;
				else
					pDynamicObject->getScutcheonContentSymbol()->haloBackdropType() = osgText::Text::BackdropType::NONE;
				pDynamicObject->setScutcheonScreenCoordinatesOffset(mScutcheonScreenCoordinatesOffset);
				pDynamicObject->setScutcheonSeparativeSign(mScutcheonSeparativeSign);
				pDynamicObject->setScutcheonVerticalSpacing(mScutcheonVerticalSpacing);
				pDynamicObject->updateScutcheonStyle();

				pDynamicObject->setGroup(pDataSignal->group);
				{
					Geo::Fields fields;
					for (int j = 0; j < pDataSignal->fields.size(); ++j)
					{
						std::string field = pDataSignal->fields.at(j).name;
						pDataSignal->fields.at(j).valid = false;
						for (int index = 0; index < mFields.size(); ++index)
						{
							if(mFields.at(index) != field)
								continue;
							pDataSignal->fields.at(j).valid = true;
						}
						fields.push_back(pDataSignal->fields.at(j));
					}
					pDynamicObject->setScutcheonVisibleFields(fields);
				}
				pObject = pDynamicObject;
				this->addObject(pDynamicObject);
				/*if (mpDynamicObjectListener)
				mpDynamicObjectListener->onCreateDynamicObject(pDynamicObject);*/
			}
		}
		else
		{
			//已有目标接收信号
			pDynamicObject = dynamic_cast<CDynamicObject*>(pObject);
			pWirelessObject = dynamic_cast<CWirelessObject*>(pObject);
		}

		if (pDynamicObject != NULL)
		{
			dealNewAddScutcheonFields2(pDynamicObject,pDataSignal->fields);
			pDynamicObject->reveiveSignal(pDataSignal);				
		}

		if(USEWIRELESSOBJECT && pWirelessObject != NULL)
		{
			pWirelessObject->reveiveSignal(pDataSignal);
		}

		if (mpDynamicObjectListener)
		{
			if (isNewObject)
			{

				mpDynamicObjectListener->onCreateDynamicObject(pObject);
			}
			else
			{
				mpDynamicObjectListener->onDynamicObjectReceiveSignal(pObject);
			}
		}
		delete pDataSignal;
	}
}

bool CObjectManager::aSynReveiveSignal(DynamicObject::CDataSingal* pRefSignal)
{
	bool r = false;
	if (pRefSignal)
	{
		if (std::abs(pRefSignal->longitude) > 180
			|| std::abs(pRefSignal->latitude) > 90)
			return r;
		bool isNewObject = false;
#if _USE_SYN_SIGNAL_
		this->mSignalMutex.lock();
		{
			DynamicObject::CDataSingal* pCloneSignal = pRefSignal->clone();
			this->mReceiveSignals.push_back(pCloneSignal);
		}
		this->mSignalMutex.unlock();
#else
		DynamicObject::CDataSingal* pDataSignal = pRefSignal;
		if (pDataSignal)
		{
			std::string id = pDataSignal->id;
			std::string name = pDataSignal->name;
			Common::IObject* pObject = this->getObject(id);
			CDynamicObject*pDynamicObject = NULL;
			CWirelessObject* pWirelessObject = NULL;
			if (pObject == NULL)
			{
				isNewObject = true;
				if(USEWIRELESSOBJECT && pDataSignal->group == DynamicObject::CDynamicObjectGroupType::WIRELESSTARGET)
				{
					pWirelessObject = new CWirelessObject(this->mpRefMapNode);
					//pWirelessObject->setID(id);
					pWirelessObject->setName(name);
					pWirelessObject->setLineType(mDirectionallineType);
					pWirelessObject->setLineWidth(mDirectionallineWidth);
					pWirelessObject->setColor(mDirectionallineColor);
					pWirelessObject->setAssociatedFacilityID(pDataSignal->deviceID);
					pWirelessObject->setGroup(pDataSignal->group);
					pWirelessObject->setVisible(mIsDirectionallineVisible);
					std::map<std::string,FacilityDetectionTargetsState>::iterator iter = mFacilityDetectionTargetsState.find(pDataSignal->deviceID);
					if(iter != mFacilityDetectionTargetsState.end())
						pWirelessObject->setVisible(iter->second.mIsDirectionallineVisible);
					pWirelessObject->setDestroyTimeInterval(mDirectionallineDestroyTimeInterval);
					pObject = pDynamicObject;
					this->addObject(pWirelessObject);
				}
				else if(pDataSignal->group == DynamicObject::CDynamicObjectGroupType::RADARTARGET)
				{
					//新目标接收信号
					pDynamicObject = new CDynamicObject(this->mpRefMapNode);
					pDynamicObject->setName(name);
					pDynamicObject->setScreenCamera(this->mpRefScreenCamera);
					pDynamicObject->initial(mpRefViewer);
					pDynamicObject->setType(pDataSignal->type);
					pDynamicObject->setID(id);
					pDynamicObject->setModelPath("earthData/model/fly.ive");
					pDynamicObject->setObjectIcon(pDataSignal->iconImageName);
					pDynamicObject->setIconScale(pDataSignal->iconscale);
					pDynamicObject->setAssociatedFacilityID(pDataSignal->deviceID);
					pDynamicObject->setTrackLineVisible(mIsTrackLineVisible);
					pDynamicObject->setIconVisible(mIsIconVisible);
					pDynamicObject->setScutcheonVisible(mIsScutcheonVisible);
					pDynamicObject->setTrackLinePointPattern(mIsTrackPointPattern);
					pDynamicObject->setIntervalBetweenTitleAndContent(mIntervalBetweenTitleAndContent);
#if 1
					pDynamicObject->setIntervalBetweenBottomAndContent(this->getIntervalBetweenBottomAndContent());
					pDynamicObject->setBackImageWidth(this->getBackImageWidth());
					pDynamicObject->setContentHorizontalOffset(this->getContentHorizontalOffset());
					pDynamicObject->setTitleImageSize(this->getTitleImageSize());
					pDynamicObject->setTitleImageOffset(this->getTitleImageOffset());
#endif
					std::map<std::string,FacilityDetectionTargetsState>::iterator iter = mFacilityDetectionTargetsState.find(pDataSignal->deviceID);
					if(iter != mFacilityDetectionTargetsState.end())
					{
						pDynamicObject->setTrackLineVisible(iter->second.mIsTrackLineVisible);
						pDynamicObject->setIconVisible(iter->second.mIsIconVisible);
						pDynamicObject->setScutcheonVisible(iter->second.mIsScutcheonVisible);
					}
					pDynamicObject->setTrackLineColor(mTrackLineColor);
					pDynamicObject->setTrackLineType(mTrackLineType);
					pDynamicObject->setTrackLineWidth(mTrackLineWidth);
					pDynamicObject->setTrackLinePointSize(mTrackLinePointSize);
					pDynamicObject->setTrackLineFadingTimeInterval(mTrackLineFadingTimeInterval);
					pDynamicObject->setIconUnVisibleTimeInterval(mIconUnVisibleTimeInterval);
					pDynamicObject->setDestroyTimeInterval(mTrackLineDestroyTimeInterval);
					pDynamicObject->setScutcheonUnVisibleTimeInterval(mScutcheonUnVisibleTimeInterval);
					pDynamicObject->setScuLineColor(mScuLineColor);
					pDynamicObject->setScuLineType(mScuLineType);
					pDynamicObject->setScuLineVisible(mIsScuLineVisible);
					pDynamicObject->setScuLineWidth(mScuLineWitdh);

					osgEarth::Symbology::Style& style = pDynamicObject->getIconStyle();
					style.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()= mIconFont;
					style.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = mIconTextSize;
					style.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = mIconTextColor;
					pDynamicObject->setIconTextVisible(mIsIconTextVisible);
					pDynamicObject->updateIconStyle();

					osgEarth::Symbology::TextSymbol* titleSymbol = pDynamicObject->getScutcheonTitleSymbol();
					titleSymbol->font() = mScutcheonTitleFont;
					titleSymbol->size() = mScutcheonTitleTextSize;
					titleSymbol->fill()->color() = mScutcheonTitleTextColor;
					osgEarth::Symbology::TextSymbol* contentSymbol = pDynamicObject->getScutcheonContentSymbol();
					contentSymbol->font() = mScutcheonContentFont;
					contentSymbol->size() = mScutcheonContentSize;
					contentSymbol->fill()->color() = mScutcheonContentColor;
					pDynamicObject->getScutcheonTitleImageStyle().getOrCreate<
						osgEarth::Annotation::IconSymbol>()->url()->setLiteral(mScutcheonTitleBackgroundImage);
					pDynamicObject->getScutcheonStyle().getOrCreate<
						osgEarth::Annotation::IconSymbol>()->url()->setLiteral(mScutcheonContentBackgroundImage);
					if(mIsScutcheonTitleUseShadow)
						pDynamicObject->getScutcheonTitleSymbol()->haloBackdropType() = osgText::Text::BackdropType::OUTLINE;
					else
						pDynamicObject->getScutcheonTitleSymbol()->haloBackdropType() = osgText::Text::BackdropType::NONE;

					if(mIsScutcheonContentUseShadow)
						pDynamicObject->getScutcheonContentSymbol()->haloBackdropType() = osgText::Text::BackdropType::OUTLINE;
					else
						pDynamicObject->getScutcheonContentSymbol()->haloBackdropType() = osgText::Text::BackdropType::NONE;
					pDynamicObject->setScutcheonScreenCoordinatesOffset(mScutcheonScreenCoordinatesOffset);
					pDynamicObject->setScutcheonSeparativeSign(mScutcheonSeparativeSign);
					pDynamicObject->setScutcheonVerticalSpacing(mScutcheonVerticalSpacing);
					pDynamicObject->updateScutcheonStyle();

					pDynamicObject->setGroup(pDataSignal->group);
					{
						Geo::Fields fields;
						for (int j = 0; j < pDataSignal->fields.size(); ++j)
						{
							std::string field = pDataSignal->fields.at(j).name;
							pDataSignal->fields.at(j).valid = false;
							for (int index = 0; index < mFields.size(); ++index)
							{
								if(mFields.at(index) != field)
									continue;
								pDataSignal->fields.at(j).valid = true;
							}
							fields.push_back(pDataSignal->fields.at(j));
						}
						pDynamicObject->setScutcheonVisibleFields(fields);
					}
					pObject = pDynamicObject;
					this->addObject(pDynamicObject);
					/*if (mpDynamicObjectListener)
					mpDynamicObjectListener->onCreateDynamicObject(pDynamicObject);*/
				}
			}
			else
			{
				//已有目标接收信号
				pDynamicObject = dynamic_cast<CDynamicObject*>(pObject);
				pWirelessObject = dynamic_cast<CWirelessObject*>(pObject);
			}

			if (pDynamicObject != NULL)
			{
				dealNewAddScutcheonFields2(pDynamicObject,pRefSignal->fields);
				pDynamicObject->reveiveSignal(pRefSignal);				
			}

			if(USEWIRELESSOBJECT && pWirelessObject != NULL)
			{
				pWirelessObject->reveiveSignal(pRefSignal);
			}

			if (mpDynamicObjectListener)
			{
				if (isNewObject)
				{

					mpDynamicObjectListener->onCreateDynamicObject(pObject);
				}
				else
				{
					mpDynamicObjectListener->onDynamicObjectReceiveSignal(pObject);
				}
			}
		}
#endif
	}
	return r;
}

void CObjectManager::setWarnningObjectLayer( DynamicObject::CSceneObjectLayer* pSceneObjectLayer )
{
	if(NULL == pSceneObjectLayer)
		return;
	mSceneObjectLayers.push_back(pSceneObjectLayer);
}

void CObjectManager::dealDynamicObjectDirectionalLine( DynamicObject::CDynamicObject * pObject )
{
	if(NULL == pObject)
		return;
	std::vector<std::string> keys;
	for (int layerIndex = 0; layerIndex < mSceneObjectLayers.size(); ++layerIndex)
	{
		DynamicObject::CSceneObjectLayer* pSceneObjectLayer = mSceneObjectLayers.at(layerIndex);
		if(NULL == pSceneObjectLayer)
			continue;
		keys.clear();
		pSceneObjectLayer->getKeys(keys);
		if (keys.empty())
			continue;
		osg::Vec3d currentPos = pObject->getModelCurrentPoint();
		for (int i = 0; i < keys.size(); ++i)
		{
			std::string id = keys.at(i);
			Graphic::CGeoCircle* pGeoCircle =dynamic_cast<Graphic::CGeoCircle*>(pSceneObjectLayer->getObject(id));
			if(pGeoCircle)
			{
				double radius = pGeoCircle->getRadius();
				osg::Vec3d circlePos = pGeoCircle->getGeoPosition();
				double distance = osgEarth::GeoMath::distance(circlePos, currentPos, mpRefMapNode->getMap()->getSRS());
				bool result = false;
				if(distance < radius)
				{
					pObject->addDirectionalPoint(id,circlePos);
					result = pObject->exsitDirectionalPoint(id);
				}
				else
				{
					pObject->removeDirectionalPoint(id);
				}
			}

			Graphic::CGeoPolygon* pGeoPolygon = dynamic_cast<Graphic::CGeoPolygon*>(pSceneObjectLayer->getObject(id));
			if(pGeoPolygon)
			{
				bool isIn = CCalculatorHelper::isPointInGeometry(currentPos,pGeoPolygon->getGeometry(),mpRefMapNode->getMap()->getSRS());
				if(isIn)
				{
					osg::Vec3d pos = pGeoPolygon->getGeometry().at(0);
					pObject->addDirectionalPoint(id,pos);
				}
				else
				{
					pObject->removeDirectionalPoint(id);
				}
			}
		}
	}
}

void CObjectManager::setTrackLineVisible( const bool& isVisible )
{
	mIsTrackLineVisible = isVisible;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setTrackLineVisible(isVisible);
	}
}

void CObjectManager::setIconVisible( const bool& isVisible )
{
	mIsIconVisible = isVisible;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setIconVisible(isVisible);
	}
}

void CObjectManager::setScutcheonVisible( const bool& isVisible )
{
	mIsScutcheonVisible = isVisible;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setScutcheonVisible(isVisible);
	}
}

void CObjectManager::setDirectionallineVisible( const bool& isVisible )
{
	mIsDirectionallineVisible = isVisible;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CWirelessObject * pObject = dynamic_cast<CWirelessObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setVisible(isVisible);
	}
}

void CObjectManager::setWholeProprietaryVisible( const bool& isVisible )
{
	mIsTrackLineVisible = isVisible;
	mIsIconVisible = isVisible;
	mIsScutcheonVisible = isVisible;
	mIsDirectionallineVisible = isVisible;
	mIsWholeProprietaryVisibles = isVisible;
	setTrackLineVisible(isVisible);
	setIconVisible(isVisible);
	setScutcheonVisible(isVisible);
	setDirectionallineVisible(isVisible);
}

bool CObjectManager::getTrackLineVisible()
{
	return mIsTrackLineVisible;
}

bool CObjectManager::getIconVisible()
{
	return mIsIconVisible;
}

bool CObjectManager::getScutcheonVisible()
{
	return mIsScutcheonVisible;
}

bool CObjectManager::getDirectionallineVisible()
{
	return mIsDirectionallineVisible;
}

void CObjectManager::setTrackLineColor( const osg::Vec4f& color )
{
	mTrackLineColor = color;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setTrackLineColor(color);
	}
}

osg::Vec4f CObjectManager::getTrackLineColor()
{
	return mTrackLineColor;
}

void CObjectManager::setTrackLineType( DynamicObject::LineType lineType )
{
	mTrackLineType = lineType;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setTrackLineType(lineType);
	}
}

DynamicObject::LineType CObjectManager::getTrackLineType()
{
	return mTrackLineType;
}

void CObjectManager::setTrackLineWidth( const float& lineWidth )
{
	mTrackLineWidth = lineWidth;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setTrackLineWidth(lineWidth);
	}
}

float CObjectManager::getTrackLineWidth()
{
	return mTrackLineWidth;
}

void CObjectManager::setDirectionallineColor( const osg::Vec4f& color )
{
	mDirectionallineColor = color;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CWirelessObject * pObject = dynamic_cast<CWirelessObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setColor(color);
	}
}

osg::Vec4f CObjectManager::getDirectionallineColor()
{
	return mDirectionallineColor;
}

void CObjectManager::setDirectionallineType( DynamicObject::LineType lineType )
{
	mDirectionallineType = lineType;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CWirelessObject * pObject = dynamic_cast<CWirelessObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setLineType(lineType);
	}
}

DynamicObject::LineType CObjectManager::getDirectionallineType()
{
	return mDirectionallineType;
}

void CObjectManager::setDirectionallineWidth( const float& lineWidth )
{
	mDirectionallineWidth = lineWidth;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CWirelessObject * pObject = dynamic_cast<CWirelessObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setLineWidth(lineWidth);
	}
}

float CObjectManager::getDirectionallineWidth()
{
	return mDirectionallineWidth;
}

void CObjectManager::setTrackLinePointSize( const int& trackLinePointSize )
{
	mTrackLinePointSize = trackLinePointSize;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setTrackLinePointSize(trackLinePointSize);
	}
}

int CObjectManager::getTrackLinePointSize()
{
	return mTrackLinePointSize;
}

void CObjectManager::setNoFlyingObjectLayer( DynamicObject::CSceneObjectLayer* pSceneObjectLayer )
{
	if(NULL == pSceneObjectLayer)
		return;
	mSceneObjectLayers.push_back(pSceneObjectLayer);
}

void CObjectManager::dealDynamicObjectInOutRegion( DynamicObject::CDynamicObject * pObject )
{
	if(NULL == pObject || NULL == mpDynamicObjectListener)
		return;
	std::vector<std::string> keys;
	for (int layerIndex = 0; layerIndex < mSceneObjectLayers.size(); ++layerIndex)
	{
		DynamicObject::CSceneObjectLayer* pSceneObjectLayer = mSceneObjectLayers.at(layerIndex);
		if(NULL == pSceneObjectLayer)
			continue;
		std::string sceneLayerType = pSceneObjectLayer->getType();
		keys.clear();
		pSceneObjectLayer->getKeys(keys);
		if (keys.empty())
			continue;
		osg::Vec3d currentPos = pObject->getModelCurrentPoint();
		for (int i = 0; i < keys.size(); ++i)
		{
			bool isInRegion = false;
			std::string id = keys.at(i);
			std::string layerObjectName = "";
			Graphic::CGeoCircle* pGeoCircle = dynamic_cast<Graphic::CGeoCircle*>(pSceneObjectLayer->getObject(id));
			if(pGeoCircle)
			{
				layerObjectName = pGeoCircle->getName();
				double radius = pGeoCircle->getRadius();
				osg::Vec3d circlePos = pGeoCircle->getGeoPosition();
				isInRegion = CCalculatorHelper::isPointInCircle(currentPos,circlePos,radius,mpRefMapNode->getMap()->getSRS());
			}

			Graphic::CGeoPolygon* pGeoPolygon = dynamic_cast<Graphic::CGeoPolygon*>(pSceneObjectLayer->getObject(id));
			if(NULL == pGeoCircle && pGeoPolygon)
			{
				layerObjectName = pGeoPolygon->getName();
				bool isIn = CCalculatorHelper::isPointInGeometry(currentPos,pGeoPolygon->getGeometry(),mpRefMapNode->getMap()->getSRS());
				if(isIn)
					isInRegion = true;
				else
					isInRegion = false;
			}

			Geo::CGeoCircleEx* pGeoCircleEx = dynamic_cast<Geo::CGeoCircleEx*>(pSceneObjectLayer->getObject(id));
			if(pGeoCircleEx)
			{
				layerObjectName = pGeoCircleEx->getName();
				double radius = pGeoCircleEx->getRadius();
				osg::Vec3d circlePos = pGeoCircleEx->getGeoPosition();
				isInRegion = CCalculatorHelper::isPointInCircle(currentPos,circlePos,radius,mpRefMapNode->getMap()->getSRS());
			}

			bool result = pObject->exsitRegion(id);
			if(isInRegion)
			{
				if(false == result)
				{
					pObject->addRegion(id,sceneLayerType);
					if(sceneLayerType == "CSceneObjectLayerType::WARNREGION")
						pObject->setIsInWarnningRegion(true);
					else if(sceneLayerType == "CSceneObjectLayerType::NOFLYREGION")
						pObject->setIsInNoFlyingRegion(true);
					else if(sceneLayerType == "CSceneObjectLayerType::SCANREGION")
						pObject->setIsInScanningRegion(true);

					mpDynamicObjectListener->objectInRegin(pObject->getName(),pObject->getID(),
						sceneLayerType,pSceneObjectLayer->getName(),layerObjectName);
				}
			}
			else
			{
				if(true == result)
				{
					pObject->removeRegion(id);
					if(!pObject->exsitRegionType("CSceneObjectLayerType::WARNREGION"))
						pObject->setIsInWarnningRegion(false);
					else if(!pObject->exsitRegionType("CSceneObjectLayerType::NOFLYREGION"))
						pObject->setIsInNoFlyingRegion(false);
					else if(sceneLayerType == "CSceneObjectLayerType::SCANREGION")
						pObject->setIsInScanningRegion(false);

					mpDynamicObjectListener->objectOutRegin(pObject->getName(),pObject->getID(),
						sceneLayerType,pSceneObjectLayer->getName(),layerObjectName);
				}
			}//isInRegion == false
		}
	}
}

void CObjectManager::addDynamicObjectListener( CDynamicObjectListener* pDynamicObjectListener )
{
	mpDynamicObjectListener = pDynamicObjectListener;
}

CDynamicObjectListener* CObjectManager::getDynamicObjectListener()
{
	return mpDynamicObjectListener;
}

void CObjectManager::setDirectionallineDestroyTimeInterval( const double& timeInterval )
{
	mDirectionallineDestroyTimeInterval = timeInterval;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CWirelessObject * pObject = dynamic_cast<CWirelessObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setDestroyTimeInterval(timeInterval);
	}
}

double CObjectManager::getDirectionallineDestroyTimeInterval()
{
	return mDirectionallineDestroyTimeInterval;
}

void CObjectManager::setTrackLineFadingTimeInterval( const double& timeInterval )
{
	mTrackLineFadingTimeInterval = timeInterval;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setTrackLineFadingTimeInterval(timeInterval);
	}
}

double CObjectManager::getTrackLineFadingTimeInterval()
{
	return mTrackLineFadingTimeInterval;
}

void CObjectManager::setIconUnVisibleTimeInterval( const double& timeInterval )
{
	mIconUnVisibleTimeInterval = timeInterval;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setIconUnVisibleTimeInterval(timeInterval);
	}
}

double CObjectManager::getIconUnVisibleTimeInterval()
{
	return mIconUnVisibleTimeInterval;
}

void CObjectManager::setTrackLineDestroyTimeInterval( const double& timeInterval )
{
	mTrackLineDestroyTimeInterval = timeInterval;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setDestroyTimeInterval(timeInterval);
	}
}

double CObjectManager::getTrackLineDestroyTimeInterval()
{
	return mTrackLineDestroyTimeInterval;
}

Core::IObjectContainer CObjectManager::getFacilityDetectionTargets( const std::string& facilityID )
{
	Core::IObjectContainer detectionTargets;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return detectionTargets;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject* pDynamicObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		CWirelessObject* pWirelessObject = dynamic_cast<CWirelessObject*>(this->getObject(id));
		if(pDynamicObject && facilityID == pDynamicObject->getAssociatedFacilityID())
			detectionTargets.addObject(pDynamicObject);
		else if(pWirelessObject && facilityID == pWirelessObject->getAssociatedFacilityID())
			detectionTargets.addObject(pWirelessObject);
	}
	return detectionTargets;
}

Core::IObjectContainer CObjectManager::getDetectionTargetsByGroup( const std::string& objctGroup )
{
	Core::IObjectContainer detectionTargets;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return detectionTargets;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		Common::IObject* pObject = dynamic_cast<Common::IObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		if(objctGroup == pObject->getGroup())
			detectionTargets.addObject(pObject);
	}
	return detectionTargets;
}

Common::IObject* CObjectManager::getDetectionTarget( const std::string& objctID )
{
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return NULL;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		Common::IObject* pObject = dynamic_cast<Common::IObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		if(pObject->getID() == objctID)
			return pObject;
	}
	return NULL;
}

Core::IObjectContainer CObjectManager::getDetectionTargetsByObjectType( const std::string& objctType )
{
	Core::IObjectContainer detectionTargets;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return detectionTargets;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		Common::IObject* pObject = dynamic_cast<Common::IObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		if(objctType == pObject->getType())
			detectionTargets.addObject(pObject);
	}
	return detectionTargets;
}

void CObjectManager::setScutcheonUnVisibleTimeInterval( const double& timeInterval )
{
	mScutcheonUnVisibleTimeInterval = timeInterval;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setScutcheonUnVisibleTimeInterval(timeInterval);
	}
}

double CObjectManager::getScutcheonUnVisibleTimeInterval()
{
	return mScutcheonUnVisibleTimeInterval;
}

void CObjectManager::setScutcheonVisibleFields( const std::vector<std::string> fields )
{
	mFields = fields;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		Geo::Fields scutcheonfields = pObject->getScutcheonVisibleFields();
		for (int j = 0; j < scutcheonfields.size(); ++j)
		{
			std::string field = scutcheonfields.at(j).name;
			scutcheonfields.at(j).valid = false;
			for (int index = 0; index < mFields.size(); ++index)
			{
				if(mFields.at(index) != field)
					continue;
				scutcheonfields.at(j).valid = true;
			}
		}
		pObject->setScutcheonVisibleFields(scutcheonfields);
	}
}

std::vector<std::string> CObjectManager::getScutcheonVisibleFields()
{
	return mFields;
}

std::vector<DynamicObject::CSceneObjectLayer*> CObjectManager::getSceneObjectLayers()
{
	return mSceneObjectLayers;
}

void CObjectManager::setFacilityDetectionTargets( 
	const std::string& facilityID, bool isTrackLineVisible,
	bool isIconVisible, bool isScutcheonVisible,bool isDirectionallineVisible )
{
	std::map<std::string,FacilityDetectionTargetsState>::iterator iter = mFacilityDetectionTargetsState.find(facilityID);
	if(iter == mFacilityDetectionTargetsState.end())
	{
		FacilityDetectionTargetsState targetsState;
		targetsState.mIsTrackLineVisible = isTrackLineVisible;
		targetsState.mIsIconVisible = isIconVisible;
		targetsState.mIsScutcheonVisible = isScutcheonVisible;
		targetsState.mIsDirectionallineVisible = isDirectionallineVisible;
		mFacilityDetectionTargetsState.insert(std::make_pair(facilityID,targetsState));
	}
	else
	{
		iter->second.mIsTrackLineVisible = isTrackLineVisible;
		iter->second.mIsIconVisible = isIconVisible;
		iter->second.mIsScutcheonVisible = isScutcheonVisible;
		iter->second.mIsDirectionallineVisible = isDirectionallineVisible;
	}

	iter = mFacilityDetectionTargetsState.find(facilityID);
	if(iter == mFacilityDetectionTargetsState.end())
		return;

	Core::IObjectContainer detectionTargets = getFacilityDetectionTargets(facilityID);
	std::vector<std::string> keys;
	detectionTargets.getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject* pDynamicObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		CWirelessObject* pWirelessObject = dynamic_cast<CWirelessObject*>(this->getObject(id));
		if(pDynamicObject && facilityID == pDynamicObject->getAssociatedFacilityID())
		{
			pDynamicObject->setTrackLineVisible(iter->second.mIsTrackLineVisible);
			pDynamicObject->setIconVisible(iter->second.mIsIconVisible);
			pDynamicObject->setScutcheonVisible(iter->second.mIsScutcheonVisible);
		}
		else if(pWirelessObject && facilityID == pWirelessObject->getAssociatedFacilityID())
		{
			pWirelessObject->setVisible(iter->second.mIsDirectionallineVisible);
		}
	}//for
}

void CObjectManager::setScanningObjectLayer( DynamicObject::CSceneObjectLayer* pSceneObjectLayer )
{
	if(NULL == pSceneObjectLayer)
		return;
	mSceneObjectLayers.push_back(pSceneObjectLayer);
}

void CObjectManager::changeIconImage( const std::string& objectID,const std::string& imagePath )
{
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		if(objectID != id)
			continue;
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setObjectIcon(imagePath);
	}
}

std::string CObjectManager::getIconImage( const std::string& objectID )
{
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return "";
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		if(objectID != id)
			continue;
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		return pObject->getObjectIcon();
	}

	return "";
}

//显隐字段，只控制显示或者隐藏，所有的字段第一次都已经全部添加
void CObjectManager::dealNewAddScutcheonFields( DynamicObject::CDynamicObject * pObject,Geo::Fields fields )
{
	if(NULL == pObject || fields.size() == 0)
		return;

#if 1
	Geo::Fields newAddFields;
	for (int index = 0; index < fields.size(); ++index)
	{
		std::string field = fields.at(index).name;
		fields.at(index).valid = false;
		for (int index2 = 0; index2 < mFields.size(); ++index2)
		{
			if(mFields.at(index2) != field)
				continue;
			fields.at(index).valid = true;
			newAddFields.push_back(fields.at(index));
		}
	}

	int fieldCount = pObject->getScutcheonVisibleFields().size();
	if(0 == fieldCount && newAddFields.size() != 0)
	{
		pObject->setScutcheonVisibleFields(newAddFields);
	}

	if(fieldCount > 0)
	{
		bool isResultChangedField = false;
		bool isResultChangedValue = false;
		bool isChangedValue = false;
		Geo::Fields newFields;
		Geo::Fields oldFields = pObject->getScutcheonVisibleFields();
		int oldIndex = 0;
		Geo::Field newField("","");

		bool needRefresh = false;
		for (int index = 0; index < newAddFields.size(); ++index)
		{
			isChangedValue = false;
			std::string field = newAddFields.at(index).name;
			std::string value = newAddFields.at(index).value;
			for (int index2 = 0; index2 < oldFields.size(); ++index2)
			{
				std::string field2 = oldFields.at(index2).name;
				std::string value2 = oldFields.at(index2).value;
				if(field2 != field)
					continue;
				if(field2 == field)
				{
					if (value != value2)
					{
						oldIndex = index2;
						newField = newAddFields.at(index);
						isChangedValue = true;
					}
				}
			}

			if(isChangedValue)
			{
				for (int index = 0; index < oldFields.size(); ++index)
				{
					if(index == oldIndex)
						newFields.push_back(newField);
					else
						newFields.push_back(oldFields.at(index));
				}
				oldFields = newFields;
				newFields.clear();
				isResultChangedValue = true;
			}
		}

		if(newAddFields.size() != oldFields.size())
		{
			Geo::Fields newFields;
			bool isChanged = false;
			bool isExit = false;
			for (int index = 0; index < oldFields.size(); ++index)
			{
				Geo::Field newField("","");
				isExit = false;
				std::string field = oldFields.at(index).name;
				std::string value = oldFields.at(index).value;
				bool valid = oldFields.at(index).valid;
				for (int index2 = 0; index2 < newAddFields.size(); ++index2)
				{
					std::string field2 = newAddFields.at(index2).name;
					std::string value2 = newAddFields.at(index2).value;
					bool valid2 = newAddFields.at(index2).valid;
					if(field2 != field)
						continue;
					isExit = true;
					break;
				}

				newField.name = field;
				newField.value = value;
				newField.valid = true;
				if(false == isExit && valid == true)
				{
					newField.valid = false;
					needRefresh = true;
					isChanged = true;
				}

				if(false == isExit && valid == false)
					newField.valid = false;

				if(true == isExit && valid == false)
				{
					newField.valid = true;
					needRefresh = true;
					isChanged = true;
				}

				newFields.push_back(newField);
			}

			if(isChanged)
			{
				oldFields.clear();
				oldFields = newFields;
			}
		}

#if DEALFIELDSREDUCE
		//if(oldFields.size() != newAddFields.size() && newAddFields.size() != 0)
		//{
		//	pObject->setScutcheonVisibleFields(newAddFields);
		//}
#endif

		if(needRefresh)
		{
			pObject->setScutcheonVisibleFields(oldFields);
		}

		if(isResultChangedValue)
		{
			pObject->updateScutcheonContent(oldFields);
		}
	}
#else
	bool isChangeField = (fields.size() != pObject->getScutcheonVisibleFields().size());
	if (!isChangeField)
	{
		for (int index = 0; index < fields.size(); ++index)
		{
			std::string field = fields.at(index).name;
			std::string value = fields.at(index).value;
		}
	}

	if(isChangeField)
	{
		pObject->setScutcheonVisibleFields(fields);
	}
#endif

}

void CObjectManager::dealNewAddScutcheonFields2( DynamicObject::CDynamicObject * pObject,Geo::Fields newFields )
{
	if(NULL == pObject)
		return;

	Geo::Fields oldfields =  pObject->getCurrentScutcheonVisibleFields();
	int oldFieldCount = oldfields.size();
	int newFieldCount = newFields.size();

	if(oldFieldCount != newFieldCount)
	{
		pObject->setCurrentScutcheonVisibleFields(newFields);
	}

	//return;
	bool isContentChanged = false;
	bool isFieldChanged = false;

	if(oldFieldCount == newFieldCount && newFieldCount > 0)
	{
		for (int newIndex = 0; newIndex < newFieldCount; ++newIndex)
		{
			bool isExit = false;
			Geo::Field newFiled = newFields.at(newIndex);
			for (int oldIndex = 0; oldIndex < oldFieldCount; ++oldIndex)
			{
				Geo::Field oldFiled = oldfields.at(oldIndex);
				if(newFiled.name == oldFiled.name)
				{
					if(newFiled.value != oldFiled.value)
						isContentChanged = true;

					//如果对应位置的字段不一样就重新生成
					if(oldIndex != newIndex)
						isFieldChanged = true;

					isExit = true;
					break;
				}
			}

			if(false == isExit)
				isFieldChanged = true;
		}

		if(isFieldChanged)
			pObject->setCurrentScutcheonVisibleFields(newFields);

		if(false == isFieldChanged && true == isContentChanged)
			pObject->updateCurrentScutcheonContent(newFields);
	}
}

void CObjectManager::setIconFont( const std::string& font )
{
	mIconFont = font;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		osgEarth::Symbology::Style& style = pObject->getIconStyle();
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->font()= font;
		pObject->updateIconStyle();
	}
}

std::string CObjectManager::getIconFont()
{
	return mIconFont;
}

void CObjectManager::setIconTextSize( const float& size )
{
	mIconTextSize = size;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		osgEarth::Symbology::Style& style = pObject->getIconStyle();
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = size;
		pObject->updateIconStyle();
	}
}

float CObjectManager::getIconTextSize()
{
	return mIconTextSize;
}

void CObjectManager::setIconTextColor( const osgEarth::Symbology::Color& color )
{
	mIconTextColor = color;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		osgEarth::Symbology::Style& style = pObject->getIconStyle();
		style.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill() = color;
		pObject->updateIconStyle();
	}
}

osgEarth::Symbology::Color CObjectManager::getIconTextColor()
{
	return mIconTextColor;
}

void CObjectManager::setScutcheonTitleFont( const std::string& font )
{
	mScutcheonTitleFont = font;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		osgEarth::Symbology::TextSymbol* titleSymbol = pObject->getScutcheonTitleSymbol();
		titleSymbol->font() = font;
		pObject->updateScutcheonStyle();
	}
}

std::string CObjectManager::getScutcheonTitleFont()
{
	return mScutcheonTitleFont;
}

void CObjectManager::setScutcheonTitleTextSize( const float& size )
{
	mScutcheonTitleTextSize = size;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		osgEarth::Symbology::TextSymbol* titleSymbol = pObject->getScutcheonTitleSymbol();
		titleSymbol->size() = size;
		pObject->updateScutcheonStyle();
	}
}

float CObjectManager::getScutcheonTitleTextSize()
{
	return mScutcheonTitleTextSize;
}

void CObjectManager::setScutcheonTitleTextColor( const osgEarth::Symbology::Color& color )
{
	mScutcheonTitleTextColor = color;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		osgEarth::Symbology::TextSymbol* titleSymbol = pObject->getScutcheonTitleSymbol();
		titleSymbol->fill()->color() = color;
		pObject->updateScutcheonStyle();
	}
}

osgEarth::Symbology::Color CObjectManager::getScutcheonTitleTextColor()
{
	return mScutcheonTitleTextColor;
}

void CObjectManager::setScutcheonTitleBackgroundImage( const std::string& image )
{
	mScutcheonTitleBackgroundImage = image;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->getScutcheonTitleImageStyle().getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral(image);
		pObject->updateScutcheonStyle();
	}
}

std::string CObjectManager::getScutcheonTitleBackgroundImage()
{
	return mScutcheonTitleBackgroundImage;
}

void CObjectManager::setScutcheonContentFont( const std::string& font )
{
	mScutcheonContentFont = font;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		osgEarth::Symbology::TextSymbol* contentSymbol = pObject->getScutcheonContentSymbol();
		contentSymbol->font() = font;
		pObject->updateScutcheonStyle();
	}
}

std::string CObjectManager::getScutcheonContentFont()
{
	return mScutcheonContentFont;
}

void CObjectManager::setScutcheonContentSize( const float& size )
{
	mScutcheonContentSize = size;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		osgEarth::Symbology::TextSymbol* contentSymbol = pObject->getScutcheonContentSymbol();
		contentSymbol->size() = size;
		pObject->updateScutcheonStyle();
	}
}

float CObjectManager::getScutcheonContentSize()
{
	return mScutcheonContentSize;
}

void CObjectManager::setScutcheonContentColor( const osgEarth::Symbology::Color& color )
{
	mScutcheonContentColor = color;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		osgEarth::Symbology::TextSymbol* contentSymbol = pObject->getScutcheonContentSymbol();
		contentSymbol->fill()->color() = color;
		pObject->updateScutcheonStyle();
	}
}

osgEarth::Symbology::Color CObjectManager::getScutcheonContentColor()
{
	return mScutcheonContentColor;
}

void CObjectManager::setScutcheonContentBackgroundImage( const std::string& image )
{
	mScutcheonContentBackgroundImage = image;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->getScutcheonStyle().getOrCreate<osgEarth::Annotation::IconSymbol>()->url()->setLiteral(
			mScutcheonContentBackgroundImage);
		pObject->updateScutcheonStyle();
	}
}

std::string CObjectManager::getScutcheonContentBackgroundImage()
{
	return mScutcheonContentBackgroundImage;
}

void CObjectManager::setTrackLinePointPattern( const bool& isPointPattern )
{
	mIsTrackPointPattern = isPointPattern;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setTrackLinePointPattern(isPointPattern);
	}
}

void CObjectManager::setScutcheonContentUseShadow( const bool& isUseShadow )
{
	mIsScutcheonContentUseShadow = isUseShadow;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		if(mIsScutcheonContentUseShadow)
			pObject->getScutcheonContentSymbol()->haloBackdropType() = osgText::Text::BackdropType::OUTLINE;
		else
			pObject->getScutcheonContentSymbol()->haloBackdropType() = osgText::Text::BackdropType::NONE;
		pObject->updateScutcheonStyle();
	}
}

bool CObjectManager::getScutcheonContentUseShadow()
{
	return mIsScutcheonContentUseShadow;
}

void CObjectManager::setScutcheonTitleUseShadow( const bool& isUseShadow )
{
	mIsScutcheonTitleUseShadow = isUseShadow;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		if(mIsScutcheonContentUseShadow)
			pObject->getScutcheonTitleSymbol()->haloBackdropType() = osgText::Text::BackdropType::OUTLINE;
		else
			pObject->getScutcheonTitleSymbol()->haloBackdropType() = osgText::Text::BackdropType::NONE;
		pObject->updateScutcheonStyle();
	}
}

bool CObjectManager::getScutcheonTitleUseShadow()
{
	return mIsScutcheonTitleUseShadow;
}

void CObjectManager::dealDynamicObjectDestory( DynamicObject::CDynamicObject * pObject )
{
	if(NULL == pObject || NULL == mpDynamicObjectListener)
		return;
	std::vector<std::string> keys;
	bool isObjectInRegion = false;
	for (int layerIndex = 0; layerIndex < mSceneObjectLayers.size(); ++layerIndex)
	{
		DynamicObject::CSceneObjectLayer* pSceneObjectLayer = mSceneObjectLayers.at(layerIndex);
		if(NULL == pSceneObjectLayer)
			continue;
		std::string sceneLayerType = pSceneObjectLayer->getType();
		keys.clear();
		pSceneObjectLayer->getKeys(keys);
		if (keys.empty())
			continue;
		osg::Vec3d currentPos = pObject->getModelCurrentPoint();
		for (int i = 0; i < keys.size(); ++i)
		{
			bool isInRegion = false;
			std::string id = keys.at(i);
			std::string layerObjectName = "";
			Graphic::CGeoCircle* pGeoCircle = dynamic_cast<Graphic::CGeoCircle*>(pSceneObjectLayer->getObject(id));
			if(pGeoCircle)
			{
				layerObjectName = pGeoCircle->getName();
				double radius = pGeoCircle->getRadius();
				osg::Vec3d circlePos = pGeoCircle->getGeoPosition();
				isInRegion = CCalculatorHelper::isPointInCircle(currentPos,circlePos,radius,mpRefMapNode->getMap()->getSRS());
			}

			Graphic::CGeoPolygon* pGeoPolygon = dynamic_cast<Graphic::CGeoPolygon*>(pSceneObjectLayer->getObject(id));
			if(NULL == pGeoCircle && pGeoPolygon)
			{
				layerObjectName = pGeoPolygon->getName();
				isInRegion = CCalculatorHelper::isPointInGeometry(currentPos,pGeoPolygon->getGeometry(),mpRefMapNode->getMap()->getSRS());
			}

			Geo::CGeoCircleEx* pGeoCircleEx = dynamic_cast<Geo::CGeoCircleEx*>(pSceneObjectLayer->getObject(id));
			if(pGeoCircleEx)
			{
				layerObjectName = pGeoCircleEx->getName();
				double radius = pGeoCircleEx->getRadius();
				osg::Vec3d circlePos = pGeoCircleEx->getGeoPosition();
				isInRegion = CCalculatorHelper::isPointInCircle(currentPos,circlePos,radius,mpRefMapNode->getMap()->getSRS());
			}

			if(isInRegion)
			{
				isObjectInRegion = true;
				mpDynamicObjectListener->objectDestroy(pObject->getName(),pObject->getID(),isInRegion,
					sceneLayerType,pSceneObjectLayer->getName(),layerObjectName);
			}//isInRegions
		}
	}

	if(true == isObjectInRegion)
		return;

	mpDynamicObjectListener->objectDestroy(pObject->getName(),pObject->getID(),false,"","","");
}

void CObjectManager::setEnableDragScutcheon( const bool& isEnable )
{
	if(mEnableDragScutcheon == isEnable)
		return;
	mEnableDragScutcheon = isEnable;
#if _USE_NEW_SCUTEON_ == 0
	if(mEnableDragScutcheon && mpRefMapNode)
	{
		if (mpModelPickerEx)
			mpModelPickerEx->addChild(mpRefMapNode);

		/*if(NULL == mpModelPickerEx)
		{
		mpModelPickerEx = new ModelPickerEx;
		mpModelPickerEx->addChild(mpRefMapNode);
		if(NULL == mpModelPickerExCallback)
		mpModelPickerExCallback = new ModelPickerExCallback(mpModelPickerEx);
		mpModelPickerEx->setDefaultCallback(mpModelPickerExCallback);
		}
		this->mpRefViewer->addEventHandler(mpModelPickerEx);*/
	}
	else
	{
		if (mpModelPickerEx)
			mpModelPickerEx->removeChild(mpRefMapNode);
		/*this->mpRefViewer->removeEventHandler(mpModelPickerEx);
		mpModelPickerEx = NULL;
		mpModelPickerExCallback = NULL;*/
	}
#endif
}

bool CObjectManager::getEnableDragScutcheon()
{
	return mEnableDragScutcheon;
}

void CObjectManager::setScutcheonScreenCoordinatesOffset( const osg::Vec2s& offset )
{
	mScutcheonScreenCoordinatesOffset = offset;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setScutcheonScreenCoordinatesOffset(mScutcheonScreenCoordinatesOffset);
		pObject->updateScutcheonStyle();
	}
}

osg::Vec2s CObjectManager::getScutcheonScreenCoordinatesOffset()
{
	return mScutcheonScreenCoordinatesOffset;
}

void CObjectManager::setScutcheonSeparativeSign( const std::string& separativeSign )
{
	mScutcheonSeparativeSign = separativeSign;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setScutcheonSeparativeSign(separativeSign);
		pObject->updateScutcheonStyle();
	}
}

std::string CObjectManager::getScutcheonSeparativeSign()
{
	return mScutcheonSeparativeSign;
}

void CObjectManager::setScutcheonVerticalSpacing( const int& verticalSpacing )
{
	mScutcheonVerticalSpacing = verticalSpacing;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setScutcheonVerticalSpacing(verticalSpacing);
		pObject->updateScutcheonStyle();
	}
}

int CObjectManager::getScutcheonVerticalSpacing()
{
	return mScutcheonVerticalSpacing;
}

void CObjectManager::setTitleImageSize(const osg::Vec2s& titleImageSize)
{
	this->mTitleImageSize = titleImageSize;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setTitleImageSize(this->mTitleImageSize);
		pObject->updateScutcheonStyle();
	}
}

osg::Vec2s CObjectManager::getTitleImageSize()
{
	return this->mTitleImageSize;
}

int CObjectManager::getTitleImageOffset()
{
	return this->mTitleImageOffset;
}

void CObjectManager::setTitleImageOffset(const int& titleImageOffset)
{
	this->mTitleImageOffset = titleImageOffset;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setTitleImageOffset(this->mTitleImageOffset);
		pObject->updateScutcheonStyle();
	}
}

void CObjectManager::setBackImageWidth(const int& backImageWidth)
{
	this->mBackImageWidth = backImageWidth;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setBackImageWidth(this->mBackImageWidth);
		pObject->updateScutcheonStyle();
	}
}

int CObjectManager::getBackImageWidth()
{
	return this->mBackImageWidth;
}

int CObjectManager::getIntervalBetweenBottomAndContent()
{
	return this->mIntervalBetweenBottomAndContent;
}

void CObjectManager::setIntervalBetweenBottomAndContent(const int& intervalBetweenBottomAndContent)
{
	this->mIntervalBetweenBottomAndContent = intervalBetweenBottomAndContent;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setIntervalBetweenBottomAndContent(this->mIntervalBetweenBottomAndContent);
		pObject->updateScutcheonStyle();
	}
}

int CObjectManager::getContentHorizontalOffset()
{
	return this->mContentHorizontalOffset;
}

void CObjectManager::setContentHorizontalOffset(const int& contentHorizontalOffset)
{
	this->mContentHorizontalOffset = contentHorizontalOffset;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setContentHorizontalOffset(this->mContentHorizontalOffset);
		pObject->updateScutcheonStyle();
	}
}

int CObjectManager::getIntervalBetweenTitleAndContent()
{
	return this->mIntervalBetweenTitleAndContent;
}

void CObjectManager::setIntervalBetweenTitleAndContent(const int& intervalBetweenTitleAndContent)
{
	this->mIntervalBetweenTitleAndContent = intervalBetweenTitleAndContent;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setIntervalBetweenTitleAndContent(this->mIntervalBetweenTitleAndContent);
		pObject->updateScutcheonStyle();
	}
}

void CObjectManager::setIconTextVisible( const bool& isVisible )
{
	mIsIconTextVisible = isVisible;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setIconTextVisible(isVisible);
		pObject->updateIconStyle();
	}
}

bool CObjectManager::getIconTextVisible()
{
	return mIsIconTextVisible;
}

void CObjectManager::setScuLineColor( const osg::Vec4f& color )
{
	mScuLineColor = color;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setScuLineColor(color);
	}
}

osg::Vec4f CObjectManager::getScuLineColor()
{
	return mScuLineColor;
}

void CObjectManager::setScuLineType( DynamicObject::LineType lineType )
{
	mScuLineType = lineType;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setScuLineType(lineType);
	}
}

DynamicObject::LineType CObjectManager::getScuLineType()
{
	return mScuLineType;
}

void CObjectManager::setScuLineVisible( const bool& isVisible )
{
	mIsScuLineVisible = isVisible;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setScuLineVisible(isVisible);
	}
}

bool CObjectManager::getScuineVisible()
{
	return mIsScuLineVisible;
}

void CObjectManager::setScuLineWidth( const float& lineWidth )
{
	mScuLineWitdh = lineWidth;
	std::vector<std::string> keys;
	this->getKeys(keys);
	if (keys.empty())
		return;
	for (int i = 0; i < keys.size(); ++i)
	{
		std::string id = keys.at(i);
		CDynamicObject * pObject = dynamic_cast<CDynamicObject*>(this->getObject(id));
		if(NULL == pObject)
			continue;
		pObject->setScuLineWidth(lineWidth);
	}
}

float CObjectManager::getScuLineWidth()
{
	return mScuLineWitdh;
}

void CDynamicObjectListener::objectInRegin( const std::string& objectName, const std::string& objectID,
	const std::string& layerType,const std::string& layerName,const std::string& layerObjectName )
{
	return;
}

void CDynamicObjectListener::objectOutRegin( const std::string& objectName, const std::string& objectID,
	const std::string& layerType,const std::string& layerName,const std::string& layerObjectName )
{
	return;
}

void CDynamicObjectListener::objectDestroy( const std::string& objectName,const std::string& objectID,const bool& isInRegin, const std::string& layerType,const std::string& layerName,const std::string& layerObjectName )
{
	return;
}

void CDynamicObjectListener::onCreateDynamicObject(Common::IObject* pNewDynamicObject)
{
	return;
}

void CDynamicObjectListener::onDynamicObjectReceiveSignal(Common::IObject* pCurDynamicObject)
{
	return;
}

void CDynamicObjectListener::onDoubleClickScutcheon(Common::IObject* pDynamicObject)
{
	return;
}

void CDynamicObjectListener::onTrackLineFadEvent(Common::IObject* pDynamicObject)
{
	return;
}	

void CDynamicObjectListener::onTrackLineRecoverEvent(Common::IObject* pDynamicObject)
{
	return;
}

void CDynamicObjectListener::onDynamicObjectDestoryEvent(Common::IObject* pDynamicObject)
{
	return;
}
