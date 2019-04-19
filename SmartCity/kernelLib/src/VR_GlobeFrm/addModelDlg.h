#ifndef ADDMODELDLG_H
#define ADDMODELDLG_H


#include <QtGui/QDialog>
#include "globe_ui_config.h"
#include "vr_globewidget.h"
#include <osgEarthUtil/ObjectLocator>

namespace Ui
{
	class addModelFrm;
}
namespace Globe
{
	

	class GLOBE_UI_EXPORT CAddModel : public QDialog
	{
		Q_OBJECT

	public:
		CAddModel(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
		CAddModel(VRGlobeWidget* globeWidget,osgEarth::Util::ObjectLocatorNode*& pRefNodeLocator);
		~CAddModel();
	private:
		Ui::addModelFrm* ui;
		VRGlobeWidget* mGlobeWidget;
		QString mModelPath;
		QString mModelName;
		float mModelScaleX;
		float mModelScaleY;
		float mModelScaleZ;
		float mModelLong;
		float mModelLati;
		float mModelAlti;
		float mModelRotateX;
		float mModelRotateY;
		float mModelRotateZ;

		private slots:
			void slotOpenFile();
			void slotOkBtn();
			void slotCancleBtn();

	private:
		osgEarth::Util::ObjectLocatorNode* mpRefNodeLocator;
	};
}




#endif // ADDMODELDLG_H
