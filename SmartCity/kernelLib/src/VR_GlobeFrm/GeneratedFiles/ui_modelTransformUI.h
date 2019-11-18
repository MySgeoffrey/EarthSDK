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
    QLineEdit *modelSrcPath;
    QPushButton *selectModelSrcPath;
    QPushButton *selectModelDestPath;
    QLabel *label_2;
    QLineEdit *modelDestPath;
    QPushButton *cancelButton;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label_4;
    QComboBox *comboBox_2;
    QComboBox *comboBox_3;
    QLabel *label_5;
    QComboBox *comboBox_4;
    QLabel *label_6;
    QComboBox *comboBox_5;
    QLabel *label_7;
    QComboBox *comboBox_6;
    QLabel *label_8;
    QWidget *tab_2;
    QLabel *label_3;
    QComboBox *comboBox;
    QComboBox *comboBox_7;
    QLabel *label_9;
    QPushButton *okButton;

    void setupUi(QDialog *ModelTransformUI)
    {
        if (ModelTransformUI->objectName().isEmpty())
            ModelTransformUI->setObjectName(QStringLiteral("ModelTransformUI"));
        ModelTransformUI->resize(687, 342);
        label = new QLabel(ModelTransformUI);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(13, 20, 81, 20));
        modelSrcPath = new QLineEdit(ModelTransformUI);
        modelSrcPath->setObjectName(QStringLiteral("modelSrcPath"));
        modelSrcPath->setGeometry(QRect(100, 19, 481, 21));
        selectModelSrcPath = new QPushButton(ModelTransformUI);
        selectModelSrcPath->setObjectName(QStringLiteral("selectModelSrcPath"));
        selectModelSrcPath->setGeometry(QRect(600, 20, 75, 23));
        selectModelDestPath = new QPushButton(ModelTransformUI);
        selectModelDestPath->setObjectName(QStringLiteral("selectModelDestPath"));
        selectModelDestPath->setGeometry(QRect(600, 60, 75, 23));
        label_2 = new QLabel(ModelTransformUI);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 60, 81, 16));
        modelDestPath = new QLineEdit(ModelTransformUI);
        modelDestPath->setObjectName(QStringLiteral("modelDestPath"));
        modelDestPath->setGeometry(QRect(100, 59, 481, 21));
        cancelButton = new QPushButton(ModelTransformUI);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setGeometry(QRect(540, 310, 75, 23));
        tabWidget = new QTabWidget(ModelTransformUI);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 100, 671, 201));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        label_4 = new QLabel(tab);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 20, 91, 16));
        comboBox_2 = new QComboBox(tab);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));
        comboBox_2->setGeometry(QRect(90, 20, 151, 22));
        comboBox_3 = new QComboBox(tab);
        comboBox_3->setObjectName(QStringLiteral("comboBox_3"));
        comboBox_3->setGeometry(QRect(390, 20, 151, 22));
        label_5 = new QLabel(tab);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(320, 20, 91, 16));
        comboBox_4 = new QComboBox(tab);
        comboBox_4->setObjectName(QStringLiteral("comboBox_4"));
        comboBox_4->setGeometry(QRect(90, 70, 151, 22));
        label_6 = new QLabel(tab);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 70, 91, 16));
        comboBox_5 = new QComboBox(tab);
        comboBox_5->setObjectName(QStringLiteral("comboBox_5"));
        comboBox_5->setGeometry(QRect(390, 70, 151, 22));
        label_7 = new QLabel(tab);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(320, 70, 91, 16));
        comboBox_6 = new QComboBox(tab);
        comboBox_6->setObjectName(QStringLiteral("comboBox_6"));
        comboBox_6->setGeometry(QRect(90, 120, 151, 22));
        label_8 = new QLabel(tab);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 120, 91, 16));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 20, 71, 16));
        comboBox = new QComboBox(tab_2);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(80, 20, 151, 22));
        comboBox_7 = new QComboBox(tab_2);
        comboBox_7->setObjectName(QStringLiteral("comboBox_7"));
        comboBox_7->setGeometry(QRect(380, 20, 151, 22));
        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(310, 20, 91, 16));
        tabWidget->addTab(tab_2, QString());
        okButton = new QPushButton(ModelTransformUI);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setGeometry(QRect(440, 310, 75, 23));

        retranslateUi(ModelTransformUI);
        QObject::connect(okButton, SIGNAL(clicked()), ModelTransformUI, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), ModelTransformUI, SLOT(reject()));
        QObject::connect(selectModelDestPath, SIGNAL(clicked()), ModelTransformUI, SLOT(slotSelectModelDestPath()));
        QObject::connect(selectModelSrcPath, SIGNAL(clicked()), ModelTransformUI, SLOT(slotSelectModelSrcPath()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ModelTransformUI);
    } // setupUi

    void retranslateUi(QDialog *ModelTransformUI)
    {
        ModelTransformUI->setWindowTitle(QApplication::translate("ModelTransformUI", "\347\256\241\347\275\221\345\217\202\346\225\260\345\214\226\345\273\272\346\250\241", Q_NULLPTR));
        label->setText(QApplication::translate("ModelTransformUI", "\347\237\242\351\207\217\350\276\223\345\205\245\347\233\256\345\275\225\357\274\232", Q_NULLPTR));
        selectModelSrcPath->setText(QApplication::translate("ModelTransformUI", "\351\200\211\346\213\251..", Q_NULLPTR));
        selectModelDestPath->setText(QApplication::translate("ModelTransformUI", "\351\200\211\346\213\251..", Q_NULLPTR));
        label_2->setText(QApplication::translate("ModelTransformUI", "\346\250\241\345\236\213\350\276\223\345\207\272\347\233\256\345\275\225\357\274\232", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("ModelTransformUI", "\345\205\263\351\227\255", Q_NULLPTR));
        label_4->setText(QApplication::translate("ModelTransformUI", "\350\265\267\345\247\213\345\235\220\346\240\207\357\274\232", Q_NULLPTR));
        label_5->setText(QApplication::translate("ModelTransformUI", "\347\273\210\346\255\242\345\235\220\346\240\207\357\274\232", Q_NULLPTR));
        label_6->setText(QApplication::translate("ModelTransformUI", "\347\256\241\346\256\265\346\235\220\350\264\250\357\274\232", Q_NULLPTR));
        label_7->setText(QApplication::translate("ModelTransformUI", "\347\256\241\346\256\265\345\260\272\345\257\270\357\274\232", Q_NULLPTR));
        comboBox_6->clear();
        comboBox_6->insertItems(0, QStringList()
         << QApplication::translate("ModelTransformUI", "\345\234\206\345\275\242", Q_NULLPTR)
         << QApplication::translate("ModelTransformUI", "\347\237\251\345\275\242", Q_NULLPTR)
         << QApplication::translate("ModelTransformUI", "\346\242\257\345\275\242", Q_NULLPTR)
         << QApplication::translate("ModelTransformUI", "\346\213\261\345\275\242", Q_NULLPTR)
        );
        label_8->setText(QApplication::translate("ModelTransformUI", "\347\256\241\346\256\265\346\210\252\351\235\242\357\274\232", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("ModelTransformUI", "\347\256\241\346\256\265\345\217\202\346\225\260", Q_NULLPTR));
        label_3->setText(QApplication::translate("ModelTransformUI", "\346\216\245\345\244\264\345\235\220\346\240\207\357\274\232", Q_NULLPTR));
        label_9->setText(QApplication::translate("ModelTransformUI", "\347\256\241\346\256\265\346\235\220\350\264\250\357\274\232", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ModelTransformUI", "\346\216\245\345\244\264\345\217\202\346\225\260", Q_NULLPTR));
        okButton->setText(QApplication::translate("ModelTransformUI", "\350\275\254\346\215\242", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ModelTransformUI: public Ui_ModelTransformUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELTRANSFORMUI_H
