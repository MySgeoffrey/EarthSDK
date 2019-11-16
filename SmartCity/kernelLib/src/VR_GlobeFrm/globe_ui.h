#ifndef GLOBE_UI_H
#define GLOBE_UI_H

#include "globe_ui_config.h"

#include <QtWidgets/QDialog>
#include "osgEarth/ImageLayer"

namespace Ui {
	class ImageTransparentUI;
	class ModelTransformUI;
} // namespace Ui

class GLOBE_UI_EXPORT ImageTransparentDlg : public QDialog
{
	Q_OBJECT

public:
	ImageTransparentDlg(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());

	~ImageTransparentDlg();

	void setLayer(osgEarth::ImageLayer* WebImageLayer);

	private slots:
	void slotTransparentValueChanged(int);
public:
	osgEarth::ImageLayer* WebImageLayer;
	Ui::ImageTransparentUI* ui;
};


class GLOBE_UI_EXPORT ModelTransformDlg : public QDialog
{
	Q_OBJECT

public:
	ModelTransformDlg(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());

	~ModelTransformDlg();

	public slots:
		void accept();
		void slotSelectModelDestPath();
		void slotSelectModelSrcPath();
	
public:
	Ui::ModelTransformUI* ui;
};
#endif // GLOBE_UI_H
