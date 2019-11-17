#ifndef MYCHARTS57MAP_H
#define MYCHARTS57MAP_H

#include "mychartdrawlib/mychartdrawlib_global.h"
#include "mycharts57datalib/imys57mapdata.h"

namespace MyChart
{
	typedef std::vector<IMyLayer*> Layers;
	/// <summary>
	/// S57��ͼ������
	/// </summary>
	class MYCHARTDRAWLIB_EXPORT IMyS57Map
		: public IMyMap
	{
	public:
		/// <summary>
		/// ���캯��
		/// </summary>
		IMyS57Map();

		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="name">��������</param>
		/// <param name="pData">�������ݶ���</param>
		IMyS57Map(const QString& name,IMyS57MapData* pData);

		/// <summary>
		/// ���캯��
		/// </summary>
		/// <param name="name">��������</param>
		IMyS57Map(const QString& name);

		/// <summary>
		/// ͼ���ʼ��
		/// </summary>
		virtual void initial();

		/// <summary>
		/// ����ͼ���û�������
		/// </summary>
		/// <param name="pData">���ݶ����ڲ����ã�</param>
		virtual void setData(IMyS57MapData* pData);

		/// <summary>
		/// ��ȡͼ��Ļ�������
		/// </summary>
		/// <returns>��������</returns>
		virtual IMyS57MapData* getData();

		/// <summary>
		/// ���ú�ͼ�Ļ��Ʒ���
		/// </summary>
		virtual void resetSymbol();

		/// <summary>
		/// ��������
		/// </summary>
		virtual ~IMyS57Map();

		/// <summary>
		/// ��ȡ��ͼ�����»��ƵĽ��
		/// </summary>
		/// <returns>���»��ƵĽ��</returns>
		virtual QPixmap& getImage();

		/// <summary>
		/// ����ͼ��ӿ�
		/// </summary>
		/// <param name="pArgs">��ǰ���Ʋ���</param>
		virtual bool draw(IMyDrawArgs* pArgs);

	protected:
		virtual bool drawGraphic(Layers& layers,IMyDrawArgs* pArgs);

		virtual bool drawText(Layers& layers,std::vector<QRect> currentTextRects,IMyDrawArgs* pArgs);

		virtual bool constructDrawLayer();

	protected:
		IMyS57MapData* mpS57Data;
		Layers mPolygonLayers;
		Layers mPolylineLayers;
		Layers mPointLayers;
		QPixmap mImage;
	};

}

#endif // MYCHARTS57MAP_H
