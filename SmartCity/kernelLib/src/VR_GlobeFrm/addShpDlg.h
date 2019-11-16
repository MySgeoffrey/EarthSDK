#ifndef ADDSHPDLG_H
#define ADDSHPDLG_H


#include <QtWidgets/QDialog>
#include "globe_ui_config.h"
#include "vr_globewidget.h"
#include <osgEarth/ModelLayer>

namespace Ui
{
	class addShpFrm;
}
namespace Globe
{
	class GLOBE_UI_EXPORT CAddShp : public QDialog
	{
		Q_OBJECT

	public:
		CAddShp(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
		CAddShp(VRGlobeWidget* globeWidget);
		~CAddShp();

	signals:
		void createVectorLayer(osgEarth::ImageLayer* pVectorLayer);

	private:
		Ui::addShpFrm* ui;
		VRGlobeWidget* mGlobeWidget;
		QString mFilepath;
		QString mFileName;

		private slots:
			void slotOpenFileDlg();
			void slotOkBtn();
			void slotCancleBtn();
	};
}




#endif // ADDSHPDLG_H
