/********************************************************************************
** Form generated from reading UI file 'ImageTransparentUI.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGETRANSPARENTUI_H
#define UI_IMAGETRANSPARENTUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageTransparentUI
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLabel *label;
    QSlider *transparentValue;
    QLabel *label_2;

    void setupUi(QDialog *ImageTransparentUI)
    {
        if (ImageTransparentUI->objectName().isEmpty())
            ImageTransparentUI->setObjectName(QStringLiteral("ImageTransparentUI"));
        ImageTransparentUI->resize(338, 97);
        layoutWidget = new QWidget(ImageTransparentUI);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(-30, 50, 351, 33));
        hboxLayout = new QHBoxLayout(layoutWidget);
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(layoutWidget);
        okButton->setObjectName(QStringLiteral("okButton"));

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        hboxLayout->addWidget(cancelButton);

        label = new QLabel(ImageTransparentUI);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 20, 54, 12));
        transparentValue = new QSlider(ImageTransparentUI);
        transparentValue->setObjectName(QStringLiteral("transparentValue"));
        transparentValue->setGeometry(QRect(90, 20, 160, 22));
        transparentValue->setMaximum(100);
        transparentValue->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(ImageTransparentUI);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(280, 20, 54, 21));
        label_2->setStyleSheet(QStringLiteral("font: 14pt \"Algerian\";"));

        retranslateUi(ImageTransparentUI);
        QObject::connect(okButton, SIGNAL(clicked()), ImageTransparentUI, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), ImageTransparentUI, SLOT(reject()));
        QObject::connect(transparentValue, SIGNAL(valueChanged(int)), ImageTransparentUI, SLOT(slotTransparentValueChanged(int)));

        QMetaObject::connectSlotsByName(ImageTransparentUI);
    } // setupUi

    void retranslateUi(QDialog *ImageTransparentUI)
    {
        ImageTransparentUI->setWindowTitle(QApplication::translate("ImageTransparentUI", "\345\233\276\345\261\202\345\261\236\346\200\247", Q_NULLPTR));
        okButton->setText(QApplication::translate("ImageTransparentUI", "\347\241\256\345\256\232", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("ImageTransparentUI", "\345\217\226\346\266\210", Q_NULLPTR));
        label->setText(QApplication::translate("ImageTransparentUI", "\351\200\217\346\230\216\345\272\246", Q_NULLPTR));
        label_2->setText(QApplication::translate("ImageTransparentUI", "%", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ImageTransparentUI: public Ui_ImageTransparentUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGETRANSPARENTUI_H
