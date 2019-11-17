#ifndef MYCHARTTILETOOL_H
#define MYCHARTTILETOOL_H

#include "mycharttiletool/mycharttiletool_global.h"
#include <QtWidgets/QDialog>

namespace Ui
{
	class charttileUI;
}

namespace MyChart
{
	class MYCHARTTILETOOL_EXPORT CMychartTileToolDlg 
		: public QDialog
	{
		Q_OBJECT

	public:
		CMychartTileToolDlg(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());

		~CMychartTileToolDlg();

	private:

		Ui::charttileUI* ui;
	};
}
#endif // MYCHARTTILETOOL_H
