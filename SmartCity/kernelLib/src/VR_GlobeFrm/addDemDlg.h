#ifndef ADDDEMDLG_H
#define ADDDEMDLG_H


#include <QtWidgets/QDialog>
#include "globe_ui_config.h"
#include "vr_globewidget.h"

namespace Ui
{
	class addDemFrm;
}

namespace Globe
{
	class GLOBE_UI_EXPORT CAddDem : public QDialog
	{
		Q_OBJECT

	public:
		CAddDem(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
		CAddDem(VRGlobeWidget* globeWidget, osgEarth::ElevationLayer*& pRefDemLayer);
		~CAddDem();

		static osgEarth::ElevationLayer* getDemLayer();

		static void setNullDemLayer();
	private:
		Ui::addDemFrm* ui;
		VRGlobeWidget* mGlobeWidget;
		QString mFilePathGdal;
		QString mFileNameGdal;
		QString mFilePathTms;
		QString mFileNameTms;


		private slots:
			void slotOpenFileGdalDlg();
			void slotOpenFileTmsDlg();
			void slotOkBtn();
			void slotCancleBtn();
	private:
			static osgEarth::ElevationLayer* s_pDemLayer;
	};
}




#endif // ADDDEMDLG_H
