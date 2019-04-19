#ifndef MYEARTHSIM_H
#define MYEARTHSIM_H

#include <QtGui/QMainWindow>
#include <QMenu>
#include "QtGui/qdialog.h"
#include "GeneratedFiles/ui_myearthsim.h"
#include "vr_globewidget.h"


class myEarthSim : public QMainWindow
{
	Q_OBJECT

public:
	myEarthSim(QWidget *parent = 0, Qt::WFlags flags = 0);

	~myEarthSim();

	
	public slots:

		void initialFireEngineScene();

		//创建并显示地球控件
		void slotShowEarthControl();
	
		//添加天地图影像
		void slotActionAddTDTImage();
		
		void slotLoadS57();
private:
	Ui::myEarthSimClass ui;
	VRGlobeWidget* mpGlobeWidget;

};


#endif // MYEARTHSIM_H
