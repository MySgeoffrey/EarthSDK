#include "modelbuilder/ModelBuilder.h"
#include<osgDB/WriteFile>

namespace MeshGenerator
{
	JointData CModelCreator::createCircleJointData(
		const osg::Vec3d& in_geoPosition,
		const std::vector<osg::Vec3d>& in_adjacentPoints,
		const double& in_radius)
	{
		JointData joint;
		if (in_geoPosition.length() > 10
			&& in_radius > 0.01
			&& in_adjacentPoints.size() > 0)
		{
			joint.pos = in_geoPosition;

			SectionDesc desc;
#if 0
			desc.type = SectionType::Circle;
			desc.params[0] = in_radius;
#else
			desc.type = SectionType::Arc;
			desc.params[0] = 3;
			desc.params[1] = 3;
			desc.params[2] = 1.5;
#endif

			for (int i = 0; i < in_adjacentPoints.size(); ++i)
			{
				joint.adjacents.emplace_back(JointAdjacent());
				joint.adjacents.back().pos = in_adjacentPoints.at(i);
				joint.adjacents.back().sectionDesc = desc;
			}
		}
		return joint;
	}

	JointData CModelCreator::createRectJointData(
		const osg::Vec3d& in_geoPosition,
		const std::vector<osg::Vec3d>& in_adjacentPoints,
		const double& in_width,
		const double& in_height)
	{
		JointData joint;
		if (in_geoPosition.length() > 10
			&& in_width > 0.01
			&& in_height > 0.01
			&& in_adjacentPoints.size() > 0)
		{
			joint.pos = in_geoPosition;

			SectionDesc desc;
			desc.type = SectionType::Rect;
			desc.params[0] = in_width;
			desc.params[1] = in_height;

			for (int i = 0; i < in_adjacentPoints.size(); ++i)
			{
				joint.adjacents.emplace_back(JointAdjacent());
				joint.adjacents.back().pos = in_adjacentPoints.at(i);
				joint.adjacents.back().sectionDesc = desc;
			}
		}
		return joint;
	}

	osg::ref_ptr<osg::Node> CModelCreator::createLinkerModel(
		const JointData& in_joint,
		const osg::Vec4& in_color,
		const std::string& in_texturePath)
	{
		std::shared_ptr<MeshData> meshJoint(MeshUtil::getInstance()->createJointLonLat(in_joint));
		if (meshJoint)
		{
			osg::ref_ptr<osg::Node> meshJointNode = MeshUtil::getInstance()->createGeodeFromMeshData({
				meshJoint.get() }, in_color, in_texturePath);
				return meshJointNode;
		}
		return NULL;
	}

	osg::ref_ptr<osg::Node> CModelCreator::createPipeModel(
		const JointData& in_startJoint,
		const JointData& in_endJoint,
		const osg::Vec4& in_color,
		const std::string& in_texturePath)
	{
		std::shared_ptr<MeshData> meshSegment(MeshUtil::getInstance()->createPipeSegmentLonLat(
			in_startJoint, in_endJoint));
		if (meshSegment)
		{
			osg::ref_ptr<osg::Node> meshSegmentNode = MeshUtil::getInstance()->createGeodeFromMeshData({
				meshSegment.get() }, in_color, in_texturePath);
				return meshSegmentNode;
		}
		return NULL;
	}
	
	bool CModelExporter::execute(
		osg::ref_ptr<osg::Node> in_node,
		const ModelFileType& in_modelType,
		const std::string& in_dir,
		const std::string& in_fileName)
	{
		bool r = false;
		if (in_node
			&& in_dir != ""
			&& in_fileName != "")
		{
			std::string filePath = in_dir + "/" + in_fileName + ".";
			std::string ext = "osgb";
			if (in_modelType == ModelFileType::OBJ)
			{
				filePath += "obj";
			}
			else if (in_modelType == ModelFileType::OSGB)
			{
				filePath += "osgb";
			}
			r = osgDB::writeNodeFile(*in_node,filePath);
		}
		return r;
	}
}
