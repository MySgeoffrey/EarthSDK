#ifndef MYCHARTS57LAYER_H
#define MYCHARTS57LAYER_H

#include "mychartdrawlib/mychartdrawlib_global.h"
#include "mycharts57datalib/imys57layerdata.h"
#include "mychartmaplib/imylayer.h"

namespace MyChart
{
	class IMyS52Symbol;
	class IMyS52LayerSymbol;
	/// <summary>
	/// S57ͼ�������
	/// </summary>
	class MYCHARTDRAWLIB_EXPORT IMyS57Layer
		: public MyChart::IMyLayer
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		IMyS57Layer();

		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="name">��������</param>
		IMyS57Layer(const QString& name);

		/// <summary>
		/// ��������
		/// </summary>
		virtual ~IMyS57Layer();

		/// <summary>
		/// ͼ���ʼ��
		/// </summary>
		virtual void initial();

		/// <summary>
		/// ��ȡͼ�����Ⱦ���ȼ�
		/// </summary>
		/// <returns>��Ⱦ���ȼ�</returns>
		virtual int getRenerPriority();

		/// <summary>
		/// ��ͼ�����û�������
		/// </summary>
		/// <param name="pData">���ݶ����ڲ����ã�</param>
		virtual void setData(IMyS57LayerData* pData);

		/// <summary>
		/// ��ȡͼ��Ļ�������
		/// </summary>
		/// <returns>��������</returns>
		virtual IMyS57LayerData* getData();

		/// <summary>
		/// ����ͼ��Ļ��Ʒ���
		/// </summary>
		virtual void resetSymbol();

		/// <summary>
		/// ��ȡͼ���Ҫ�ؼ���
		/// </summary>
		/// <returns>Ҫ�ؼ���</returns>
		virtual std::vector<IMyShapeRecord*>& getRecords();

		/// <summary>
		/// ��ȡͼ������Լ���
		/// </summary>
		/// <returns>���Լ���</returns>
		virtual IMyProperty* getProperty();

		
		/// <summary>
		/// ��ȡͼ��ļ�������
		/// </summary>
		/// <returns>��������</returns>
		virtual GeometryType getType();

		/// <summary>
		/// ��ȡͼ�������
		/// </summary>
		/// <returns>����</returns>
		virtual QString getLayerType();

		/// <summary>
		/// ��ȡͼ������»��ƵĽ��
		/// </summary>
		/// <returns>���»��ƵĽ��</returns>
		virtual QPixmap& getImage();

		/// <summary>
		/// ����ͼ��ӿ�
		/// </summary>
		/// <param name="pArgs">��ǰ���Ʋ���</param>
		virtual bool draw(IMyDrawArgs* pArgs);

		/// <summary>
		/// ����ͼ���ı��ӿ�
		/// </summary>
		/// <param name="currentTextRects">��ǰ�ı����ƾ��μ���</param>
		/// <param name="pArgs">��ǰ���Ʋ���</param>
		virtual bool drawText(std::vector<QRect>& currentTextRects,IMyDrawArgs* pArgs);

	protected:

		virtual bool drawRecord(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,IMyDrawArgs* pArgs);
		virtual bool drawPointRecord(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,IMyDrawArgs* pArgs);
		virtual bool drawMultiPointRecord(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,IMyDrawArgs* pArgs);
		virtual bool drawPolylineRecord(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,IMyDrawArgs* pArgs);
		virtual bool drawPolygonRecord(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,IMyDrawArgs* pArgs);
		
		virtual bool drawRecordText(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,std::vector<QRect>& currentTextRects,IMyDrawArgs* pArgs);
		virtual bool drawPointRecordText(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,std::vector<QRect>& currentTextRects,IMyDrawArgs* pArgs);
		virtual bool drawMultiPointRecordText(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,std::vector<QRect>& currentTextRects,IMyDrawArgs* pArgs);
		virtual bool drawPolylineRecordText(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,std::vector<QRect>& currentTextRects,IMyDrawArgs* pArgs);
		virtual bool drawPolygonRecordText(IMyShapeRecord* pRecord,IMyS52Symbol* pSymbol,std::vector<QRect>& currentTextRects,IMyDrawArgs* pArgs);
		virtual bool drawText(QString& text,QRect& rect,QFont& font,std::vector<QRect>& currentTextRects,IMyDrawArgs* pArgs);

		virtual QImage symbolShow(QString codeOfTargetClass);
		virtual QPoint getDirectionBetweenCenterAndPivot(QString codeOfTargetClass);
	protected:
		IMyS57LayerData* mpData;
		IMySymbol* mpSymbol;
	};
}
#endif // MYCHARTS57LAYER_H
