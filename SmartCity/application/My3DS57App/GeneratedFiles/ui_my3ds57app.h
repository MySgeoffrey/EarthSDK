/********************************************************************************
** Form generated from reading UI file 'my3ds57app.ui'
**
** Created: Wed Jun 26 18:13:22 2019
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MY3DS57APP_H
#define UI_MY3DS57APP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_My3DS57AppClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *My3DS57AppClass)
    {
        if (My3DS57AppClass->objectName().isEmpty())
            My3DS57AppClass->setObjectName(QString::fromUtf8("My3DS57AppClass"));
        My3DS57AppClass->resize(733, 536);
        centralWidget = new QWidget(My3DS57AppClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

        horizontalLayout->addLayout(gridLayout);

        My3DS57AppClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(My3DS57AppClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 733, 23));
        My3DS57AppClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(My3DS57AppClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        My3DS57AppClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(My3DS57AppClass);

        QMetaObject::connectSlotsByName(My3DS57AppClass);
    } // setupUi

    void retranslateUi(QMainWindow *My3DS57AppClass)
    {
        My3DS57AppClass->setWindowTitle(QApplication::translate("My3DS57AppClass", "\350\232\202\350\232\201\346\265\267\345\233\2763D\346\265\217\350\247\210\345\231\250", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class My3DS57AppClass: public Ui_My3DS57AppClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MY3DS57APP_H
