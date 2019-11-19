/********************************************************************************
** Form generated from reading UI file 'modelTransformUI.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELTRANSFORMUI_H
#define UI_MODELTRANSFORMUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModelTransformUI
{
public:
    QLabel *label;
    QLineEdit *PipeSrcPath;
    QPushButton *selectModelSrcPath;
    QPushButton *selectModelDestPath;
    QLabel *label_2;
    QLineEdit *ModelExportDir;
    QPushButton *cancelButton;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label_6;
    QComboBox *SizeInfo;
    QLabel *label_7;
    QComboBox *SectionType;
    QLabel *label_8;
    QPushButton *pipeLineColor;
    QLabel *label_10;
    QLineEdit *materialTexturePath;
    QPushButton *selectPipeLineMaterialTexturePath;
    QWidget *tab_2;
    QPushButton *selectModelDestPath_3;
    QLabel *label_11;
    QLineEdit *modelSrcPath_3;
    QPushButton *pushButton_2;
    QLabel *label_9;
    QPushButton *okButton;

    void setupUi(QDialog *ModelTransformUI)
    {
        if (ModelTransformUI->objectName().isEmpty())
            ModelTransformUI->setObjectName(QStringLiteral("ModelTransformUI"));
        ModelTransformUI->resize(687, 298);
        label = new QLabel(ModelTransformUI);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(13, 20, 111, 20));
        PipeSrcPath = new QLineEdit(ModelTransformUI);
        PipeSrcPath->setObjectName(QStringLiteral("PipeSrcPath"));
        PipeSrcPath->setGeometry(QRect(130, 19, 451, 21));
        selectModelSrcPath = new QPushButton(ModelTransformUI);
        selectModelSrcPath->setObjectName(QStringLiteral("selectModelSrcPath"));
        selectModelSrcPath->setGeometry(QRect(600, 20, 75, 23));
        selectModelDestPath = new QPushButton(ModelTransformUI);
        selectModelDestPath->setObjectName(QStringLiteral("selectModelDestPath"));
        selectModelDestPath->setGeometry(QRect(600, 60, 75, 23));
        label_2 = new QLabel(ModelTransformUI);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 60, 181, 16));
        ModelExportDir = new QLineEdit(ModelTransformUI);
        ModelExportDir->setObjectName(QStringLiteral("ModelExportDir"));
        ModelExportDir->setGeometry(QRect(130, 59, 451, 21));
        cancelButton = new QPushButton(ModelTransformUI);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setGeometry(QRect(540, 270, 75, 23));
        tabWidget = new QTabWidget(ModelTransformUI);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 100, 671, 161));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        label_6 = new QLabel(tab);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 60, 91, 16));
        SizeInfo = new QComboBox(tab);
        SizeInfo->setObjectName(QStringLiteral("SizeInfo"));
        SizeInfo->setGeometry(QRect(390, 20, 151, 22));
        label_7 = new QLabel(tab);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(320, 20, 91, 16));
        SectionType = new QComboBox(tab);
        SectionType->setObjectName(QStringLiteral("SectionType"));
        SectionType->setGeometry(QRect(90, 20, 151, 22));
        label_8 = new QLabel(tab);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 20, 91, 16));
        pipeLineColor = new QPushButton(tab);
        pipeLineColor->setObjectName(QStringLiteral("pipeLineColor"));
        pipeLineColor->setGeometry(QRect(90, 60, 151, 21));
        pipeLineColor->setStyleSheet(QStringLiteral("background-color: rgb(194, 194, 194);"));
        label_10 = new QLabel(tab);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(20, 100, 91, 16));
        materialTexturePath = new QLineEdit(tab);
        materialTexturePath->setObjectName(QStringLiteral("materialTexturePath"));
        materialTexturePath->setGeometry(QRect(90, 100, 471, 21));
        selectPipeLineMaterialTexturePath = new QPushButton(tab);
        selectPipeLineMaterialTexturePath->setObjectName(QStringLiteral("selectPipeLineMaterialTexturePath"));
        selectPipeLineMaterialTexturePath->setGeometry(QRect(580, 100, 75, 23));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        selectModelDestPath_3 = new QPushButton(tab_2);
        selectModelDestPath_3->setObjectName(QStringLiteral("selectModelDestPath_3"));
        selectModelDestPath_3->setGeometry(QRect(580, 60, 75, 23));
        label_11 = new QLabel(tab_2);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(20, 60, 91, 16));
        modelSrcPath_3 = new QLineEdit(tab_2);
        modelSrcPath_3->setObjectName(QStringLiteral("modelSrcPath_3"));
        modelSrcPath_3->setGeometry(QRect(90, 60, 471, 21));
        pushButton_2 = new QPushButton(tab_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(90, 20, 151, 21));
        pushButton_2->setStyleSheet(QStringLiteral("background-color: rgb(112, 112, 112);"));
        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(20, 20, 91, 16));
        tabWidget->addTab(tab_2, QString());
        okButton = new QPushButton(ModelTransformUI);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setGeometry(QRect(440, 270, 75, 23));

        retranslateUi(ModelTransformUI);
        QObject::connect(okButton, SIGNAL(clicked()), ModelTransformUI, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), ModelTransformUI, SLOT(reject()));
        QObject::connect(selectModelDestPath, SIGNAL(clicked()), ModelTransformUI, SLOT(slotSelectModelDestPath()));
        QObject::connect(selectModelSrcPath, SIGNAL(clicked()), ModelTransformUI, SLOT(slotSelectModelSrcPath()));
        QObject::connect(selectPipeLineMaterialTexturePath, SIGNAL(clicked()), ModelTransformUI, SLOT(slotSelectPipeLineMaterialTexturePath()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ModelTransformUI);
    } // setupUi

    void retranslateUi(QDialog *ModelTransformUI)
    {
        ModelTransformUI->setWindowTitle(QApplication::translate("ModelTransformUI", "PipeNet Builder", Q_NULLPTR));
        label->setText(QApplication::translate("ModelTransformUI", "PipeLine FilePath\357\274\232", Q_NULLPTR));
        PipeSrcPath->setText(QApplication::translate("ModelTransformUI", "E:\\dataService\\GX\\YSGLINE_line.shp", Q_NULLPTR));
        selectModelSrcPath->setText(QApplication::translate("ModelTransformUI", "Select", Q_NULLPTR));
        selectModelDestPath->setText(QApplication::translate("ModelTransformUI", "Select", Q_NULLPTR));
        label_2->setText(QApplication::translate("ModelTransformUI", "Export Directory\357\274\232", Q_NULLPTR));
        ModelExportDir->setText(QApplication::translate("ModelTransformUI", "D:\\export", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("ModelTransformUI", "Close", Q_NULLPTR));
        label_6->setText(QApplication::translate("ModelTransformUI", "Color\357\274\232", Q_NULLPTR));
        SizeInfo->clear();
        SizeInfo->insertItems(0, QStringList()
         << QApplication::translate("ModelTransformUI", "2x2", Q_NULLPTR)
         << QApplication::translate("ModelTransformUI", "2x2x1", Q_NULLPTR)
        );
        label_7->setText(QApplication::translate("ModelTransformUI", "Size\357\274\232", Q_NULLPTR));
        SectionType->clear();
        SectionType->insertItems(0, QStringList()
         << QApplication::translate("ModelTransformUI", "Circle", Q_NULLPTR)
         << QApplication::translate("ModelTransformUI", "Rectangle", Q_NULLPTR)
         << QApplication::translate("ModelTransformUI", "Ladder", Q_NULLPTR)
         << QApplication::translate("ModelTransformUI", "Arc", Q_NULLPTR)
        );
        label_8->setText(QApplication::translate("ModelTransformUI", "Section\357\274\232", Q_NULLPTR));
        pipeLineColor->setText(QString());
        label_10->setText(QApplication::translate("ModelTransformUI", "Material\357\274\232", Q_NULLPTR));
        materialTexturePath->setText(QApplication::translate("ModelTransformUI", "D:\\gitReource\\EarthSDK\\SmartCity\\Resource\\red.bmp", Q_NULLPTR));
        selectPipeLineMaterialTexturePath->setText(QApplication::translate("ModelTransformUI", "Select", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("ModelTransformUI", "PipeLine Param", Q_NULLPTR));
        selectModelDestPath_3->setText(QApplication::translate("ModelTransformUI", "Select", Q_NULLPTR));
        label_11->setText(QApplication::translate("ModelTransformUI", "Material\357\274\232", Q_NULLPTR));
        modelSrcPath_3->setText(QApplication::translate("ModelTransformUI", "D:\\gitReource\\EarthSDK\\SmartCity\\Resource\\blue.bmp", Q_NULLPTR));
        pushButton_2->setText(QString());
        label_9->setText(QApplication::translate("ModelTransformUI", "Color\357\274\232", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ModelTransformUI", "PipeLinker Param", Q_NULLPTR));
        okButton->setText(QApplication::translate("ModelTransformUI", "Build", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ModelTransformUI: public Ui_ModelTransformUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELTRANSFORMUI_H
