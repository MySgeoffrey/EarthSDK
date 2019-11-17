#ifndef MYCHARTS57DATALIB_H
#define MYCHARTS57DATALIB_H

#include "mycharts57datalib/mycharts57datalib_global.h"

class OGRLayer;
class OGRFeature;
class OGRFeatureDefn;
namespace MyChart
{
	class IMyS57MapData;
	class IMyS57LayerData;
	class IMyShapeRecord;
	class IMyProperty;
	/// <summary>
	/// S57��ͼ����������
	/// </summary>
	class MYCHARTS57DATALIB_EXPORT CMyShpDataDriver
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		CMyShpDataDriver();

		/// <summary>
		/// ��������
		/// </summary>
		virtual ~CMyShpDataDriver();

		/// <summary>
		/// ��ȡ��ͼ����
		/// </summary>
		/// <param name="chartFilePath">��ͼ·��</param>
		/// <returns>��ͼ����</returns>
		virtual IMyS57MapData* readS57Map(const QString& chartFilePath);

		/// <summary>
		/// ��ȡͼ������
		/// </summary>
		/// <param name="pLayer">��ͼͼ��</param>
		/// <param name="pMap">��ͼ����</param>
		/// <returns>�Ƿ��ȡ�ɹ�</returns>
		virtual bool readLayer(OGRLayer* pLayer, IMyS57MapData* pMap);

		/// <summary>
		/// ��ȡͼ��Ҫ������
		/// </summary>
		/// <param name="pFeature">Ҫ��</param>
		/// <param name="pFeatureDefn">Ҫ�ض������</param>
		/// <param name="inout_pShapeRecord">Ҫ�ض���</param>
		/// <param name="inout_pProperty">Ҫ�����Զ���</param>
		/// <returns>�Ƿ��ȡ�ɹ�</returns>
		virtual bool readFeature(OGRFeature* pFeature, OGRFeatureDefn* pFeatureDefn,
			IMyShapeRecord*& inout_pShapeRecord,QStringList*& inout_pProperty);

	private:

	};

	class MYCHARTS57DATALIB_EXPORT CMyChartS57DataDriver
		: public CMyShpDataDriver
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		CMyChartS57DataDriver()
		{}

		/// <summary>
		/// ��������
		/// </summary>
		virtual ~CMyChartS57DataDriver()
		{}

		/// <summary>
		/// ��ȡ��ͼ����
		/// </summary>
		/// <param name="chartFilePath">��ͼ·��</param>
		/// <returns>��ͼ����</returns>
		virtual IMyS57MapData* readS57Map(const QString& chartFilePath);
	};
}


#endif // MYCHARTS57DATALIB_H
