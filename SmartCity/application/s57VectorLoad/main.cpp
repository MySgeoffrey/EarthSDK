#include "myearthsim.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	myEarthSim w;
	w.show();
	return a.exec();
}
