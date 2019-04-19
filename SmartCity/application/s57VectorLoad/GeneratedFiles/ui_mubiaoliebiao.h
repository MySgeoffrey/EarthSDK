/********************************************************************************
** Form generated from reading UI file 'mubiaoliebiao.ui'
**
** Created: Mon Jul 16 17:32:35 2018
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MUBIAOLIEBIAO_H
#define UI_MUBIAOLIEBIAO_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mubiaoliebiao
{
public:
    QFrame *frame_mblb;
    QLabel *label_mblb;
    QToolButton *tbtn_mblb_left;
    QToolButton *tbtn_mblb_right;
    QPushButton *pushButton;

    void setupUi(QWidget *mubiaoliebiao)
    {
        if (mubiaoliebiao->objectName().isEmpty())
            mubiaoliebiao->setObjectName(QString::fromUtf8("mubiaoliebiao"));
        mubiaoliebiao->resize(904, 466);
        frame_mblb = new QFrame(mubiaoliebiao);
        frame_mblb->setObjectName(QString::fromUtf8("frame_mblb"));
        frame_mblb->setGeometry(QRect(0, 0, 904, 466));
        frame_mblb->setStyleSheet(QString::fromUtf8(""));
        frame_mblb->setFrameShape(QFrame::StyledPanel);
        frame_mblb->setFrameShadow(QFrame::Raised);
        label_mblb = new QLabel(frame_mblb);
        label_mblb->setObjectName(QString::fromUtf8("label_mblb"));
        label_mblb->setGeometry(QRect(410, 30, 80, 30));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_mblb->sizePolicy().hasHeightForWidth());
        label_mblb->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        label_mblb->setFont(font);
        tbtn_mblb_left = new QToolButton(frame_mblb);
        tbtn_mblb_left->setObjectName(QString::fromUtf8("tbtn_mblb_left"));
        tbtn_mblb_left->setGeometry(QRect(80, 380, 25, 25));
        tbtn_mblb_right = new QToolButton(frame_mblb);
        tbtn_mblb_right->setObjectName(QString::fromUtf8("tbtn_mblb_right"));
        tbtn_mblb_right->setGeometry(QRect(110, 380, 25, 25));
        pushButton = new QPushButton(frame_mblb);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(410, 250, 120, 30));

        retranslateUi(mubiaoliebiao);

        QMetaObject::connectSlotsByName(mubiaoliebiao);
    } // setupUi

    void retranslateUi(QWidget *mubiaoliebiao)
    {
        mubiaoliebiao->setWindowTitle(QApplication::translate("mubiaoliebiao", "mubiaoliebiao", 0, QApplication::UnicodeUTF8));
        label_mblb->setText(QApplication::translate("mubiaoliebiao", "\347\233\256\346\240\207\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        tbtn_mblb_left->setText(QString());
        tbtn_mblb_right->setText(QString());
        pushButton->setText(QApplication::translate("mubiaoliebiao", "\345\244\204\347\275\256", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class mubiaoliebiao: public Ui_mubiaoliebiao {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MUBIAOLIEBIAO_H
