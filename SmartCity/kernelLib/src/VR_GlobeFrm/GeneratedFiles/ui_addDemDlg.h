/********************************************************************************
** Form generated from reading UI file 'addDemDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDDEMDLG_H
#define UI_ADDDEMDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_addDemFrm
{
public:
    QPushButton *cancleBtn;
    QPushButton *okBtn;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGroupBox *groupBox;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *demPathGdalBtn;
    QLineEdit *demNameGdalBtn;
    QPushButton *selectPathGdalBtn;
    QWidget *tab_2;
    QGroupBox *groupBox_2;
    QPushButton *selectPathTmsBtn;
    QLineEdit *demNameTmsBtn;
    QLineEdit *demPathTmsBtn;
    QLabel *label_4;
    QLabel *label_3;

    void setupUi(QWidget *addDemFrm)
    {
        if (addDemFrm->objectName().isEmpty())
            addDemFrm->setObjectName(QStringLiteral("addDemFrm"));
        addDemFrm->resize(692, 206);
        cancleBtn = new QPushButton(addDemFrm);
        cancleBtn->setObjectName(QStringLiteral("cancleBtn"));
        cancleBtn->setGeometry(QRect(390, 160, 75, 23));
        okBtn = new QPushButton(addDemFrm);
        okBtn->setObjectName(QStringLiteral("okBtn"));
        okBtn->setGeometry(QRect(233, 160, 75, 23));
        tabWidget = new QTabWidget(addDemFrm);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(20, 11, 661, 141));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(9, 5, 631, 101));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(13, 32, 61, 16));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(11, 74, 61, 16));
        demPathGdalBtn = new QLineEdit(groupBox);
        demPathGdalBtn->setObjectName(QStringLiteral("demPathGdalBtn"));
        demPathGdalBtn->setGeometry(QRect(79, 72, 461, 20));
        demNameGdalBtn = new QLineEdit(groupBox);
        demNameGdalBtn->setObjectName(QStringLiteral("demNameGdalBtn"));
        demNameGdalBtn->setGeometry(QRect(79, 30, 461, 20));
        selectPathGdalBtn = new QPushButton(groupBox);
        selectPathGdalBtn->setObjectName(QStringLiteral("selectPathGdalBtn"));
        selectPathGdalBtn->setGeometry(QRect(560, 70, 61, 23));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(9, 5, 631, 101));
        selectPathTmsBtn = new QPushButton(groupBox_2);
        selectPathTmsBtn->setObjectName(QStringLiteral("selectPathTmsBtn"));
        selectPathTmsBtn->setGeometry(QRect(563, 68, 51, 23));
        demNameTmsBtn = new QLineEdit(groupBox_2);
        demNameTmsBtn->setObjectName(QStringLiteral("demNameTmsBtn"));
        demNameTmsBtn->setGeometry(QRect(74, 24, 481, 20));
        demPathTmsBtn = new QLineEdit(groupBox_2);
        demPathTmsBtn->setObjectName(QStringLiteral("demPathTmsBtn"));
        demPathTmsBtn->setGeometry(QRect(74, 68, 481, 20));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(11, 26, 61, 16));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(9, 70, 61, 16));
        tabWidget->addTab(tab_2, QString());

        retranslateUi(addDemFrm);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(addDemFrm);
    } // setupUi

    void retranslateUi(QWidget *addDemFrm)
    {
        addDemFrm->setWindowTitle(QApplication::translate("addDemFrm", "\346\267\273\345\212\240\351\253\230\347\250\213", Q_NULLPTR));
        cancleBtn->setText(QApplication::translate("addDemFrm", "\345\217\226\346\266\210", Q_NULLPTR));
        okBtn->setText(QApplication::translate("addDemFrm", "\347\241\256\345\256\232", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("addDemFrm", "GDAL\351\251\261\345\212\250", Q_NULLPTR));
        label_5->setText(QApplication::translate("addDemFrm", "DEM\345\220\215\347\247\260\357\274\232", Q_NULLPTR));
        label_6->setText(QApplication::translate("addDemFrm", "\351\200\211\346\213\251\350\267\257\345\276\204\357\274\232", Q_NULLPTR));
        demPathGdalBtn->setText(QString());
        demNameGdalBtn->setText(QString());
        selectPathGdalBtn->setText(QApplication::translate("addDemFrm", "\351\200\211\346\213\251..", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("addDemFrm", "\345\215\225\345\271\205DEM", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("addDemFrm", "TMS\351\251\261\345\212\250", Q_NULLPTR));
        selectPathTmsBtn->setText(QApplication::translate("addDemFrm", "\351\200\211\346\213\251..", Q_NULLPTR));
        demNameTmsBtn->setText(QString());
        demPathTmsBtn->setText(QString());
        label_4->setText(QApplication::translate("addDemFrm", "\351\205\215\347\275\256\346\226\207\344\273\266\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("addDemFrm", "\351\200\211\346\213\251\350\267\257\345\276\204\357\274\232", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("addDemFrm", "DEM\345\210\207\347\211\207", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class addDemFrm: public Ui_addDemFrm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDDEMDLG_H
