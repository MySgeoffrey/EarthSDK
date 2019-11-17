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
    QPushButton *okButton;

    void setupUi(QDialog *ModelTransformUI)
    {
        if (ModelTransformUI->objectName().isEmpty())
            ModelTransformUI->setObjectName(QStringLiteral("ModelTransformUI"));
        ModelTransformUI->resize(693, 530);
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
        cancelButton->setGeometry(QRect(570, 500, 75, 23));
        tabWidget = new QTabWidget(ModelTransformUI);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(20, 100, 661, 391));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tabWidget->addTab(tab, QString());
        okButton = new QPushButton(ModelTransformUI);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setGeometry(QRect(470, 500, 75, 23));

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
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("ModelTransformUI", "\345\217\202\346\225\260\350\256\276\347\275\256", Q_NULLPTR));
        okButton->setText(QApplication::translate("ModelTransformUI", "\350\275\254\346\215\242", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ModelTransformUI: public Ui_ModelTransformUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELTRANSFORMUI_H
