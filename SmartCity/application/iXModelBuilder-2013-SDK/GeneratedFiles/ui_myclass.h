/********************************************************************************
** Form generated from reading UI file 'myclass.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYCLASS_H
#define UI_MYCLASS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_iXModeBuilderSDK
{
public:
    QAction *action;
    QAction *action_2;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *circlePipe_1;
    QPushButton *rectPipe_1;
    QPushButton *circlePipe_2;
    QPushButton *circlePipe_4;
    QPushButton *circlePipe_3;
    QPushButton *rectPipe_2;
    QPushButton *rectPipe_3;
    QPushButton *rectPipe_4;
    QPushButton *rectPipeLine;
    QPushButton *circlePipeLine;
    QWidget *tab_2;
    QPushButton *rectPipe_5;
    QPushButton *rectPipe_6;
    QPushButton *rectPipe_7;
    QPushButton *circlePipe_5;
    QPushButton *rectPipe_8;
    QPushButton *circlePipe_6;
    QPushButton *circlePipe_7;
    QPushButton *circlePipe_8;
    QGridLayout *gridLayout;

    void setupUi(QMainWindow *iXModeBuilderSDK)
    {
        if (iXModeBuilderSDK->objectName().isEmpty())
            iXModeBuilderSDK->setObjectName(QStringLiteral("iXModeBuilderSDK"));
        iXModeBuilderSDK->resize(770, 386);
        action = new QAction(iXModeBuilderSDK);
        action->setObjectName(QStringLiteral("action"));
        action_2 = new QAction(iXModeBuilderSDK);
        action_2->setObjectName(QStringLiteral("action_2"));
        centralWidget = new QWidget(iXModeBuilderSDK);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setMinimumSize(QSize(0, 150));
        tabWidget->setMaximumSize(QSize(16777215, 150));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        circlePipe_1 = new QPushButton(tab);
        circlePipe_1->setObjectName(QStringLiteral("circlePipe_1"));
        circlePipe_1->setGeometry(QRect(10, 10, 211, 23));
        rectPipe_1 = new QPushButton(tab);
        rectPipe_1->setObjectName(QStringLiteral("rectPipe_1"));
        rectPipe_1->setGeometry(QRect(250, 10, 211, 23));
        circlePipe_2 = new QPushButton(tab);
        circlePipe_2->setObjectName(QStringLiteral("circlePipe_2"));
        circlePipe_2->setGeometry(QRect(10, 40, 211, 23));
        circlePipe_4 = new QPushButton(tab);
        circlePipe_4->setObjectName(QStringLiteral("circlePipe_4"));
        circlePipe_4->setGeometry(QRect(10, 100, 211, 23));
        circlePipe_3 = new QPushButton(tab);
        circlePipe_3->setObjectName(QStringLiteral("circlePipe_3"));
        circlePipe_3->setGeometry(QRect(10, 70, 211, 23));
        rectPipe_2 = new QPushButton(tab);
        rectPipe_2->setObjectName(QStringLiteral("rectPipe_2"));
        rectPipe_2->setGeometry(QRect(250, 40, 211, 23));
        rectPipe_3 = new QPushButton(tab);
        rectPipe_3->setObjectName(QStringLiteral("rectPipe_3"));
        rectPipe_3->setGeometry(QRect(250, 70, 211, 23));
        rectPipe_4 = new QPushButton(tab);
        rectPipe_4->setObjectName(QStringLiteral("rectPipe_4"));
        rectPipe_4->setGeometry(QRect(250, 100, 211, 23));
        rectPipeLine = new QPushButton(tab);
        rectPipeLine->setObjectName(QStringLiteral("rectPipeLine"));
        rectPipeLine->setGeometry(QRect(490, 40, 181, 23));
        circlePipeLine = new QPushButton(tab);
        circlePipeLine->setObjectName(QStringLiteral("circlePipeLine"));
        circlePipeLine->setGeometry(QRect(490, 10, 181, 23));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        rectPipe_5 = new QPushButton(tab_2);
        rectPipe_5->setObjectName(QStringLiteral("rectPipe_5"));
        rectPipe_5->setGeometry(QRect(240, 70, 211, 23));
        rectPipe_6 = new QPushButton(tab_2);
        rectPipe_6->setObjectName(QStringLiteral("rectPipe_6"));
        rectPipe_6->setGeometry(QRect(240, 100, 211, 23));
        rectPipe_7 = new QPushButton(tab_2);
        rectPipe_7->setObjectName(QStringLiteral("rectPipe_7"));
        rectPipe_7->setGeometry(QRect(240, 40, 211, 23));
        circlePipe_5 = new QPushButton(tab_2);
        circlePipe_5->setObjectName(QStringLiteral("circlePipe_5"));
        circlePipe_5->setGeometry(QRect(10, 100, 211, 23));
        rectPipe_8 = new QPushButton(tab_2);
        rectPipe_8->setObjectName(QStringLiteral("rectPipe_8"));
        rectPipe_8->setGeometry(QRect(240, 10, 211, 23));
        circlePipe_6 = new QPushButton(tab_2);
        circlePipe_6->setObjectName(QStringLiteral("circlePipe_6"));
        circlePipe_6->setGeometry(QRect(10, 10, 211, 23));
        circlePipe_7 = new QPushButton(tab_2);
        circlePipe_7->setObjectName(QStringLiteral("circlePipe_7"));
        circlePipe_7->setGeometry(QRect(10, 70, 211, 23));
        circlePipe_8 = new QPushButton(tab_2);
        circlePipe_8->setObjectName(QStringLiteral("circlePipe_8"));
        circlePipe_8->setGeometry(QRect(10, 40, 211, 23));
        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));

        verticalLayout->addLayout(gridLayout);


        horizontalLayout->addLayout(verticalLayout);

        iXModeBuilderSDK->setCentralWidget(centralWidget);

        retranslateUi(iXModeBuilderSDK);
        QObject::connect(circlePipe_1, SIGNAL(clicked()), iXModeBuilderSDK, SLOT(slotCirclePipe1()));
        QObject::connect(circlePipe_2, SIGNAL(clicked()), iXModeBuilderSDK, SLOT(slotCirclePipe2()));
        QObject::connect(circlePipe_3, SIGNAL(clicked()), iXModeBuilderSDK, SLOT(slotCirclePipe3()));
        QObject::connect(circlePipe_4, SIGNAL(clicked()), iXModeBuilderSDK, SLOT(slotCirclePipe4()));
        QObject::connect(rectPipe_1, SIGNAL(clicked()), iXModeBuilderSDK, SLOT(slotRectPipe1()));
        QObject::connect(rectPipe_2, SIGNAL(clicked()), iXModeBuilderSDK, SLOT(slotRectPipe2()));
        QObject::connect(rectPipe_3, SIGNAL(clicked()), iXModeBuilderSDK, SLOT(slotRectPipe3()));
        QObject::connect(rectPipe_4, SIGNAL(clicked()), iXModeBuilderSDK, SLOT(slotRectPipe4()));
        QObject::connect(circlePipeLine, SIGNAL(clicked()), iXModeBuilderSDK, SLOT(slotCirclePipeLine()));
        QObject::connect(rectPipeLine, SIGNAL(clicked()), iXModeBuilderSDK, SLOT(slotRectPipeLine()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(iXModeBuilderSDK);
    } // setupUi

    void retranslateUi(QMainWindow *iXModeBuilderSDK)
    {
        iXModeBuilderSDK->setWindowTitle(QApplication::translate("iXModeBuilderSDK", "iXModelBuilder-2013-SDK", Q_NULLPTR));
        action->setText(QApplication::translate("iXModeBuilderSDK", "\345\234\206\345\275\242\346\210\252\351\235\242\347\256\241\351\201\223\345\273\272\346\250\241", Q_NULLPTR));
        action_2->setText(QApplication::translate("iXModeBuilderSDK", "\346\226\271\345\275\242\346\210\252\351\235\242\347\256\241\351\201\223\345\273\272\346\250\241", Q_NULLPTR));
        circlePipe_1->setText(QApplication::translate("iXModeBuilderSDK", "\345\234\206\345\275\242\346\210\252\351\235\242\357\274\232 \344\270\244\351\200\232\345\274\257\345\244\264\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        rectPipe_1->setText(QApplication::translate("iXModeBuilderSDK", "\346\226\271\345\275\242\346\210\252\351\235\242\357\274\232\344\270\244\351\200\232\345\274\257\345\244\264\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        circlePipe_2->setText(QApplication::translate("iXModeBuilderSDK", "\345\234\206\345\275\242\346\210\252\351\235\242\357\274\232 \346\260\264\345\271\263\344\270\211\351\200\232\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        circlePipe_4->setText(QApplication::translate("iXModeBuilderSDK", "\345\234\206\345\275\242\346\210\252\351\235\242\357\274\232 \345\233\233\351\200\232\346\216\245\345\244\264\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        circlePipe_3->setText(QApplication::translate("iXModeBuilderSDK", "\345\234\206\345\275\242\346\210\252\351\235\242\357\274\232 \347\253\226\347\233\264\344\270\211\351\200\232\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        rectPipe_2->setText(QApplication::translate("iXModeBuilderSDK", "\346\226\271\345\275\242\346\210\252\351\235\242\357\274\232\346\260\264\345\271\263\344\270\211\351\200\232\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        rectPipe_3->setText(QApplication::translate("iXModeBuilderSDK", "\346\226\271\345\275\242\346\210\252\351\235\242\357\274\232\347\253\226\347\233\264\344\270\211\351\200\232\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        rectPipe_4->setText(QApplication::translate("iXModeBuilderSDK", "\346\226\271\345\275\242\346\210\252\351\235\242\357\274\232\345\233\233\351\200\232\346\216\245\345\244\264\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        rectPipeLine->setText(QApplication::translate("iXModeBuilderSDK", "\346\226\271\345\275\242\346\210\252\351\235\242\357\274\232\347\256\241\346\256\265\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        circlePipeLine->setText(QApplication::translate("iXModeBuilderSDK", "\345\234\206\345\275\242\346\210\252\351\235\242\357\274\232\347\256\241\346\256\265\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("iXModeBuilderSDK", "\347\256\241\351\201\223\346\216\245\345\244\264\345\217\202\346\225\260\345\214\226", Q_NULLPTR));
        rectPipe_5->setText(QApplication::translate("iXModeBuilderSDK", "\346\242\257\345\275\242\346\210\252\351\235\242\357\274\232\346\260\264\345\271\263\345\233\233\351\200\232\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        rectPipe_6->setText(QApplication::translate("iXModeBuilderSDK", "\346\242\257\345\275\242\346\210\252\351\235\242\357\274\232\345\233\233\351\200\232\346\216\245\345\244\264\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        rectPipe_7->setText(QApplication::translate("iXModeBuilderSDK", "\346\242\257\345\275\242\346\210\252\351\235\242\357\274\232\346\260\264\345\271\263\344\270\211\351\200\232\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        circlePipe_5->setText(QApplication::translate("iXModeBuilderSDK", "\346\213\261\345\275\242\346\210\252\351\235\242\357\274\232 \345\233\233\351\200\232\346\216\245\345\244\264\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        rectPipe_8->setText(QApplication::translate("iXModeBuilderSDK", "\346\242\257\345\275\242\346\210\252\351\235\242\357\274\232\344\270\244\351\200\232\345\274\257\345\244\264\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        circlePipe_6->setText(QApplication::translate("iXModeBuilderSDK", "\346\213\261\345\275\242\346\210\252\351\235\242\357\274\232 \344\270\244\351\200\232\345\274\257\345\244\264\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        circlePipe_7->setText(QApplication::translate("iXModeBuilderSDK", "\346\213\261\345\275\242\346\210\252\351\235\242\357\274\232 \346\260\264\345\271\263\345\233\233\351\200\232\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        circlePipe_8->setText(QApplication::translate("iXModeBuilderSDK", "\346\213\261\345\275\242\346\210\252\351\235\242\357\274\232 \346\260\264\345\271\263\344\270\211\351\200\232\345\273\272\346\250\241\344\270\216\350\276\223\345\207\272", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("iXModeBuilderSDK", "\351\200\232\351\201\223\347\256\241\345\273\212\345\217\202\346\225\260\345\214\226", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class iXModeBuilderSDK: public Ui_iXModeBuilderSDK {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYCLASS_H
