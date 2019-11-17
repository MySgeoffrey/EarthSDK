#ifndef _IMYTRAILLINELAYER_H
#define _IMYTRAILLINELAYER_H

#include "mymeaturelib/mychartmeaturelib_global.h"
#include "mychartmaplib/imylayer.h"
#include "mychartmapcontrol/mychartmapcontrol.h"
#include "mymeaturelib/imytrailline.h"
#include "mymeaturelib/GPRMC.h"
#include <map>

namespace MyChart
{
	class MYCHARTMEATURELIB_EXPORT IMyTrailLineSymbol 
		: public IMyLineSymbol
	{
	public:
		IMyTrailLineSymbol();

		virtual ~IMyTrailLineSymbol();

		virtual void setNeedDrawTrailPoint(const bool& needDrawTrailPoint);

		virtual bool getNeedDrawTrailPoint();

		virtual IMyPointSymbol* getTrailPointSymbol();

	private:
		IMyPointSymbol* mpTrailPointSymbol;
		bool mNeedDrawTrailPoint;
	};

	class MYCHARTMEATURELIB_EXPORT IMyTrailQueryResult
		: public IMyQueryResult
	{
	public:
		IMyTrailQueryResult()
		{}

		virtual ~IMyTrailQueryResult()
		{
		
		}

	public:
		GPS_DATA Data;
	};

	class MYCHARTMEATURELIB_EXPORT IMyPolyLineLayer
		: public IMyLayer
	{
	public:
		IMyPolyLineLayer();

		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="name">��������</param>
		IMyPolyLineLayer(const QString& name);

		virtual ~IMyPolyLineLayer();

		virtual QPixmap& getImage() ;

		virtual IMySymbol* getSymbol();

		virtual IMyTrailShapeRecord* createRecord();

		/// <summary>
		/// ����ͼ��ӿ�
		/// </summary>
		/// <param name="pArgs">��ǰ���Ʋ���</param>
		virtual bool draw(IMyDrawArgs* pArgs);

		virtual void update();

		/// <summary>
		/// ִ�е�ѡ 
		/// </summary>
		/// <param name="screenPosX">��ĻX����</param>
		/// <param name="screenPosY">��ĻY����</param>
		/// <param name="pDrawArgs">��ͼ����</param>
		/// <returns>ִ�е�ѡ���</returns>
		IMyQueryResult* excutePointSelection(
			const float& screenPosX,const float& screenPosY,
			IMyDrawArgs* pDrawArgs);

		int getPointIndex(const float& screenPosX,const float& screenPosY);

		void updatePointByIndex(const int& pointIndex,MyChart::IVector3& position);

		/// <summary>
		/// ��ͼ�����ݱ����������ļ� 
		/// </summary>
		/// <param name="filePath">�ļ�·��</param>
		/// <returns>�Ƿ񱣴�ɹ�</returns>
		bool toFile(const QString& filePath);

		bool toTxtFile(const QString& filePath);

		bool fromTxtFile(const QString& filePath);

		bool toDxfFile(const QString& filePath);

		bool fromDxfFile(const QString& filePath);

		/// <summary>
		/// ��ͼ�����ݱ����������ļ� 
		/// </summary>
		/// <param name="filePath">�ļ�·��</param>
		/// <returns>�Ƿ񱣴�ɹ�</returns>
		bool fromFile(const QString& filePath);

	protected:
		IMyTrailLineSymbol* mpSymbol;
		std::map<int,QPointF> mLastRenderPoints;
		int mCurrentRecordIndex; 
	};

	class MYCHARTMEATURELIB_EXPORT IMyPointLayer
		: public IMyPolyLineLayer
	{
	public:
		IMyPointLayer();

		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="name">��������</param>
		IMyPointLayer(const QString& name);

		virtual ~IMyPointLayer();

		/// <summary>
		/// ����ͼ��ӿ�
		/// </summary>
		/// <param name="pArgs">��ǰ���Ʋ���</param>
		virtual bool draw(IMyDrawArgs* pArgs);

		virtual IMyTrailShapeRecord* createRecord();
	};

	class MYCHARTMEATURELIB_EXPORT IMyPolyLineLayerEditor
		: public MyChart::IMyTool
	{
	public:
		IMyPolyLineLayerEditor();

		virtual ~IMyPolyLineLayerEditor();

		void setMapContorl(MyChartMapControl* pRefControl);

		void setEditingLayer(IMyPolyLineLayer* pRefLayer);

		virtual IMyTrailShapeRecord* createTrailShapeRecord();

		void setEditRecord(IMyTrailShapeRecord* pRefRecord);

		IMyTrailShapeRecord* getEditRecord();

		/// <summary>
		/// �¼�����֮ǰ�Ļص��ӿڣ�������д��
		/// </summary>
		/// <param name="evt">���ڷ������¼�</param>
		virtual bool onBeforeEvent(QEvent* evt);

		/// <summary>
		/// �¼�����֮��Ļص��ӿڣ�������д��
		/// </summary>
		/// <param name="evt">���ڷ������¼�</param>
		virtual bool onAfterEvent(QEvent* evt);

		/// <summary>
		/// �����¼�����֮ǰ�Ļص��ӿڣ�������д��
		/// </summary>
		/// <param name="evt">���ڷ������¼�</param>
		virtual bool onBeforePaintEvent(IMyDrawArgs* pArgs);

		/// <summary>
		/// �����¼�����֮��Ļص��ӿڣ�������д��
		/// </summary>
		/// <param name="evt">���ڷ������¼�</param>
		virtual bool onAfterPaintEvent(IMyDrawArgs* pArgs);

		virtual void beginEdit();

		virtual void endEdit();

		virtual bool isNeedMovePoint();

		virtual void setNeedMovePoint(const bool& value);

	protected:
		bool mIsOver;
		bool mIsMovePoint;
		bool mNeedMovePoint;
		int mLastHoverPointIndex;
		bool mNeedReStart;
		IMyPolyLineLayer* mpRefLayer;
		MyChartMapControl* mpRefControl;
		IMyTrailShapeRecord* mpRefShapeRecord;
	};

	class MYCHARTMEATURELIB_EXPORT IMyDistanceMeatureTool
		: public IMyPolyLineLayerEditor,public IMyPolyLineLayer
	{
	public:
		IMyDistanceMeatureTool();

		IMyDistanceMeatureTool(const QString& name);

		virtual ~IMyDistanceMeatureTool();

		/// <summary>
		/// ����ͼ��ӿ�
		/// </summary>
		/// <param name="pArgs">��ǰ���Ʋ���</param>
		virtual bool draw(IMyDrawArgs* pArgs);

		virtual void beginEdit();

		virtual void endEdit();

	};

	class MYCHARTMEATURELIB_EXPORT IMyAreaMeatureTool
		: public IMyDistanceMeatureTool
	{
	public:
		IMyAreaMeatureTool();

		IMyAreaMeatureTool(const QString& name);

		virtual ~IMyAreaMeatureTool();

		/// <summary>
		/// ����ͼ��ӿ�
		/// </summary>
		/// <param name="pArgs">��ǰ���Ʋ���</param>
		virtual bool draw(IMyDrawArgs* pArgs);
	};

	class MYCHARTMEATURELIB_EXPORT IMyPointMeatureTool
		: public IMyDistanceMeatureTool
	{
	public:
		IMyPointMeatureTool();

		IMyPointMeatureTool(const QString& name);

		virtual ~IMyPointMeatureTool();

		/// <summary>
		/// ����ͼ��ӿ�
		/// </summary>
		/// <param name="pArgs">��ǰ���Ʋ���</param>
		virtual bool draw(IMyDrawArgs* pArgs);
	};

	enum GraphicType
	{
		Arrow1 = 0,

		Arrow2 = 1,

		Point = 10 ,

		Polyline = 11,

		Polygon = 12,

		Ellipse = 13,

		Rectangle = 14
	};

	class IMyDrawShapeRecord : public IMyTrailShapeRecord
	{
	public :
		IMyDrawShapeRecord(const GeometryType& geometryType);

		virtual ~IMyDrawShapeRecord();

		virtual void setGraphicType(const GraphicType& type);

		virtual GraphicType getGraphicType();

	protected:
		GraphicType mGraphicType;
	};


	class MYCHARTMEATURELIB_EXPORT IMyGraphicDrawTool
		: public IMyDistanceMeatureTool
	{
	public:
		IMyGraphicDrawTool();

		IMyGraphicDrawTool(const QString& name);

		virtual ~IMyGraphicDrawTool();

		virtual IMyTrailShapeRecord* createRecord();

		/// <summary>
		/// ����ͼ��ӿ�
		/// </summary>
		/// <param name="pArgs">��ǰ���Ʋ���</param>
		virtual bool draw(IMyDrawArgs* pArgs);

		virtual void setGraphicType(const GraphicType& type);

		virtual GraphicType getGraphicType();

	protected:
		GraphicType mGraphicType;
	};

	class MYCHARTMEATURELIB_EXPORT IMyPolygonLayer
		: public IMyPolyLineLayer
	{
	public:
		IMyPolygonLayer();

		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="name">��������</param>
		IMyPolygonLayer(const QString& name);

		virtual ~IMyPolygonLayer();

		/// <summary>
		/// ����ͼ��ӿ�
		/// </summary>
		/// <param name="pArgs">��ǰ���Ʋ���</param>
		virtual bool draw(IMyDrawArgs* pArgs);
	};
	

	class MYCHARTMEATURELIB_EXPORT IMyPointLayerEditor
		: public IMyPolyLineLayerEditor
	{
	public:
		IMyPointLayerEditor();

		virtual ~IMyPointLayerEditor();


		/// <summary>
		/// �¼�����֮ǰ�Ļص��ӿڣ�������д��
		/// </summary>
		/// <param name="evt">���ڷ������¼�</param>
		virtual bool onBeforeEvent(QEvent* evt);
	};

}

#endif //_IMYTRAILLINELAYER_H