#ifndef MY3DS57APP_H
#define MY3DS57APP_H

#include <QtGui/QMainWindow>
#include "ui_my3ds57app.h"
#include "localengine/SceneViewer.h"

class My3DS57App : public QMainWindow
{
	Q_OBJECT

public:
	My3DS57App(QWidget *parent = 0, Qt::WFlags flags = 0);
	
	~My3DS57App();

	void loadS57Chart();

	osg::ref_ptr<osg::Geode> createNode(osg::ref_ptr<osg::Vec3dArray> in_points,osg::Vec4 in_color);

private:
	Ui::My3DS57AppClass ui;
	Engine::CSceneViewer* mpScenViewer;
};

#endif // MY3DS57APP_H
