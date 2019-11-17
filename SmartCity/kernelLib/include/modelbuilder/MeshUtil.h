#ifndef MeshGenerator_h__
#define MeshGenerator_h__

#include "Macro.h"
#include "MeshDefine.h"
#include "osgEarth\GeoData"


namespace MeshGenerator
{
	struct MeshData;
	class SectionBase;

	class MESH_GENERATOR_EXPORT MeshUtil
	{
	public:
		static MeshUtil* getInstance();

		void setTextureRepeatLength(float l);

		void setReferenceCenter(const osgEarth::GeoPoint& center);


		MeshData* createJoint(const JointData& joint);

		MeshData* createJointLonLat(const JointData& joint);

		MeshData* createPipeSegment(const JointData& first, const JointData& second);

		MeshData* createPipeSegmentLonLat(const JointData& first, const JointData& second);


		MeshData* mergeMeshDatas(const std::vector<MeshData*>& meshDatas,bool clearOldData = true);

		osg::Node* createGeodeFromMeshData(const std::vector<MeshData*>& meshDatas,
			const osg::Vec4& color = osg::Vec4(1.f, 1.f, 1.f, 1.f), const std::string& imgPath = "");

	private:

		MeshData* createLoftLonLat(const osg::Vec3d& start, const osg::Vec3d& end, const SectionDesc& desc);

		MeshData* createLoft(const osg::Vec3& start, const osg::Vec3& end, const SectionDesc& desc);

		MeshData* createLoft(const std::vector<osg::Vec3>& pts, const SectionDesc& desc, bool isColsed = false);

		MeshData* createLoft(const std::vector<osg::Vec3>& pts, const std::vector<osg::Vec3>& outline,
			const osg::Vec3& frontDir = osg::Y_AXIS, bool isColsed = false);


		// convert into the local tangent place around this geopoint
		bool convertToLocalFromLonLat(const osg::Vec3d& lla,osg::Vec3d& local);

		MeshData* createJointCircle(const JointData& joint);
		MeshData* createJointGeneral(const JointData& joint);

		MeshData* createJointOnlyTwo(const JointData& joint);

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

	private:
		MeshUtil() = default;
		~MeshUtil() = default;

	private:
		osgEarth::GeoPoint _referencePos;
		osg::Matrix _w2l;
		float _textureRepeatLength = 15.f;
	};
}

#endif // MeshGenerator_h__
