#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QDialog>
#include <QThread>
#include "vr_globewidget.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtGui/QStandardItemModel>
#include "geometryeditor/geometryeditor.h"

namespace Ui {
class frmMain;
 class splashScreen;
}

class CSplashScreenDlg
	: public QDialog, public QThread
{
	Q_OBJECT
public:	
	CSplashScreenDlg(QWidget *parent = 0, Qt::WFlags flags = 0);

	~CSplashScreenDlg();

	void updateState(QString info);

	void waitMinites();

protected:
	Ui::splashScreen* ui;
};

class frmMain : public QDialog
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
	void keyPressEvent(QKeyEvent *);

	//bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

	void loadAis();

private slots:
    void on_btnMenu_Close_clicked();

    void on_btnMenu_Max_clicked();

    void on_btnMenu_Min_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

	void slotShowToolControl();

	void slotSearchPlaceName();

	void replyFinished(QNetworkReply*);

	void placeSearchClicked(const QModelIndex & index);

	void slotDisMeature();
	void slotAreaMeature();
	void slotAzimuthMeature();
	void slotClearMeatureResult();



private:
    Ui::frmMain *ui;

    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;

    void InitStyle();

private:
	VRGlobeWidget* mpGlobeWidget;
	//≤‚¡ø∂‘œÛ
	GeometryEditor::IGeometryEditor* mpGeometryEditor;
};

#endif // FRMMAIN_H
