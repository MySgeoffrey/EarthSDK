#include "TestCase.h"
#include "osgEarth\GeoTransform"
#include "..\Osg340\include\osg\Geode"
#include "..\Osg340\include\osg\ShapeDrawable"

namespace MeshGenerator
{
	osg::Node* TestCase::exec(const osgEarth::GeoPoint& geoPoint)
	{
		//GeoPoint geoPoint(getSRS(),lon, lat, alt, osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);

		auto geoTransform = new osgEarth::GeoTransform();
		geoTransform->setPosition(geoPoint);

		osg::Group* group = new osg::Group;
		geoTransform->addChild(group);

		// segment + joint
		{
			SectionDesc desc;
			desc.type = SectionType::Ladder;
			desc.params[0] = 4.1f;
			desc.params[1] = 5.1f;
			desc.params[2] = 2.1f;
			//desc.type = SectionType::Ladder;
			//desc.params[0] = 3.1f;
			//desc.params[1] = 8.1f;
			//desc.params[2] = 5.1f;
			//desc.params[3] = 50.f;

			SectionDesc desc2;
			desc2.type = SectionType::Ladder;
			desc2.params[0] = 4.1f;
			desc2.params[1] = 5.1f;
			desc2.params[2] = 2.1f;

			osg::Vec3d p1(116.000, 39.002, 15);
			osg::Vec3d p2(116.002, 39.002, 20);
			osg::Vec3d p3(116.001, 39.001, 24);
			osg::Vec3d p4(116.002, 39.000, 25);
			osg::Vec3d p5(116.001, 39.000, 27);

			JointData joint;
			joint.pos = p1;

			joint.adjacents.emplace_back(JointAdjacent());
			joint.adjacents.back().pos = p3;
			joint.adjacents.back().sectionDesc = desc;

			joint.adjacents.emplace_back(JointAdjacent());
			joint.adjacents.back().pos = p2;
			joint.adjacents.back().sectionDesc = desc;


			JointData joint2;
			joint2.pos = p2;

			joint2.adjacents.emplace_back(JointAdjacent());
			joint2.adjacents.back().pos = p3;
			joint2.adjacents.back().sectionDesc = desc;

			joint2.adjacents.emplace_back(JointAdjacent());
			joint2.adjacents.back().pos = p1;
			joint2.adjacents.back().sectionDesc = desc;


			JointData joint3;
			joint3.pos = p3;

			joint3.adjacents.emplace_back(JointAdjacent());
			joint3.adjacents.back().pos = p5;
			joint3.adjacents.back().sectionDesc = desc2;

			joint3.adjacents.emplace_back(JointAdjacent());
			joint3.adjacents.back().pos = p2;
			joint3.adjacents.back().sectionDesc = desc;

			joint3.adjacents.emplace_back(JointAdjacent());
			joint3.adjacents.back().pos = p1;
			joint3.adjacents.back().sectionDesc = desc;


			JointData joint4;
			joint4.pos = p5;

			joint4.adjacents.emplace_back(JointAdjacent());
			joint4.adjacents.back().pos = p3;
			joint4.adjacents.back().sectionDesc = desc2;


			MeshUtil::getInstance()->setReferenceCenter(geoPoint);
			MeshUtil::getInstance()->setTextureRepeatLength(15.f);

			std::shared_ptr<MeshData> meshJoint1(MeshUtil::getInstance()->createJointLonLat(joint));
			std::shared_ptr<MeshData> meshJoint2(MeshUtil::getInstance()->createJointLonLat(joint2));
			std::shared_ptr<MeshData> meshJoint3(MeshUtil::getInstance()->createJointLonLat(joint3));

			std::shared_ptr<MeshData> meshSegment1_2(MeshUtil::getInstance()->createPipeSegmentLonLat(joint,joint2));
			std::shared_ptr<MeshData> meshSegment1_3(MeshUtil::getInstance()->createPipeSegmentLonLat(joint, joint3));
			std::shared_ptr<MeshData> meshSegment2_3(MeshUtil::getInstance()->createPipeSegmentLonLat(joint3, joint2));
			std::shared_ptr<MeshData> meshSegment5_3(MeshUtil::getInstance()->createPipeSegmentLonLat(joint4, joint3));

			auto geodeJoint = MeshUtil::getInstance()->createGeodeFromMeshData({ meshJoint1.get(), meshJoint2.get(), meshJoint3.get()},
				osg::Vec4(0.0, 0.0, 1.0, 1.0), "D:/blue.bmp");

			auto geodeSegment = MeshUtil::getInstance()->createGeodeFromMeshData({ meshSegment1_2.get(), meshSegment1_3.get(), 
				meshSegment2_3.get(), meshSegment5_3.get() },
				osg::Vec4(1.0, 0.0, 0.0, 1.0), "D:/red.bmp");

			group->addChild(geodeJoint);
			group->addChild(geodeSegment);
		}

		return geoTransform;
	}
}