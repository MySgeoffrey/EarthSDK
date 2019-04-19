#if !defined(_WIRELESSOBJECT_H)
#define _WIRELESSOBJECT_H

#include "dynamicobject/dynamicobjectconfig.h"
#include "framework/common/isceneobject.h"
#include "geoobject/geosegment.h"
#include "dynamicobject/dynamicobject.h"

namespace DynamicObject
{
	class VR_DYNAMICOBJECT_EXPORT CWirelessObject : public Geo::CGeoSegment
	{
	public:
		CWirelessObject(osgEarth::MapNode* mMapNode);
		~CWirelessObject();
		void setWirelessObjectPos(const osg::Vec3d& objctPos);
		osg::Vec3d getWirelessObjectPos();
		void setTargetInfo(const float& pitchAngle,const float& radius);
		void  getTargetInfo(float& pitchAngle,float& radius);
		virtual bool synUpdate();
		bool reveiveSignal(DynamicObject::CDataSingal* pRefSignal);
		virtual bool hasLoadResource();
		virtual bool aSynLoad();
		void setCreateTime(const double& createTime);
		double getCreateTime();
		void setDestroyTimeInterval(const double& timeInterval);
		double getDestroyTimeInterval();

		void setAssociatedFacilityID(const std::string& facilityID);
		std::string getAssociatedFacilityID();
	private:
		osg::Vec3d mpWirelessObjectPos;
		float mPitchAngle;
		float mRadius;
		OpenThreads::Mutex mPathMutex;
		bool mHasLoadResource;
		bool mIsRenderToScene;
		double mCreateTime;
		double mDestroyTimeInterval;
		std::string mAssociatedFacilityID;
	};
}

#endif