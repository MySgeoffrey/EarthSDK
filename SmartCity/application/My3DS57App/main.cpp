#include "my3ds57app.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	My3DS57App w;
	w.show();
	return a.exec();
}
