#ifndef MYCHARTMAPCONTROL_H
#define MYCHARTMAPCONTROL_H

#include <QtWidgets/QWidget>
#include <QTime>
#include "mychartmapcontrol/mychartmapcontrol_global.h"
#include "mychartcommonlib/mychartcoorsystem.h"
#include "ogr_core.h"
#include "mychartmaplib/imymap.h"
#include "mychartmaplib/imygridlayer.h"

/// <summary>
/// ��ͼ��������ö��
/// </summary>
enum EMapOperateType
{
	MOT_PAN,///<����
	MOT_ZOOMIN,///<�Ŵ�
	MOT_ZOOMOUT,///<��С
	MOT_ZOOMMAPIMAGE,///<ͼƬ����
	MOT_MAPEDIT,///<��ͼ�༭
	MOT_POINTSELECT,///<��ѡ 
	MOT_POLYGONSELECT,///<�����ѡ
	MOT_NONE///<�޶���
};

/// <summary>
/// ��ͼ�ؼ�
/// </summary>
class MYCHARTMAPCONTROL_EXPORT MyChartMapControl : public QWidget
{
	Q_OBJECT

public:
	/// <summary>
	/// ��ͼ�ؼ����캯��
	/// </summary>
	MyChartMapControl(QWidget *parent = 0);

	/// <summary>
	/// ��ͼ�ؼ���������
	/// </summary>
	~MyChartMapControl();

	/// <summary>
	/// ���õ�ǰ��ͼ���Ƶ�����ϵͳ����
	/// </summary>
	/// <param name="coorsystemType">����ϵͳ����</param>
	void setCoorsystemType(const MyChart::EMyCoorsystemType& coorsystemType);

	/// <summary>
	/// ��ȡ��ǰ��ͼ���Ƶ�����ϵͳ����
	/// </summary>
	/// <returns>����ϵͳ����</returns>
	MyChart::EMyCoorsystemType getCoorsystemType();

	/// <summary>
	/// ��ȡ��ǰ��ͼ���Ƶķ�Χ
	/// </summary>
	/// <returns>��ǰ��ͼ���Ƶķ�Χ</returns>
	virtual OGREnvelope& getExtend();

	/// <summary>
	/// ��ȡ��ǰ��ͼ���Ƶı�����
	/// </summary>
	/// <returns>��ǰ��ͼ���Ƶı�����</returns>
	virtual double getCurScale();

	/// <summary>
	/// ��ȡ��ǰ��ͼ�ľ�������ͼ��
	/// </summary>
	/// <returns>��������ͼ��</returns>
	virtual MyChart::IMyGridLayer* getGridLayer();

	/// <summary>
	/// ���õ�ǰ��ͼ���ƵĲ�������
	/// </summary>
	/// <param name="type">��������</param>
	void setOperateType(const EMapOperateType& type);

	/// <summary>
	/// ��ȡ��ǰ��ͼ���ƵĲ�������
	/// </summary>
	/// <returns>��������</returns>
	EMapOperateType getOperateType();

	/// <summary>
	/// ������Ļ������
	/// </summary>
	/// <param name="path">ͼƬ·��</param>
	bool saveImage(const QString& path);

	/// <summary>
	/// ��Ӳ�ѯ����
	/// </summary>
	/// <param name="pRefListener">��ѯ����</param>
	void addQueryListener(MyChart::IMyQueryListener* pRefListener);

	/// <summary>
	/// �Ƴ���ѯ����
	/// </summary>
	/// <param name="pRefListener">��ѯ����</param>
	void removeQueryListener(MyChart::IMyQueryListener* pRefListener);

	/// <summary>
	/// �Ƴ����в�ѯ����
	/// </summary>
	void removeAllQueryListeners();

	/// <summary>
	/// ֪ͨ��ѯ�������ҷ��ص�ǰ�Ĳ�ѯ���
	/// </summary>
	void notifyQueryListeners(std::vector<MyChart::IMyQueryResult*>& result);

	/// <summary>
	/// ���õ�ǰ��ͼ���Ƶı�����ɫ
	/// </summary>
	/// <param name="color">������ɫ</param>
	void setBackgroundColor(const QColor& color);

	/// <summary>
	/// ��ȡ��ǰ��ͼ���Ƶı�����ɫ
	/// </summary>
	/// <returns>������ɫ</returns>
	QColor getBackgroundColor();
	
	/// <summary>
	/// ��ӵ�ͼ
	/// </summary>
	/// <param name="pRefMap">��ͼ�����ڲ����ã�</param>
	bool addMap(MyChart::IMyMap* pRefMap);

	/// <summary>
	/// ���ݵ�ͼ���ƻ�ȡ��ͼ����
	/// </summary>
	/// <param name="name">��ͼ����</param>
	/// <returns>��ͼ����</returns>
	MyChart::IMyMap* getMap(const QString& name);

	/// <summary>
	/// ���õ�ͼ������ǰ��������
	/// </summary>
	/// <param name="pActiveTool">��ǰ��������</param>
	void setActiveTool(MyChart::IMyTool* pActiveTool);

	/// <summary>
	/// ��ȡ��ͼ������ǰ��������
	/// </summary>
	/// <returns>��ǰ��������</returns>
	MyChart::IMyTool* getActiveTool();

	/// <summary>
	/// ֪ͨ��ͼ�ؼ���ǰ���߲��������仯
	/// </summary>
	void notifyActiveToolChanged();

	/// <summary>
	/// ֪ͨ��ͼ�ؼ��ػ���ͼ
	/// </summary>
	void notifyRedrawActiveLayer();

	/// <summary>
	/// ��ǰ�Ƿ�ִ���϶�����
	/// </summary>
	/// <returns>�Ƿ�ִ���϶�����</returns>
	bool isMouseDragging();

	/// <summary>
	/// ���µ�ǰ��ͼ���İ�Χ��
	/// </summary>
	void updateBounds();

	/// <summary>
	/// ȫͼ��ʾ
	/// </summary>
	void fullExtend();

	/// <summary>
	/// ˢ�µ�ͼ
	/// </summary>
	void refresh();

	/// <summary>
	/// ��ȡ��ͼ��ʼ״̬�µİ�Χ��
	/// </summary>
	/// <returns>��ʼ״̬�µİ�Χ��</returns>
	virtual OGREnvelope& getInitialExtend();

	/// <summary>
	/// ���õ�ͼ��ʼ״̬�µİ�Χ��
	/// </summary>
	/// <param name="extend">��ʼ״̬�µİ�Χ��</param>
	virtual void setInitialExtend(const OGREnvelope& extend);

	/// <summary>
	/// ������Ļ����ת�ɵ�ǰͶӰ����ϵ�µ�����
	/// </summary>
	/// <param name="screenCoor">��Ļ����</param>
	/// <param name="prjX">X����</param>
	/// <param name="prjY">Y����</param>
	void screenCoor2PrjCoor(const QPointF& screenCoor,
		double& prjX,double& prjY);

	/// <summary>
	/// �ؼ��ػ��¼�
	/// </summary>
	/// <param name="evt">���Ʋ���</param>
	virtual void paintEvent(QPaintEvent * evt);

	/// <summary>
	/// ��갴���¼�
	/// </summary>
	/// <param name="evt">����</param>
	virtual void mousePressEvent(QMouseEvent * evt);

	/// <summary>
	/// ���˫���¼�
	/// </summary>
	/// <param name="evt">����</param>
	virtual void mouseDoubleClickEvent(QMouseEvent* evt);
	
	/// <summary>
	/// ����ͷ��¼�
	/// </summary>
	/// <param name="evt">����</param>
	virtual void mouseReleaseEvent(QMouseEvent * evt);

	/// <summary>
	/// ����ƶ��¼�
	/// </summary>
	/// <param name="evt">����</param>
	virtual void mouseMoveEvent(QMouseEvent * evt);

	/// <summary>
	/// �������¼�
	/// </summary>
	/// <param name="evt">����</param>
	virtual void wheelEvent(QWheelEvent * evt);

	/// <summary>
	/// �ؼ������¼�
	/// </summary>
	/// <param name="evt">����</param>
	virtual void resizeEvent(QResizeEvent * evt);

	/// <summary>
	/// ���������¼�
	/// </summary>
	/// <param name="evt">����</param>
	virtual void keyPressEvent(QKeyEvent * event);

	/// <summary>
	/// �����ͷ��¼�
	/// </summary>
	/// <param name="evt">����</param>
	virtual void keyReleaseEvent(QKeyEvent * event);

	/// <summary>
	/// �ؼ�ʱ���¼�
	/// </summary>
	/// <param name="evt">����</param>
	virtual void timerEvent(QTimerEvent* event);

	/// <summary>
	/// ���Ʊ�����
	/// </summary>
	/// <param name="evt">����</param>
	virtual void drawScale(QPainter& painter);

	/// <summary>
	/// ���ñ����ߵ�����
	/// </summary>
	/// <param name="visible">����</param>
	virtual void setScaleVisible(const bool& visible);

	/// <summary>
	/// ��ȡ�����ߵ�����
	/// </summary>
	virtual bool getScaleVisible();
private:
	void panMap();
	void zoomInMap();
	void zoomOutMap();
	void computeDrawExtend();
	void reComputeDrawExtend(const QPointF& lastPostion,
		const QPointF& curPostion);
	void scaleMapAtPoint(const QPointF& curMousePos,const double& scale);
	void computeCurPrjPosition(const QPointF& pos);
	void excutePointSelection(const QPointF& curMousePos);

	bool drawMap(const int& width,const int& height);
	void drawMap(QPainter& painter);
	void drawZoomShape(QPainter& painter);
	void reDrawActiveMap(QPainter& painter);
	void notifyUpdate(QPainter& painter);
	void drawMapScale(QPainter& painter);
	void drawPanMap(QPainter& painter);
	void drawZoomImage(QPainter& painter);
	void drawEditMap(QPainter& painter);
protected:
	OGREnvelope mInitialExtend;
	OGREnvelope mGeoExtend;
	OGREnvelope mPrjExtend;
	QColor mBackgroundColor;
	QPixmap mBackRenderImage;
	QPixmap mEditRenderImage;
	QPixmap mTopRenderImage;
	QPixmap mRenderBufferImage;
	bool mIsMousePress;
	bool mIsMouseDragging;
	bool mNeedRedrawMap;
	QPointF mMouseLastPos;
	QPointF mMoveDirection;
	EMapOperateType mCurMapOperateType;
	EMapOperateType mLastMapOperateType;
	QPointF mLeftTopPrjPos;
	QPointF mRightBottomPrjPos;
	QPointF mCurMousePrjPos;
	double mCurScale;
	double mCurScaleFactor;
	QTime mScaleTimer; 
	QPointF mLastLeftTopPrjPos;
	QDateTime mLastWheelTime;
	bool mScaleVisible;
	MyChart::EMyCoorsystemType mCoorsystemType;
	int mTimerID;
	MyChart::IMyTool* mpRefTool;
	bool mActiveToolChanged;
	bool mRedrawActiveLayer;
	MyChart::IMyGridLayer* mpGridLayer;
	MyChart::IMyDrawArgs mLastDrawArgs;
	std::vector<MyChart::IMyQueryListener*> mQueryListeners;
};

#endif // MYCHARTMAPCONTROL_H
