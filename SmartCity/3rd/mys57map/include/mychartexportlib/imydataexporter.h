#ifndef IMYDATAEXPORTER_H
#define IMYDATAEXPORTER_H

#include "mycharts57datalib/mycharts57datalib.h"
#include "mychartdrawlib/imys57map.h"
#include "mycharts57datalib/imys57mapdata.h"
#include "mycharts57datalib/imys57layerdata.h"
#include "mychartmaplib/imygeometry.h"
#include "mychartmaplib/imyshaperecord.h"
#include "mychartexportlib/mychartexportlib_global.h"


namespace MyChart
{
	// ��γ���������ݽṹ
	typedef  struct{
		double fLon1; // ���ϽǾ���
		double fLat1; // ���Ͻ�γ��
		double fLon2; // ���½Ǿ���
		double fLat2; // ���½�γ��
	}DefLonLatRegion;

	// �����Ϣ�ṹ��
	typedef struct{
		// �������, 1: ������2�����꣬���������廹�����������Ҳ���һ��
		int nType;

		// ���ͼ������
		std::string layerName;

		// ���λ�ã�����Ⱦͼ�е�����λ��
		int nX;
		int nY;

		// ����С������Ⱦͼ�е����ش�С
		int nX1; // ���Ͻ�����X����
		int nY1; // ���Ͻ�����Y����
		int nX2; // ���½�����X����
		int nY2; // ���½�����Y����
	}DefSymbol;

	//// ������Ⱦ��
	class MYCHARTEXPORTLIB_EXPORT encReader
	{
	public:
		encReader();

		virtual ~encReader();

		// ��ȡ�Ƿ���Ҫ�������
		virtual bool getNeedDepthData();

		// �����Ƿ���Ҫ�������
		virtual void setNeedDepthData(const bool& value);

		// ��ȡs57��������ͼ����
		void parseEnc(const char* cchFilename);

		// ��ȡ��������ͼ�ľ�γ�ȷ�Χ
		void getRegionSize(DefLonLatRegion &region);

		// ��Ⱦ���Ӻ�ͼ
		// uchMapImage�������������Ⱦ��ͼ�Ļ�����bgra��ͨ������ָ��ռ��ڸ����ⲿ���䡣
		// �ñ�������Ϊunsigned char *��Ҳ����ΪQImage& qMapImage��QImageͼ���ʽΪQImage::Format_ARGB32��
		// 
		// nImgWidth����������������uchImage�Ŀ��
		// nImgHeight����������������uchImage�ĸ߶�
		// fDepthImage���������: ��СnImgWidth��nImgHeight��double���飬ÿ������λ�ö�Ӧ��ͼ��Ӧλ�õ����ֵ����λΪ�ף�С�������λ��Ч����
		// encRoi����������������Ⱦ��ͼ�ľ�γ������
		// vSymbol���������: ������������������
		void renderImage(QImage& mapImage, 
			int nImgWidth, int nImgHeight, 
			int *uchDepthImage,
			DefLonLatRegion encRoi,
			std::vector<DefSymbol> &vSymbol);

		// ��Ⱦ���Ӻ�ͼ
		// uchMapImage�������������Ⱦ��ͼ�Ļ�����bgra��ͨ������ָ��ռ��ڸ����ⲿ���䡣
		// �ñ�������Ϊunsigned char *��Ҳ����ΪQImage& qMapImage��QImageͼ���ʽΪQImage::Format_ARGB32��
		// nImgWidth����������������uchImage�Ŀ��
		// nImgHeight����������������uchImage�ĸ߶�
		void renderImage(QImage& mapImage, 
			int nImgWidth, int nImgHeight, 
			int *uchDepthImage);

		/// <summary>
		/// ���껻��
		/// </summary>
		/// <param name="screenX">��ĻX����</param>
		/// <param name="screenY">��ĻY����</param>
		/// <param name="prjX">ͶӰ����X</param>
		/// <param name="prjY">ͶӰ����Y</param>
		/// <param name="pDrawArgs">���Ʋ���</param>
		void screen2PrjCoor(const double& screenX,const double& screenY,
			double& prjX,double& prjY,
			int nImgWidth, int nImgHeight, 
			double mapScale,
			DefLonLatRegion encRoi);

		void prj2ScreenCoor(const double& prjX,const double& prjY,
			double& screenX,double& screenY,
			int nImgWidth, int nImgHeight, 
			double mapScale,
			DefLonLatRegion encRoi);

	protected:
		int getDepthMark(IMyS57LayerData* pDepareLayer,double geoLon,double geoLat);
		bool contain(IMyPolygon* pGeometry,double geoLon,double geoLat);
	private:
		MyChart::IMyS57MapData* mpChartMapData;
		MyChart::IMyS57Map* mpS57DrawMap;
		bool mNeedDepthData;
	};
}

#endif // IMYDATAEXPORTER_H
