#include "modelbuilder/ModelBuilder.h"
#include <osgDB/WriteFile>
#include "osgDB\ReadFile"

namespace MeshGenerator
{
	std::map<std::string, osg::ref_ptr<osg::Image>> g_images;
	OpenThreads::Mutex g_imageMutex;
	osg::ref_ptr<osg::Image> getImage(const std::string& in_path)
	{
		osg::ref_ptr<osg::Image> r = NULL;
		g_imageMutex.lock();
		{
			std::map<std::string, osg::ref_ptr<osg::Image>>::iterator itr =
				g_images.find(in_path);
			if (itr == g_images.end())
			{
				osg::ref_ptr<osg::Image> image = osgDB::readImageFile(in_path);
				g_images[in_path] = image;
				r = image;
			}
			else
			{
				r = itr->second;
			}
		}
		g_imageMutex.unlock();
		return r;
	}

	JointData CModelCreator::createCircleJointData(
		const osg::Vec3d& in_geoPosition,
		const std::vector<osg::Vec3d>& in_adjacentPoints,
		const std::vector<double>& in_radiusVecs)
	{
		JointData joint;
		if (in_geoPosition.length() > 10
			&& in_radiusVecs.size() > 0
			&& in_adjacentPoints.size() > 0)
		{
			joint.pos = in_geoPosition;

			SectionDesc desc;
			desc.type = SectionType::Circle;

			for (int i = 0; i < in_adjacentPoints.size(); ++i)
			{
				joint.adjacents.emplace_back(JointAdjacent());
				joint.adjacents.back().pos = in_adjacentPoints.at(i);
				desc.params[0] = (in_radiusVecs.size() == in_adjacentPoints.size()) ? in_radiusVecs.at(i) : in_radiusVecs.at(0);
				joint.adjacents.back().sectionDesc = desc;
			}
		}
		return joint;
	}

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
#if 1
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
		const std::vector<double>& in_widths,
		const std::vector<double>& in_heights)
	{
		JointData joint;
		if (in_geoPosition.length() > 10
			&& in_widths.size() > 1
			&& in_heights.size() > 1
			&& in_adjacentPoints.size() > 0)
		{
			joint.pos = in_geoPosition;

			SectionDesc desc;
			desc.type = SectionType::Rect;
			

			for (int i = 0; i < in_adjacentPoints.size(); ++i)
			{
				joint.adjacents.emplace_back(JointAdjacent());
				joint.adjacents.back().pos = in_adjacentPoints.at(i);
				if (in_widths.size() == in_adjacentPoints.size()
					&& in_heights.size() == in_adjacentPoints.size())
				{
					desc.params[0] = in_widths.at(i);
					desc.params[1] = in_heights.at(i);
				}
				else
				{
					desc.params[0] = in_widths.at(0);
					desc.params[1] = in_heights.at(0);
				}
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

	JointData CModelCreator::createArcJointData(
		const osg::Vec3d& in_geoPosition,
		const std::vector<osg::Vec3d>& in_adjacentPoints,
		const double& in_width,
		const double& in_height,
		const double& in_arcHeight)
	{
		JointData joint;
		if (in_geoPosition.length() > 10
			&& in_width > 0.01
			&& in_height > 0.01
			&& in_arcHeight > 0.01
			&& in_adjacentPoints.size() > 0)
		{
			joint.pos = in_geoPosition;

			SectionDesc desc;
			desc.type = SectionType::Arc;
			desc.params[0] = in_width;
			desc.params[1] = in_height;
			desc.params[2] = in_arcHeight;

			for (int i = 0; i < in_adjacentPoints.size(); ++i)
			{
				joint.adjacents.emplace_back(JointAdjacent());
				joint.adjacents.back().pos = in_adjacentPoints.at(i);
				joint.adjacents.back().sectionDesc = desc;
			}
		}
		return joint;
	}

	JointData CModelCreator::createLadderJointData(
		const osg::Vec3d& in_geoPosition,
		const std::vector<osg::Vec3d>& in_adjacentPoints,
		const double& in_upEdge,
		const double& in_downEdge,
		const double& in_height)
	{
		JointData joint;
		if (in_geoPosition.length() > 10
			&& in_upEdge > 0.01
			&& in_downEdge > 0.01
			&& in_height > 0.01
			&& in_adjacentPoints.size() > 0)
		{
			joint.pos = in_geoPosition;

			SectionDesc desc;
			desc.type = SectionType::Ladder;
			desc.params[0] = in_upEdge;
			desc.params[1] = in_downEdge;
			desc.params[2] = in_height;

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
				meshJoint.get() }, in_color, getImage(in_texturePath));
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
				meshSegment.get() }, in_color, getImage(in_texturePath));
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
