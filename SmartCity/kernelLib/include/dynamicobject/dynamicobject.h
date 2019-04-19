#if !defined(_DYNAMICOBJECT_H)
#define _DYNAMICOBJECT_H

#include "dynamicobject/dynamicobjectconfig.h"
#include "framework/common/isceneobject.h"
#include "geographic/geopoint.h"
#include "geoobject/geoscutcheon.h"
#include <osg/AnimationPath>
#include "geoobject/geosegment.h"
#include <osgViewer/Viewer>

namespace DynamicObject
{
	class VR_DYNAMICOBJECT_EXPORT CDynamicObjectType
	{
	public:
		static const std::string UAV;
		static const std::string BIRD;
	};

	class VR_DYNAMICOBJECT_EXPORT CDynamicObjectGroupType
	{
	public:
		static const std::string RADARTARGET;
		static const std::string WIRELESSTARGET;
	};

	//�״������źŶ���
	class VR_DYNAMICOBJECT_EXPORT CDataSingal : public Common::IObject
	{
	public:
		CDataSingal()
		{
			name = "unname";
			id = "0";
			type = "none";
			group= "none";
			longitude = 0;
			latitude = 0;
			altitude = 0;
			iconImageName = "";
			azimuth = 0;
			radius = 0;
			deviceID = "";
			iconscale = 1;
		}		

		virtual ~CDataSingal()
		{
		}

		CDataSingal* clone()
		{
			CDataSingal* pCloneObject = new CDataSingal();
			pCloneObject->name = this->name;
			pCloneObject->id = this->id;
			pCloneObject->type = this->type;
			pCloneObject->group = this->group;
			pCloneObject->deviceID = this->deviceID;
			pCloneObject->longitude = this->longitude;
			pCloneObject->latitude = this->latitude;
			pCloneObject->altitude = this->altitude;
			pCloneObject->iconImageName = this->iconImageName;
			pCloneObject->azimuth = this->azimuth;
			pCloneObject->radius = this->radius;
			pCloneObject->iconscale = this->iconscale;
			pCloneObject->fields = this->fields;
			return pCloneObject;
		}
		
	public:
		//���ƣ��������޸�
		std::string name;
		//̽��Ŀ���id
		std::string id;
		//̽��Ŀ�������(���˻����񣬵���)
		std::string type;
		//̽��Ŀ��ķ���(�״�̽��Ŀ��������ߵ�̽��Ŀ��)
		std::string group;
		//̽���豸id
		std::string deviceID;
		/*�״�̽��Ŀ���ʵʱλ�û������ߵ�̽���豸��λ��*/
		double longitude;
		double latitude;
		double altitude;
		std::string iconImageName;
		//���ߵ�̽��Ŀ�귽λ��
		double azimuth;
		//���ߵ�̽��뾶
		double radius;
		double iconscale;//ͼ������ű���
		Geo::Fields fields;//Ŀ�������ֶμ���(ÿ���ֶΰ����ֶ������ֶ�ֵ���Ƿ���Ч��)
	};

	//�켣��ʽ
	enum LineType
	{
		//ʵ�߸�ʽ
		SOLID_LT,

		//���߸�ʽ
		DOTTED_LT,

		//.  .  .  .  .  .  .  .  .  .  .  .  .  .
		//���ʽ0x0101
		POINT_LT,

		//____ . ____ . _____ . _____. _____
		//�㻮��0x27FF
		DOTDASH_LT,

		//_____ _ _____ _ _____ _ _____ _ _____
		//������0x33FF,
		CENTRELINE_LT,

		// ____ . . ____ . . ____ . . ____ . . ____
		//˫�㻮��0x24FF,
		DOUBLEDOT_LT,

		// ____ . . .  ____ . . . ____ . . . ____ . . . ____
		//���㻮��0x55FF
		TRIDOT_LT
	};

	class CDynamicObjectCallback;
	class VR_DYNAMICOBJECT_EXPORT CDynamicObject : public Common::ISceneObject
	{
	public:
		CDynamicObject(osgEarth::MapNode* mMapNode);
		~CDynamicObject();

		std::map<std::string,std::string>& getUserProperty();

		void initial(osgViewer::Viewer* pRefViewer);

		double contain(const int& x, const int& y ,osg::Camera* pCamera);
		//���ù켣�ߵ����
		void setTrackLinePointSize(const int& trackLinePointSize);
		int getTrackLinePointSize();

		void setTrackLinePointPattern(const bool& isPointPattern);

		void setTrackLineColor(const osg::Vec4f& color);
		osg::Vec4f getTrackLineColor();
		void setTrackLineType(LineType lineType);
		LineType getTrackLineType();
		void setTrackLineVisible(const bool& isVisible);
		bool getTrackLineVisible();
		void setTrackLineWidth(const float& lineWidth);
		float getTrackLineWidth();

		//���ñ����˶��켣��ĸ���
		void setPathPointSize(const int& pathPointSize);
		int getPathPointSize();
		//���ö���ģ��·��
		void setModelPath(const std::string& modelPath);

		//���ñ�����ʾ����
		void setScutcheonContent(const std::string& content);
		//��ȡ������ʾ����
		std::string getScutcheonContent();
		void setScutcheonVisible(const bool& isVisible);
		bool getScutcheonVisible();

		//֪ͨ���ر���
		void notifyHideScutcheon();

		//����ͼ����ʾͼƬ
		void setObjectIcon(const std::string& imagePath);
		//��ȡͼ����ʾͼƬ
		std::string getObjectIcon();
		void setIconVisible(const bool& isVisible);
		bool getIconVisible();

		//֪ͨ����ͼ��
		void notifyHideIcon();

		//added by sjl->2018.7.22
		bool reveiveSignal(DynamicObject::CDataSingal* pRefSignal);

		//����ģ���˶��켣�㣬�Լ������ϸ��켣��ļ��ʱ��
		void addModelPathPoint(const osg::Vec3d& pos,const double& time);
		//��ȡģ��ָ��ʱ��Ĺ켣��
		osg::Vec3d getModelPathPoint(const double& time);
		//��ȡģ�͵�ǰ�켣��
		osg::Vec3d getModelCurrentPoint();

		osg::Node* getDynamicObjctNode();

		//���ö�̬�����������λ��
		void setPosition(osg::Vec3d pos);

		osg::Vec3d getPosition();

		void setScreenCamera(osg::Camera* pScreenCamera);

		osg::Camera* getScreenCamera();

		void listenObjectCull(osg::NodeVisitor* nv);
#if 1
		/************************************************************************/
		/* �������ݵ������������겻��Ⱦ�������ο����û���Ҫʵ�ִ˽ӿ�

		*/
		/************************************************************************/
		virtual bool aSynLoad();

		/************************************************************************/
		/* �����ص�������������Ⱦ�����������ο����û���Ҫʵ�ִ˽ӿ�

		*/
		/************************************************************************/
		virtual bool synUpdate(osg::Camera* pCamera);

		virtual bool hasLoadResource();

		void clearSceneTrackLines();

		//��γ�ȵ�
		void addDirectionalPoint(const std::string& id,const osg::Vec3d& point);
		bool exsitDirectionalPoint(const std::string& id);
		void removeDirectionalPoint(const std::string& id);
		int getDirectionalPointsSize();
		void setDirectionallineVisible(const bool& isVisible);
		bool getDirectionallineVisible();
		void setDirectionallineColor(const osg::Vec4f& color);
		osg::Vec4f getDirectionallineColor();
		void setDirectionallineType(LineType lineType);
		LineType getDirectionallineType();
		void setDirectionallineWidth(const float& lineWidth);
		float getDirectionallineWidth();

		void addRegion(const std::string& id,const std::string& regionType);
		bool exsitRegion(const std::string& id);
		bool exsitRegionType(const std::string& regionType);
		void removeRegion(const std::string& id);
		int getRegionSize();

		void setIsInWarnningRegion(const bool& isInRegion);
		bool isInWarnningRegion();

		void setIsInNoFlyingRegion(const bool& isInRegion);
		bool isInNoFlyingRegion();

		void setIsInScanningRegion(const bool& isInRegion);
		bool isInScanningRegion();

		void setNearestUpdateTime(const double& nearestUpdateTime);
		double getNearestUpdateTime();

		void setTrackLineFadingTimeInterval(const double& timeInterval);
		double getTrackLineFadingTimeInterval();

		void setIconUnVisibleTimeInterval(const double& timeInterval);
		double getIconUnVisibleTimeInterval();

		void setDestroyTimeInterval(const double& timeInterval);
		double getDestroyTimeInterval();

		int getNeedUpatePathPointSize();

		void setAssociatedFacilityID(const std::string& facilityID);

		std::string getAssociatedFacilityID();

		void setIconScale(const double& scale);

		double getIconScale();

		void setScutcheonUnVisibleTimeInterval(const double& timeInterval);
		double getScutcheonUnVisibleTimeInterval();

		void changeScutcheonHeaderColor(const osgEarth::Symbology::Color& headerColor);
		void changeTrackLineColor(const osg::Vec4f& trackLineColor);


		bool getNeedRecoverTrackLineFadState();
		void setIsTrackLineFadState(const bool& value);
		bool getIsTrackLineFadState();
		void setTrackLineFadColor(const osg::Vec4f& trackLineColor);
		void recoverScutcheonHeaderColor();
		void recoverTrackLineColor();

		//��˸��ʱ�䣬��λ��
		void setObejctBlink(const double& time);
		double getObjectBlink();

		//ʱ��������λ����
		void setObejctBlinkInterval(const double& timeInterval);
		double getObejctBlinkInterval();

		void setScutcheonVisibleFields(const Geo::Fields fields);

		Geo::Fields getScutcheonVisibleFields();

		void updateScutcheonContent(const Geo::Fields fields);

		void setCurrentScutcheonVisibleFields(const Geo::Fields fields);

		Geo::Fields getCurrentScutcheonVisibleFields();

		void updateCurrentScutcheonContent(const Geo::Fields fields);

		void setObjectVisible(const bool& isVisible);

		osgEarth::Symbology::Style& getIconStyle();
		osgEarth::Symbology::TextSymbol* getScutcheonTitleSymbol();
		osgEarth::Symbology::TextSymbol* getScutcheonContentSymbol();
		osgEarth::Symbology::Style& getScutcheonStyle();
		osgEarth::Symbology::Style& getScutcheonTitleImageStyle();

		void updateIconStyle();
		void updateScutcheonStyle();

		void setScutcheonScreenCoordinatesOffset(const osg::Vec2s& offset);

		osg::Vec2s getScutcheonScreenCoordinatesOffset();

		void setScutcheonSeparativeSign(const std::string& separativeSign);

		std::string getScutcheonSeparativeSign();

		void setScutcheonVerticalSpacing(const int& verticalSpacing);

		int getScutcheonVerticalSpacing();

		void setTitleImageSize(const osg::Vec2s& titleImageSize);

		osg::Vec2s getTitleImageSize();

		int getTitleImageOffset();

		void setTitleImageOffset(const int& titleImageOffset);

		int getIntervalBetweenTitleAndContent();

		void setIntervalBetweenTitleAndContent(const int& intervalBetweenTitleAndContent);

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
		void setScuLineType(LineType lineType);
		LineType getScuLineType();
		void setScuLineVisible(const bool& isVisible);
		bool getScuineVisible();
		void setScuLineWidth(const float& lineWidth);
		float getScuLineWidth();

		private:
			osg::Vec3d worldToScreen(const osg::Vec3d& worldPoint);
			osg::Vec3d screenToWorld(const osg::Vec3d& screenPoint);
#endif
		friend class CDynamicObjectCallback;
	private:
		osg::Camera* mpScreenCamera;
		Graphic::CGeoPoint* mpGeoPoint;
		Geo::CGeoScutcheon* mpGeoScutcheon;
		osg::ref_ptr<osg::Geode> mpLineGeode;
		osg::ref_ptr<osg::Geode> mpScuLineGeode;
		osg::ref_ptr<osg::LOD> mpLineNode;
		osg::ref_ptr<osg::LOD> mpScuLineNode;
		std::map<double,osg::Vec3d> mPathPoints;
		OpenThreads::Mutex mPathMutex;
		OpenThreads::Mutex mScuteonFieldMutex;
		int mTrackLinePointSize;
		osg::Vec4f mTrackLineColor;
		LineType mTrackLineType;
		float mTrackLineWitdh;
		int mPathPointSize;
		bool mIsTrackLineVisible;
		bool mIsScutcheonVisible;
		bool mIsIconVisible;
		std::string mIconImagePath;
		osgEarth::MapNode* mpMapNode;
		std::string mModelPath;
		osg::AnimationPathCallback* mpAnimationPathCallback;
		osg::Vec3d mModelPos;
		osg::Node* mpCallbackNode;
		bool mHasLoadResource;
		bool mIsFirstPos;
		osg::Vec3d mFirstPos;
		osg::Vec3d mFirstWPos;
		bool mIsDirectionallineVisible;
		//��������
		std::map<std::string,Geo::CGeoSegment*> mDirectionalLines;
		osg::Vec4f mDirectionalLineColor;
		LineType mDirectionalLineType;
		float mDirectionalLineWitdh;
		osg::Vec3d mTrackLastPos;
		std::map<std::string,std::string> mRegions;
		bool mIsInWarnningRegion;
		bool mIsInScanningRegion;
		bool mIsInNoFlyingRegion;
		double mNearestUpdateTime;
		double mTrackLineFadingTimeInterval;
		double mIconUnVisibleTimeInterval;
		double mDestroyTimeInterval;
		std::string mAssociatedFacilityID;
		double mIconScale;
		double mScutcheonUnVisibleTimeInterval;
		osgEarth::Symbology::Color mScutcheonHeaderColor;
		double mObejctBlinkTime;//��˸ʱ��
		double mObejctBlinkTimeInterval;//��˸ʱ����
		Geo::Fields mFields;
		Geo::Fields mCurrentFields;
		bool mIsFieldsChanged;
		bool mIsScutcheonContentChanged;
		bool mIsCurrentFieldsChanged;
		bool mIsScutcheonCurrentContentChanged;
		bool mIsScutcheonHeaderColorChanged;
		bool mIsIconImageChanged;
		bool mIsBlink;
		//added by sjl 2018-10-1
		bool mNeedUpdateIconVisible;
	
		bool mIsIconStyleChanged;
		bool mIsScutcheonStyleChanged;

		bool mIsTrackPointPattern;
		bool mIsTrackLinePatternChanged;
		bool mIsTrackLineColorChanged;
		bool mIsTrackLineColorFad;
		bool mIsTrackLinePointSizeChanged;

		bool mIsScutcheonScreenCoordinatesChanged;
		osg::Vec2s mScutcheonScreenCoordinatesOffset;

		std::string mScutcheonSeparativeSign;

		int mScutcheonVerticalSpacing;
		int mIntervalBetweenTitleAndContent;
		int mTitleImageOffset;

		osg::Matrixd mScuLineMatrix;
		osgViewer::Viewer* mpRefViewer;

		bool mIsIconTextVisible;
		osg::Vec4f mScuLineColor;
		LineType mScuLineType;
		float mScuLineWitdh;
		bool mIsScuLineVisible;
		bool mIsScuLineStyleChanged;
		bool mIsScutcheonRenderToScene;
		bool mIsIconRenderToScene;

		osg::Vec3d mCurrentWorldPos;
		osg::Vec4f mTrackLineOriginalColor;
		std::map<std::string,std::string> mUserProperty;
		int mBackImageWidth;
		int mIntervalBetweenBottomAndContent;
		int mContentHorizontalOffset;
		osg::Vec2s mTitleImageSize;
	};
}

#endif