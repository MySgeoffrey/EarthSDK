#include "frmmain.h"
#include <QApplication>
#include "myhelper.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    myHelper::SetUTF8Code();
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
    //myHelper::SetStyle("black");//黑色风格
    //myHelper::SetStyle("gray");//灰色风格
    myHelper::SetStyle("navy");//天蓝色风格
   /* myHelper::SetChinese();*/
	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    frmMain w;
    w.show();

    return a.exec();
}
