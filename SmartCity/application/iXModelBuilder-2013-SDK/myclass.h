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

private:
	Ui::iXModeBuilderSDK ui;
	VRGlobeWidget* mpGlobeWidget;
	osg::ref_ptr<osgEarth::GeoTransform> mpGroup;
	osgEarth::GeoPoint mRefGeoCenter;
	std::string mLinkerTexturePath;
	std::string mPipeTexturePath;
};

#endif // MYCLASS_H
