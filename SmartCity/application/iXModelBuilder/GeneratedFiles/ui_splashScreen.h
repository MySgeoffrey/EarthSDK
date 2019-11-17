/********************************************************************************
** Form generated from reading UI file 'splashScreen.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPLASHSCREEN_H
#define UI_SPLASHSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_splashScreen
{
public:
    QWidget *widget;
    QLabel *systemLoadInfo;

    void setupUi(QWidget *splashScreen)
    {
        if (splashScreen->objectName().isEmpty())
            splashScreen->setObjectName(QStringLiteral("splashScreen"));
        splashScreen->resize(490, 249);
        splashScreen->setStyleSheet(QStringLiteral(""));
        widget = new QWidget(splashScreen);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(0, 0, 491, 251));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setStyleSheet(QStringLiteral("border-image: url(:/image/iXModelBuilder.png);"));
        systemLoadInfo = new QLabel(splashScreen);
        systemLoadInfo->setObjectName(QStringLiteral("systemLoadInfo"));
        systemLoadInfo->setGeometry(QRect(60, 220, 431, 20));
        systemLoadInfo->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"font: 75 9pt \"Arial\";"));

        retranslateUi(splashScreen);

        QMetaObject::connectSlotsByName(splashScreen);
    } // setupUi

    void retranslateUi(QWidget *splashScreen)
    {
        splashScreen->setWindowTitle(QApplication::translate("splashScreen", "Form", Q_NULLPTR));
        systemLoadInfo->setText(QApplication::translate("splashScreen", "\346\255\243\345\234\250\345\212\240\350\275\275\345\272\224\347\224\250\347\250\213\345\272\217\350\265\204\346\272\220...", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class splashScreen: public Ui_splashScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPLASHSCREEN_H
