#ifndef ADDDOMDLG_H
#define ADDDOMDLG_H


#include <QtWidgets/QDialog>
#include "globe_ui_config.h"
#include "vr_globewidget.h"

namespace Ui
{
	class addDomFrm;
}
namespace Globe
{
	class GLOBE_UI_EXPORT CAddDom : public QDialog
	{
		Q_OBJECT

	public:
		CAddDom(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
		CAddDom(VRGlobeWidget* globeWidget, osgEarth::ImageLayer*& pRefImageLayer);
		~CAddDom();

		static osgEarth::ImageLayer* getImageLayer();

		static void setNullImageLayer();

	private:
		Ui::addDomFrm* ui;
		VRGlobeWidget* mGlobeWidget;
		QString mFilePathGdal;
		QString mFileNameGdal;
		QString mFilePathTms;
		QString mFileNameTms;
	signals:
		void signalAddLocalImage(osgEarth::ImageLayer* pImageLayer);

		private slots:
			void slotOpenFileGdalDlg();
			void slotOpenFileTmsDlg();
			void slotOkBtn();
			void slotCancleBtn();
	private:
		static osgEarth::ImageLayer* s_pImageLayer;
	};
}




#endif // ADDDOMDLG_H
