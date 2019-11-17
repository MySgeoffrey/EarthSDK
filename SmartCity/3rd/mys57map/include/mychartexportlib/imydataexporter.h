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
	// 经纬度区域数据结构
	typedef  struct{
		double fLon1; // 左上角经度
		double fLat1; // 左上角纬度
		double fLon2; // 右下角经度
		double fLat2; // 右下角纬度
	}DefLonLatRegion;

	// 物标信息结构体
	typedef struct{
		// 物标类型, 1: 灯塔，2：浮标，。。。具体还有其它物标帮我补充一下
		int nType;

		// 物标图层名称
		std::string layerName;

		// 物标位置，在渲染图中的像素位置
		int nX;
		int nY;

		// 物标大小，在渲染图中的像素大小
		int nX1; // 左上角像素X坐标
		int nY1; // 左上角像素Y坐标
		int nX2; // 右下角像素X坐标
		int nY2; // 右下角像素Y坐标
	}DefSymbol;

	//// 解析渲染类
	class MYCHARTEXPORTLIB_EXPORT encReader
	{
	public:
		encReader();

		virtual ~encReader();

		// 获取是否需要深度数据
		virtual bool getNeedDepthData();

		// 设置是否需要深度数据
		virtual void setNeedDepthData(const bool& value);

		// 读取s57并解析海图数据
		void parseEnc(const char* cchFilename);

		// 读取被解析海图的经纬度范围
		void getRegionSize(DefLonLatRegion &region);

		// 渲染电子海图
		// uchMapImage（输出量）：渲染地图的画布，bgra四通道，此指针空间在该类外部分配。
		// 该变量类型为unsigned char *，也可以为QImage& qMapImage，QImage图像格式为QImage::Format_ARGB32。
		// 
		// nImgWidth（输入量）：画布uchImage的宽度
		// nImgHeight（输入量）：画布uchImage的高度
		// fDepthImage（输出量）: 大小nImgWidth×nImgHeight的double数组，每个像素位置对应地图相应位置的深度值，单位为米，小数点后两位有效数字
		// encRoi（输入量）：被渲染海图的经纬度区域
		// vSymbol（输出量）: 该区域的所有物标容器
		void renderImage(QImage& mapImage, 
			int nImgWidth, int nImgHeight, 
			int *uchDepthImage,
			DefLonLatRegion encRoi,
			std::vector<DefSymbol> &vSymbol);

		// 渲染电子海图
		// uchMapImage（输出量）：渲染地图的画布，bgra四通道，此指针空间在该类外部分配。
		// 该变量类型为unsigned char *，也可以为QImage& qMapImage，QImage图像格式为QImage::Format_ARGB32。
		// nImgWidth（输入量）：画布uchImage的宽度
		// nImgHeight（输入量）：画布uchImage的高度
		void renderImage(QImage& mapImage, 
			int nImgWidth, int nImgHeight, 
			int *uchDepthImage);

		/// <summary>
		/// 坐标换算
		/// </summary>
		/// <param name="screenX">屏幕X坐标</param>
		/// <param name="screenY">屏幕Y坐标</param>
		/// <param name="prjX">投影坐标X</param>
		/// <param name="prjY">投影坐标Y</param>
		/// <param name="pDrawArgs">绘制参数</param>
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
