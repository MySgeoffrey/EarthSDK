#ifndef MYCLASS_H
#define MYCLASS_H

#include <QtWidgets/QMainWindow>
#include "ui_myclass.h"

#include "vr_globewidget.h"
#include "globemap/globemap.h"
#include <osgDB/FileUtils>
#include "modelbuilder/ModelBuilder.h"

class MyClass : public QMainWindow
{
	Q_OBJECT

public:
	MyClass(QWidget *parent = 0);
	~MyClass();

	private slots:
	void slotCirclePipe1();
	void slotCirclePipe2();
	void slotCirclePipe3();
	void slotCirclePipe4();
	void slotRectPipe1();
	void slotRectPipe2();
	void slotRectPipe3();
	void slotRectPipe4();
	void slotCirclePipeLine();
	void slotRectPipeLine();
	void slotArcPipe_2();
	void slotArcPipe_3_h();
	void slotArcPipe_4_h();
	void slotArcPipeLine();
	void slotLadderPipe_2();
	void slotLadderPipe_3_h();
	void slotLadderPipe_4_h();
	void slotLadderPipeLine();

	void init();
	void loadPipeData();
	MeshGenerator::JointData createCircleJoint_2_h();
	MeshGenerator::JointData createCircleJoint_3_h();
	MeshGenerator::JointData createCircleJoint_3_v();
	MeshGenerator::JointData createCircleJoint_4_h();

	MeshGenerator::JointData createRectJoint_2_h();
	MeshGenerator::JointData createRectJoint_3_h();
	MeshGenerator::JointData createRectJoint_3_v();
	MeshGenerator::JointData createRectJoint_4_h();

	MeshGenerator::JointData createArcJoint_2_h();
	MeshGenerator::JointData createArcJoint_3_h();
	MeshGenerator::JointData createArcJoint_4_h();
	
	MeshGenerator::JointData createLadderJoint_2_h();
	MeshGenerator::JointData createLadderJoint_3_h();
	MeshGenerator::JointData createLadderJoint_4_h();

	void createJointModelAndExport(const MeshGenerator::JointData& in_jointData,
		const osg::Vec4& in_color,const std::string& in_name,
		const std::string& in_texturePath);

	void createSegmentModelAndExport(const MeshGenerator::JointData& in_jointData1,
		const MeshGenerator::JointData& in_jointData2,
		const osg::Vec4& in_color, const std::string& in_name,
		const std::string& in_texturePath);

private:
	Ui::iXModeBuilderSDK ui;
	VRGlobeWidget* mpGlobeWidget;
	osg::ref_ptr<osgEarth::GeoTransform> mpGroup;
	osgEarth::GeoPoint mRefGeoCenter;
	std::string mLinkerTexturePath;
	std::string mPipeTexturePath;
};

#endif // MYCLASS_H
