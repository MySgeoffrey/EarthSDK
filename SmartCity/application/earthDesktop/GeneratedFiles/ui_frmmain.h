/********************************************************************************
** Form generated from reading UI file 'frmmain.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRMMAIN_H
#define UI_FRMMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_frmMain
{
public:
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_title;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QLabel *lab_Title;
    QWidget *widget_menu;
    QHBoxLayout *horizontalLayout;
    QLabel *lab_Ico;
    QPushButton *btnMenu_About;
    QPushButton *btnMenu;
    QPushButton *btnMenu_Min;
    QPushButton *btnMenu_Max;
    QPushButton *btnMenu_Close;
    QHBoxLayout *horizontalLayout_4;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *keyPlaceName;
    QPushButton *searchPlaceName;
    QTableView *placeSearchWidget;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_2;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *layerManagerLayout;
    QWidget *tab_4;
    QHBoxLayout *horizontalLayout_10;
    QToolBox *toolBox_2;
    QWidget *page_3;
    QPushButton *disMeature;
    QPushButton *areaMeature;
    QPushButton *azimuthMeature;
    QPushButton *clearMeatureResult;
    QWidget *page_4;
    QWidget *page;
    QWidget *tab_6;
    QHBoxLayout *horizontalLayout_11;
    QGridLayout *serviceGridLayout;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_9;
    QToolBox *toolBox_3;
    QWidget *page_5;
    QWidget *page_6;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_3;
    QGridLayout *globeLayerOut;

    void setupUi(QDialog *frmMain)
    {
        if (frmMain->objectName().isEmpty())
            frmMain->setObjectName(QStringLiteral("frmMain"));
        frmMain->resize(738, 549);
        frmMain->setSizeGripEnabled(true);
        horizontalLayout_5 = new QHBoxLayout(frmMain);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        widget_title = new QWidget(frmMain);
        widget_title->setObjectName(QStringLiteral("widget_title"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_title->sizePolicy().hasHeightForWidth());
        widget_title->setSizePolicy(sizePolicy);
        widget_title->setMinimumSize(QSize(100, 70));
        horizontalLayout_2 = new QHBoxLayout(widget_title);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton = new QPushButton(widget_title);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(50, 50));
        pushButton->setMaximumSize(QSize(50, 50));
        pushButton->setStyleSheet(QStringLiteral("border-image: url(:/image/main.ico);"));

        horizontalLayout_2->addWidget(pushButton);

        lab_Title = new QLabel(widget_title);
        lab_Title->setObjectName(QStringLiteral("lab_Title"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lab_Title->sizePolicy().hasHeightForWidth());
        lab_Title->setSizePolicy(sizePolicy1);
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(18);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        lab_Title->setFont(font);
        lab_Title->setStyleSheet(QString::fromUtf8("font: 18pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"color: rgb(255, 255, 255);\n"
""));
        lab_Title->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lab_Title->setMargin(20);

        horizontalLayout_2->addWidget(lab_Title, 0, Qt::AlignVCenter);

        widget_menu = new QWidget(widget_title);
        widget_menu->setObjectName(QStringLiteral("widget_menu"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget_menu->sizePolicy().hasHeightForWidth());
        widget_menu->setSizePolicy(sizePolicy2);
        horizontalLayout = new QHBoxLayout(widget_menu);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lab_Ico = new QLabel(widget_menu);
        lab_Ico->setObjectName(QStringLiteral("lab_Ico"));
        sizePolicy2.setHeightForWidth(lab_Ico->sizePolicy().hasHeightForWidth());
        lab_Ico->setSizePolicy(sizePolicy2);
        lab_Ico->setMinimumSize(QSize(30, 0));
        lab_Ico->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(lab_Ico);

        btnMenu_About = new QPushButton(widget_menu);
        btnMenu_About->setObjectName(QStringLiteral("btnMenu_About"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btnMenu_About->sizePolicy().hasHeightForWidth());
        btnMenu_About->setSizePolicy(sizePolicy3);
        btnMenu_About->setMinimumSize(QSize(31, 0));
        btnMenu_About->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_About->setFocusPolicy(Qt::NoFocus);
        btnMenu_About->setFlat(true);

        horizontalLayout->addWidget(btnMenu_About);

        btnMenu = new QPushButton(widget_menu);
        btnMenu->setObjectName(QStringLiteral("btnMenu"));
        sizePolicy3.setHeightForWidth(btnMenu->sizePolicy().hasHeightForWidth());
        btnMenu->setSizePolicy(sizePolicy3);
        btnMenu->setMinimumSize(QSize(31, 0));
        btnMenu->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu->setFocusPolicy(Qt::NoFocus);
        btnMenu->setFlat(true);

        horizontalLayout->addWidget(btnMenu);

        btnMenu_Min = new QPushButton(widget_menu);
        btnMenu_Min->setObjectName(QStringLiteral("btnMenu_Min"));
        sizePolicy3.setHeightForWidth(btnMenu_Min->sizePolicy().hasHeightForWidth());
        btnMenu_Min->setSizePolicy(sizePolicy3);
        btnMenu_Min->setMinimumSize(QSize(31, 0));
        btnMenu_Min->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Min->setFocusPolicy(Qt::NoFocus);
        btnMenu_Min->setFlat(true);

        horizontalLayout->addWidget(btnMenu_Min);

        btnMenu_Max = new QPushButton(widget_menu);
        btnMenu_Max->setObjectName(QStringLiteral("btnMenu_Max"));
        sizePolicy3.setHeightForWidth(btnMenu_Max->sizePolicy().hasHeightForWidth());
        btnMenu_Max->setSizePolicy(sizePolicy3);
        btnMenu_Max->setMinimumSize(QSize(31, 0));
        btnMenu_Max->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Max->setFocusPolicy(Qt::NoFocus);
        btnMenu_Max->setFlat(true);

        horizontalLayout->addWidget(btnMenu_Max);

        btnMenu_Close = new QPushButton(widget_menu);
        btnMenu_Close->setObjectName(QStringLiteral("btnMenu_Close"));
        sizePolicy3.setHeightForWidth(btnMenu_Close->sizePolicy().hasHeightForWidth());
        btnMenu_Close->setSizePolicy(sizePolicy3);
        btnMenu_Close->setMinimumSize(QSize(40, 0));
        btnMenu_Close->setCursor(QCursor(Qt::ArrowCursor));
        btnMenu_Close->setFocusPolicy(Qt::NoFocus);
        btnMenu_Close->setFlat(true);

        horizontalLayout->addWidget(btnMenu_Close);


        horizontalLayout_2->addWidget(widget_menu);


        verticalLayout_3->addWidget(widget_title);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        tabWidget = new QTabWidget(frmMain);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy4);
        tabWidget->setMinimumSize(QSize(260, 0));
        tabWidget->setMaximumSize(QSize(260, 16777215));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setWeight(50);
        font1.setKerning(true);
        tabWidget->setFont(font1);
        tabWidget->setLayoutDirection(Qt::LeftToRight);
        tabWidget->setStyleSheet(QStringLiteral(""));
        tabWidget->setTabPosition(QTabWidget::West);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setIconSize(QSize(18, 18));
        tabWidget->setMovable(true);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalLayout_8 = new QHBoxLayout(tab);
        horizontalLayout_8->setSpacing(0);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 15, 0, 0);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(8, -1, 6, -1);
        widget = new QWidget(tab);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout_7 = new QHBoxLayout(widget);
        horizontalLayout_7->setSpacing(0);
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        verticalLayout->addWidget(label);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 3, -1, 3);
        keyPlaceName = new QLineEdit(widget);
        keyPlaceName->setObjectName(QStringLiteral("keyPlaceName"));

        horizontalLayout_6->addWidget(keyPlaceName);

        searchPlaceName = new QPushButton(widget);
        searchPlaceName->setObjectName(QStringLiteral("searchPlaceName"));
        searchPlaceName->setMaximumSize(QSize(16777215, 23));
        searchPlaceName->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/image/png-0087.png"), QSize(), QIcon::Normal, QIcon::Off);
        searchPlaceName->setIcon(icon);

        horizontalLayout_6->addWidget(searchPlaceName);


        verticalLayout->addLayout(horizontalLayout_6);

        placeSearchWidget = new QTableView(widget);
        placeSearchWidget->setObjectName(QStringLiteral("placeSearchWidget"));
        placeSearchWidget->setMaximumSize(QSize(16777215, 220));

        verticalLayout->addWidget(placeSearchWidget);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(-1, 15, -1, -1);

        verticalLayout->addLayout(verticalLayout_6);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("font: 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));

        verticalLayout->addWidget(label_2);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(-1, 10, -1, 0);

        verticalLayout->addLayout(verticalLayout_5);

        layerManagerLayout = new QGridLayout();
        layerManagerLayout->setSpacing(6);
        layerManagerLayout->setObjectName(QStringLiteral("layerManagerLayout"));

        verticalLayout->addLayout(layerManagerLayout);


        horizontalLayout_7->addLayout(verticalLayout);


        verticalLayout_4->addWidget(widget);


        horizontalLayout_8->addLayout(verticalLayout_4);

        QIcon icon1;
        icon1.addFile(QStringLiteral(":/image/fullExtend.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab, icon1, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        horizontalLayout_10 = new QHBoxLayout(tab_4);
        horizontalLayout_10->setSpacing(0);
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        toolBox_2 = new QToolBox(tab_4);
        toolBox_2->setObjectName(QStringLiteral("toolBox_2"));
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        page_3->setGeometry(QRect(0, 0, 98, 28));
        disMeature = new QPushButton(page_3);
        disMeature->setObjectName(QStringLiteral("disMeature"));
        disMeature->setGeometry(QRect(10, 10, 71, 31));
        disMeature->setStyleSheet(QStringLiteral(""));
        disMeature->setCheckable(true);
        disMeature->setAutoRepeat(false);
        areaMeature = new QPushButton(page_3);
        areaMeature->setObjectName(QStringLiteral("areaMeature"));
        areaMeature->setGeometry(QRect(100, 10, 81, 31));
        areaMeature->setStyleSheet(QStringLiteral(""));
        areaMeature->setCheckable(true);
        areaMeature->setAutoRepeat(false);
        azimuthMeature = new QPushButton(page_3);
        azimuthMeature->setObjectName(QStringLiteral("azimuthMeature"));
        azimuthMeature->setGeometry(QRect(10, 50, 71, 31));
        azimuthMeature->setStyleSheet(QStringLiteral(""));
        azimuthMeature->setCheckable(true);
        azimuthMeature->setAutoRepeat(false);
        clearMeatureResult = new QPushButton(page_3);
        clearMeatureResult->setObjectName(QStringLiteral("clearMeatureResult"));
        clearMeatureResult->setGeometry(QRect(100, 50, 81, 31));
        clearMeatureResult->setStyleSheet(QStringLiteral(""));
        clearMeatureResult->setCheckable(true);
        clearMeatureResult->setAutoRepeat(false);
        toolBox_2->addItem(page_3, QString::fromUtf8("\346\265\213\351\207\217\345\210\206\346\236\220"));
        page_4 = new QWidget();
        page_4->setObjectName(QStringLiteral("page_4"));
        page_4->setGeometry(QRect(0, 0, 98, 28));
        toolBox_2->addItem(page_4, QString::fromUtf8("\345\234\260\345\275\242\345\210\206\346\236\220"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        page->setGeometry(QRect(0, 0, 228, 387));
        toolBox_2->addItem(page, QString::fromUtf8("\347\256\241\347\275\221\345\210\206\346\236\220"));

        horizontalLayout_10->addWidget(toolBox_2);

        QIcon icon2;
        icon2.addFile(QStringLiteral(":/image/analyse.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab_4, icon2, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QStringLiteral("tab_6"));
        horizontalLayout_11 = new QHBoxLayout(tab_6);
        horizontalLayout_11->setSpacing(0);
        horizontalLayout_11->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        serviceGridLayout = new QGridLayout();
        serviceGridLayout->setSpacing(0);
        serviceGridLayout->setObjectName(QStringLiteral("serviceGridLayout"));

        horizontalLayout_11->addLayout(serviceGridLayout);

        QIcon icon3;
        icon3.addFile(QStringLiteral(":/image/tool.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab_6, icon3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        horizontalLayout_9 = new QHBoxLayout(tab_2);
        horizontalLayout_9->setSpacing(0);
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        toolBox_3 = new QToolBox(tab_2);
        toolBox_3->setObjectName(QStringLiteral("toolBox_3"));
        page_5 = new QWidget();
        page_5->setObjectName(QStringLiteral("page_5"));
        page_5->setGeometry(QRect(0, 0, 98, 28));
        toolBox_3->addItem(page_5, QString::fromUtf8("\345\237\272\347\241\200\345\234\272\346\231\257"));
        page_6 = new QWidget();
        page_6->setObjectName(QStringLiteral("page_6"));
        page_6->setGeometry(QRect(0, 0, 98, 28));
        toolBox_3->addItem(page_6, QString::fromUtf8("\345\210\206\346\236\220\351\243\216\346\240\274"));
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        page_2->setGeometry(QRect(0, 0, 228, 387));
        toolBox_3->addItem(page_2, QString::fromUtf8("\346\265\267\345\233\276\345\217\202\346\225\260"));

        horizontalLayout_9->addWidget(toolBox_3);

        QIcon icon4;
        icon4.addFile(QStringLiteral(":/image/setting.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget->addTab(tab_2, icon4, QString());

        horizontalLayout_4->addWidget(tabWidget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        widget_3 = new QWidget(frmMain);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setStyleSheet(QStringLiteral("background-color: rgb(85, 100, 255);"));
        horizontalLayout_3 = new QHBoxLayout(widget_3);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        globeLayerOut = new QGridLayout();
        globeLayerOut->setSpacing(0);
        globeLayerOut->setObjectName(QStringLiteral("globeLayerOut"));

        horizontalLayout_3->addLayout(globeLayerOut);


        verticalLayout_2->addWidget(widget_3);


        horizontalLayout_4->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout_4);


        horizontalLayout_5->addLayout(verticalLayout_3);


        retranslateUi(frmMain);
        QObject::connect(lab_Ico, SIGNAL(linkActivated(QString)), frmMain, SLOT(slotShowLayerManager()));
        QObject::connect(btnMenu, SIGNAL(clicked()), frmMain, SLOT(slotShowToolControl()));
        QObject::connect(searchPlaceName, SIGNAL(clicked()), frmMain, SLOT(slotSearchPlaceName()));
        QObject::connect(disMeature, SIGNAL(clicked()), frmMain, SLOT(slotDisMeature()));
        QObject::connect(areaMeature, SIGNAL(clicked()), frmMain, SLOT(slotAreaMeature()));
        QObject::connect(azimuthMeature, SIGNAL(clicked()), frmMain, SLOT(slotAzimuthMeature()));
        QObject::connect(clearMeatureResult, SIGNAL(clicked()), frmMain, SLOT(slotClearMeatureResult()));

        tabWidget->setCurrentIndex(0);
        toolBox_2->setCurrentIndex(2);
        toolBox_2->layout()->setSpacing(0);
        toolBox_3->setCurrentIndex(2);
        toolBox_3->layout()->setSpacing(0);


        QMetaObject::connectSlotsByName(frmMain);
    } // setupUi

    void retranslateUi(QDialog *frmMain)
    {
        frmMain->setWindowTitle(QApplication::translate("frmMain", "\350\232\202\350\232\201\346\227\266\347\251\272\344\277\241\346\201\257\346\234\215\345\212\241\345\271\263\345\217\260", Q_NULLPTR));
        pushButton->setText(QString());
        lab_Title->setText(QApplication::translate("frmMain", "iXServiceProvider 2.0", Q_NULLPTR));
        lab_Ico->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnMenu_About->setToolTip(QApplication::translate("frmMain", "\345\205\263\344\272\216", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        btnMenu_About->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnMenu->setToolTip(QApplication::translate("frmMain", "\345\233\276\345\261\202\351\235\242\346\235\277", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        btnMenu->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnMenu_Min->setToolTip(QApplication::translate("frmMain", "\346\234\200\345\260\217\345\214\226", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        btnMenu_Min->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnMenu_Max->setToolTip(QApplication::translate("frmMain", "\346\234\200\345\244\247\345\214\226", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        btnMenu_Max->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnMenu_Close->setToolTip(QApplication::translate("frmMain", "\345\205\263\351\227\255", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        btnMenu_Close->setText(QString());
        label->setText(QApplication::translate("frmMain", "\345\234\260\345\220\215\346\237\245\350\257\242", Q_NULLPTR));
        keyPlaceName->setText(QApplication::translate("frmMain", "\345\214\227\344\272\254", Q_NULLPTR));
        searchPlaceName->setText(QApplication::translate("frmMain", "\346\220\234\344\270\200\344\270\213", Q_NULLPTR));
        label_2->setText(QApplication::translate("frmMain", "\345\233\276\345\261\202\347\256\241\347\220\206", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("frmMain", "\345\233\276\345\261\202", Q_NULLPTR));
        disMeature->setText(QApplication::translate("frmMain", "\350\267\235\347\246\273\346\265\213\351\207\217", Q_NULLPTR));
        areaMeature->setText(QApplication::translate("frmMain", "\351\235\242\347\247\257\346\265\213\351\207\217", Q_NULLPTR));
        azimuthMeature->setText(QApplication::translate("frmMain", "\346\226\271\344\275\215\346\265\213\351\207\217", Q_NULLPTR));
        clearMeatureResult->setText(QApplication::translate("frmMain", "\346\270\205\351\231\244\347\273\223\346\236\234", Q_NULLPTR));
        toolBox_2->setItemText(toolBox_2->indexOf(page_3), QApplication::translate("frmMain", "\346\265\213\351\207\217\345\210\206\346\236\220", Q_NULLPTR));
        toolBox_2->setItemText(toolBox_2->indexOf(page_4), QApplication::translate("frmMain", "\345\234\260\345\275\242\345\210\206\346\236\220", Q_NULLPTR));
        toolBox_2->setItemText(toolBox_2->indexOf(page), QApplication::translate("frmMain", "\347\256\241\347\275\221\345\210\206\346\236\220", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("frmMain", "\345\210\206\346\236\220", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QApplication::translate("frmMain", "\346\234\215\345\212\241", Q_NULLPTR));
        toolBox_3->setItemText(toolBox_3->indexOf(page_5), QApplication::translate("frmMain", "\345\237\272\347\241\200\345\234\272\346\231\257", Q_NULLPTR));
        toolBox_3->setItemText(toolBox_3->indexOf(page_6), QApplication::translate("frmMain", "\345\210\206\346\236\220\351\243\216\346\240\274", Q_NULLPTR));
        toolBox_3->setItemText(toolBox_3->indexOf(page_2), QApplication::translate("frmMain", "\346\265\267\345\233\276\345\217\202\346\225\260", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("frmMain", "\350\256\276\347\275\256", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class frmMain: public Ui_frmMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRMMAIN_H
