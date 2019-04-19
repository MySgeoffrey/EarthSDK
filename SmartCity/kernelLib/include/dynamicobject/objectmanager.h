#ifndef _OBJECTMANAGER_H
#define _OBJECTMANAGER_H

#include "dynamicobject/dynamicobjectconfig.h"
#include "dynamicobject/dynamicobject.h"
#include "dynamicobject/objectcontainerex.h"
#include <OpenThreads/Mutex>
#include <OpenThreads/Thread>
#include <math.h>
#include <osg/NodeCallback>
#include <osgEarth/MapNode>
#include <osgViewer/Viewer>
#include "dynamicobject/sceneobjectlayer.h"
#include "dynamicobject/dynamicobject.h"
#include <osgEarthUtil/RTTPicker>

class CBackgroundProcessor;
class ModelPickerEx;
class ModelPickerExCallback;
class CObjectManager;
class VR_DYNAMICOBJECT_EXPORT CDynamicObjectListener
{
public:
	virtual void objectInRegin(const std::string& objectName,const std::string& objectID,
		const std::string& layerType,const std::string& layerName,const std::string& layerObjectName);
	virtual void objectOutRegin(const std::string& objectName,const std::string& objectID,
		const std::string& layerType,const std::string& layerName,const std::string& layerObjectName);
	/**对象销毁接口
	*objectName：对象名称
	*objectID：对象的ID
	*isInRegin：对象是否在区域，在的话后面返回具体的值，不在返回空
	*layerType：所在区域图层类型
	*layerName：所在区域图层名称
	*layerObjectName：所在区域图层具体的区域名称
	**/
	virtual void objectDestroy(const std::string& objectName,const std::string& objectID,const bool& isInRegin,
		const std::string& layerType,const std::string& layerName,const std::string& layerObjectName);

	/**对象创建时回调
	*pNewDynamicObject：新对象指针
	**/
	virtual void onCreateDynamicObject(Common::IObject* pNewDynamicObject);

	/**对象接信号的回调
	*pCurDynamicObject：当前对象指针
	**/
	virtual void onDynamicObjectReceiveSignal(Common::IObject* pCurDynamicObject);

	/**对象标牌双击的回调
	*pCurDynamicObject：与标牌对应的目标对象指针
	**/
	virtual void onDoubleClickScutcheon(Common::IObject* pDynamicObject);

	/**对象航迹变灰的回调
	*pCurDynamicObject：与标牌对应的目标对象指针
	**/
	virtual void onTrackLineFadEvent(Common::IObject* pDynamicObject);

	/**对象航迹变灰后的恢复的回调
	*pCurDynamicObject：与标牌对应的目标对象指针
	**/
	virtual void onTrackLineRecoverEvent(Common::IObject* pDynamicObject);

	/**对象航迹销毁的回调
	*pCurDynamicObject：与标牌对应的目标对象指针
	**/
	virtual void onDynamicObjectDestoryEvent(Common::IObject* pDynamicObject);
};

class VR_DYNAMICOBJECT_EXPORT CObjectManagerCallBack 
	: public osg::NodeCallback
{
public:
	CObjectManagerCallBack();

	virtual ~CObjectManagerCallBack();

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
public:
	CObjectManager* Owner;
};

class VR_DYNAMICOBJECT_EXPORT CObjectManager
	: public CSafeObjectContainer, public osg::NodeCallback
{
public:
	CObjectManager();

	virtual ~CObjectManager();

	virtual void initial(osg::Group* pRefRoot
		, osgEarth::MapNode* pRefMapNode,
		osgViewer::Viewer* pRefViewer,
		osg::Camera* pRefScreenCamera);

	static CObjectManager* instance()
	{
		static CObjectManager* g_ObjectManagerInstance = NULL;
		if (NULL == g_ObjectManagerInstance)
			g_ObjectManagerInstance = new CObjectManager();
		return g_ObjectManagerInstance;
	}

	DynamicObject::CDynamicObject* getDynamicObject(const int& x, const int& y ,osg::Camera* pCamera);

	void closeDataProcessThread();

	virtual void frame(osg::Node* node, osg::NodeVisitor* nv);

	virtual void findVisibleSceneObject(std::vector<Common::IObject*>& sceneObjects,
		osg::Camera* pCamera, osg::NodeVisitor* nv, const bool& needToRender);
	void clearSceneTrackLines();
	/// <summary>
	/// 异步接收信号
	/// </summary>
	virtual bool aSynReveiveSignal(DynamicObject::CDataSingal* pRefSignal);

	void setWarnningObjectLayer(DynamicObject::CSceneObjectLayer* pSceneObjectLayer);

	void setNoFlyingObjectLayer(DynamicObject::CSceneObjectLayer* pSceneObjectLayer);

	void setScanningObjectLayer(DynamicObject::CSceneObjectLayer* pSceneObjectLayer);

	std::vector<DynamicObject::CSceneObjectLayer*> getSceneObjectLayers();

	void setTrackLineVisible(const bool& isVisible);

	bool getTrackLineVisible();

	void setIconVisible(const bool& isVisible);

	bool getIconVisible();

	void setScutcheonVisible(const bool& isVisible);

	bool getScutcheonVisible();

	void setDirectionallineVisible(const bool& isVisible);

	bool getDirectionallineVisible();

	void setWholeProprietaryVisible(const bool& isVisible);

	void setTrackLineColor(const osg::Vec4f& color);
	
	osg::Vec4f getTrackLineColor();
	
	void setTrackLineType(DynamicObject::LineType lineType);
	
	DynamicObject::LineType getTrackLineType();

	void setTrackLineWidth(const float& lineWidth);

	float getTrackLineWidth();

	void setDirectionallineColor(const osg::Vec4f& color);

	osg::Vec4f getDirectionallineColor();

	void setDirectionallineType(DynamicObject::LineType lineType);

	DynamicObject::LineType getDirectionallineType();

	void setDirectionallineWidth(const float& lineWidth);

	float getDirectionallineWidth();

	void setTrackLinePointSize(const int& trackLinePointSize);

	int getTrackLinePointSize();

	void addDynamicObjectListener(CDynamicObjectListener* pDynamicObjectListener);

	CDynamicObjectListener* getDynamicObjectListener();

	void setDirectionallineDestroyTimeInterval(const double& timeInterval);

	double getDirectionallineDestroyTimeInterval();

	void setTrackLineFadingTimeInterval(const double& timeInterval);
	double getTrackLineFadingTimeInterval();

	void setIconUnVisibleTimeInterval(const double& timeInterval);
	double getIconUnVisibleTimeInterval();

	void setTrackLineDestroyTimeInterval(const double& timeInterval);
	double getTrackLineDestroyTimeInterval();

	Core::IObjectContainer getFacilityDetectionTargets(const std::string& facilityID);

	//objctGroup指雷达类RADARTARGET、无线电类WIRELESSTARGET
	Core::IObjectContainer getDetectionTargetsByGroup(const std::string& objctGroup);

	//objctType指UAV、BIRD
	Core::IObjectContainer getDetectionTargetsByObjectType(const std::string& objctType);

	Common::IObject* getDetectionTarget(const std::string& objctID);

	void setScutcheonUnVisibleTimeInterval(const double& timeInterval);

	double getScutcheonUnVisibleTimeInterval();

	void setScutcheonVisibleFields(const std::vector<std::string> fields);

	std::vector<std::string> getScutcheonVisibleFields();

	void setFacilityDetectionTargets(const std::string& facilityID,
		bool isTrackLineVisible,bool isIconVisible,
		bool isScutcheonVisible,bool isDirectionallineVisible);

	//修改指定对象的注记图片
	void changeIconImage(const std::string& objectID,const std::string& imagePath);

	std::string getIconImage(const std::string& objectID);

	void setIconFont(const std::string& font);
	std::string getIconFont();

	void setIconTextSize(const float& size);
	float getIconTextSize();

	void setIconTextColor(const osgEarth::Symbology::Color& color);
	osgEarth::Symbology::Color getIconTextColor();

	void setScutcheonTitleFont(const std::string& font);
	std::string getScutcheonTitleFont();

	void setScutcheonTitleTextSize(const float& size);
	float getScutcheonTitleTextSize();

	void setScutcheonTitleTextColor(const osgEarth::Symbology::Color& color);
	osgEarth::Symbology::Color getScutcheonTitleTextColor();

	void setScutcheonTitleBackgroundImage(const std::string& image);
	std::string getScutcheonTitleBackgroundImage();

	void setScutcheonContentFont(const std::string& font);
	std::string getScutcheonContentFont();

	void setScutcheonContentSize(const float& size);
	float getScutcheonContentSize();	

	void setScutcheonContentUseShadow(const bool& isUseShadow);
	bool getScutcheonContentUseShadow();

	void setScutcheonTitleUseShadow(const bool& isUseShadow);
	bool getScutcheonTitleUseShadow();

	void setScutcheonContentColor(const osgEarth::Symbology::Color& color);
	osgEarth::Symbology::Color getScutcheonContentColor();

	void setScutcheonContentBackgroundImage(const std::string& image);
	std::string getScutcheonContentBackgroundImage();

	void setTrackLinePointPattern(const bool& isPointPattern);

	void setEnableDragScutcheon(const bool& isEnable);
	bool getEnableDragScutcheon();

	void setScutcheonScreenCoordinatesOffset(const osg::Vec2s& offset);
	osg::Vec2s getScutcheonScreenCoordinatesOffset();

	void setScutcheonSeparativeSign(const std::string& separativeSign);
	std::string getScutcheonSeparativeSign();

	void setScutcheonVerticalSpacing(const int& verticalSpacing);
	int getScutcheonVerticalSpacing();

	void setTitleImageSize(const osg::Vec2s& titleImageSize);

	osg::Vec2s getTitleImageSize();

	int getIntervalBetweenTitleAndContent();

	void setIntervalBetweenTitleAndContent(const int& intervalBetweenTitleAndContent);

	int getTitleImageOffset();

	void setTitleImageOffset(const int& titleImageOffset);

	void setBackImageWidth(const int& backImageWidth);

	int getBackImageWidth();

	int getIntervalBetweenBottomAndContent();

	void setIntervalBetweenBottomAndContent(const int& intervalBetweenBottomAndContent);

	int getContentHorizontalOffset();

	void setContentHorizontalOffset(const int& contentHorizontalOffset);

	void setIconTextVisible(const bool& isVisible);
	bool getIconTextVisible();

	void setScuLineColor(const osg::Vec4f& color);
	osg::Vec4f getScuLineColor();
	void setScuLineType(DynamicObject::LineType lineType);
	DynamicObject::LineType getScuLineType();
	void setScuLineVisible(const bool& isVisible);
	bool getScuineVisible();
	void setScuLineWidth(const float& lineWidth);
	float getScuLineWidth();
	void synHandleSignals();
private:
	void dealDynamicObjectDirectionalLine(DynamicObject::CDynamicObject * pObject);
	void dealDynamicObjectInOutRegion(DynamicObject::CDynamicObject * pObject);
	void dealNewAddScutcheonFields(DynamicObject::CDynamicObject * pObject,Geo::Fields fields);
	void dealNewAddScutcheonFields2(DynamicObject::CDynamicObject * pObject,Geo::Fields newFields);
	void dealDynamicObjectDestory(DynamicObject::CDynamicObject * pObject);
	struct FacilityDetectionTargetsState
	{
		FacilityDetectionTargetsState()
		{
			mIsTrackLineVisible = true;
			mIsIconVisible = true;
			mIsScutcheonVisible = true;
			mIsDirectionallineVisible = true;
		}
		bool mIsTrackLineVisible;
		bool mIsIconVisible;
		bool mIsScutcheonVisible;
		bool mIsDirectionallineVisible;
	};
protected:
	osg::Group* mpRefRoot;
	osg::ref_ptr<osgEarth::MapNode> mpRefMapNode;
	osgViewer::Viewer* mpRefViewer;
	osg::Camera* mpRefScreenCamera;
	std::vector<DynamicObject::CDynamicObject*> mTempSceneObjectsNeedToRender;
	std::vector<DynamicObject::CDynamicObject*> mTempSceneObjectsNeedToRelease;
	OpenThreads::Mutex mTempMutexOfNeedToRender;
	OpenThreads::Mutex mTempMutexOfNeedToRelease;
	CBackgroundProcessor* mpBackgroundProcessor;
	std::vector<DynamicObject::CSceneObjectLayer*> mSceneObjectLayers;
	CDynamicObjectListener* mpDynamicObjectListener;
	osg::Vec4f mDirectionallineColor;
	DynamicObject::LineType mDirectionallineType;
	float mDirectionallineWidth;
	std::vector<std::string> mFields;

	bool mIsTrackLineVisible;
	bool mIsIconVisible;
	bool mIsScutcheonVisible;
	bool mIsDirectionallineVisible;
	bool mIsWholeProprietaryVisibles;
	osg::Vec4f mTrackLineColor;
	DynamicObject::LineType mTrackLineType;
	float mTrackLineWidth;
	int mTrackLinePointSize;
	double mDirectionallineDestroyTimeInterval;
	double mTrackLineFadingTimeInterval;
	double mIconUnVisibleTimeInterval;
	double mTrackLineDestroyTimeInterval;
	double mScutcheonUnVisibleTimeInterval;
	std::map<std::string,FacilityDetectionTargetsState> mFacilityDetectionTargetsState;

	std::string mIconFont;
	float mIconTextSize;
	osgEarth::Symbology::Color mIconTextColor;
	std::string mScutcheonTitleFont;
	float mScutcheonTitleTextSize;
	osgEarth::Symbology::Color mScutcheonTitleTextColor;
	std::string mScutcheonTitleBackgroundImage;
	std::string mScutcheonContentFont;
	float mScutcheonContentSize;
	osgEarth::Symbology::Color mScutcheonContentColor;
	std::string mScutcheonContentBackgroundImage;
	bool mIsTrackPointPattern;
	bool mIsScutcheonContentUseShadow;
	bool mIsScutcheonTitleUseShadow;

	bool mEnableDragScutcheon;
	ModelPickerExCallback* mpModelPickerExCallback;
	ModelPickerEx* mpModelPickerEx;
	osg::Vec2s mScutcheonScreenCoordinatesOffset;

	std::string mScutcheonSeparativeSign;
	int mScutcheonVerticalSpacing;
	int mIntervalBetweenTitleAndContent;
	int mTitleImageOffset;
	bool mIsIconTextVisible;
	osg::Vec4f mScuLineColor;
	DynamicObject::LineType mScuLineType;
	float mScuLineWitdh;
	bool mIsScuLineVisible;
	int mBackImageWidth;
	int mIntervalBetweenBottomAndContent;
	int mContentHorizontalOffset;
	osg::Vec2s mTitleImageSize;
	CObjectManagerCallBack* mpObjectManagerCallBack;
	//信号接收
	std::vector<DynamicObject::CDataSingal*> mReceiveSignals;
	OpenThreads::Mutex mSignalMutex;
	friend class ModelPickerEx;
};
#endif