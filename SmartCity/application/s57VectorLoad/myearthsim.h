#ifndef MYEARTHSIM_H
#define MYEARTHSIM_H

#include <QtGui/QMainWindow>
#include <QMenu>
#include "QtGui/qdialog.h"
#include "GeneratedFiles/ui_myearthsim.h"
#include "vr_globewidget.h"

class MyChartMapControl;
class myEarthSim : public QMainWindow
{
	Q_OBJECT

public:
	myEarthSim(QWidget *parent = 0, Qt::WFlags flags = 0);

	~myEarthSim();

	
	public slots:

		void initialFireEngineScene();

		//��������ʾ����ؼ�
		void slotShowEarthControl();

		void slotShowMapControl();
	
		//������ͼӰ��
		void slotActionAddTDTImage();
		
		void slotLoadS57();
private:
	Ui::myEarthSimClass ui;
	VRGlobeWidget* mpGlobeWidget;
	MyChartMapControl* mpMapWidget;
};


#endif // MYEARTHSIM_H
