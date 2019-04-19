#ifndef MYEARTHSIM_H
#define MYEARTHSIM_H

#include <QtGui/QMainWindow>
#include <QMenu>
#include "QtGui/qdialog.h"
#include "GeneratedFiles/ui_myearthsim.h"
#include "GeneratedFiles/ui_graphictypeui.h"
#include "GeneratedFiles/ui_titleFrm.h"
#include "vr_globewidget.h"
#include "geometryeditor/polygoneditor.h"
#include "geoobject/geocircle.h"
#include "geoobject/geogrid.h"
#include "geoobject/geosegment.h"
#include "geoobject/geoscutcheon.h"
#include "scenetool/scenepicker.h"
#include "geographic/geopoint.h"
#include "geographic/geoline.h"
#include "geographic/geopolygon.h"
#include "geographic/geocirlce.h"
#include "scenetool/gridguieventhandle.h"

class IAreaToolListener;
class CMouseCoordsCallback;
class CCustomGUIEventHandler;
class TitleDlg;

class GraphicTypeDlg : public QDialog
{
public:
	GraphicTypeDlg(QWidget *parent = 0, Qt::WFlags flags = 0);

	~GraphicTypeDlg();

	QString getGraphicType();
private:
	Ui::GraphicTypeUI ui;
};

class myEarthSim : public QMainWindow
{
	Q_OBJECT

public:
	myEarthSim(QWidget *parent = 0, Qt::WFlags flags = 0);

	~myEarthSim();

	virtual void handlePickerEvent(Common::IObject* pResult);
	public slots:

		void initialFireEngineScene();

#if 1 //�����ؼ�ģ��
		//��������ʾ����ؼ�
		void slotShowEarthControl();
		//���õ�����λ��
		void slotSetNavigationPosition();
		//���ж�λ
		void slotFlyToTarget();
		//����
		void slotActionStartGetMourseGeoPosition();
		//�ر�
		void slotActionEndGetMourseGeoPosition();
		//GUI�¼�
		void slotActionGUIEventHandler();
		//״̬���������С
		void slotActionStateTextSize();
		//��ȡ�����λ��
		void slotActionGetCameraPosition();
		//����������㷽λ��
		void slotActionComputeAzimuth();
		//��������ĸ߶ȷ�Χ
		void slotActionSetCameraHeight();
		//��������Ĳ���ģʽ
		void slotActionSetCameraMode();
		//���ñ����ߵ�λ��
		void slotActionSetGlobeScalePosition();
		void initialMeatureStyle();

#endif
		
#if 1 //��Ϣ��ѯ
		void slotStartPointQuery();
		void slotEndPointQuery();
#endif

#if 1 //ͼ�����ģ��
		void slotAddTMSImage();
		void slotAddTMSElevation();
		void slotAddGoogleImage();
		void slotUnVisibleTMSImage();
		void slotVisibleTMSImage();
		void slotUnVisibleGoogleImage();
		void slotVisibleGoogleImage();
		void slotUnVisibleTMSElevation();
		void slotVisibleTMSElevation();
		void slotAddCountryLineShp();
		void slotUnVisibleLineShp();
		void slotVisibleLineShp();
		void slotAddPolygonShp();
		void slotUnVisiblePolygonShp();
		void slotVisiblePolygonShp();
		void slotAddPointShp();
		void slotUnVisiblePointShp();
		void slotVisiblePointShp();
		//�Զ�����Ƭ
		void slotLoadCustomTile();
		void slotShowCustomTile();
		void slotHideCustomTile();

		//������ͼӰ��
		void slotActionAddTDTImage();
		//��ӱ���dem
		void slotActionAddTifDem();

		//��ά��������ͼ�����
		void slotActionCreateJFQLayer();
		void slotActionVisibleJFQLayer();
		void slotActionDeleteJFQLayer();
#endif

#if 1 //�ռ�����
		void slotBeginFlatDistance();
		void slotEndFlatDistance();
		void slotBeginSphereDistance();
		void slotEndSphereDistance();
		void slotBeginFlatArea();
		void slotEndFlatArea();
		void slotBeginSphereArea();
		void slotEndSphereArea();
		void slotBeginAzimuth();
		void slotEndAzimuth();
#endif

#if 1 //������ģ��
		void slotDrawCircle();
		//����Բ
		void slotActionCreateCircle();
		//����Բ
		void slotActionDeleteCircle();
		//�������θ���
		void slotActionCreateRectGrid();
		//���ٷ��θ���
		void slotActionDeleteRectGrid();
		//����Բ�θ���
		void slotActionDeleteRingGrid();
		//����Բ�θ���
		void slotActionCreateRingGrid();
		//���������
		void slotActionCreatePoint();
		//�����߶���
		void slotActionCreateLine();
		//���������
		void slotActionCreatePolygon();
		//���ٵ����
		void slotActionDeletePoint();
		//�����߶���
		void slotActionDeleteLine();
		//���������
		void slotActionDeletePolygon();
		//��������
		void slotActionCreateScutcheon();
		//���ٱ���
		void slotActionDeleteScutcheon();
		//�����߶�
		void slotActionCreateSegment();
		//�����߶�
		void slotActionDeleteSegment();
		//�״�ɨ����
		void slotActionCreateRadarLine();
		void slotActionDeleteRadarLine();
		void slotActionModifyRadarColor();

#endif

#if 1 //�༭���
		void slotBeginEditorPoint();
		void slotEndEditorPoint();
		void slotBeginEditorLine();
		void slotEndEditorLine();
		void slotBeginEditorPolygon();
		void slotEndEditorPolygon();

		//��������ͼ�α��
		void slotActionBeginGraphicEdit();
		void slotActionBeginSelectedGraphicEdit();
		//�رյ���ͼ�α��
		void slotActionEndGraphicEdit();
#endif

#if 1 //̬����ʾ
		void slotActionSetScutcheonFields();
		void slotAddDynamicObjects();
		void slotActionControlIconVisible();
		void slotActionControlScutVisible();
		void slotActionControlTrackLineVisible();
		void slotActionChangeTrackLineColor();
		void slotActionChangeTrackLineType();
		void slotActionChangeTrackLineWidth();
		void slotActionChangeTrackLineLength();
		void slotActionDirLineVisible();
		void slotActionChangeDirLineColor();
		void slotActionChangeDirLineType();
		void slotActionChangeDirLineWidth();
		void slotActionCircleTangentAngle();
		void slotActionPolygonTangentAngle();
		void slotActionComputeAngle();
		void slotActionComputeBanyCenter();
		void slotActionDynamicObjectVisible();
		void slotActionObjectVisibleByFacility();
		void slotActionChangeIconStyle();
		void slotActionChangeObjectIconStyle();
		void slotActionChangeScuStyle();
		void slotActionChangeObjecctScuStyle();
		void slotActionChangeTrackPoints();
		void slotActionChangeObjectTrackPoint();
		void slotActionChangeScuOffset();
		void slotActionChangeObjectScuShadow();
		void slotActionChangeScuShadow();
		void slotActionJudgePointInCircle();
		void slotActionJudgePointInGeometry();
		void slotActionEnableDragScu();
		void slotActionVisibleIconText();
		void slotActionVisibleObjectIconText();
		void slotActionVisibleScuLine();
		void slotActionChangeScuLineColor();
		void slotActionChangeScuLineType();
		void slotActionChangeScuLineWidth();
		//��Ӹ澯����
		void slotAddWarnningRegion();
		void slotAddScanningRegion();
		void slotActionAddObjectListener();
		void slotActionDynamicObjectBlink();
#endif

		void getObjects();

private:
	Ui::myEarthSimClass ui;
	VRGlobeWidget* mpGlobeWidget;

	//����

	TitleDlg* mpTitleDlg;


	//��������
	GeometryEditor::IGeometryEditor* mpGeometryEditor;

	//�༭����
	GeometryEditor::IGeometryEditor* mpEditorObject;

	//����������
	Geo::CGeoCircleEx* mpGeoRadarLine;
	Geo::CGeoRingGrid* mpGeoRingGrid;
	SceneTool::CGridMeasure* mpGeoRingGridMeature;
	Geo::CGeoRectGrid* mpGeoRectGrid;
	SceneTool::CGridMeasure* mpGeoRectGridMeature;
	Geo::CGeoScutcheon* mpGeoScutcheon;
	Geo::CGeoSegment* mpGeoSegment;

	//ͼ�ζ���
	Graphic::CGeoCircle* mpGeoCircle;
	Graphic::CGeoPoint* mpGeoPoint;
	Graphic::CGeoLine* mpGeoLine;
	Graphic::CGeoPolygon* mpGeoPolygon;
	
	//��Ϣ��ѯ
	SceneTool::CModelPickerTool* mpModelPicker;
	//������λ�ü���
	CMouseCoordsCallback* mpMouseCoordsCallback;
	//����GUI�¼�����
	CCustomGUIEventHandler* mpCustomGUIEventHandler;
	//����˵�
	QMenu* mpMenuMap;
	QMenu* mpMenuTarget;
	QMenu* mpMenuDevice;
};

class TitleDlg : public QDialog
{
	Q_OBJECT
public:
	TitleDlg(QWidget *parent = 0, Qt::WFlags flags = 0);

	~TitleDlg();

	void timerEvent ( QTimerEvent * event ); 

private slots:
	void slotExit();
	void slotFireSim();
	void slotDataSim();
	void slotAnalyse();
	void slotSetting();
public:
	myEarthSim* Owner;
private:
	Ui::TitleForm ui;
};

#endif // MYEARTHSIM_H
