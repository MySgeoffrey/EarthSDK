/********************************************************************************
** Form generated from reading UI file 'addShpDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDSHPDLG_H
#define UI_ADDSHPDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_addShpFrm
{
public:
    QLabel *label;
    QPushButton *OkBtn;
    QPushButton *cancleBtn;
    QLineEdit *dataName;
    QPushButton *selectDataPathBtn;
    QLineEdit *dataPath;
    QLabel *label_2;

    void setupUi(QWidget *addShpFrm)
    {
        if (addShpFrm->objectName().isEmpty())
            addShpFrm->setObjectName(QStringLiteral("addShpFrm"));
        addShpFrm->resize(470, 152);
        label = new QLabel(addShpFrm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(18, 22, 61, 16));
        OkBtn = new QPushButton(addShpFrm);
        OkBtn->setObjectName(QStringLiteral("OkBtn"));
        OkBtn->setGeometry(QRect(115, 108, 75, 23));
        cancleBtn = new QPushButton(addShpFrm);
        cancleBtn->setObjectName(QStringLiteral("cancleBtn"));
        cancleBtn->setGeometry(QRect(260, 110, 75, 23));
        dataName = new QLineEdit(addShpFrm);
        dataName->setObjectName(QStringLiteral("dataName"));
        dataName->setGeometry(QRect(86, 20, 281, 20));
        selectDataPathBtn = new QPushButton(addShpFrm);
        selectDataPathBtn->setObjectName(QStringLiteral("selectDataPathBtn"));
        selectDataPathBtn->setGeometry(QRect(390, 50, 51, 23));
        dataPath = new QLineEdit(addShpFrm);
        dataPath->setObjectName(QStringLiteral("dataPath"));
        dataPath->setGeometry(QRect(87, 56, 281, 20));
        label_2 = new QLabel(addShpFrm);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(18, 58, 61, 16));

        retranslateUi(addShpFrm);

        QMetaObject::connectSlotsByName(addShpFrm);
    } // setupUi

    void retranslateUi(QWidget *addShpFrm)
    {
        addShpFrm->setWindowTitle(QApplication::translate("addShpFrm", "\345\212\240\350\275\275\347\237\242\351\207\217\346\225\260\346\215\256", Q_NULLPTR));
        label->setText(QApplication::translate("addShpFrm", "\346\225\260\346\215\256\345\220\215\347\247\260\357\274\232", Q_NULLPTR));
        OkBtn->setText(QApplication::translate("addShpFrm", "\347\241\256\345\256\232", Q_NULLPTR));
        cancleBtn->setText(QApplication::translate("addShpFrm", "\345\217\226\346\266\210", Q_NULLPTR));
        dataName->setText(QString());
        selectDataPathBtn->setText(QApplication::translate("addShpFrm", "\351\200\211\346\213\251..", Q_NULLPTR));
        dataPath->setText(QString());
        label_2->setText(QApplication::translate("addShpFrm", "\351\200\211\346\213\251\350\267\257\345\276\204\357\274\232", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class addShpFrm: public Ui_addShpFrm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDSHPDLG_H
