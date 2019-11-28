#ifndef MeshGenerator_h__
#define MeshGenerator_h__

#include "Macro.h"
#include "MeshDefine.h"
#include "osgEarth\GeoData"

namespace osg
{
	Image;
}

namespace MeshGenerator
{
	struct MeshData;
	class SectionBase;

	class MESH_GENERATOR_EXPORT MeshUtil
	{
	public:
		static MeshUtil* getInstance();

		void setTextureRepeatLength(float l);

		// 弯道平滑参数 [0.0,1.0]
		void setBlendParam(float p);

		void setReferenceCenter(const osgEarth::GeoPoint& center);

		void setReferenceCenterXYZ(const osg::Vec3& xyz);

		void setHatParam(float length,float thickness);

		void setSameOffset(bool b) { _sameOffset = b; }

		void setFixedOffset(float v) { _fixedOffset = v; }

	
		MeshData* createJointLonLat(const JointData& joint, bool withHat = true);

		MeshData* createJointXYZ(const JointData& joint, bool withHat = true);

		
		MeshData* createPipeSegmentLonLat(const JointData& first, const JointData& second);

		MeshData* createPipeSegmentXYZ(const JointData& first, const JointData& second);


		bool calcutePipeSegmentPosLonLat(const JointData& first, const JointData& second, osg::Vec3d& start, osg::Vec3d& end);

		bool calcutePipeSegmentPosXYZ(const JointData& first, const JointData& second, osg::Vec3d& start, osg::Vec3d& end);


		MeshData* mergeMeshDatas(const std::vector<MeshData*>& meshDatas,bool clearOldData = true);

		osg::Node* createGeodeFromMeshData(const std::vector<MeshData*>& meshDatas,
			const osg::Vec4& color = osg::Vec4(1.f, 1.f, 1.f, 1.f), const std::string& imgPath = "");

		osg::Node* createGeodeFromMeshData(const std::vector<MeshData*>& meshDatas,
			const osg::Vec4& color = osg::Vec4(1.f, 1.f, 1.f, 1.f), osg::ref_ptr<osg::Image> image = nullptr);

	private:
		MeshData* createJoint(const JointData& joint, bool withHat = true);

		MeshData* createPipeSegment(const JointData& first, const JointData& second);

		MeshData* createLoftLonLat(const osg::Vec3d& start, const osg::Vec3d& end, const SectionDesc& desc);

		MeshData* createLoft(const osg::Vec3& start, const osg::Vec3& end, const SectionDesc& desc);

		MeshData* createLoft(const std::vector<osg::Vec3>& pts, const SectionDesc& desc, bool isColsed = false);

		MeshData* createLoft(const std::vector<osg::Vec3>& pts, const std::vector<osg::Vec3>& outline,
			const osg::Vec3& frontDir = osg::Y_AXIS, bool isColsed = false);

		bool calcutePipeSegmentPosImpl(const JointData& first, const JointData& second, osg::Vec3d& start, osg::Vec3d& end,bool islla);

		// convert into the local tangent place around this geopoint
		bool convertToLocalFromLonLat(const osg::Vec3d& lla,osg::Vec3d& local);
		bool convertToLonlatFromLocal(const osg::Vec3d& local, osg::Vec3d& lla);

		bool convertToLocalFromXYZ(const osg::Vec3d& xyz, osg::Vec3d& local);
		bool convertToXYZFromLocal(const osg::Vec3d& local, osg::Vec3d& xyz);

		MeshData* createJointCircle(const JointData& joint, bool withHat = true);
		MeshData* createJointGeneral(const JointData& joint, bool withHat = true);

		MeshData* createJointOnlyTwo(const JointData& joint, bool withHat = true);

		MeshData* createJointGeneralCrossBottom(const JointData& joint, const std::vector<SectionBase*>& sections, 
			const std::vector<osg::Matrix>& transform);
		MeshData* createJointGeneralCrossTop(const JointData& joint, const std::vector<SectionBase*>& sections,
			const std::vector<osg::Matrix>& transform);
		MeshData* createJointGeneralCrossBodySide(const JointData& joint, const std::vector<SectionBase*>& sections,
			const std::vector<osg::Matrix>& transform);
		MeshData* createJointGeneralCrossTopSide(const JointData& joint, const std::vector<SectionBase*>& sections,
			const std::vector<osg::Matrix>& transform);

		MeshData* createLoft(const std::vector<osg::Vec3>& pts, const SectionDesc& desc,
			const osg::Vec3& startFront, const osg::Vec3& endFront);

		MeshData* createJointHat(const SectionDesc& desc, const osg::Vec3& pos, const osg::Vec3& dir);

		float calcuteOffset(const osg::Vec3& v1, const osg::Vec3& v2, float w1, float w2);

		JointData calcuteJointData(const JointData& joint);

		void clockWiseSortJoint(JointData& joint);

		MeshData* createSphere(const osg::Vec3& pos, float radius);
	private:
		MeshUtil() = default;
		~MeshUtil() = default;

	private:
		osgEarth::GeoPoint _referencePos;	
		osg::Matrix _w2l;

		osg::Vec3 _referenceXYZ;

		float _textureRepeatLength = 15.f;
		float _blendParam = 0.5f;

		float _hatLength = 0.2f;
		float _hatThickness = 0.1f;

		float _fixedOffset = 0.5f;

		bool _circleSpecialProcess = true;

		bool _sameOffset = false;
	};
}

#endif // MeshGenerator_h__
