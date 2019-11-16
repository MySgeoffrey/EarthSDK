/********************************************************************************
** Form generated from reading UI file 'addDomDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDDOMDLG_H
#define UI_ADDDOMDLG_H

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

class Ui_addDomFrm
{
public:
    QPushButton *cancleBtn;
    QPushButton *okBtn;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGroupBox *groupBox;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *domPathGdalBtn;
    QLineEdit *domNameGdalBtn;
    QPushButton *selectPathGdalBtn;
    QWidget *tab_2;
    QGroupBox *groupBox_2;
    QPushButton *selectPathTmsBtn;
    QLineEdit *domNameTmsBtn;
    QLineEdit *domPathTmsBtn;
    QLabel *label_4;
    QLabel *label_3;

    void setupUi(QWidget *addDomFrm)
    {
        if (addDomFrm->objectName().isEmpty())
            addDomFrm->setObjectName(QStringLiteral("addDomFrm"));
        addDomFrm->resize(691, 207);
        cancleBtn = new QPushButton(addDomFrm);
        cancleBtn->setObjectName(QStringLiteral("cancleBtn"));
        cancleBtn->setGeometry(QRect(360, 160, 75, 23));
        okBtn = new QPushButton(addDomFrm);
        okBtn->setObjectName(QStringLiteral("okBtn"));
        okBtn->setGeometry(QRect(200, 159, 75, 23));
        tabWidget = new QTabWidget(addDomFrm);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(21, 16, 651, 131));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(9, 5, 621, 91));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(13, 26, 61, 16));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(11, 62, 61, 16));
        domPathGdalBtn = new QLineEdit(groupBox);
        domPathGdalBtn->setObjectName(QStringLiteral("domPathGdalBtn"));
        domPathGdalBtn->setGeometry(QRect(79, 60, 471, 20));
        domNameGdalBtn = new QLineEdit(groupBox);
        domNameGdalBtn->setObjectName(QStringLiteral("domNameGdalBtn"));
        domNameGdalBtn->setGeometry(QRect(79, 24, 471, 20));
        selectPathGdalBtn = new QPushButton(groupBox);
        selectPathGdalBtn->setObjectName(QStringLiteral("selectPathGdalBtn"));
        selectPathGdalBtn->setGeometry(QRect(560, 60, 51, 23));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(9, 5, 621, 91));
        selectPathTmsBtn = new QPushButton(groupBox_2);
        selectPathTmsBtn->setObjectName(QStringLiteral("selectPathTmsBtn"));
        selectPathTmsBtn->setGeometry(QRect(550, 60, 51, 23));
        domNameTmsBtn = new QLineEdit(groupBox_2);
        domNameTmsBtn->setObjectName(QStringLiteral("domNameTmsBtn"));
        domNameTmsBtn->setGeometry(QRect(74, 24, 461, 20));
        domPathTmsBtn = new QLineEdit(groupBox_2);
        domPathTmsBtn->setObjectName(QStringLiteral("domPathTmsBtn"));
        domPathTmsBtn->setGeometry(QRect(74, 60, 461, 20));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(11, 26, 61, 16));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(9, 62, 61, 16));
        tabWidget->addTab(tab_2, QString());

        retranslateUi(addDomFrm);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(addDomFrm);
    } // setupUi

    void retranslateUi(QWidget *addDomFrm)
    {
        addDomFrm->setWindowTitle(QApplication::translate("addDomFrm", "\346\267\273\345\212\240\345\275\261\345\203\217", Q_NULLPTR));
        cancleBtn->setText(QApplication::translate("addDomFrm", "\345\217\226\346\266\210", Q_NULLPTR));
        okBtn->setText(QApplication::translate("addDomFrm", "\347\241\256\345\256\232", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("addDomFrm", "GDAL\351\251\261\345\212\250", Q_NULLPTR));
        label->setText(QApplication::translate("addDomFrm", "\345\275\261\345\203\217\345\220\215\347\247\260\357\274\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("addDomFrm", "\351\200\211\346\213\251\350\267\257\345\276\204\357\274\232", Q_NULLPTR));
        domPathGdalBtn->setText(QString());
        domNameGdalBtn->setText(QString());
        selectPathGdalBtn->setText(QApplication::translate("addDomFrm", "\351\200\211\346\213\251..", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("addDomFrm", "\345\215\225\345\271\205\345\275\261\345\203\217", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("addDomFrm", "TMS\351\251\261\345\212\250", Q_NULLPTR));
        selectPathTmsBtn->setText(QApplication::translate("addDomFrm", "\351\200\211\346\213\251..", Q_NULLPTR));
        domNameTmsBtn->setText(QString());
        domPathTmsBtn->setText(QString());
        label_4->setText(QApplication::translate("addDomFrm", "\351\205\215\347\275\256\346\226\207\344\273\266\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("addDomFrm", "\351\200\211\346\213\251\350\267\257\345\276\204\357\274\232", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("addDomFrm", "\345\275\261\345\203\217\345\210\207\345\233\276", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class addDomFrm: public Ui_addDomFrm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDDOMDLG_H
