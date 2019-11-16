/********************************************************************************
** Form generated from reading UI file 'addModelDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDMODELDLG_H
#define UI_ADDMODELDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_addModelFrm
{
public:
    QPushButton *cancleBtn;
    QLineEdit *modelName;
    QLabel *label;
    QPushButton *okBtn;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QGroupBox *groupBox;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QDoubleSpinBox *setModelLong;
    QDoubleSpinBox *setModelLati;
    QDoubleSpinBox *setModelAlti;
    QLabel *label_2;
    QPushButton *selectModelPathBtn;
    QLineEdit *modelPath;
    QWidget *tab;
    QGroupBox *groupBox_2;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QDoubleSpinBox *setModelScaleX;
    QDoubleSpinBox *setModelScaleY;
    QDoubleSpinBox *setModelScaleZ;
    QWidget *tab_3;
    QGroupBox *groupBox_3;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QDoubleSpinBox *setModelRouteX;
    QDoubleSpinBox *setModelRouteY;
    QDoubleSpinBox *setModelRouteZ;

    void setupUi(QWidget *addModelFrm)
    {
        if (addModelFrm->objectName().isEmpty())
            addModelFrm->setObjectName(QStringLiteral("addModelFrm"));
        addModelFrm->resize(303, 315);
        cancleBtn = new QPushButton(addModelFrm);
        cancleBtn->setObjectName(QStringLiteral("cancleBtn"));
        cancleBtn->setGeometry(QRect(172, 280, 75, 23));
        modelName = new QLineEdit(addModelFrm);
        modelName->setObjectName(QStringLiteral("modelName"));
        modelName->setGeometry(QRect(96, 17, 151, 20));
        label = new QLabel(addModelFrm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 20, 61, 16));
        okBtn = new QPushButton(addModelFrm);
        okBtn->setObjectName(QStringLiteral("okBtn"));
        okBtn->setGeometry(QRect(57, 281, 75, 23));
        tabWidget = new QTabWidget(addModelFrm);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(16, 54, 276, 211));
        tabWidget->setTabShape(QTabWidget::Triangular);
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 70, 251, 111));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(14, 20, 54, 12));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(14, 47, 54, 12));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(14, 75, 54, 12));
        setModelLong = new QDoubleSpinBox(groupBox);
        setModelLong->setObjectName(QStringLiteral("setModelLong"));
        setModelLong->setGeometry(QRect(54, 15, 175, 22));
        setModelLong->setDecimals(8);
        setModelLong->setMinimum(-180);
        setModelLong->setMaximum(180);
        setModelLong->setSingleStep(0.1);
        setModelLong->setValue(116);
        setModelLati = new QDoubleSpinBox(groupBox);
        setModelLati->setObjectName(QStringLiteral("setModelLati"));
        setModelLati->setGeometry(QRect(54, 44, 175, 22));
        setModelLati->setDecimals(8);
        setModelLati->setMinimum(-90);
        setModelLati->setMaximum(90);
        setModelLati->setSingleStep(0.1);
        setModelLati->setValue(39);
        setModelAlti = new QDoubleSpinBox(groupBox);
        setModelAlti->setObjectName(QStringLiteral("setModelAlti"));
        setModelAlti->setGeometry(QRect(55, 72, 174, 22));
        setModelAlti->setDecimals(8);
        label_2 = new QLabel(tab_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 32, 61, 16));
        selectModelPathBtn = new QPushButton(tab_2);
        selectModelPathBtn->setObjectName(QStringLiteral("selectModelPathBtn"));
        selectModelPathBtn->setGeometry(QRect(231, 28, 24, 23));
        modelPath = new QLineEdit(tab_2);
        modelPath->setObjectName(QStringLiteral("modelPath"));
        modelPath->setGeometry(QRect(70, 30, 155, 20));
        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 20, 231, 131));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(30, 35, 54, 12));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(30, 62, 54, 12));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(30, 90, 54, 12));
        setModelScaleX = new QDoubleSpinBox(groupBox_2);
        setModelScaleX->setObjectName(QStringLiteral("setModelScaleX"));
        setModelScaleX->setGeometry(QRect(70, 30, 120, 22));
        setModelScaleX->setDecimals(1);
        setModelScaleX->setMaximum(100000);
        setModelScaleX->setSingleStep(0.1);
        setModelScaleX->setValue(1);
        setModelScaleY = new QDoubleSpinBox(groupBox_2);
        setModelScaleY->setObjectName(QStringLiteral("setModelScaleY"));
        setModelScaleY->setGeometry(QRect(70, 59, 120, 22));
        setModelScaleY->setDecimals(1);
        setModelScaleY->setMaximum(100000);
        setModelScaleY->setSingleStep(0.1);
        setModelScaleY->setValue(1);
        setModelScaleZ = new QDoubleSpinBox(groupBox_2);
        setModelScaleZ->setObjectName(QStringLiteral("setModelScaleZ"));
        setModelScaleZ->setGeometry(QRect(71, 87, 120, 22));
        setModelScaleZ->setDecimals(1);
        setModelScaleZ->setMaximum(100000);
        setModelScaleZ->setSingleStep(0.1);
        setModelScaleZ->setValue(1);
        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        groupBox_3 = new QGroupBox(tab_3);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 19, 248, 151));
        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(9, 38, 54, 12));
        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(9, 67, 54, 12));
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(9, 93, 54, 12));
        setModelRouteX = new QDoubleSpinBox(groupBox_3);
        setModelRouteX->setObjectName(QStringLiteral("setModelRouteX"));
        setModelRouteX->setGeometry(QRect(49, 33, 172, 22));
        setModelRouteX->setDecimals(1);
        setModelRouteX->setMinimum(-180);
        setModelRouteX->setMaximum(180);
        setModelRouteX->setSingleStep(0.1);
        setModelRouteY = new QDoubleSpinBox(groupBox_3);
        setModelRouteY->setObjectName(QStringLiteral("setModelRouteY"));
        setModelRouteY->setGeometry(QRect(49, 62, 171, 22));
        setModelRouteY->setDecimals(1);
        setModelRouteY->setMinimum(-180);
        setModelRouteY->setMaximum(180);
        setModelRouteY->setSingleStep(0.1);
        setModelRouteZ = new QDoubleSpinBox(groupBox_3);
        setModelRouteZ->setObjectName(QStringLiteral("setModelRouteZ"));
        setModelRouteZ->setGeometry(QRect(50, 90, 171, 22));
        setModelRouteZ->setDecimals(1);
        setModelRouteZ->setMinimum(-180);
        setModelRouteZ->setMaximum(180);
        setModelRouteZ->setSingleStep(0.1);
        tabWidget->addTab(tab_3, QString());

        retranslateUi(addModelFrm);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(addModelFrm);
    } // setupUi

    void retranslateUi(QWidget *addModelFrm)
    {
        addModelFrm->setWindowTitle(QApplication::translate("addModelFrm", "\346\267\273\345\212\240\346\250\241\345\236\213", Q_NULLPTR));
        cancleBtn->setText(QApplication::translate("addModelFrm", "\345\217\226\346\266\210", Q_NULLPTR));
        modelName->setText(QString());
        label->setText(QApplication::translate("addModelFrm", "\346\250\241\345\236\213\345\220\215\347\247\260\357\274\232", Q_NULLPTR));
        okBtn->setText(QApplication::translate("addModelFrm", "\345\212\240\350\275\275", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("addModelFrm", "\344\275\215\347\275\256", Q_NULLPTR));
        label_3->setText(QApplication::translate("addModelFrm", "\347\273\217\345\272\246\357\274\232", Q_NULLPTR));
        label_4->setText(QApplication::translate("addModelFrm", "\347\272\254\345\272\246\357\274\232", Q_NULLPTR));
        label_5->setText(QApplication::translate("addModelFrm", "\351\253\230\345\272\246\357\274\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("addModelFrm", "\351\200\211\346\213\251\350\267\257\345\276\204\357\274\232", Q_NULLPTR));
        selectModelPathBtn->setText(QApplication::translate("addModelFrm", "...", Q_NULLPTR));
        modelPath->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("addModelFrm", "\346\250\241\345\236\213\350\267\257\345\276\204", Q_NULLPTR));
        groupBox_2->setTitle(QString());
        label_6->setText(QApplication::translate("addModelFrm", "X\350\275\264\357\274\232", Q_NULLPTR));
        label_7->setText(QApplication::translate("addModelFrm", "Y\350\275\264\357\274\232", Q_NULLPTR));
        label_8->setText(QApplication::translate("addModelFrm", "Z\350\275\264\357\274\232", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("addModelFrm", "\347\274\251\346\224\276\347\263\273\346\225\260", Q_NULLPTR));
        groupBox_3->setTitle(QString());
        label_9->setText(QApplication::translate("addModelFrm", "X\350\275\264\357\274\232", Q_NULLPTR));
        label_10->setText(QApplication::translate("addModelFrm", "Y\350\275\264\357\274\232", Q_NULLPTR));
        label_11->setText(QApplication::translate("addModelFrm", "Z\350\275\264\357\274\232", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("addModelFrm", "\346\250\241\345\236\213\346\234\235\345\220\221", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class addModelFrm: public Ui_addModelFrm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDMODELDLG_H
